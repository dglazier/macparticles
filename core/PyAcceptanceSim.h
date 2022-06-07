#pragma once

#include <string>
#include <TPython.h>
#include <TBenchmark.h>
#include "AcceptanceSim.h"

using std::string;

class PyAcceptanceSim : public AcceptanceSim {

 public:
  
    PyAcceptanceSim(const ConfigureSimulation& con):AcceptanceSim{con}{
      
      if(con.acc_gaustrans)SetGaussianTransform();
      if(con.use_reweights)SetReweight(con.ReWeightDir()+con.reweight_model);
      SetModelName(con.acc_model);
      _accDir = con.AcceptanceDir().c_str();
      
     _pyDir = std::string(gSystem->Getenv("MACP"));
     if(_pyDir.empty()==false) _pyDir+="/python/";
   }

  void Track(DataLoader& df) override {
    if(OutputDir().String().Length()==0){
      std::cerr<<"PyAcceptanceSim::Track need an output directory"<<std::endl;
      exit(0);
    }
   
    TPython::Bind( &df, "df" );
    TPython::Bind( &_modelName, "model_name" );
    TPython::Bind( &_accDir, "acc_dir" );
    TPython::Bind( &OutputDir(), "out_dir" );
    TPython::Bind( &_do_gauss, "do_gauss" );
    TPython::Bind( &_do_bdt_reweight, "do_bdt_reweight" );
    TPython::Bind( &_reweight_name, "reweight_name" );
    
    gBenchmark->Start("tracking");
    TPython::LoadMacro( (_pyDir+Macro()).data() );
    gBenchmark->Stop("tracking");
    gBenchmark->Print("tracking");
  }

  void SetGaussianTransform(){_do_gauss="TRUE";}
  void SetPyMacro(const string& mac){_macro=mac;}
  const string& Macro(){return _macro;}
  void SetModelName(const string& mac){_modelName=mac.c_str();}
  void SetReweight(string mac){
    mac+=".joblib";
    _reweight_name=mac.c_str();
    _do_bdt_reweight="TRUE";
  }

private:
  
  TObjString _do_gauss="FALSE";
  TObjString _do_bdt_reweight="FALSE";
  TObjString _reweight_name="";
  TObjString _modelName = "";
  TObjString _accDir = "";
  std::string _macro = "KerasAcceptanceSim.py";
  std::string _pyDir;

};
