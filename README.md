# macparticles
Machine Learned Particle Simulations
====================================

**Authors :** Richard Tyson, Derek Glazier, Darius Darulis;

**Affliation :** Nuclear and Hadron Physics, School of Physics and Astronomy, University of Glasgow

**Aim:** Produce very fast simulation results via Neural Networks and Decision Trees.

Users supply their own truth and reconstructed events from full simulations
to train the machine learning algorithms to produce replicable outputs as
a fast alternative for full simulations.

This interface is based around [CERN ROOT](https://root.cern/) libraries and interactive sessions, in particular : the [TPython](https://root.cern/doc/master/classTPython.html) class for running training and predictions in the python ecosystem; the [RDataFrame](https://root.cern/doc/master/classROOT_1_1RDataFrame.html) to allow seamless transfer of data between ROOT and Python sessions, while providing fast filtering and plotting.

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


Examples :

see examples directory.

Overview
========

To simplify and coordinate training of different particles using different data structures a TrainingInfo object
can be created. All input files are assumed to be ROOT (or at least readable by RDataFrame).
Two types of training data are considered

    1) File contains generated and reconstructed events, with a flag to specify whether a particular
    event had a particular particle successfully reconstructed.

    2) We have 2 files, one with just generated events and one with just reconstructed events.

If you use the TrainingInfo class it will work out which data type you have depending on whether you specify a
generated file or not. If you do not use a generated file, you must have a variable flag for whether a particle 
or event was accepted.
i.e. in Confiure macro need either  info.acceptVar = "accepted"; or info.genFile =  "/scratch/dglazier/fast_sim_gen_dev/FinalState.root";

Example code for TrainingInfo :

	//create trained model for particle pi+
        auto info = TrainingInfo{"pi+"};

	//training tree name and file
  	info.treeName = "simtree";
	info.reconFile = "toy_training.root";
	
	//what fraction of events in each file do you want to use?
	info.recFrac = 1;

	//define different varibale types for training. Running simulation can be different
	//truth name, generated name, reconstructed name, title, range
	info.variables ={{"truP","truP","recP","#pi+ momentum",0,10},
	{"truTheta","truTheta","recTheta","#pi+ #theta",0,TMath::Pi()},
	{"truPhi","truPhi","recPhi","#pi+ #phi",-TMath::Pi(),TMath::Pi()}};

	//variable to flag if particle was reconstructed in this event
	info.acceptVar = "accepted";

	//in case need further fltering of reconstructed events (e.g. truth matching,..)
	info.recFilter = "";

	//save in training.root file. This argument can then be given to specific training scripts
	info.WriteTo("training.root");



