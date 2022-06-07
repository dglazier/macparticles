#include "ToySimulation.C"

//output data structure
//same for truth and recontructed
struct event{
     Double_t pP=0;
     Double_t pTheta=0;
     Double_t pPhi=0;
     Double_t pipP=0;
     Double_t pipTheta=0;
     Double_t pipPhi=0;
     Double_t pimP=0;
     Double_t pimTheta=0;
     Double_t pimPhi=0;

     };

//Function which takes input momentum for a particle and creates
//reconstructed momentum and acceptance 
Bool_t ReconstructParticle(const Int_t i,const Double_t& trP,
			   const Double_t& trTheta,
			   const Double_t& trPhi,
			   Double_t& recP,Double_t& recTheta,
			   Double_t& recPhi){
  auto accP = Check_A_P(trP);
  auto  accThPh = Check_A_ThetaPhi(trTheta,trPhi);    
 
  recP=Resolution_P(trP,i)+trP;
  recTheta=trTheta;
  recPhi=trPhi;  
  Resolution_ThPhi(trP,recTheta,recPhi,i);

  return accThPh*accP;
}

//Generate the events
//Use TGenPhase
//Use ToySimulation to reconstruct each particle

void Reaction(){
  Int_t Nevents=1E6;
  Create_A_P();
  Create_Res_P(Nevents*3); //3 particles
  
  Create_A_ThetaPhi();
  Create_Res_ThPh(Nevents*3); // 3 particles

  //gamma + p -> pi+ + pi- + proton'
  //create output tree
  TFile* outFile =TFile::Open("toy_reaction.root", "RECREATE") ;
  auto outtree = std::make_unique<TTree>("tree", "Training data");
  
  
  event truth;
  event recon;
  
  outtree->Branch(Form("truth"),&truth);
  outtree->Branch(Form("recon"),&recon);
  
  Bool_t accepted_p=kTRUE;
  outtree->Branch(Form("accepted_p"),&accepted_p);
  Bool_t accepted_pip=kTRUE;
  outtree->Branch(Form("accepted_pip"),&accepted_pip);
  Bool_t accepted_pim=kTRUE;
  outtree->Branch(Form("accepted_pim"),&accepted_pim);

  TLorentzVector target(0.0, 0.0, 0.0, 0.938);
  TLorentzVector beam(0.0, 0.0, 10, 10);
  TLorentzVector W = beam + target;
  
  
  //(Momentum, Energy units are Gev/C, GeV)
  Double_t masses[3] = { 0.938, 0.139, 0.139} ;
  
  TGenPhaseSpace event;
  
  event.SetDecay(W, 3, masses);
  Long64_t count=0;
  Int_t n=0;
  while(count<Nevents){
    
    Double_t weight = event.Generate();
    if(weight>gRandom->Uniform()) continue;
    count++;
    //  int n =count;
    TLorentzVector *pProton = event.GetDecay(0);   
    TLorentzVector *pPip    = event.GetDecay(1);
    TLorentzVector *pPim    = event.GetDecay(2);
    
    truth.pP=pProton->Rho();
    truth.pTheta=pProton->Theta()*TMath::RadToDeg();
    truth.pPhi=pProton->Phi()*TMath::RadToDeg();
    truth.pipP=pPip->Rho();
    truth.pipTheta=pPip->Theta()*TMath::RadToDeg();
    truth.pipPhi=pPip->Phi()*TMath::RadToDeg();
    truth.pimP=pPim->Rho();
    truth.pimTheta=pPim->Theta()*TMath::RadToDeg();
    truth.pimPhi=pPim->Phi()*TMath::RadToDeg();
    
    accepted_p=ReconstructParticle(n++,truth.pP,truth.pTheta,truth.pPhi,
				 recon.pP,recon.pTheta,recon.pPhi);
    accepted_pip=ReconstructParticle(n++,truth.pipP,truth.pipTheta,truth.pipPhi,
				 recon.pipP,recon.pipTheta,recon.pipPhi);
    accepted_pim=ReconstructParticle(n++,truth.pimP,truth.pimTheta,truth.pimPhi,
				 recon.pimP,recon.pimTheta,recon.pimPhi);

    truth.pTheta=truth.pTheta*TMath::DegToRad();
    truth.pPhi=truth.pPhi*TMath::DegToRad();
    truth.pipTheta=truth.pipTheta*TMath::DegToRad();
    truth.pipPhi=truth.pipPhi*TMath::DegToRad();
    truth.pimTheta=truth.pimTheta*TMath::DegToRad();
    truth.pimPhi=truth.pimPhi*TMath::DegToRad();
    
    recon.pTheta=recon.pTheta*TMath::DegToRad();
    recon.pPhi=recon.pPhi*TMath::DegToRad();
    recon.pipTheta=recon.pipTheta*TMath::DegToRad();
    recon.pipPhi=recon.pipPhi*TMath::DegToRad();
    recon.pimTheta=recon.pimTheta*TMath::DegToRad();
    recon.pimPhi=recon.pimPhi*TMath::DegToRad();
    
    outtree->Fill();
  }
  
  outtree->Write();
  
}
