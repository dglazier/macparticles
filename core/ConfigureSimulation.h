#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <TNamed.h>
#include <TSystem.h>
#include <TFile.h>

class ConfigureSimulation: public TNamed{

  public:
  ConfigureSimulation() =default;
  
  ConfigureSimulation(const std::string& dir){
    SetTrainingDir(dir);
  }
  
  void Save(){
    SetName(conf_name.c_str());
    auto file=std::unique_ptr<TFile>{ TFile::Open((training_dir+conf_name+".root").c_str(),"recreate") };
    this->Write();
   }

  void Simulate(const std::string& dir){
    sim_dir=dir;//output directory
    doing_sim=true;
  }
  void Load(const std::string& dir){
    //Open saved configuration
    auto file=std::unique_ptr<TFile>{ TFile::Open((dir+'/'+conf_name+".root").c_str()) };
    if(file==nullptr){ //no previous configuration, start a new one
      SetTrainingDir(dir);
    }
    else{    //got a previous configuration
      auto saved = file->Get<ConfigureSimulation>(conf_name.c_str());
      //Copy to this
      *this = *saved;
    }
  }
  
  void SetTrainingDir(const std::string& dir) {
    gSystem->Exec(Form("mkdir -p  %s",dir.c_str() ));
    training_dir=dir + '/';
    gSystem->Exec(Form("mkdir -p  %s",AcceptanceDir().c_str() ));
    gSystem->Exec(Form("mkdir -p  %s",ResolutionDir().c_str() ));
    gSystem->Exec(Form("mkdir -p  %s",ReWeightDir().c_str() ));
   }
  string  AcceptanceDir() const {
    return training_dir + acc_dir + current_pdg + '/';
  }
  string  ReWeightDir() const {
    return training_dir + reweight_dir + current_pdg + '/';
  }
  string  ResolutionDir() const {
    return training_dir + res_dir + current_pdg + '/';
  }

  string  SimulationDir() const {
    if(particle_name.empty()==false)
      return sim_dir +'/'+ particle_name + '/';
      
    return sim_dir + current_pdg + '/';
  }
  string  AcceptanceName() const {
    return "accept_"+GetParticleName();
  }
  void SetParticleName(const string& pdg){
    particle_name=pdg;
  }
  const string& GetParticleName() const {return particle_name;}

  void AddPid(const string& pdg){
    if((std::find(_pdgs.begin(),_pdgs.end(),pdg))!=_pdgs.end()){
      UsePid(pdg);
      return;
    }
    _pdgs.push_back(pdg);
    _scaleFactors.push_back(1);
    UsePid(pdg);
  }
  void UsePid(const string& pdg){
    if((std::find(_pdgs.begin(),_pdgs.end(),pdg))==_pdgs.end())
      Fatal("ConfigureSimulation::UsePid","invalid pdg %s",pdg.c_str());
   
    //find position in vector
    auto it = std::find(_pdgs.begin(),_pdgs.end(),pdg);
    currentPidIndex=it-_pdgs.begin();

    current_pdg = pdg;

  }
  Double_t CurrentScaleFactor() const {
    return _scaleFactors[currentPidIndex];
  }
  void SetScaleFactor(Double_t val){
    std::cout<<"ConfigureSimulation::SetScaleFactor "<<val<<" to index "<<currentPidIndex <<" of "<<_scaleFactors.size()<<std::endl;
    _scaleFactors[currentPidIndex]=val;
  }
  bool ContainsPDG(const string& pdg){
    if(std::find(_pdgs.begin(),_pdgs.end(),pdg)!=_pdgs.end())
      return true;
    return false;
  }
  
  const std::string& CurrentPid() const {
    return current_pdg;
  }

  std::string conf_name="configure";
  std::string training_dir= "training/"; //top level dir
 
  std::string acc_model="kerasDNN"; //name of the main acceptance model
  std::string reweight_model="BDT"; //name of the acceptance reweighting model
  std::string res_model="dtregress"; //name of the main resolution model
  bool acc_gaustrans = {true};
  bool use_reweights = {false};
  bool doing_sim = {false};


private:
  std::vector<Double_t > _scaleFactors; //one for each pdg training
  Int_t currentPidIndex=0;

  std::string current_pdg = "";  //current pdg name being processed
  std::vector<std::string> _pdgs; //store all pdg name that have been trained.

  std::string acc_dir= "acceptance/"; //acceptance training sub dir
  std::string reweight_dir= "reweight/"; //reweight training sub dir
  std::string res_dir= "resolution/"; //resolution training sub dir


  std::string sim_dir; //top directory for simulation output
  std::string particle_name; //!for testing in case only 1 simulation for all pdgs

  //Any TOject class used in py script needs ClassDef
  ClassDef(ConfigureSimulation,1);
};
