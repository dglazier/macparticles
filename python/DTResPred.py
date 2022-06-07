import ROOT
import numpy as np
import copy
from sklearn.tree import DecisionTreeRegressor 
from joblib import load, dump

############################################################################
###PREPARE DATA
############################################################################
dfdata=df.DataFrame()
x_vars =df.GetNormTruthVars()
n_xvars= x_vars.size()
print('DTResPred.py : will simulate with x variables ',x_vars)

#Need all data to synchronise, will mask accepted
x_data = dfdata.AsNumpy(x_vars);


x_array = np.vstack([x_data[xkey] for xkey in x_data.keys()]).T

#only want to predict for accepted events
acc_var = df.GetAcceptVar()
acc_data =  dfdata.AsNumpy({acc_var});
acc_array = np.vstack([acc_data[akey] for akey in acc_data.keys()])
############################################################################
###MAKE PREDICTIONS ON TRAINING DATA
############################################################################

saveto = save_dir.GetName() + '/'
def addRandomInputs(X_orig):
    
    nbSamples=len(X_orig)
    X_new=X_orig
    
    for i in range(dtconf.n_rand_in):
        randomP = np.random.uniform(0,1,nbSamples).reshape(nbSamples,1) 
        X_new=np.hstack((randomP,X_new))
        
    return X_new

#get a model index for each event
whichModel=np.random.randint(dtconf.n_regs, size=x_array.shape[0])
#create empty array for predictions
#Need all data to synchronise, will mask accepted
#get truth vars
tru_vars =df.GetTruthVars()
n_truvars= tru_vars.size()
print('DTResPred.py : will correct truth variables ',tru_vars)
tru_data = dfdata.AsNumpy(tru_vars);
tru_array = np.vstack([tru_data[xkey] for xkey in tru_data.keys()]).T
print(tru_array.shape)

#y_pred=copy.deepcopy(tru_array)
y_pred=np.zeros_like(x_array)
accepted = (acc_array==1)[0,:] #mask events flagged as accepted


#y_pred=np.zeros((3,3))                    #better way to initialise numpy arrays?
#y_pred=np.zeros((x_array.shape[0],3))
#index=np.zeros(x_array.shape[0])
   
for imodel in range(dtconf.n_regs):
    whichm = (whichModel==imodel) #mask events for this model
    acc_and_model = whichm*accepted
    x_here = x_array[acc_and_model]
    indices= np.where(acc_and_model)[0] #in data for this model
#    print(indices)
    x_pred=addRandomInputs(x_here)
#    print(x_pred)
#    print(x_here)
    model = load(saveto+dtconf.model_name+ str(imodel) +'.joblib')
    y_pred[indices]=model.predict(x_pred)

#    y_predTemp=model.predict(x_pred[whichModel==imodel])
#    indexTemp=np.where(whichModel==imodel)[0]
        #print(indexTemp.shape)
        #print(y_predTemp.shape)
        #print(indexTemp)
    #if imodel==0:
#    y_pred[indexTemp]=y_predTemp
#    index=indexTemp
    #else:
    #    y_pred=np.vstack((y_pred,y_predTemp))
    #    index=np.hstack((index,indexTemp))
     
    
    print(y_pred)
    print(y_pred.shape)
    del x_pred
    del x_here
    del model
    del indices
    del whichm
    del acc_and_model
    print('DTResPred.py : done predictions  for model ',imodel)

#print(index)
#unshuf_order = np.zeros_like(index)
#unshuf_order[index.astype(int)] = np.arange(x_array.shape[0])
#y_pred=y_pred[unshuf_order]

############################################################################
###WRITE TO ROOT FILE
############################################################################
#create dictionary to give branchnames to arrays
y_dict = {}
y_vars=df.GetReconVars()
print(y_pred.shape)

##add the differences to the truth values
y_pred = tru_array - y_pred
for A, B in zip(y_vars, y_pred.T):
    y_dict[str(A)] = B

print(y_dict)
print(tru_data)

pred_rdf = ROOT.RDF.MakeNumpyDataFrame(y_dict)
pred_rdf.Snapshot("recon", str(out_dir)+"predictions.root")


del whichModel
del y_pred

del acc_array
del accepted
del x_array
del y_dict
del y_vars
del tru_array
del tru_data
del tru_vars
del dfdata
del x_data
del x_vars
del n_xvars
del acc_data
del acc_var
