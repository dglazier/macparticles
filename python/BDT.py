import ROOT
import numpy as np
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.preprocessing import QuantileTransformer
from joblib import load, dump
#from KerasTest import data_array

print('BDT.py : Acceptance Training')

############################################################################
###PREPARE DATA
############################################################################

dfdata=df.DataFrame()
vars = df.GetTruthVars()
nvars= vars.size()
print('BDT.py : will train with variables ',vars)

#use extra variable flag to ID generated(=0) and accepted(=1) datasets
vars.push_back("flag")

#create dataframes for generated and accepted datasets
accdf = dfdata.Filter(df.GetAcceptCondition(1)).Define('flag','1').AsNumpy(vars);
alldf= dfdata.Filter(df.GetInitAcceptCondition(1)).Define('flag','0').AsNumpy(vars);

#extract numpy arrays from dictionary and combine accepted and generated into 1 dataset
acc_array = np.vstack([accdf[xkey] for xkey in accdf.keys()])
all_array = np.vstack([alldf[xkey] for xkey in alldf.keys()])
data_array = np.hstack((acc_array,all_array)).T

saveto = save_dir.GetName() + '/'

##########################################################################
###GAUSSIAN TRANSFORM TRAINING VARIABLES
##########################################################################

if(do_gauss.GetName()=="TRUE"):
    gauss_transform = QuantileTransformer(output_distribution="normal",copy=False)
    #data_array[:,:nvars] => all rows and comumns 0 -> nvars-1 i.e. not 'flag'
    #gauss_array = gauss_transform.fit_transform(data_array[:,:nvars].copy())
    #replace data_array with new values
    #gauss_transform.fit_transform(data_array[:,:nvars])
    #Save Gaussian transformer.
    dump(gauss_transform,saveto+'gaussscaler.joblib')
    print('BDT.py : saved gaussian transform to ',saveto+'gaussscaler.joblib')

##########################################################################
###CONFIGURE MODEL
##########################################################################
print('BDT.py : configure Model, n_estimators = ',bdtconf.n_estimators)
#Fit the BDT on the new dataset and save model.
bdt = GradientBoostingClassifier(n_estimators = bdtconf.n_estimators,
                                 max_depth=bdtconf.max_depth,
                                 learning_rate=bdtconf.learning_rate,
                                 min_impurity_decrease=bdtconf.min_impurity_decrease,
                                 verbose=bdtconf.verbose)

##########################################################################
###TRAIN MODEL
##########################################################################
bdt.fit(data_array[:,:nvars], data_array[:,nvars]) #variable arrays, flag

print('BDT.py : save bdt model to ',saveto + bdtconf.bdt_name + '.joblib')
dump(bdt,saveto + bdtconf.bdt_name + '.joblib')

##########################################################################
###CALCULATE NEW ACCEPTANCES
##########################################################################
del data_array
del alldf
del all_array

##Make sure we are using all data here when reweighting
alldf= dfdata.Define('flag','0').AsNumpy(vars);
data_array = np.vstack([alldf[xkey] for xkey in alldf.keys()]).T
if(do_gauss.GetName()=="TRUE"):
    gauss_transform = QuantileTransformer(output_distribution="normal",copy=False)
    #data_array[:,:nvars] => all rows and comumns 0 -> nvars-1 i.e. not 'flag'
    #gauss_array = gauss_transform.fit_transform(data_array[:,:nvars].copy())
    #replace data_array with new values
   # gauss_transform.fit_transform(data_array[:,:nvars])

#calculate acceptance per event
probs = bdt.predict_proba(data_array[:,:nvars][data_array[:,nvars]==0])[:,1]#bdt.predict(data_array[:,:nvars][data_array[:,nvars]==0])
weights_acc = probs/(1-probs)


def rejection_sample(weights, data):
    rands = np.random.uniform(0, 1, data.shape[0])
    mask = rands[:] < weights[:]
    return mask.astype(int)

#fast_acc = rejection_sample(weights_acc[:,0], all_array.T.shape[0])

#fast_acc = rejection_sample(weights_acc, data_array[data_array[:,nvars]==0])
fast_acc = rejection_sample(weights_acc, data_array)

#save to root file via rdataframe
acc_rdf = ROOT.RDF.MakeNumpyDataFrame({"fast_accept":fast_acc,"fast_weight":weights_acc})
acc_rdf.Snapshot(bdtconf.bdt_name, saveto+"training_acceptances.root")

#To run another script must make sure all new objects have been deleted
del acc_rdf
del fast_acc
del probs
del weights_acc
del acc_array
#del all_array
del data_array
del accdf
del alldf
del dfdata
