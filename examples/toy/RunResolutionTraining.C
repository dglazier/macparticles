{
  auto pdg = GetPID(); //give pid on command line e.g. pid=proton
 
  DataLoader  dl("simtree", "toy_training.root");
  dl.Range(0,1.6E6); //gives approx 1M accepted
  dl.DetailedTruthVars({{"truP",0,10},
			{"truTheta",0,180},
			{"truPhi",-180,180}});
  dl.SetReconVars({"recP","recTheta","recPhi"});
  dl.SetAcceptVar("accepted");
  
  ConfigureSimulation config;
  config.Load("fast_simulation/");
  config.AddPdg(pdg.Data());

  DecisionTreeResolModel res(config);
  res.SetNRegressors(2);
  res.SetNRandInputs(1);
  res.Train(dl);

  config.Save();//In case any changes

  //make some branches for plotting
  dl.AddFriend("recon",config.ResolutionDir()+"predictions.root");
  dl.DefineColumn("diffP","truP-recon.recP");
  dl.DefineColumn("diffTheta","truTheta-recon.recTheta");
  dl.DefineColumn("diffPhi","truPhi-recon.recPhi");
  dl.DefineColumn("diffRealPhi","truPhi-recPhi");
  dl.DefineColumn("diffRealTheta","truTheta-recTheta");
 
  auto accEvents = dl.GetAcceptedFrame();

  //plot histrograms via RDataFrame
 auto h0 = accEvents.Histo2D({"DPvP","#DeltaP v P",100,0,10,100,-2,4},"truP","diffP");
  new TCanvas();
  h0->DrawCopy();

  auto h1 = accEvents.Histo1D({"diffTheta","#theta",100,-0.3,0.3},"diffTheta");
  new TCanvas();
  h1->DrawCopy();
  auto h2 = accEvents.Histo1D({"Phi","#phi",100,-TMath::Pi(),TMath::Pi()},"recon.recPhi");
  new TCanvas();
  h2->DrawCopy();

  auto h3 = accEvents.Histo2D({"DPhiVTh","#phi v theta",100,0,TMath::Pi(),100,-20*TMath::DegToRad(),20*TMath::DegToRad()},"truTheta","diffPhi");
  new TCanvas();
  h3->DrawCopy();
  
  auto h4 = accEvents.Histo2D({"DPhiVTh","#phi v theta",100,0,TMath::Pi(),100,-20*TMath::DegToRad(),20*TMath::DegToRad()},"truTheta","diffRealPhi");
  new TCanvas();
  h4->DrawCopy();

  auto h6 = accEvents.Histo1D({"DPhir","#phi v theta",100,-0.5,0.5},"diffRealPhi");
  new TCanvas();
  h6->SetLineColor(2);
  h6->DrawCopy("same");
  auto h5 = accEvents.Histo1D({"DPhif","#phi v theta",100,-0.5,0.5},"diffPhi");
  new TCanvas();
  h5->DrawCopy();

  auto h7 = accEvents.Histo2D({"DPhiDThr","D #phi v theta",100,-0.5,0.5,100,-0.2,0.2},"diffPhi","diffTheta");
  new TCanvas();
  h7->DrawCopy("col");
  auto h8 = accEvents.Histo2D({"DPhiDThf","D #phi v theta",100,-0.5,0.5,100,-0.2,0.2},"diffRealPhi","diffRealTheta");
  new TCanvas();
  h8->DrawCopy("col");

 
 }
