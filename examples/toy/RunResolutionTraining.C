{
  auto pdg = GetPID();
 
  DataLoader  dl("simtree", "toy_training.root");
  dl.Range(0,1E6);
  dl.DetailedTruthVars({{"truP",0,10},
			{"truTheta",0,TMath::Pi()},
			{"truPhi",-TMath::Pi(),TMath::Pi()}});
  dl.SetReconVars({"recP","recTheta","recPhi"});
  dl.SetAcceptVar("accepted");
  
  ConfigureSimulation config;
  // config.Load("/home/dglazier/fast_sim/test");
  config.Load("fast_simulation/");
  config.AddPdg(pdg.Data());

  DecisionTreeResolModel res(config);
  res.SetNRegressors(10);
  res.SetNRandInputs(1);
  res.Train(dl);

  config.Save();//In case any changes

  dl.AddFriend("recon",config.ResolutionDir()+"predictions.root");
  dl.DefineColumn("diffP","truP-recon.recP");
  dl.DefineColumn("diffTheta","truTheta-recon.recTheta");
  dl.DefineColumn("diffPhi","truPhi-recon.recPhi");
  dl.DefineColumn("diffRealPhi","truPhi-recPhi");

  // auto accEvents = dl.DataFrame();
 auto accEvents = dl.GetAcceptedFrame();

  
  auto h1 = accEvents.Histo1D({"diffTheta","#theta",100,-0.3,0.3},"diffTheta");
  new TCanvas();
  h1->DrawCopy();
  auto h2 = accEvents.Histo1D({"Phi","#phi",100,-TMath::Pi(),TMath::Pi()},"recon.recPhi");
  new TCanvas();
  h2->DrawCopy();
  auto h3 = accEvents.Histo2D({"DPhiVTh","#phi v theta",100,0,TMath::Pi(),100,-0.5,0.5},"truTheta","diffPhi");
  new TCanvas();
  h3->DrawCopy();
  auto h4 = accEvents.Histo2D({"DPhiVTh","#phi v theta",100,0,TMath::Pi(),100,-0.5,0.5},"truTheta","diffRealPhi");
  new TCanvas();
  h4->DrawCopy();

  auto h5 = accEvents.Histo1D({"DPhir","#phi v theta",100,-0.5,0.5},"diffPhi");
  new TCanvas();
  h5->DrawCopy();
  auto h6 = accEvents.Histo1D({"DPhif","#phi v theta",100,-0.5,0.5},"diffRealPhi");
  h6->SetLineColor(2);
  h6->DrawCopy("same");

  
  /*
  auto h2 = accEvents.Histo2D({"diffP","diff P v P",100,-3,3,100,0,12},"truP_m_recP","truP");
  new TCanvas();
  h2->DrawCopy("col1");
  auto h3 = accEvents.Histo2D({"diffAngle","diff Th V Ph",100,-0.1,0.1,100,-0.3,0.3},"truTheta_m_recTheta","truPhi_m_recPhi");
  new TCanvas();
  h3->DrawCopy("col1");

  dl.DefineColumn("frecP","truP-truP_m_recP");
  dl.DefineColumn("frecTheta","truTheta-truTheta_m_recTheta");
  dl.DefineColumn("frecPhi","truPhi-truPhi_m_recPhi");

  auto recEvents= dl.DataFrame().Filter("accepted==1");
  auto hacc1 = recEvents.Histo1D({"recP","reconstructed P",100,-1,12},"recP");
  new TCanvas();
  hacc1->DrawCopy("");
  auto hrec1 = recEvents.Histo1D({"reconP","reconstructed P",100,-1,12},"frecP");
  hrec1->SetLineColor(2);
  hrec1->DrawCopy("same");

  */
 }
