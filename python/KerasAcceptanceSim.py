#turnoff gpu
import os
os.environ["CUDA_VISIBLE_DEVICES"]="-1"
import ROOT
#import keras
import numpy as np
import tensorflow as tf
from tensorflow import keras
from keras import models
from keras import layers
from joblib import load, dump
from sklearn.preprocessing import QuantileTransformer


############################################################################
###PREPARE DATA
############################################################################
#dfdata=df.DataFrame()
dfdata= df.GetGeneratedFrame();

vars = df.GetGenVars()
nvars= vars.size()
print('KerasAcceptanceSim.py : will simulate with variables ',vars)
simdf=dfdata.AsNumpy(vars);
sim_data = np.vstack([simdf[xkey] for xkey in simdf.keys()]).T
print(sim_data)
print('KerasAcceptanceSim.py : number of  events to process ',sim_data.shape)

############################################################################
###PREPARE TRAINED MODEL
############################################################################
accdir = acc_dir.GetName() + '/'


print('KerasAcceptanceSim.py : attempt to load model , '+accdir+model_name.GetName()+'.keras')

acc_model= keras.models.load_model(accdir + model_name.GetName()+'.keras')

##########################################################################
###USE BDT REWEIGHTER
##########################################################################
if(do_bdt_reweight.GetName()=="TRUE"):
    bdt = load(reweight_name.GetName())

##########################################################################
###GAUSSIAN TRANSFORM SIMULATION VARIABLES IF REQUIRED
##########################################################################
#sim_data_gaus=sim_data
if(do_gauss.GetName()=="TRUE"):
    gauss_transform = load(accdir+'gaussscaler.joblib')
    sim_data_gaus=gauss_transform.transform(sim_data[:,:nvars].copy())
    probs = acc_model.predict(sim_data_gaus)
    #print('gaus ',sim_data_gaus)
    #print('probs ',probs)
    del gauss_transform
    del sim_data_gaus
else:
    ##########################################################################
    ###GET DETECTION PROBABILIES FROM MODEL
    ##########################################################################
    probs = acc_model.predict(sim_data)

   
weights = probs/(1-probs)
print('KerasAcceptanceSim.py : initial weights ',weights)
##########################################################################
###USE BDT REWEIGHTER IF REQUESTED
##########################################################################
if(do_bdt_reweight.GetName()=="TRUE"):
    extra_probs = bdt.predict_proba(sim_data)[:,1]
    extra_weights = extra_probs/(1-extra_probs)
    #print(weights)
    #print(extra_weights)
    weights = weights.T*(extra_weights)
    weights = weights.T
    #print(weights)
    
    del extra_probs
    del extra_weights
    del bdt

print('KerasAcceptanceSim.py : reweighted ',weights)
   
##########################################################################
###PRODUCE ACCEPTANCE MASK
##########################################################################
def rejection_sample(weights, nev):
    rands = np.random.uniform(0, 1, nev)
    mask = rands[:] < weights[:]
    return mask.astype(int)

accepted_mask  = rejection_sample(weights[:,0], sim_data.shape[0])
print('KerasAcceptanceSim.py : save to  ',str(out_dir)+"simulation_acceptances.root")
acc_rdf = ROOT.RDF.FromNumpy({str(part_acc) :accepted_mask})#,"fast_weight":weights_acc})
acc_rdf.Snapshot("acceptance", str(out_dir)+"simulation_acceptances.root")

del dfdata
del simdf
del sim_data
del acc_model
del probs
del weights
del accepted_mask
del acc_rdf
del vars
del nvars

tf.keras.backend.clear_session()
