# macparticles
Machine Learned Particle Simulations
====================================

**Authors :** Richard Tyson, Derek Glazier, Darius Darulis;

**Affliation :** Nuclear and Hadron Physics, School of Physics, University of Glasgow

**Aim:** Produce very fast simulation results via Neural Networks and Decision Trees.

Users supply their own truth and reconstructed events from full simulations
to train the machine learning algorithms to produce replicable outputs as
a fast alternative for full simulations.

This interface is based around [CERN ROOT](https://root.cern/) libraries and interactive sessions, in particular : the [TPython](https://root.cern/doc/master/classTPython.html) class for running training and predictions in the python ecosystem; the [RDataFrame](https://root.cern/doc/master/classROOT_1_1RDataFrame.html) to allow seamless transfer of data between ROOT and Python sessions, while prviding fast filtering and plotting.

We provide some C++ classes for automating the training and Python scripts to
perform the machine learning parts using [tensorflow](https://www.tensorflow.org/]/[keras](https://keras.io/) and [scikit-learn](https://scikit-learn.org/stable/).

Additional training algorithms may be added to replace parts of the simulation.

Requirements :

  CERN ROOT

  python

  keras

  scikitlearn

  tensorflow
 
Install :

      git clone https://github.com/dglazier/macparticles


Setup :

      setenv MACP /cloned/directory/macparticles
      alias macparticles root $MACP/macros/Load.C
      macparticles	 #this compiles all the classes