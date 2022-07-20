struct Hists{
  TH2D Res_2pP={"Res_2pP","resolution proton P (GeV)",50,0,10,100,-1,1};
  TH1D Res_pP={"Res_pP","resolution proton P (GeV)",100,-1,1};
  TH1D Diff_pP={"Diff_pP","difference proton P (GeV)",100,-2,2};
  // TH1D Res_pP={"Res_pP","resolution proton P",100,-1,1};
  // TH1D Diff_pP={"Diff_pP","difference proton P",100,-1,1};

  TH2D Res_2pipP={"Res_2pipP","resolution #pi+ P",50,0,10,100,-1,1};
  TH1D Res_pipP={"Res_pipP","resolution #pi+ P",100,-1,1};
  TH1D Diff_pipP={"Diff_pipP","difference #pi+ P",100,-2,2};
  // TH1D Res_pipP={"Res_pipP","resolution #pi+ P",100,-1,1};
  // TH1D Diff_pipP={"Diff_pipP","difference #pi+ P",100,-1,1};

  TH2D Res_2pimP={"Res_2pimP","resolution #pi- P",50,0,10,100,-1,1};
  TH1D Res_pimP={"Res_pimP","resolution #pi- P",100,-1,1};
  TH1D Diff_pimP={"Diff_pimP","difference #pi- P",100,-2,2};
  // TH1D Res_pimP={"Res_pimP","resolution #pi- P",100,-1,1};
  // TH1D Diff_pimP={"Diff_pimP","difference #-1 P",100,-1,1};

  TH1D Res_TotP={"Res_totP","resolution total P",100,-1,1};
  TH1D Diff_TotP={"Diff_totP","difference total P",100,-2,2};

  void Draw(){
    auto can=new TCanvas;
    can->Divide(2,1);
    can->cd(1);
    Res_pP.DrawCopy();
    can->cd(2);
    Diff_pP.DrawCopy();
    can=new TCanvas;
    can->Divide(2,1);
    can->cd(1);
    Res_pipP.DrawCopy();
    can->cd(2);
    Diff_pipP.DrawCopy();
    can=new TCanvas;
    can->Divide(2,1);
    can->cd(1);
    Res_pimP.DrawCopy();
    can->cd(2);
    Diff_pimP.DrawCopy();
    can=new TCanvas;
    can->Divide(2,1);
    can->cd(1);
    Res_TotP.DrawCopy();
    can->cd(2);
    Diff_TotP.DrawCopy();
   can=new TCanvas;
   can->Divide(3,1);
   can->cd(1);
   Res_2pP.DrawCopy("col2");
   can->cd(2);
   Res_2pipP.DrawCopy("col2");
   can->cd(3);
   Res_2pimP.DrawCopy("col2");

  }
};
struct HistsPhys{
  TH1D hM2pi={"M2pi","Invariant Mass #pi+#pi- (GeV)",110,0.3,3.6};
  TH1D hW={"W","W (GeV)",110,3,4.8};
  TH1D hCosTh={"CosTh","cos(#theta_{GJ})",110,-1,1};
  TH1D hPhi={"Phi","#phi_{GJ} (#circ)",110,-180,180};
  TH1D hCMCosTh={"CosTh","cos(#theta_{CM})",110,-1,1};
  TH1D hCMPhi={"Phi","#phi_{CM} (#circ)",110,-180,180};

