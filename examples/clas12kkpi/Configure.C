{


  auto info = TrainingInfo{"Kp"};


  info.treeName = "FINALOUTTREE";
  info.reconFile = "/scratch/dglazier/fast_sim_data_devFT/FinalState.root";
  info.genFile =  "/scratch/dglazier/fast_sim_gen_dev/FinalState.root";
  
  //what fraction of events in each file do you want to use?
  info.recFrac = 0.3;
  info.genFrac = 0.3;

  //truth name, generated name, reconstructed name, title, range
  info.variables = {{"nkkpitruKp_P","nkkpiKp_P","nkkpiKp_P","Kaon Momentum",0,7},
	       {"nkkpitruKp_Theta","nkkpiKp_Theta","nkkpiKp_Theta","Kaon #theta",0.,1.0},
	       {"nkkpitruKp_Phi","nkkpiKp_Phi","nkkpiKp_Phi","Kaon #phi",-TMath::Pi(),TMath::Pi()}};


  //in case need further fltering of reconstructed events (e.g. truth matching,..)
  info.recFilter = "Truth==1&&nkkpiaccept_Kp==1&&Topo==4";

  // ConfigureSimulation config;
  info.saveDir = "/scratch/dglazier/fast_simulation/test/";
 

  info.WriteTo("/scratch/dglazier/fast_simulation/test/training.root");
  /*********************************************************************************************************/

  info = TrainingInfo{"Km"};
 
  info.treeName = "FINALOUTTREE";
  info.reconFile = "/scratch/dglazier/fast_sim_data_devFT/FinalState.root";
  info.genFile =  "/scratch/dglazier/fast_sim_gen_dev/FinalState.root";
  
  //what fraction of events in each file do you want to use?
  info.recFrac = 1;
  info.genFrac = 1;

  //truth name, generated name, reconstructed name, title, range
  info.variables = {{"nkkpitruKm_P","nkkpiKm_P","nkkpiKm_P","Kaon Momentum",0,7},
	       {"nkkpitruKm_Theta","nkkpiKm_Theta","nkkpiKm_Theta","Kaon #theta",0.,1.0},
	       {"nkkpitruKm_Phi","nkkpiKm_Phi","nkkpiKm_Phi","Kaon #phi",-TMath::Pi(),TMath::Pi()}};


  //in case need further fltering of reconstructed events (e.g. truth matching,..)
  info.recFilter = "Truth==1&&nkkpiaccept_Km==1&&Topo==5";

  // ConfigureSimulation config;
  info.saveDir = "/scratch/dglazier/fast_simulation/test/";
 

  info.WriteTo("/scratch/dglazier/fast_simulation/test/training.root");
 /*********************************************************************************************************/

  info = TrainingInfo{"Pip"};
 
  info.treeName = "FINALOUTTREE";
  info.reconFile = "/scratch/dglazier/fast_sim_data_devFT/FinalState.root";
  info.genFile =  "/scratch/dglazier/fast_sim_gen_dev/FinalState.root";
  
  //what fraction of events in each file do you want to use?
  info.recFrac = 0.25;
  info.genFrac = 0.25;

  //truth name, generated name, reconstructed name, title, range
  info.variables = {{"nkkpitruPip_P","nkkpiPip_P","nkkpiPip_P","Kaon Momentum",0,7},
	       {"nkkpitruPip_Theta","nkkpiPip_Theta","nkkpiPip_Theta","Kaon #theta",0.,1.0},
	       {"nkkpitruPip_Phi","nkkpiPip_Phi","nkkpiPip_Phi","Kaon #phi",-TMath::Pi(),TMath::Pi()}};


  //in case need further fltering of reconstructed events (e.g. truth matching,..)
  info.recFilter = "Truth==1&&nkkpiaccept_Pip==1&&Topo==3";

  // ConfigureSimulation config;
  info.saveDir = "/scratch/dglazier/fast_simulation/test/";
 

  info.WriteTo("/scratch/dglazier/fast_simulation/test/training.root");
 /*********************************************************************************************************/

  info = TrainingInfo{"El"};
 
  info.treeName = "FINALOUTTREE";
  info.reconFile = "/scratch/dglazier/fast_sim_data_devFT/FinalState.root";
  info.genFile =  "/scratch/dglazier/fast_sim_gen_dev/FinalState.root";
  
  //what fraction of events in each file do you want to use?
  info.recFrac = 0.15;
  info.genFrac = 0.15;

  //truth name, generated name, reconstructed name, title, range
  info.variables = {{"nkkpitruEl_P","nkkpiEl_P","nkkpiEl_P","Kaon Momentum",0,7},
	       {"nkkpitruEl_Theta","nkkpiEl_Theta","nkkpiEl_Theta","Kaon #theta",0,1.0},
	       {"nkkpitruEl_Phi","nkkpiEl_Phi","nkkpiEl_Phi","Kaon #phi",-TMath::Pi(),TMath::Pi()}};


  //in case need further fltering of reconstructed events (e.g. truth matching,..)
  info.recFilter = "Truth==1&&nkkpiaccept_e==1&&Topo==2";

  // ConfigureSimulation config;
  info.saveDir = "/scratch/dglazier/fast_simulation/test/";
 

  info.WriteTo("/scratch/dglazier/fast_simulation/test/training.root");


}
