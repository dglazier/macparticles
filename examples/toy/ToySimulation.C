//Toy detector simulation
//1a) gnereate events for training
//1b) generate phase space events for reaction g+p->pi+ + pi- + p' 
//2) Check acceptance
//3) Check resolutions
//
// Detector :  A(P) = Gaussin_CDF(mean=2, sigma=0.5)
//             Res(P) = CrystalBarrel
//             A(theta) = theta >5deg && theta  <175deg
//             Res(theta) = Gaussian sigma=1 correlated with phi
//             A(phi)  = diagnal hole in theta-phi plane
//             Res(phi) = Gaussian/sin(theta) sigma=0.5 correlated with theta

using namespace RooFit;

RooRealVar arg_P("p", "p", 0,10);
RooRealVar arg_resP("resp", "resp", -10,10);
RooAbsReal *func_A_P=nullptr;
RooAbsReal *func_ResP=nullptr;
RooDataSet *cache_ResP=nullptr;
Double_t maxAccP=0.9;

void Create_A_P(){
  RooGaussian gx("gx", "gx", arg_P, RooConst(2), RooConst(0.5));
  func_A_P = gx.createCdf(arg_P);
}

Bool_t Check_A_P(Double_t p){
  arg_P.setVal(p);
  return gRandom->Uniform()<func_A_P->getVal()*maxAccP;
}

void Create_Res_P(Long64_t Nevents){
  func_ResP=new RooCBShape("CrBaP", "CrBaP", arg_resP,RooConst(0.0), RooConst(0.05),RooConst(0.5),RooConst(10));
  cache_ResP = static_cast<RooAbsPdf*>(func_ResP)->generate(RooArgSet(arg_resP),Nevents);
  
}

Double_t Resolution_P(Double_t p,Int_t icache){
  auto ev=cache_ResP->get(icache);
  auto result= static_cast<RooRealVar*>(ev->first())->getVal()*(0.5*p+0.5);
  return result;// p+result > 0 ? result :-result;
}


Double_t maxAccThPh=0.95;

void Create_A_ThetaPhi(){
  //RooGaussian gx("gx", "gx", arg_P, RooConst(2), RooConst(1));
  //func_A_P = gx.createCdf(arg_P);
  
}
RooRealVar arg_resTh("resth", "resth", -100,100);
RooRealVar arg_resPh("resph", "resph", -100,100);
RooDataSet *cache_ResThPh=nullptr;

void Create_Res_ThPh(Long64_t Nevents){
  // make a covariance matrix that is all 1's
  auto dim=2;
  TMatrixDSym cov(dim);
 
  cov(0,0)=1; cov(1,0)=-0.5;
  cov(0,1)=-0.5; cov(1,1)=0.5;
  
  RooArgList xVec(arg_resTh,arg_resPh);
  RooArgList meanVec(RooConst(0), RooConst(0));
  
  // now make the multivariate Gaussian
  RooMultiVarGaussian func_ResThPh("mvg", "mvg", xVec, meanVec, cov);
  
  cache_ResThPh = func_ResThPh.generate(xVec,Nevents);
  
}
Bool_t Check_A_ThetaPhi(Double_t th,Double_t ph){
  if(th<5)return kFALSE;
  if(th>175)return kFALSE;

  if(TMath::Abs(ph+(th-90)/5)<10)return kFALSE;
  if(TMath::Abs(ph+(th-90)/5)>170)return kFALSE;

  return gRandom->Uniform()<maxAccThPh;
}
void Resolution_ThPhi(Double_t p,Double_t &th,Double_t &ph,Int_t icache){
  auto ev=cache_ResThPh->get(icache);
  Double_t vals[2];
  UInt_t ivar=0;
  for(auto& var:*ev){
    vals[ivar]=static_cast<RooRealVar*>(var)->getVal();
    ++ivar;
  }
  ph+=vals[1]/TMath::Sin(th*TMath::DegToRad());//phase space dependence
  th+=vals[0];
  return;
}


void ToySimulation(Long64_t Nevents=1.6E6){
  Create_A_P();
  Create_Res_P(Nevents);
  
  Create_A_ThetaPhi();
  Create_Res_ThPh(Nevents);

  std::unique_ptr<TFile> file{new TFile("toy_training.root","recreate")} ;
  auto tree = std::make_unique<TTree>("simtree", "toy sim events");

  //////////////P
  Double_t P=0;
  Double_t recP=0;
  Bool_t accP=0;
  tree->Branch("truP",&P);
  tree->Branch("accepted",&accP,"accepted/O");
  tree->Branch("recP",&recP,"recP/D");
  /////////////Theta
  Double_t Th=0;
  Double_t recTh=0;
  Bool_t accThPh=0;
  tree->Branch("truTheta",&Th);
  // tree->Branch("accThPh",&accThPh,"accThPh/O");
  tree->Branch("recTheta",&recTh,"recTheta/D");
  /////////////Phi
  Double_t Ph=0;
  Double_t recPh=0;
  tree->Branch("truPhi",&Ph);
  tree->Branch("recPhi",&recPh,"recPhi/D");

  gBenchmark->Start("generate");
  for(Long64_t i=0;i<Nevents;++i){
    //P
    accP = Check_A_P(P=gRandom->Uniform(0,10));    
    recP=Resolution_P(P,i)+P;
    //ThetaPhi
    auto cosTh=gRandom->Uniform(-1,1);
    Th=TMath::ACos(cosTh)*TMath::RadToDeg();
    accThPh = Check_A_ThetaPhi(Th,Ph=gRandom->Uniform(-180,180));    
    recTh=Th;
    recPh=Ph;
    Resolution_ThPhi(P,recTh,recPh,i);

    // Th=Th*TMath::DegToRad();
    // Ph=Ph*TMath::DegToRad();
    // recTh=recTh*TMath::DegToRad();
    // recPh=recPh*TMath::DegToRad();
    // Th=Th;
    // Ph=Ph;
    // recTh=recTh;
    // recPh=recPh;

    accP=accP*accThPh;
    
    tree->Fill();
  }
  gBenchmark->Stop("generate");
  gBenchmark->Print("generate");

  tree->Write();

}