  TH1D hpP={"pP","proton Momentum (GeV)",100,0,10};
  TH1D hpTheta={"pTheta","proton #theta (#circ)",100,0,60};
  TH1D hpPhi={"pPhi","proton #phi (#circ)",100,-180,180};
  TH1D hpimP={"pimP","#pi- Momentum (GeV)",100,0,10};
  TH1D hpimTheta={"pimTheta","#pi- #theta (#circ)",100,0,60};
  TH1D hpimPhi={"pimPhi","#pi- #phi (#circ)",100,-180,180};
  TH1D hpipP={"pipP","#pi+ Momentum (GeV)",100,0,10};
  TH1D hpipTheta={"pipTheta","#pi+ #theta (#circ)",100,0,60};
  TH1D hpipPhi={"pipPhi","#pi+ #phi (#circ)",100,-180,180};

  
  void Draw(){
    auto can=new TCanvas;
    can->Divide(3,2);

    can->cd(1);
    hW.DrawCopy();
    can->cd(3);
    hM2pi.DrawCopy();
    can->cd(5);
    hCMCosTh.DrawCopy();
    can->cd(2);
    hCMPhi.DrawCopy();
    can->cd(4);
    hCosTh.DrawCopy();
    can->cd(6);
    hPhi.DrawCopy();
    
  }
  HistsPhys Divide(const HistsPhys& di){
    auto ratio=*this;
    ratio.hW.Sumw2();
    ratio.hM2pi.Sumw2();
    ratio.hCMCosTh.Sumw2();
    ratio.hCMPhi.Sumw2();
    ratio.hCosTh.Sumw2();
    ratio.hPhi.Sumw2();
    
    ratio.hW.SetMaximum(2);
    ratio.hM2pi.SetMaximum(2);
    ratio.hCMCosTh.SetMaximum(2);
    ratio.hCMPhi.SetMaximum(2);
    ratio.hCosTh.SetMaximum(2);
    ratio.hPhi.SetMaximum(2);
    
    ratio.hW.Divide(&di.hW);
    ratio.hM2pi.Divide(&di.hM2pi);
    ratio.hCMCosTh.Divide(&di.hCMCosTh);
    ratio.hCMPhi.Divide(&di.hCMPhi);
    ratio.hCosTh.Divide(&di.hCosTh);
    ratio.hPhi.Divide(&di.hPhi);
   
    return ratio;
  }
  void DrawPOnCanvas(TCanvas* can,TString opt,Int_t colour){
    can->cd(1);
    hpP.SetMinimum(0);
    hpP.SetLineColor(colour);
    hpP.GetXaxis()->SetTitle(hpP.GetTitle());
    hpP.GetXaxis()->CenterTitle();
    hpP.DrawCopy(opt);
    can->cd(2);
    hpTheta.SetMinimum(0);
    hpTheta.SetLineColor(colour); 
    hpTheta.GetXaxis()->SetTitle(hpTheta.GetTitle());
    hpTheta.GetXaxis()->CenterTitle();
    hpTheta.DrawCopy(opt);
    can->cd(3);
    hpPhi.SetMinimum(0);
    hpPhi.SetLineColor(colour); 
    hpPhi.GetXaxis()->SetTitle(hpPhi.GetTitle());
    hpPhi.GetXaxis()->CenterTitle();
    hpPhi.DrawCopy(opt);

    can->cd(4);
    hpipP.SetMinimum(0);
    hpipP.SetLineColor(colour); 
    hpipP.GetXaxis()->SetTitle(hpipP.GetTitle());
    hpipP.GetXaxis()->CenterTitle();
    hpipP.DrawCopy(opt);
    can->cd(5);
    hpipTheta.SetMinimum(0);
    hpipTheta.SetLineColor(colour); 
    hpipTheta.GetXaxis()->SetTitle(hpipTheta.GetTitle());
    hpipTheta.GetXaxis()->CenterTitle();
    hpipTheta.DrawCopy(opt);
    can->cd(6);
    hpipPhi.SetMinimum(0);
    hpipPhi.SetLineColor(colour); 
    hpipPhi.GetXaxis()->SetTitle(hpipPhi.GetTitle());
    hpipPhi.GetXaxis()->CenterTitle();
    hpipPhi.DrawCopy(opt);

    can->cd(7);
    hpimP.SetMinimum(0);
    hpimP.SetLineColor(colour); 
    hpimP.GetXaxis()->SetTitle(hpimP.GetTitle());
    hpimP.GetXaxis()->CenterTitle();
    hpimP.DrawCopy(opt);
    can->cd(8);
    hpimTheta.SetMinimum(0);
    hpimTheta.SetLineColor(colour); 
    hpimTheta.GetXaxis()->SetTitle(hpimTheta.GetTitle());
    hpimTheta.GetXaxis()->CenterTitle();
    hpimTheta.DrawCopy(opt);
    can->cd(9);
    hpimPhi.SetMinimum(0);
    hpimPhi.SetLineColor(colour); 
    hpimPhi.GetXaxis()->SetTitle(hpimPhi.GetTitle());
    hpimPhi.GetXaxis()->CenterTitle();
    hpimPhi.DrawCopy(opt);

  }
  void DrawOnCanvas(TCanvas* can,TString opt,Int_t colour){
 
    can->cd(1);
    hW.SetMinimum(0);
    hW.GetXaxis()->SetTitle(hW.GetTitle());
    hW.SetLineColor(colour); 
    hW.GetXaxis()->CenterTitle();
    hW.DrawCopy(opt);

    can->cd(4);
    hM2pi.SetMinimum(0);
    hM2pi.SetLineColor(colour);  
    hM2pi.GetXaxis()->SetTitle(hM2pi.GetTitle());
    hM2pi.GetXaxis()->CenterTitle();
    hM2pi.DrawCopy(opt);

    can->cd(2);
    hCMCosTh.SetMinimum(0);
    hCMCosTh.SetLineColor(colour);  
    hCMCosTh.GetXaxis()->SetTitle(hCMCosTh.GetTitle());
    hCMCosTh.GetXaxis()->CenterTitle();
    hCMCosTh.DrawCopy(opt);
    
    can->cd(5);
    hCMPhi.SetMinimum(0);
    hCMPhi.SetLineColor(colour);  
    hCMPhi.GetXaxis()->SetTitle(hCMPhi.GetTitle());
    hCMPhi.GetXaxis()->CenterTitle();
    hCMPhi.DrawCopy(opt);

    can->cd(3);
    hCosTh.SetMinimum(0);
    hCosTh.SetLineColor(colour);  
    hCosTh.GetXaxis()->SetTitle(hCosTh.GetTitle());
    hCosTh.GetXaxis()->CenterTitle();
    hCosTh.DrawCopy(opt);

    can->cd(6);
    hPhi.SetMinimum(0);
    hPhi.SetLineColor(colour);  
    hPhi.GetXaxis()->SetTitle(hPhi.GetTitle());
    hPhi.GetXaxis()->CenterTitle();
    hPhi.DrawCopy(opt);
    
  }

