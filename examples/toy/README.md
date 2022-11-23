# Toy Parameterised Simulation

## To generate training events

      root ToySimulation.C

Generates file : toy_training.root

Note, to exit root session, type .q and return.

## To generate reaction data

      root Reaction.C

Generates file : toy_reaction.root

## To train for a particular particle,

First we run an overall configure script for the training data. 

      macparticles Configure.C

Now we can run acceptance training for each particle. 

First argument pi+ => how this training is linked to a particualr particle when simulating.

Second argument "training.root", the output file created in Configure.C to control the training data.

Third argument "fast_sim" the output directory, should be somewhere safe for large files. Subdirectories will be created here for each training method and within those each particle training type.

Note, acceptance training diagnostic plots will go to fast_sim/acceptance/pi+/acceptance_plots/

      macparticles 'RunAcceptanceTraining.C( "pi+","training.root","fast_sim" )'

This is a slightly speeded up configuration, for the publication the following
parameters were set,

	   accKeras.SetMaxEpochs(100);
	   accKeras.SetLearnRate(1E-3);
	   accKeras.SetNetwork({1024,512, 256,128, 64,32,16});

More accurate acceptances can be acheived through reweighting with a BDT

      macparticles 'RunReweightTraining.C( "pi+","training.root","fast_sim" )'

and we can then train resolutions

      macparticles 'RunResolutionTraining.C( "pi+","training.root","fast_sim" )'

By default it is set to train 5 regressors, to smooth the distribution further,
this can be increased to 20, although the training time and disk space
increases proportionaly.

If you are happy with the results you can go on and train the pi- and proton.
If you think the acceptances are not so great you can try tuning the DNN
via the RunAcceptanceTraining.C script. Similar for the other two.

## To run simulation of reactions gp-> pi+ pi- p'

Now we have to run for each particle, which have different branches in the
reaction tree

	 macparticles 'RunSimulation.C( "pip","pi+","fast_sim/","results10" )'
	 macparticles 'RunSimulation.C( "pim","pi-","fast_sim/","results10" )'
	 macparticles 'RunSimulation.C( "p","proton","fast_sim/","results10" )'

## To plot the results

     	 root Reconstruct_Reaction_Mac.C