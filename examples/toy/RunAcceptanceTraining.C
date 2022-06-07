{
  auto pdg = GetPID();
  
  DataLoader  dl("simtree", "toy_training.root");
  dl.Range(0,1000000); //select number of events to train with < N in tree

  //Give branch names of variables to generate acceptance against
  //ranges used for normalisation
  dl.DetailedTruthVars({{"truP",0,10},
			{"truTheta",0,TMath::Pi()},
			{"truPhi",-TMath::Pi(),TMath::Pi()}});

  //Give the name for acceptance flag branch in the event data
  //This should be 0 or 1 for detected or not
  dl.SetAcceptVar("accepted");

  //Give toplevel configuration directory
  ConfigureSimulation config;
  config.Load("fast_simulation/");
  //give pdg name for particle we are training
  config.AddPdg(pdg.Data());
  //Apply Gaussian transform to variables (recommended)
  config.acc_gaustrans = true;
 

  //train a keras DNN on the training data acceptance
  KerasAcceptanceModel accKeras(config,ProcessType::Acceptance);
  accKeras.SetMaxEpochs(100);
  accKeras.SetLearnRate(1E-4);
  // accKeras.SetNetwork({2});
  accKeras.Train(dl);

  //Use a BDT to fine tune the DNN response (recommended but not required)
  BDTAcceptanceModel rewBdt(config,ProcessType::ReWeight);
  rewBdt.SetNEstimators(100);
  rewBdt.Train(dl);

  //Save configuration for this step
  config.Save();


  //And some plotting
  gBenchmark->Start("plotting");
  AcceptancePlotter(dl).PlotTruthVars();
  AcceptancePlotter(dl).PlotSliceVars("truTheta",1);
  AcceptancePlotter(dl).PlotSliceVars("truPhi",1);
  AcceptancePlotter(dl).PlotSliceVars("truP",1);
  gBenchmark->Stop("plotting");
  gBenchmark->Print("plotting");

}
