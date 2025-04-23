# DVCS Example

Code to train and apply the macparticles framework to code produced using the [dvcs analysis code found here](https://github.com/sbhawani/clas12_analysis_software/blob/main/processing_scripts/processing_dvcs.groovy).

## Requirements

You will need CERN ROOT (version > 6.24). Set-up macparticles as described in the [main README](https://github.com/dglazier/macparticles/blob/main/README.md)

All required *python* libraries are detailed in env_requirements.txt. To install using venvs and pip in a new environment called newenvname:

      python3 -m venv /path/to/env/location/newenvname
      source /path/to/env/location/newenvname/bin/activate.csh
      pip install -r python_requirements.txt

***N.B.:*** You will have to modify the path and you should give a sensible name to the environment. When running the code, make sure the environment is activated using *source /path/to/env/location/newenvname/bin/activate.csh* .

## Running the code

First make two directories, training_output that will contain validation plots and training models and pred_output that will contain the output when applying the algorithms to unseen testing data and validation plots made from these. Use the commands:

      mkdir training_output
      mkdir -p pred_output/testing_plots

 Next we will want to create our training dataset from the output of the dvcs analysis code. You can do so with eg:

      python3 create_trainingData.py \
        --genPath "/path/to/training/data/Gen_data.root" \
        --recPath "/path/to/training/data/Recon_data.root" \
        --endName ""

The script will use the first 5M events to create the training data and write it to a ROOT tree called tree in a file called all_events.root. You can also create a training data set with:

      python3 create_trainingData.py \
        --genPath "/path/to/training/data/Gen_data.root" \
        --recPath "/path/to/training/data/Recon_data.root" \
        --createTest \
        --endName "_test"

The createTest argument will make the script write out the following 5M events. The endName argument will write the dataset to all_events_test.root.

Next we want to configure the macparticles training. This is done in the ConfigureAll.C script with:

      macparticles ConfigureALL.C

We then want to run the scripts to train the Acceptance model, the ReWeight model to correct mistakes in the acceptance model, and the Resolution model. This is done with the commands:

      macparticles 'RunAcceptanceTraining.C("all","training.root","training_output/")'
      macparticles 'RunReWeightTraining.C("all","training.root","training_output/")'
      macparticles 'RunResolutionTraining.C("all","training.root","training_output/")'

The arguments *all* and *training.root* refer to the training set-up defined in ConfigureAll.C, in this case training with all final state particles. The argument *training_output/* specifies where to write out validation plots and trained models. The longest training time is for the Acceptance training, which takes about an hour. The other two scripts take a few minutes.

Finally we want to apply the models to unseen data in the test file. This is done with the RunSimulation script:

      macparticles 'RunSimulation.C("all","all","","training_output/","pred_output/")'

The same arguments are used as before. The third, empty, argument refers to the path to the directory containing the *training.root* file. *pred_output* refers to where we want the output to be written. The written output will consist of some validation plots and the fast simulated data. At present, the simulated variables with resolution applied are written in a tree called recon in a the file pred_output/all/predictions.root, and the accepted flag contained in the variable accept_all is written to a tree called acceptance in the file pred_output/all/simulation_acceptances.root. You will therefore need to read both trees and use the indices where accept_all==1 along with the variables with resolution to get the full simulation (an example of this is shown in the make_plots script).

We can then plot the output of the fast simulation and compare it to the output of GEMC. This is done by running the make_plots script with

      python3 make_plots.py

The plots will be contained in pred_output/testing_plots. These include comparisons of the generated, GEMC and FastMC distributions of the training variables (ie, P, Theta, Phi for final state particles) and of exclusivity variables (Q2,t,x,W), along with the acceptance as a function of the exclusivity variables. The plots ending with *_ReconVars* are made without the generated distributions.


