//macparticles 'RunReWeightTraining.C("all","training.root","training_output/")'
void RunReWeightTraining(string particle,string filename,string simdir){
 
  auto dload  = TrainingInfo(particle,filename).TrainingData();

  
  //Give toplevel configuration directory
  ConfigureSimulation config;
  config.Load(simdir.data());
  //give pdg name for particle we are training
  config.UsePid(particle);

 //Use a BDT to fine tune the DNN response (recommended but not required)
  BDTAcceptanceModel rewBdt(config,ProcessType::ReWeight);
  rewBdt.SetFastAcceptanceWeights(dload.get());

  rewBdt.SetNEstimators(100);
  rewBdt.SetLearnRate(0.1);
  rewBdt.SetMinImpurityDecrease(1);
  rewBdt.Train(dload.get());
  

  //And some plotting
  AcceptancePlotter(*dload.get(),config,2000,1200).PlotTruthVars();
  AcceptancePlotter(*dload.get(),config,2000,1200).PlotSliceVars(0,1);
  AcceptancePlotter(*dload.get(),config,2000,1200).PlotSliceVars(1,1);
  AcceptancePlotter(*dload.get(),config,2000,1200).PlotSliceVars(2,1);

}
