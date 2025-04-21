//macparticles 'RunResolutionTraining.C("all","training.root","training_output/")'
void RunResolutionTraining(string particle,string filename,string simdir){
 
  auto dload  = TrainingInfo(particle,filename).TrainingData();

  
  //Give toplevel configuration directory
  ConfigureSimulation config;
  config.Load(simdir);
  //give pdg name for particle we are training
  config.UsePid(particle);

  DecisionTreeResolModel res(config);
  res.SetNRegressors(50);
  res.SetNRandInputs(10);
  //res.SetApplyExtraScaling();
  res.AddFilter("accepted==1");
  res.Train(dload.get());

  vector<Double_t> diffRanges={0.1,0.005,0.05,0.1,0.08,0.03,0.5,0.003,0.03};
  ResolutionPlotter(*dload.get(),config,1500,1200,diffRanges).PlotTraining();

 }
