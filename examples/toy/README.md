*Toy Parameterised Simulation

**To generate training events

      root ToySimulation.C

Generates file : toy_training.root

**To generate reaction data

      root Reaction.C

Generates file : toy_reaction.root

**To train for a particular particle,

Here we assume all particles have the same acceptance

      macparticles RunAcceptanceTraining.C pid=all

and resolution

      macparticles RunAcceptanceTraining.C pid=all

You may prefer to set pid=pi- etc and run the training 3 times for
{"pi-","pi+","proton"}

**To run simulation of reactions gp-> pi+ pi- p'

Now we have to run for each particle, which have different branches in the
reaction tree

	 macparticles RunSimulation.C pid=pi+
	 macparticles RunSimulation.C pid=pi-
	 macparticles RunSimulation.C pid=proton


**To plot the results

     	 root Reconstruct_Reaction_Mac.C