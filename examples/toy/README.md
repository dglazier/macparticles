# Toy Parameterised Simulation

## To generate training events

      root ToySimulation.C

Generates file : toy_training.root

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

More accurate acceptances can be acheived through reweighting with a BDT

      macparticles 'RunReweightTraining.C( "pi+","training.root","fast_sim" )'

and we can then train resolutions

      macparticles 'RunResolutionTraining.C( "pi+","training.root","fast_sim" )'


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