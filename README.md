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
perform the machine learning parts using [tensorflow](https://www.tensorflow.org/), [keras](https://keras.io/) and [scikit-learn](https://scikit-learn.org/stable/).

Additional training algorithms may be added to replace parts of the simulation.

Requirements :

  CERN ROOT (6.24)

  python (3.8.10)

  keras (2.8.0)

  scikit-learn (1.0.2)

  tensorflow (2.8.0)
 
* parentheses indicate the versions tested with.

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

Training Sample
===============

The training sample can be set up using either single particles or final states composed of several particles. These come with advantages and drawbacks.

Training with single particles reduces the number of dimensions involved in the training. This will typically lead to better reproducing the multi-dimensional correlations that exist between the momentum and direction of a particle, and non-trivial dependencies of the resolution in given variables on other training variables.

Training with multiple final state particles reduces the number of trained models (ie N models for one final state instead of NxM models for M final state particles), which in turn reduces training and prediction times. Multi-particle effects (eg overlapping tracks) also affect the acceptance modelling and need to be taken into account, these are automically included when training on final states. The larger phase space that comes from added variables will also help produce smoother resolution distributions, although this can easily be achieved by adding random inputs.

As such, it is recommended to first train on single particles, and adding a correction for multi-particle effects, to achieve the best fidelity in reproducing the training data. However, training with multiple particles will still achieve good fidelity. This route can be preferred for eg toy simulation studies where the simpler set-up and shorter training and prediction times are desirable over optimal fidelity.

The toy example works on a single particle basis. The dvcs example is trained using all DVCS final state particles (electron, proton, photon). The clas12kkpi example is a combination of both, first training using single particles before accounting for multi-particle effects with a correction to the acceptance derived using all final state particles.

