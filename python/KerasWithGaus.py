import ROOT
import gc
import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from scipy.stats import chisquare
from keras import callbacks
from sklearn.preprocessing import QuantileTransformer
from joblib import load, dump

print('Keras.py  AcceptanceTraining')

############################################################################
###PREPARE DATA
############################################################################

dfdata=df.DataFrame()
vars = df.GetTruthVars()
nvars= vars.size()
print('Keras.py : will train with variables ',vars)

#use extra variable flag to ID generated(=0) and accepted(=1) datasets
vars.push_back("flag")

#create dataframes for generated and accepted datasets
accdf = dfdata.Filter(df.GetAcceptCondition(1)).Define('flag','1').AsNumpy(vars);
alldf= dfdata.Define('flag','0').AsNumpy(vars);

#extract numpy arrays from dictionary and combine accepted and generated into 1 dataset
acc_array = np.vstack([accdf[xkey] for xkey in accdf.keys()])
all_array = np.vstack([alldf[xkey] for xkey in alldf.keys()])
data_array = np.hstack((acc_array,all_array)).T

##########################################################################
###GAUSSIAN TRANSFORM TRAINING VARIABLES
##########################################################################
saveto = save_dir.GetName() + '/'

if(do_gauss.GetName()=="TRUE"):
    gauss_transform = QuantileTransformer(output_distribution="normal",copy=False)
    #data_array[:,:nvars] => all rows and comumns 0 -> nvars-1 i.e. not 'flag'
    #gauss_array = gauss_transform.fit_transform(data_array[:,:nvars].copy())
    #replace data_array with new values
    gauss_transform.fit_transform(data_array[:,:nvars])
    #Save Gaussian transformer.
    dump(gauss_transform,saveto+'gaussscaler.joblib')
    print('Keras.py : saved gaussian transform to ',saveto+'gaussscaler.joblib')

##########################################################################
###CONFIGURE MODEL
##########################################################################
dnn_name = kerasconf.dnn_name
print('Keras.py : dnn name ', dnn_name)
n_epochs = kerasconf.max_epochs
print('Keras.py : max number of epochs ', n_epochs)
lr = kerasconf.learn_rate
print('Keras.py : learning rate ', lr)
n_batch = kerasconf.batch_size
print('Keras.py : batch size ', n_batch)
sizes = kerasconf.layout
print('Keras.py : network layout ', sizes)
act_function = kerasconf.activation
print('Keras.py : activation type  ', act_function)

#Create, train, and save Keras model.
#The hidden layers are fully connected layers with sizes and an activation function defined in the parameters.
layer_list = [layers.Dense(size, activation=act_function) for size in sizes]
layer_list.append(layers.Dense(1, activation="sigmoid"))
dnn = keras.Sequential(layer_list)

##########################################################################
###TRAIN MODEL
##########################################################################

dnn.compile(
    optimizer=keras.optimizers.Adam(learning_rate=lr),
    loss=keras.losses.BinaryCrossentropy(from_logits=False),
    metrics=[keras.metrics.BinaryCrossentropy()]
)

earlystopping = callbacks.EarlyStopping(monitor ="loss", 
                                        mode ="min", patience = 5, 
                                        restore_best_weights = True)

progress = callbacks.ModelCheckpoint(filepath=saveto +"check", verbose=1)

#data_array[:,:nvars] => train with gaussian transform vars, split on data_array[:,nvars]='flag'
history = dnn.fit(data_array[:,:nvars], data_array[:,nvars], 
                  batch_size = n_batch,
                  epochs = n_epochs,
                  callbacks = [earlystopping,progress]
)

#training done
print('Keras.py : save dnn model to ',saveto + dnn_name )
dnn.save(saveto + dnn_name )

##########################################################################
###CALCULATE NEW ACCEPTANCES
##########################################################################

#calculate acceptance per event
probs = dnn.predict(data_array[:,:nvars][data_array[:,nvars]==0])
weights_acc = probs/(1-probs)


def rejection_sample(weights, nev):
    rands = np.random.uniform(0, 1, nev)
    mask = rands[:] < weights[:]
    return mask.astype(int)

#fast_acc = rejection_sample(weights_acc[:,0], data_array[data_array[:,nvars]==0].shape[0])
fast_acc = rejection_sample(weights_acc[:,0], all_array.T.shape[0])

#save to root file via rdataframe
acc_rdf = ROOT.RDF.MakeNumpyDataFrame({"fast_accept":fast_acc,"fast_weight":weights_acc})
acc_rdf.Snapshot(dnn_name, saveto+"training_acceptances.root")

#To run another script must make sure all new objects have been deleted
del acc_rdf
del fast_acc
del probs
del weights_acc
del acc_array
del all_array
del data_array
del accdf
del alldf
del dfdata
del df
del dnn

tf.keras.backend.clear_session()
