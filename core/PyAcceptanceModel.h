#pragma once

#include <string>
#include <TPython.h>
#include <TBenchmark.h>
#include "AcceptanceModel.h"

using std::string;

class PyAcceptanceModel : public AcceptanceModel {

 public:
  
  PyAcceptanceModel(ConfigureSimulation& con, ProcessType type):AcceptanceModel{con,type}{

    if(con.acc_gaustrans)_do_gauss="TRUE";
    else _do_gauss="FALSE";

    _pyDir = std::string(gSystem->Getenv("MACP"));
    if(_pyDir.empty()==false) _pyDir+="/python/";
  }

  void Train(DataLoader& dl) override {
    
    gBenchmark->Start(ModelName().c_str());

    TPython::Bind( &dl, "df" );
    TPython::Bind( ModelDir(), "save_dir" );
    TPython::Bind( &_do_gauss, "do_gauss" );
    TPython::LoadMacro( (_pyDir + Macro()).data() );
    
    gBenchmark->Stop(ModelName().c_str());
    gBenchmark->Print(ModelName().c_str());

    //Give weights to the data
    //Note "training_acceptances" is fixed in py script SnapShot
    if(IsAcceptance()==kTRUE)
      dl.SetFastAccWeights(ModelName(),(ModelDir()->String()+"training_acceptances.root").Data());
    else
      dl.SetFastReWeights(ModelName(),(ModelDir()->String()+"training_acceptances.root").Data());
  
  }
  virtual const string& Macro()  = 0;

  void SetGaussianTransform(){_do_gauss="TRUE";}
  
private:
  
  TObjString _do_gauss="FALSE";
  std::string _pyDir;
  
};
