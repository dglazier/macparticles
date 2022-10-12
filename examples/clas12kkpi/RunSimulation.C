void RunSimulation(string particle,string simparticle,string simdir,string outdir){
  
  auto info  = TrainingInfo(particle,simdir+"training.root");

  ConfigureSimulation config;
  config.Load(simdir);
  //output directory for simulated data
  config.Simulate(outdir);
  config.SetParticleName(particle);
  config.UsePid(simparticle);

  //configure the data loader for requested particle
  DataLoader  dl("FINALOUTTREE", "/scratch/dglazier/fast_sim_gen_dev//FinalState.root");
 
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

  ResolutionPlotter(dl,config).PlotSimulation();

   
}
