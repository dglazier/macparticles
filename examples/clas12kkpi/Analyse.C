using namespace ROOT;
using namespace ROOT::RDF;



void Analyse(const string& treename="",const string& filename="",const string& treename0="",const string& filename0=""){
  gBenchmark->Start("analyse");
  auto MK = 0.49367700;
  auto MPI = 0.13957000;
  auto ME = 0.00051099891;
  float m1 = MK;
  float m2 = MK;

  auto CompInvMass= [m1,m2](double pmag1, double theta1, double phi1,double pmag2, double theta2, double phi2){
    auto px = pmag1*sin(theta1)*cos(phi1);
    auto py = pmag1*sin(theta1)*sin(phi1);
    auto pz = pmag1*cos(theta1);
    ROOT::Math::PxPyPzMVector p1(px, py, pz, m1);
    px = pmag2*sin(theta2)*cos(phi2);
    py = pmag2*sin(theta2)*sin(phi2);
    pz = pmag2*cos(theta2);
    ROOT::Math::PxPyPzMVector p2(px, py, pz, m2);
    return (p1 + p2).mass();
  };
  auto CompMissMass= [](double pmagKm, double thetaKm, double phiKm,
			     double pmagKp, double thetaKp, double phiKp,
			     double pmagPip, double thetaPip, double phiPip,
			double pmagEl, double thetaEl, double phiEl
			//			     double pmagN, double thetaN, double phiN
			     ){
    auto px = pmagKm*sin(thetaKm)*cos(phiKm);
    auto py = pmagKm*sin(thetaKm)*sin(phiKm);
    auto pz = pmagKm*cos(thetaKm);
    ROOT::Math::PxPyPzMVector km(px, py, pz, 0.49367700);
    px = pmagKp*sin(thetaKp)*cos(phiKp);
    py = pmagKp*sin(thetaKp)*sin(phiKp);
    pz = pmagKp*cos(thetaKp);
    ROOT::Math::PxPyPzMVector kp(px, py, pz, 0.49367700);
    px = pmagPip*sin(thetaPip)*cos(phiPip);
    py = pmagPip*sin(thetaPip)*sin(phiPip);
    pz = pmagPip*cos(thetaPip);
    ROOT::Math::PxPyPzMVector pip(px, py, pz, 0.13957000);
    px = pmagEl*sin(thetaEl)*cos(phiEl);
    py = pmagEl*sin(thetaEl)*sin(phiEl);
    pz = pmagEl*cos(thetaEl);
    ROOT::Math::PxPyPzMVector el(px, py, pz, 0.00051099891);
    // px = pmagN*sin(thetaN)*cos(phiN);
    // py = pmagN*sin(thetaN)*sin(phiN);
    // pz = pmagN*cos(thetaN);
    // ROOT::Math::PxPyPzMVector n(px, py, pz, 0.93956500);
    ROOT::Math::PxPyPzMVector pr(0,0,0,0.93827200);
    ROOT::Math::PxPyPzMVector elbeam(0,0,10.6,0.00051099891);


    return (elbeam + pr - el - pip - kp - km  ).mass();
  };

  auto df = RDataFrame(treename,filename);
 
  auto hisKm = df.Filter("accept_Km==1").Histo1D( TH1DModel{"KmP","Kmp",100,0,1},{"nkkpiKm_Theta"} );
  auto hisKp = df.Filter("accept_Kp==1").Histo1D( TH1DModel{"KpP","Kpp",100,0,1},{"nkkpiKp_Theta"} );
  auto hisPip = df.Filter("accept_Pip==1").Histo1D( TH1DModel{"PipP","Pip",100,0,1},{"nkkpiPip_Theta"} );
  auto hisEl = df.Filter("accept_El==1").Histo1D( TH1DModel{"ElP","Elp",100,0,0.1},{"nkkpiEl_Theta"} );
  
  auto dfex = df.Filter("accept_Km==1").Filter("accept_Kp==1").Filter("accept_El==1").Filter("accept_Pip==1");
  auto k2 =    dfex.Define("M2K", CompInvMass, {"nkkpiKm_P","nkkpiKm_Theta","nkkpiKm_Phi","nkkpiKp_P","nkkpiKp_Theta","nkkpiKp_Phi"});
  auto miss=    dfex.Define("Mmiss", CompMissMass, {"nkkpiKm_P","nkkpiKm_Theta","nkkpiKm_Phi","nkkpiKp_P","nkkpiKp_Theta","nkkpiKp_Phi","nkkpiPip_P","nkkpiPip_Theta","nkkpiPip_Phi","nkkpiEl_P","nkkpiEl_Theta","nkkpiEl_Phi"});

  auto hisk2 = k2.Histo1D( TH1DModel{"Mkk","Invariant mass 2K",100,0.8,3},{"M2K"} );
  auto hismiss	 = miss.Histo1D( TH1DModel{"Mmiss","Missing mass",200,-1,3},{"Mmiss"} );
  
  auto df0 = RDataFrame(treename0,filename0);

  auto hisKm0 = df0.Filter("Truth==1&&nkkpiaccept_Km==1&&Topo==5").Histo1D( TH1DModel{"KmP","Kmp",100,0,1},{"nkkpiKm_Theta"} );
  auto hisKp0 = df0.Filter("Truth==1&&nkkpiaccept_Kp==1&&Topo==4").Histo1D( TH1DModel{"KpP","Kpp",100,0,1},{"nkkpiKp_Theta"} );
  auto hisPip0 = df0.Filter("Truth==1&&nkkpiaccept_Pip==1&&Topo==3").Histo1D( TH1DModel{"PipP","Pip",100,0,1},{"nkkpiPip_Theta"} );
  auto hisEl0 = df0.Filter("Truth==1&&nkkpiaccept_e==1&&Topo==2").Histo1D( TH1DModel{"ElP","Elp",100,0,0.1},{"nkkpiEl_Theta"} );
  
   auto dfex0 = df0.Filter("Truth==1&&Topo==6").Filter("nkkpiaccept_e==1").Filter("nkkpiaccept_Km==1").Filter("nkkpiaccept_Kp==1").Filter("nkkpiaccept_Pip==1");
  auto k20 =    dfex0.Define("M2K", CompInvMass, {"nkkpiKm_P","nkkpiKm_Theta","nkkpiKm_Phi","nkkpiKp_P","nkkpiKp_Theta","nkkpiKp_Phi"});
  auto miss0=    dfex0.Define("Mmiss", CompMissMass, {"nkkpiKm_P","nkkpiKm_Theta","nkkpiKm_Phi","nkkpiKp_P","nkkpiKp_Theta","nkkpiKp_Phi","nkkpiPip_P","nkkpiPip_Theta","nkkpiPip_Phi","nkkpiEl_P","nkkpiEl_Theta","nkkpiEl_Phi"});

  auto hisk20 = k20.Histo1D( TH1DModel{"Mkk","Invariant mass 2K",100,0.8,3},{"M2K"} );
  auto hismiss0	 = miss0.Histo1D( TH1DModel{"Mmiss","Missing mass",200,-1,3},{"Mmiss"} );
  
  //////////////////////////////////////////
  auto can1 = new TCanvas();
  can1->Divide(2,2);
  can1->cd(1);
  hisKm0->DrawCopy();
  can1->cd(2);
  hisKp0->DrawCopy();
  can1->cd(3);
  hisPip0->DrawCopy();
  can1->cd(4);
  hisEl0->DrawCopy();


  auto can2 = new TCanvas();
  can2->Divide(2,1);
  can2->cd(1);
  hisk20->DrawCopy();
  can2->cd(2);
  hismiss0->DrawCopy();

  /////////////////////////////////////////
  can1->cd(1);
  hisKm->SetLineColor(2);
  hisKm->DrawCopy("same");
  can1->cd(2);
  hisKp->SetLineColor(2);
  hisKp->DrawCopy("same");
  can1->cd(3);
  hisPip->SetLineColor(2);
  hisPip->DrawCopy("same");
  can1->cd(4);
  hisEl->SetLineColor(2);
  hisEl->DrawCopy("same");


  can2->cd(1);
  hisk2->SetLineColor(2);
  hisk2->DrawCopy("same");
  can2->cd(2);
  hismiss->SetLineColor(2);
  hismiss->DrawCopy("same");


 
  gBenchmark->Stop("analyse");
  gBenchmark->Print("analyse");

}
void AnalyseA(Bool_t isFast=true,const string& treename="",const string& filename=""){
  gBenchmark->Start("analyse");
  auto MK = 0.49367700;
  auto MPI = 0.13957000;
  auto ME = 0.00051099891;

  auto df = RDataFrame(treename,filename);
  // auto df = df0.Range(0,3E6);

  auto hisKm = df.Filter("accept_Km==1").Histo1D( TH1DModel{"KmP","Kmp",100,0,1},{"nkkpiKm_Theta"} );
  auto hisKp = df.Filter("accept_Kp==1").Histo1D( TH1DModel{"KpP","Kpp",100,0,1},{"nkkpiKp_Theta"} );
  auto hisPip = df.Filter("accept_Pip==1").Histo1D( TH1DModel{"PipP","Pip",100,0,1},{"nkkpiPip_Theta"} );
  auto hisEl = df.Filter("accept_El==1").Histo1D( TH1DModel{"ElP","Elp",100,0,0.1},{"nkkpiEl_Theta"} );
  
  // auto hisKm = df.Filter("accept_all==1").Histo1D( TH1DModel{"KmP","Kmp",100,0,1},{"nkkpiKm_Theta"} );
  // auto hisKp = df.Filter("accept_all==1").Histo1D( TH1DModel{"KpP","Kpp",100,0,1},{"nkkpiKp_Theta"} );
  // auto hisPip = df.Filter("accept_all==1").Histo1D( TH1DModel{"PipP","Pip",100,0,1},{"nkkpiPip_Theta"} );
  // auto hisEl = df.Filter("accept_all==1").Histo1D( TH1DModel{"ElP","Elp",100,0,0.1},{"nkkpiEl_Theta"} );
  
  // auto hisKm = df.Filter("Truth==1&&nkkpiaccept_Km==1&&Topo==5").Histo1D( TH1DModel{"KmP","Kmp",100,0,1},{"nkkpiKm_Theta"} );
  // auto hisKp = df.Filter("Truth==1&&nkkpiaccept_Kp==1&&Topo==4").Histo1D( TH1DModel{"KpP","Kpp",100,0,1},{"nkkpiKp_Theta"} );
  // auto hisPip = df.Filter("Truth==1&&nkkpiaccept_Pip==1&&Topo==3").Histo1D( TH1DModel{"PipP","Pip",100,0,1},{"nkkpiPip_Theta"} );
  // auto hisEl = df.Filter("Truth==1&&nkkpiaccept_e==1&&Topo==2").Histo1D( TH1DModel{"ElP","Elp",100,0,0.1},{"nkkpiEl_Theta"} );
  
  // auto hisKm = df.Filter("Truth==0").Histo1D( TH1DModel{"KmP","Kmp",100,0,1},{"nkkpiKm_Theta"} );
  // auto hisKp = df.Filter("Truth==0").Histo1D( TH1DModel{"KpP","Kpp",100,0,1},{"nkkpiKp_Theta"} );
  // auto hisPip = df.Filter("Truth==0").Histo1D( TH1DModel{"PipP","Pip",100,0,1},{"nkkpiPip_Theta"} );
  // auto hisEl = df.Filter("Truth==0").Histo1D( TH1DModel{"ElP","Elp",100,0,0.1},{"nkkpiEl_Theta"} );
  // auto his = df.Filter("accept_Km==1").Histo1D( TH1DModel{"KmP","Kmp",100,0,1},{"nkkpiKm_Theta"} );

 
  float m1 = MK;
  float m2 = MK;
  auto CompInvMass= [m1,m2](double pmag1, double theta1, double phi1,double pmag2, double theta2, double phi2){
    auto px = pmag1*sin(theta1)*cos(phi1);
    auto py = pmag1*sin(theta1)*sin(phi1);
    auto pz = pmag1*cos(theta1);
    ROOT::Math::PxPyPzMVector p1(px, py, pz, m1);
    px = pmag2*sin(theta2)*cos(phi2);
    py = pmag2*sin(theta2)*sin(phi2);
    pz = pmag2*cos(theta2);
    ROOT::Math::PxPyPzMVector p2(px, py, pz, m2);
    return (p1 + p2).mass();
  };
  auto CompMissMass= [](double pmagKm, double thetaKm, double phiKm,
			     double pmagKp, double thetaKp, double phiKp,
			     double pmagPip, double thetaPip, double phiPip,
			double pmagEl, double thetaEl, double phiEl
			//			     double pmagN, double thetaN, double phiN
			     ){
    auto px = pmagKm*sin(thetaKm)*cos(phiKm);
    auto py = pmagKm*sin(thetaKm)*sin(phiKm);
    auto pz = pmagKm*cos(thetaKm);
    ROOT::Math::PxPyPzMVector km(px, py, pz, 0.49367700);
    px = pmagKp*sin(thetaKp)*cos(phiKp);
    py = pmagKp*sin(thetaKp)*sin(phiKp);
    pz = pmagKp*cos(thetaKp);
    ROOT::Math::PxPyPzMVector kp(px, py, pz, 0.49367700);
    px = pmagPip*sin(thetaPip)*cos(phiPip);
    py = pmagPip*sin(thetaPip)*sin(phiPip);
    pz = pmagPip*cos(thetaPip);
    ROOT::Math::PxPyPzMVector pip(px, py, pz, 0.13957000);
    px = pmagEl*sin(thetaEl)*cos(phiEl);
    py = pmagEl*sin(thetaEl)*sin(phiEl);
    pz = pmagEl*cos(thetaEl);
    ROOT::Math::PxPyPzMVector el(px, py, pz, 0.00051099891);
    // px = pmagN*sin(thetaN)*cos(phiN);
    // py = pmagN*sin(thetaN)*sin(phiN);
    // pz = pmagN*cos(thetaN);
    // ROOT::Math::PxPyPzMVector n(px, py, pz, 0.93956500);
    ROOT::Math::PxPyPzMVector pr(0,0,0,0.93827200);
    ROOT::Math::PxPyPzMVector elbeam(0,0,10.6,0.00051099891);


    return (elbeam + pr - el - pip - kp - km  ).mass();
  };

  auto dfex = df.Filter("accept_Km==1").Filter("accept_Kp==1").Filter("accept_El==1").Filter("accept_Pip==1");
  //auto dfex = df.Filter("accept_all==1");
  // auto dfex = df.Filter("Truth==1&&Topo==6").Filter("nkkpiaccept_e==1").Filter("nkkpiaccept_Km==1").Filter("nkkpiaccept_Kp==1").Filter("nkkpiaccept_Pip==1");
  // auto dfex = df.Filter("Truth==0");
  auto k2=    dfex.Define("M2K", CompInvMass, {"nkkpiKm_P","nkkpiKm_Theta","nkkpiKm_Phi","nkkpiKp_P","nkkpiKp_Theta","nkkpiKp_Phi"});
  // auto miss=    dfex.Define("Mmiss", CompMissMass, {"nkkpiKm_P","nkkpiKm_Theta","nkkpiKm_Phi","nkkpiKp_P","nkkpiKp_Theta","nkkpiKp_Phi","nkkpiPip_P","nkkpiPip_Theta","nkkpiPip_Phi","nkkpiEl_P","nkkpiEl_Theta","nkkpiEl_Phi","nkkpiN_P","nkkpiN_Theta","nkkpiN_Phi"});
 auto miss=    dfex.Define("Mmiss", CompMissMass, {"nkkpiKm_P","nkkpiKm_Theta","nkkpiKm_Phi","nkkpiKp_P","nkkpiKp_Theta","nkkpiKp_Phi","nkkpiPip_P","nkkpiPip_Theta","nkkpiPip_Phi","nkkpiEl_P","nkkpiEl_Theta","nkkpiEl_Phi"});
  //auto k2 = df.Filter("Truth==1&&Topo==0").Filter("nkkpiaccept_Km==1").Filter("nkkpiaccept_Kp==1").Filter("nkkpiaccept_Pip==1")
  //.Define("M2K", CompInvMass, {"nkkpiKm_P","nkkpiKm_Theta","nkkpiKm_Phi","nkkpiKp_P","nkkpiKp_Theta","nkkpiKp_Phi"});


  auto hisk2 = k2.Histo1D( TH1DModel{"Mkk","Invariant mass 2K",100,0.8,3},{"M2K"} );
  auto hismiss	 = miss.Histo1D( TH1DModel{"Mmiss","Missing mass",200,-1,3},{"Mmiss"} );

  auto can = new TCanvas();
  can->Divide(2,2);
  can->cd(1);
  hisKm->DrawCopy();
  can->cd(2);
  hisKp->DrawCopy();
  can->cd(3);
  hisPip->DrawCopy();
  can->cd(4);
  hisEl->DrawCopy();


  can = new TCanvas();
  can->Divide(2,1);
  can->cd(1);
  hisk2->DrawCopy();
  can->cd(2);
  hismiss->DrawCopy();

  gBenchmark->Stop("analyse");
  gBenchmark->Print("analyse");

}