  void Fill(const TLorentzVector &fBar,const TLorentzVector &fGamma,const TLorentzVector &fMes_d1,const TLorentzVector &fMes_d2){

    auto meson=fMes_d1+fMes_d2;
    hM2pi.Fill(meson.M());

    auto CM=meson+fBar;
    hW.Fill(CM.M());
    
    MesonDecayGJ(meson,fBar,fGamma,fMes_d1,fMes_d2);

    hCosTh.Fill(_cosTh);
    hPhi.Fill(_phi);

    CMDecay(meson,fBar,fGamma);

    hCMCosTh.Fill(_CMcosTh);
    hCMPhi.Fill(_CMphi);

    hpP.Fill(fBar.Rho());
    hpTheta.Fill(fBar.Theta()*TMath::RadToDeg());
    hpPhi.Fill(fBar.Phi()*TMath::RadToDeg());
    hpipP.Fill(fMes_d1.Rho());
    hpipTheta.Fill(fMes_d1.Theta()*TMath::RadToDeg());
    hpipPhi.Fill(fMes_d1.Phi()*TMath::RadToDeg());
    hpimP.Fill(fMes_d2.Rho());
    hpimTheta.Fill(fMes_d2.Theta()*TMath::RadToDeg());
    hpimPhi.Fill(fMes_d2.Phi()*TMath::RadToDeg());
 
  }
 
  void CMDecay(TLorentzVector fMes,TLorentzVector fBar,TLorentzVector fGamma){
     auto decBoost=-(fMes+fBar).BoostVector();
     fBar.Boost(decBoost);

     _CMcosTh=TMath::Cos(fBar.Theta());
     _CMphi=fBar.Phi()*TMath::RadToDeg();
     
   }
  ////////////////////////////////////////////////////////
  ///z-axis along gamma direction in meson rest frame
  void MesonDecayGJ(TLorentzVector fMes,TLorentzVector fBar,TLorentzVector fGamma,TLorentzVector fMes_d1,TLorentzVector fMes_d2){
     
    auto decBoost=-fMes.BoostVector();
    fBar.Boost(decBoost);
    fGamma.Boost(decBoost);
    
    auto zV=-fBar.Vect().Unit();
    auto yV=fBar.Vect().Cross(fGamma.Vect()).Unit();
    auto xV=yV.Cross(zV).Unit();
    
    fMes_d1.Boost(decBoost);
    TVector3 angles(fMes_d1.Vect().Dot(xV),fMes_d1.Vect().Dot(yV),fMes_d1.Vect().Dot(zV));
    
    _cosTh=angles.CosTheta();
    _phi=angles.Phi()*TMath::RadToDeg();

     
   }
  
  Double_t _cosTh=0;
  Double_t _phi=0;
  Double_t _CMcosTh=0;
  Double_t _CMphi=0;
};

struct HistsPhysRes{
  TH1D hM2pi={"M2pi","Invariant Mass #pi+#pi- (GeV)",110,-1,1};
  TH1D hW={"W","W (GeV)",110,-1,1};
  TH1D hCosTh={"CosTh","cos(#theta_{GJ})",110,-0.3,0.3};
  TH1D hPhi={"Phi","#phi_{GJ} (#circ)",110,-2,2};
  TH1D hCMCosTh={"CosTh","cos(#theta_{CM})",110,-0.3,0.3};
  TH1D hCMPhi={"Phi","#phi_{CM} (#circ)",110,-2,2};

