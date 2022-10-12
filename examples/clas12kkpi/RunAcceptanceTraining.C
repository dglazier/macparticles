//macparticles 'RunAcceptanceTraining.C("Km","/home/dglazier/fast_sim/fast_nkkpi/training.root","/home/dglazier/fast_sim/fast_nkkpi/")'
void RunAcceptanceTraining(string particle,string filename,string simdir){
 
  auto dload  = TrainingInfo(particle,filename).TrainingData();


  
  //Give toplevel configuration directory
  ConfigureSimulation config;
  config.Load(simdir);
  //give pdg name for particle we are training
  config.AddPid(particle);
  //Apply Gaussian transform to variables (recommended)
  config.acc_gaustrans = true;
 

  //train a keras DNN on the training data acceptance
  KerasAcceptanceModel accKeras(config,ProcessType::Acceptance);
  accKeras.SetMaxEpochs(100);
  accKeras.SetLearnRate(1E-3);
  accKeras.SetNetwork({1024, 1024, 512, 256,128, 64,32,16});
  accKeras.Train(dload.get());
  
 

  //And some plotting
  AcceptancePlotter(*dload.get(),config).PlotTruthVars();
  // AcceptancePlotter(*dload.get(),config).PlotSliceVars(0,1);
  // AcceptancePlotter(*dload.get(),config).PlotSliceVars(1,1);
  // AcceptancePlotter(*dload.get(),config).PlotSliceVars(2,1);
 
}
