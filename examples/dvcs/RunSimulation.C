//macparticles 'RunSimulation.C("all","all","","training_output/","pred_output/")'
void RunSimulation(string particle,string simparticle,string simdir,string simoutdir,string outdir){
  
  auto info  = TrainingInfo(particle,simdir+"training.root");

  ConfigureSimulation config;
  config.Load(simoutdir);
  //output directory for simulated data
  config.Simulate(outdir);
  config.SetParticleName(particle);
  config.UsePid(simparticle);

  //configure the data loader for requested particle
  DataLoader  dl("tree", "all_events_test.root");
 
  //if variables in new tree are different from original simulated data
  // auto varPrefix = std::string("nkkpi")+particle;
  // accepted name, generated name, reconstructed name
  // dl.ConfigVars({{"",varPrefix+"_P",varPrefix+"_P","Momentum",0,7},
  // 	{"",varPrefix+"_Theta",varPrefix+"_Theta","#theta",0,1.0},
  // 	  {"",varPrefix+"_Phi",varPrefix+"_Phi","#phi",-TMath::Pi(),TMath::Pi()}});
  
  //if variables are just the same as the original simulation
  dl.SimVars(info.variables);

  dl.SetFractionToProcess(1);

  //and run fast simulation
  SimWithKerasAccDTRes(config,dl);

  vector<Double_t> diffRanges={0.1,0.005,0.05,0.1,0.08,0.03,0.5,0.003,0.03};
  ResolutionPlotter(dl,config,1700,1500,diffRanges).PlotSimulation();

   
}