  TH1D hpP={"pP","proton Momentum Resolution (GeV)",100,-2,2};
  TH1D hpTheta={"pTheta","proton #theta Resolution (#circ)",100,-3,3};
  TH1D hpPhi={"pPhi","proton #phi Resolution (#circ)",100,-15,15};
  TH1D hpimP={"pimP","#pi- Momentum Resolution (GeV)",100,-2,2};
  TH1D hpimTheta={"pimTheta","#pi- #theta Resolution(#circ)",100,-3,3};
  TH1D hpimPhi={"pimPhi","#pi- #phi Resolution (#circ)",100,-15,15};
  TH1D hpipP={"pipP","#pi+ Momentum Resolution (GeV)",100,-2,2};
  TH1D hpipTheta={"pipTheta","#pi+ #theta Resolution (#circ)",100,-3,3};
  TH1D hpipPhi={"pipPhi","#pi+ #phi Resolution (#circ)",100,-15,15};
  void Draw(){
    auto can=new TCanvas;
    can->Divide(3,2);

    can->cd(1);
    hW.DrawCopy();
    can->cd(3);
    hM2pi.DrawCopy();
    can->cd(5);
    hCMCosTh.DrawCopy();
    can->cd(2);
    hCMPhi.DrawCopy();
    can->cd(4);
    hCosTh.DrawCopy();
    can->cd(6);
    hPhi.DrawCopy();
    
  }
 
