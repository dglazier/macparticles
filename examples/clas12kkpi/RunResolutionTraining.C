void RunResolutionTraining(string particle,string filename,string simdir){
 
  auto dload  = TrainingInfo(particle,filename).TrainingData();

  
  //Give toplevel configuration directory
  ConfigureSimulation config;
  config.Load(simdir);
  //give pdg name for particle we are training
  config.UsePid(particle);

  DecisionTreeResolModel res(config);
  res.SetNRegressors(5);
  res.SetNRandInputs(4);
  //res.SetApplyExtraScaling();
  res.Train(dload.get());

  ResolutionPlotter(*dload.get(),config).PlotTraining();

 }
