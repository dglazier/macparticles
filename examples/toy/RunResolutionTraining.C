void RunResolutionTraining(string particle,string filename,string simdir){
 
  auto dload  = TrainingInfo(particle,filename).TrainingData();

  
  //Give toplevel configuration directory
  ConfigureSimulation config;
  config.Load(simdir);
  //give pdg name for particle we are training
  config.UsePid(particle);

  ///////////////////////////////	
  ///////////////////////////////
  ///Here can configure the regressors
  DecisionTreeResolModel res(config);
  res.SetNRegressors(5);
  res.SetNRandInputs(5);
  res.Train(dload.get());

  ResolutionPlotter(*dload.get(),config).PlotTraining();

 }
