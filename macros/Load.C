{
  gROOT->SetMacroPath(TString(gSystem->Getenv("MACP"))+"/core:"+gROOT->GetMacroPath());
  gROOT->SetMacroPath(TString(gSystem->Getenv("MACP"))+"/python:"+gROOT->GetMacroPath());

  gROOT->ProcessLine(".L ConfigureSimulation.h+");
  gROOT->ProcessLine(".L DataLoader.h+");
  gROOT->ProcessLine(".L AcceptanceModel.h+");
  gROOT->ProcessLine(".L PyAcceptanceModel.h+");
  gROOT->ProcessLine(".L KerasAcceptanceModel.h+");
  gROOT->ProcessLine(".L BDTAcceptanceModel.h+");
  gROOT->ProcessLine(".L AcceptancePlotter.h+");
  
  gROOT->ProcessLine(".L ResolutionModel.h+");
  gROOT->ProcessLine(".L DecisionTreeResolModel.h+");
 
  gROOT->ProcessLine(".L AcceptanceSim.h+");
  gROOT->ProcessLine(".L PyAcceptanceSim.h+");
  gROOT->ProcessLine(".L ResolutionSim.h+");
  gROOT->ProcessLine(".L DTResolutionSim.h+");

  gROOT->ProcessLine(".L SimulationFrameworks.h+");
}

TString GetPID(){
  //Allow command line options for pdg code
  auto argc = gApplication->Argc();
  auto argv = gApplication->Argv();
  TString pdg = "proton";
  for(int i=1;i<argc;i++){
    auto opt = TString(argv[i]);
    if(opt.Contains("pid")){
      pdg = opt;
      pdg.ReplaceAll("pid=","");
    }
  }
  cout<<"Doing training for "<<pdg<<endl;
  return pdg;
}