  void DrawOnCanvas(TCanvas* can,TString opt,Int_t colour){
 
    can->cd(1);
    hW.SetMinimum(0);
    hW.SetLineColor(colour); 
    hW.GetXaxis()->SetTitle(hW.GetTitle());
    hW.GetXaxis()->CenterTitle();
    hW.DrawCopy(opt);

    can->cd(4);
    hM2pi.SetMinimum(0);
    hM2pi.SetLineColor(colour);  
    hM2pi.GetXaxis()->SetTitle(hM2pi.GetTitle());
    hM2pi.GetXaxis()->CenterTitle();
    hM2pi.DrawCopy(opt);

    can->cd(2);
    hCMCosTh.SetMinimum(0);
    hCMCosTh.SetLineColor(colour);  
    hCMCosTh.GetXaxis()->SetTitle(hCMCosTh.GetTitle());
    hCMCosTh.GetXaxis()->CenterTitle();
    hCMCosTh.DrawCopy(opt);
    
    can->cd(5);
    hCMPhi.SetMinimum(0);
    hCMPhi.SetLineColor(colour);  
    hCMPhi.GetXaxis()->SetTitle(hCMPhi.GetTitle());
    hCMPhi.GetXaxis()->CenterTitle();
    hCMPhi.DrawCopy(opt);

    can->cd(3);
    hCosTh.SetMinimum(0);
    hCosTh.SetLineColor(colour);  
    hCosTh.GetXaxis()->SetTitle(hCosTh.GetTitle());
    hCosTh.GetXaxis()->CenterTitle();
    hCosTh.DrawCopy(opt);

    can->cd(6);
    hPhi.SetMinimum(0);
    hPhi.SetLineColor(colour);  
    hPhi.GetXaxis()->SetTitle(hPhi.GetTitle());
    hPhi.GetXaxis()->CenterTitle();
    hPhi.DrawCopy(opt);
    
  }
void DrawPOnCanvas(TCanvas* can,TString opt,Int_t colour){
    can->cd(1);
    hpP.SetMinimum(0);
    hpP.SetLineColor(colour); 
    hpP.GetXaxis()->SetTitle(hpP.GetTitle());
    hpP.GetXaxis()->CenterTitle();
    hpP.DrawCopy(opt);
    can->cd(2);
    hpTheta.SetMinimum(0);
    hpTheta.SetLineColor(colour); 
    hpTheta.GetXaxis()->SetTitle(hpTheta.GetTitle());
    hpTheta.GetXaxis()->CenterTitle();
    hpTheta.DrawCopy(opt);
    can->cd(3);
    hpPhi.SetMinimum(0);
    hpPhi.SetLineColor(colour); 
    hpPhi.GetXaxis()->SetTitle(hpPhi.GetTitle());
    hpPhi.GetXaxis()->CenterTitle();
    hpPhi.DrawCopy(opt);

    can->cd(4);
    hpipP.SetMinimum(0);
    hpipP.SetLineColor(colour); 
    hpipP.GetXaxis()->SetTitle(hpipP.GetTitle());
    hpipP.GetXaxis()->CenterTitle();
    hpipP.DrawCopy(opt);
    can->cd(5);
    hpipTheta.SetMinimum(0);
    hpipTheta.SetLineColor(colour); 
    hpipTheta.GetXaxis()->SetTitle(hpipTheta.GetTitle());
    hpipTheta.GetXaxis()->CenterTitle();
    hpipTheta.DrawCopy(opt);
    can->cd(6);
    hpipPhi.SetMinimum(0);
    hpipPhi.SetLineColor(colour); 
    hpipPhi.GetXaxis()->SetTitle(hpipPhi.GetTitle());
    hpipPhi.GetXaxis()->CenterTitle();
    hpipPhi.DrawCopy(opt);

    can->cd(7);
    hpimP.SetMinimum(0);
    hpimP.SetLineColor(colour); 
    hpimP.GetXaxis()->SetTitle(hpimP.GetTitle());
    hpimP.GetXaxis()->CenterTitle();
    hpimP.GetXaxis()->CenterTitle();
    hpimP.DrawCopy(opt);
    can->cd(8);
    hpimTheta.SetMinimum(0);
    hpimTheta.SetLineColor(colour); 
    hpimTheta.GetXaxis()->SetTitle(hpimTheta.GetTitle());
    hpimTheta.GetXaxis()->CenterTitle();
    hpimTheta.DrawCopy(opt);
    can->cd(9);
    hpimPhi.SetMinimum(0);
    hpimPhi.SetLineColor(colour); 
    hpimPhi.GetXaxis()->SetTitle(hpimPhi.GetTitle());
    hpimPhi.GetXaxis()->CenterTitle();
    hpimPhi.DrawCopy(opt);

  }
  void Fill(const TLorentzVector &fBar,const TLorentzVector &fGamma,const TLorentzVector &fMes_d1,const TLorentzVector &fMes_d2,const TLorentzVector &truBar,const TLorentzVector &truGamma,const TLorentzVector &truMes_d1,const TLorentzVector &truMes_d2){

    auto meson=fMes_d1+fMes_d2;
    auto trumeson=truMes_d1+truMes_d2;
    hM2pi.Fill(meson.M()-trumeson.M());

    auto CM=meson+fBar;
    auto truCM=trumeson+truBar;
    hW.Fill(CM.M()-truCM.M());
    
    MesonDecayGJ(meson,fBar,fGamma,fMes_d1,fMes_d2);
    auto cosThR=_cosTh;
    auto phiR=_phi;
    MesonDecayGJ(trumeson,truBar,fGamma,truMes_d1,truMes_d2);
    
    hCosTh.Fill(_cosTh-cosThR);
    hPhi.Fill(_phi-phiR);

    CMDecay(meson,fBar,fGamma);
    auto CMcosThR=_CMcosTh;
    auto CMphiR=_CMphi;
    CMDecay(trumeson,truBar,fGamma);
 
    hCMCosTh.Fill(_CMcosTh-CMcosThR);
    hCMPhi.Fill(_CMphi-CMphiR);

    hpP.Fill(fBar.Rho()-truBar.Rho());
    hpTheta.Fill((fBar.Theta()-truBar.Theta())*TMath::RadToDeg());
    hpPhi.Fill((fBar.Phi()-truBar.Phi())*TMath::RadToDeg());
    hpipP.Fill((fMes_d1.Rho()-truMes_d1.Rho()));
    hpipTheta.Fill((fMes_d1.Theta()-truMes_d1.Theta())*TMath::RadToDeg());
    hpipPhi.Fill((fMes_d1.Phi()-truMes_d1.Phi())*TMath::RadToDeg());
    hpimP.Fill(fMes_d2.Rho()-truMes_d2.Rho());
    hpimTheta.Fill((fMes_d2.Theta()-truMes_d2.Theta())*TMath::RadToDeg());
    hpimPhi.Fill((fMes_d2.Phi()-truMes_d2.Phi())*TMath::RadToDeg());
   
  }
   void CMDecay(TLorentzVector fMes,TLorentzVector fBar,TLorentzVector fGamma){
     auto decBoost=-(fMes+fBar).BoostVector();
     fBar.Boost(decBoost);

     _CMcosTh=TMath::Cos(fBar.Theta());
     _CMphi=fBar.Phi();
     
   }
  ////////////////////////////////////////////////////////
  ///z-axis along gamma direction in meson rest frame
  void MesonDecayGJ(TLorentzVector fMes,TLorentzVector fBar,TLorentzVector fGamma,TLorentzVector fMes_d1,TLorentzVector fMes_d2){
     
    auto decBoost=-fMes.BoostVector();
    fBar.Boost(decBoost);
    fGamma.Boost(decBoost);
    
    auto zV=-fBar.Vect().Unit();
    auto yV=fBar.Vect().Cross(fGamma.Vect()).Unit();
    auto xV=yV.Cross(zV).Unit();
    
    fMes_d1.Boost(decBoost);
    TVector3 angles(fMes_d1.Vect().Dot(xV),fMes_d1.Vect().Dot(yV),fMes_d1.Vect().Dot(zV));
    
    _cosTh=angles.CosTheta();
    _phi=angles.Phi();

     
   }
  
  Double_t _cosTh=0;
  Double_t _phi=0;
  Double_t _CMcosTh=0;
  Double_t _CMphi=0;
};


void CreateP4(TLorentzVector& v,Double_t p,Double_t theta,Double_t phi,Double_t M){

  v.SetXYZM(0,0,p,M);
  v.SetTheta(theta);
  v.SetPhi(phi);
  
}


