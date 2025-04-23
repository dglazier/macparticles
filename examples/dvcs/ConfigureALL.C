{


  auto info = TrainingInfo{"all"};


  info.treeName = "tree";
  info.reconFile = "all_events.root";
  
  //what fraction of events in each file do you want to use?
  info.recFrac = 1;

  //truth name, generated name, reconstructed name, title, range
  info.variables = {{"truel_p","truel_p","recel_p","Electron Momentum",0,10},
		    {"truel_theta","truel_theta","recel_theta","Electron #theta",0.,0.6},
		    {"truel_phi","truel_phi","recel_phi","Electron #phi",0,2*TMath::Pi()},
		    {"trup1_p","trup1_p","recp1_p","Proton Momentum",0,1.5},
		    {"trup1_theta","trup1_theta","recp1_theta","Proton #theta",0.,1.5},
		    {"trup1_phi","trup1_phi","recp1_phi","Proton #phi",0,2*TMath::Pi()},
		    {"trup2_p","trup2_p","recp2_p","Photon Momentum",0,10},
		    {"trup2_theta","trup2_theta","recp2_theta","Photon #theta",0.,0.5},
		    {"trup2_phi","trup2_phi","recp2_phi","Photon #phi",0,2*TMath::Pi()}};


  //variable to flag if particle was reconstructed in this event
  info.acceptVar = "accepted";

  //in case need further fltering of reconstructed events (e.g. truth matching,..)
  info.recFilter = "";

  // ConfigureSimulation config;
  info.saveDir = "";
 

  info.WriteTo("training.root");
  /*********************************************************************************************************/

}
