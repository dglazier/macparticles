import ROOT
import numpy as np
from sklearn.tree import DecisionTreeRegressor 
from joblib import load, dump


############################################################################
###PREPARE DATA
############################################################################
dfdata=df.DataFrame()
x_vars =df.GetNormTruthVars()
n_xvars= x_vars.size()
y_vars=df.GetNormDiffVars()
n_yvars=y_vars.size()

print('DTRes.py : will train with x variables ',x_vars)
print('DTRes.py : will train with y variables ',y_vars)

#only want accepted events in arrays
x_data = dfdata.Filter(df.GetAcceptCondition(1)).AsNumpy(x_vars);
x_array = np.vstack([x_data[xkey] for xkey in x_data.keys()]).T

y_data = dfdata.Filter(df.GetAcceptCondition(1)).AsNumpy(y_vars);
y_array = np.vstack([y_data[ykey] for ykey in y_data.keys()]).T

############################################################################
###TRAIN MODELS AND SAVE TO DISC
############################################################################

def addRandomInputs(X_orig):
    nbSamples=len(X_orig)
    X_new=X_orig
    for i in range(dtconf.n_rand_in):
        randomP = np.random.uniform(0,1,nbSamples).reshape(nbSamples,1) 
        X_new=np.hstack((randomP,X_new))
        
    return X_new


saveto = save_dir.GetName() + '/'

for i in range(dtconf.n_regs):
    model= DecisionTreeRegressor() 
    x_fit= addRandomInputs(x_array)
    print('shape of input ',x_fit.shape)
    print('shape of output ',y_array.shape)
    model.fit(x_fit,y_array)
    dump(model,saveto+dtconf.model_name+ str(i) +'.joblib')
  
    del x_fit
    del model
    print('DTRes.py : done training regressor',i)


del x_array
del y_array
del x_data
del y_data


    