void Reconstruct_Reaction_Mac(){
  gStyle->SetLabelSize(0.06);
  gStyle->SetLabelSize(0.06,"Y");
  gStyle->SetTitleXSize(0.08);
  gStyle->SetTitleXOffset(0.87);
  gStyle->SetTitleYSize(0.08);
  gStyle->SetTitleYOffset(0.8);
  gStyle->SetHistLineWidth(2);
  gStyle->SetTitleAlign(0);
  // set margin sizes
  
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);
  //Int_t font=42; // Helvetica
  Int_t font=2; // Times new Roman bold
  gStyle->SetTextFont(font);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);






  TTreeReader     fReader;  //!the tree reader
  
  auto file= TFile::Open("toy_reaction.root");
  auto tree= file->Get<TTree>("tree");
  tree->SetEntries(5E6);
  auto filePip= TFile::Open("results/pi+/predictions.root");
  auto treeFastPip= filePip->Get<TTree>("recon");
  treeFastPip->SetName("tree_fast");
  tree->AddFriend(treeFastPip);
  auto fileAccPip= TFile::Open("results/pi+/simulation_acceptances.root");
  auto treeFastAccPip= fileAccPip->Get<TTree>("acceptance");
  treeFastAccPip->SetName("acc_pip");
  tree->AddFriend(treeFastAccPip);
  
  auto filePim= TFile::Open("results/pi-/predictions.root");
  auto treeFastPim= filePim->Get<TTree>("recon");
  treeFastPim->SetName("tree_fast");
  tree->AddFriend(treeFastPim);
  auto fileAccPim= TFile::Open("results/pi-/simulation_acceptances.root");
  auto treeFastAccPim= fileAccPim->Get<TTree>("acceptance");
  treeFastAccPim->SetName("acc_pim");
  tree->AddFriend(treeFastAccPim);

  auto fileP= TFile::Open("results/proton/predictions.root");
  auto treeFastP= fileP->Get<TTree>("recon");
  treeFastP->SetName("tree_fast");
  tree->AddFriend(treeFastP);
  auto fileAccP= TFile::Open("results/proton/simulation_acceptances.root");
  auto treeFastAccP= fileAccP->Get<TTree>("acceptance");
  treeFastAccP->SetName("acc_p");
  tree->AddFriend(treeFastAccP);
  
  TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Double_t> tr_pP = {fReader, "truth.pP"};
   TTreeReaderValue<Double_t>  tr_pTheta = {fReader, "truth.pTheta"};
   TTreeReaderValue<Double_t>  tr_pPhi = {fReader, "truth.pPhi"};
   TTreeReaderValue<Double_t>  tr_pipP = {fReader, "truth.pipP"};
   TTreeReaderValue<Double_t>  tr_pipTheta = {fReader, "truth.pipTheta"};
   TTreeReaderValue<Double_t>  tr_pipPhi = {fReader, "truth.pipPhi"};
   TTreeReaderValue<Double_t>  tr_pimP = {fReader, "truth.pimP"};
   TTreeReaderValue<Double_t>  tr_pimTheta = {fReader, "truth.pimTheta"};
   TTreeReaderValue<Double_t>  tr_pimPhi = {fReader, "truth.pimPhi"};

   TTreeReaderValue<Double_t> rec_pP = {fReader, "recon.pP"};
   TTreeReaderValue<Double_t>  rec_pTheta = {fReader, "recon.pTheta"};
   TTreeReaderValue<Double_t>  rec_pPhi = {fReader, "recon.pPhi"};
   TTreeReaderValue<Double_t>  rec_pipP = {fReader, "recon.pipP"};
   TTreeReaderValue<Double_t>  rec_pipTheta = {fReader, "recon.pipTheta"};
   TTreeReaderValue<Double_t>  rec_pipPhi = {fReader, "recon.pipPhi"};
   TTreeReaderValue<Double_t>  rec_pimP = {fReader, "recon.pimP"};
   TTreeReaderValue<Double_t>  rec_pimTheta = {fReader, "recon.pimTheta"};
   TTreeReaderValue<Double_t>  rec_pimPhi = {fReader, "recon.pimPhi"};

   TTreeReaderValue<Double_t> fast_pP = {fReader, "tree_fast.pP"};
   TTreeReaderValue<Double_t>  fast_pTheta = {fReader, "tree_fast.pTheta"};
   TTreeReaderValue<Double_t>  fast_pPhi = {fReader, "tree_fast.pPhi"};
   TTreeReaderValue<Double_t>  fast_pipP = {fReader, "tree_fast.pipP"};
   TTreeReaderValue<Double_t>  fast_pipTheta = {fReader, "tree_fast.pipTheta"};
   TTreeReaderValue<Double_t>  fast_pipPhi = {fReader, "tree_fast.pipPhi"};
   TTreeReaderValue<Double_t>  fast_pimP = {fReader, "tree_fast.pimP"};
   TTreeReaderValue<Double_t>  fast_pimTheta = {fReader, "tree_fast.pimTheta"};
   TTreeReaderValue<Double_t>  fast_pimPhi = {fReader, "tree_fast.pimPhi"};

   TTreeReaderValue<Bool_t> accepted_p = {fReader, "accepted_p"};
   TTreeReaderValue<Bool_t> accepted_pip = {fReader, "accepted_pip"};
   TTreeReaderValue<Bool_t> accepted_pim = {fReader, "accepted_pim"};


   TTreeReaderValue<Long64_t> fast_acc_p = {fReader, "acc_p.accept"};
   TTreeReaderValue<Long64_t> fast_acc_pip = {fReader, "acc_pip.accept"};
   TTreeReaderValue<Long64_t> fast_acc_pim = {fReader, "acc_pim.accept"};


   fReader.SetTree(tree);

   Hists hr;
   Hists hf;
   
   HistsPhys physt;
   HistsPhys physr;
   HistsPhys physf;

   HistsPhysRes physResr;
   HistsPhysRes physResf;
  
   auto Mproton=0.938;
   auto Mpi=0.139;

   gBenchmark->Start("time");
   TLorentzVector truP4_p(0,0,1,Mproton);
   TLorentzVector truP4_pip(0,0,1,Mpi);
   TLorentzVector truP4_pim(0,0,1,Mpi);
   TLorentzVector recP4_p(0,0,1,Mproton);
   TLorentzVector recP4_pip(0,0,1,Mpi);
   TLorentzVector recP4_pim(0,0,1,Mpi);
   TLorentzVector fastP4_p(0,0,1,Mproton);
   TLorentzVector fastP4_pip(0,0,1,Mpi);
   TLorentzVector fastP4_pim(0,0,1,Mpi);

   TLorentzVector photon(0.0, 0.0, 10, 10);

   TH2D rpiDiff("rpi","pi",100,-2,2,100,-2,2);
   TH2D fpiDiff("fpi","pi",100,-2,2,100,-2,2);
   
   while(fReader.Next()){
     if(*accepted_p){
       hr.Res_pP.Fill(*tr_pP-*rec_pP);
       hr.Res_2pP.Fill(*tr_pP,*tr_pP-*rec_pP);
     }
     if(*fast_acc_p){
       hf.Res_pP.Fill(*tr_pP-*fast_pP);
     }
     if(*accepted_p&&*fast_acc_p){
       hf.Res_2pP.Fill(*tr_pP,*tr_pP-*fast_pP);
     }    
     if(*accepted_p&&*fast_acc_p) hf.Diff_pP.Fill(*rec_pP-*fast_pP);
     
     if(*accepted_pip){
       hr.Res_pipP.Fill(*tr_pipP-*rec_pipP);
       hr.Res_2pipP.Fill(*tr_pipP,*tr_pipP-*rec_pipP);
     }
     if(*fast_acc_pip){
       hf.Res_pipP.Fill(*tr_pipP-*fast_pipP);
     }
     if(*accepted_pip&&*fast_acc_pip){
        hf.Res_2pipP.Fill(*tr_pipP,*tr_pipP-*fast_pipP);
     }
     if(*accepted_pip&&*fast_acc_pip) hf.Diff_pipP.Fill(*rec_pipP-*fast_pipP);

     if(*accepted_pim){
       hr.Res_pimP.Fill(*tr_pimP-*rec_pimP);
       hr.Res_2pimP.Fill(*tr_pimP,*tr_pimP-*rec_pimP);
     }
     if(*fast_acc_pim){
       hf.Res_pimP.Fill(*tr_pimP-*fast_pimP);
     }
     if(*accepted_pim&&*fast_acc_pim){
       hf.Res_pimP.Fill(*tr_pimP-*fast_pimP);
       hf.Res_2pimP.Fill(*tr_pimP,*tr_pimP-*fast_pimP);
     }
     if(*accepted_pim&&*fast_acc_pim) hf.Diff_pimP.Fill(*rec_pimP-*fast_pimP);
     
     if(*accepted_pim&&*accepted_pip&&*accepted_p)
       hr.Res_TotP.Fill(*tr_pimP-*rec_pimP + *tr_pipP-*rec_pipP + *tr_pP-*rec_pP);
     if(*fast_acc_pim&&*fast_acc_pip&&*fast_acc_p)
       hf.Res_TotP.Fill(*tr_pimP-*fast_pimP + *tr_pipP-*fast_pipP + *tr_pP-*fast_pP);

     if(*accepted_pim&&*fast_acc_pim&&*accepted_pip&&*fast_acc_pip&&*accepted_p&&*fast_acc_p)
       hf.Diff_TotP.Fill(*rec_pimP-*fast_pimP+*rec_pipP-*fast_pipP+*rec_pP-*fast_pP);
     
     if(*accepted_pim&&*fast_acc_pim&&*accepted_pip&&*fast_acc_pip&&*accepted_p&&*fast_acc_p){
       rpiDiff.Fill(*rec_pipP-*tr_pipP,(*rec_pimP-*tr_pimP));
       fpiDiff.Fill(*fast_pipP-*tr_pipP,(*fast_pimP-*tr_pimP));
     }
     CreateP4(truP4_p,*tr_pP,*tr_pTheta,*tr_pPhi,Mproton);
     CreateP4(truP4_pip,*tr_pipP,*tr_pipTheta,*tr_pipPhi,Mpi);
     CreateP4(truP4_pim,*tr_pimP,*tr_pimTheta,*tr_pimPhi,Mpi);
     
     CreateP4(recP4_p,*rec_pP,*rec_pTheta,*rec_pPhi,Mproton);
     CreateP4(recP4_pip,*rec_pipP,*rec_pipTheta,*rec_pipPhi,Mpi);
     CreateP4(recP4_pim,*rec_pimP,*rec_pimTheta,*rec_pimPhi,Mpi);
     
     // CreateP4(fastP4_p,*fast_pP,*fast_pTheta*TMath::DegToRad(),*fast_pPhi*TMath::DegToRad(),Mproton);
     // CreateP4(fastP4_pip,*fast_pipP,*fast_pipTheta*TMath::DegToRad(),*fast_pipPhi*TMath::DegToRad(),Mpi);
     // CreateP4(fastP4_pim,*fast_pimP,*fast_pimTheta*TMath::DegToRad(),*fast_pimPhi*TMath::DegToRad(),Mpi);
     CreateP4(fastP4_p,*fast_pP,*fast_pTheta,*fast_pPhi,Mproton);
     CreateP4(fastP4_pip,*fast_pipP,*fast_pipTheta,*fast_pipPhi,Mpi);
     CreateP4(fastP4_pim,*fast_pimP,*fast_pimTheta,*fast_pimPhi,Mpi);




     //truth
     physt.Fill(truP4_p,photon,truP4_pip,truP4_pim);

     //toy accepted
     if(*accepted_p&&*accepted_pip&&*accepted_pim){
     //  if(*fast_acc_p&&*fast_acc_pip&&*fast_acc_pim){
       physr.Fill(recP4_p,photon,recP4_pip,recP4_pim);
       physResr.Fill(recP4_p,photon,recP4_pip,recP4_pim,truP4_p,photon,truP4_pip,truP4_pim);
     }
     //fast accepted
     if(*fast_acc_p&&*fast_acc_pip&&*fast_acc_pim){
     //if(*accepted_p&&*accepted_pip&&*accepted_pim){
       // physf.Fill(recP4_p,photon,recP4_pip,recP4_pim);
       //physResf.Fill(recP4_p,photon,recP4_pip,recP4_pim,truP4_p,photon,truP4_pip,truP4_pim);
       physf.Fill(fastP4_p,photon,fastP4_pip,fastP4_pim);
       physResf.Fill(fastP4_p,photon,fastP4_pip,fastP4_pim,truP4_p,photon,truP4_pip,truP4_pim);
 
     }

   }

   gBenchmark->Stop("time");
   gBenchmark->Print("time");
   hr.Draw();
   hf.Draw();

    auto can =new TCanvas();
   can->Divide(3,2);
   // physt.DrawOnCanvas(can,"",1);
   physr.DrawOnCanvas(can,"",2);
   physf.DrawOnCanvas(can,"same",4);

   can =new TCanvas();
   can->Divide(3,3);
   // physt.DrawOnCanvas(can,"",1);
   physr.DrawPOnCanvas(can,"",2);
   physf.DrawPOnCanvas(can,"same",4);

   can =new TCanvas();
   can->Divide(3,2);
   physResr.DrawOnCanvas(can,"",2);
   physResf.DrawOnCanvas(can,"same",4);
  can =new TCanvas();
   can->Divide(3,3);
   physResr.DrawPOnCanvas(can,"",2);
   physResf.DrawPOnCanvas(can,"same",4);

   can =new TCanvas();
   can->Divide(2,1);
   can->cd(1);
   rpiDiff.DrawCopy("col1");
   can->cd(2);
   fpiDiff.DrawCopy("col1");

   can =new TCanvas();
   can->Divide(3,2);
   auto ratio=physf.Divide(physr);
   ratio.DrawOnCanvas(can,"e",2);
}
