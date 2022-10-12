#pragma once

#include <string>
#include <TPython.h>
#include <TBenchmark.h>
#include "AcceptanceModel.h"
#include "PythonBinding.h"

using std::string;

class PyAcceptanceModel : public AcceptanceModel {

 public:
  
  PyAcceptanceModel(ConfigureSimulation& con, ProcessType type):AcceptanceModel{con,type}{

    if(con.acc_gaustrans)_do_gauss="TRUE";
    else _do_gauss="FALSE";

    _pyDir = std::string(gSystem->Getenv("MACP"));
    if(_pyDir.empty()==false) _pyDir+="/python/";
  }

  void Train(DataLoader* dl) override {
    //if reweighting apply acceptance weights 
    /* if(IsReWeight()==kTRUE)  */
    /*   SetFastAcceptanceWeights(dl); */

    dl->InitTrainingData();
    TPython::Bind( dl, "df" );
    TPython::Bind( ModelDir(), "save_dir" );
    TPython::Bind( &_do_gauss, "do_gauss" );
    
    gBenchmark->Start(ModelName().c_str());

    if(_dontTrain==false)TPython::LoadMacro( (_pyDir + Macro()).data() );
    
    gBenchmark->Stop(ModelName().c_str());
    gBenchmark->Print(ModelName().c_str());

    //Give weights to the data
    //Note "training_acceptances" is fixed in py script SnapShot
    if(IsAcceptance()==kTRUE) SetFastAcceptanceWeights(dl);
    else  SetFastReWeights(dl);

    Finish(dl);
  }
  virtual const string& Macro()  = 0;

 
  void SetGaussianTransform(){_do_gauss="TRUE";}
  void DontTrain(){_dontTrain=true;}
  void SetSecondPass(){ _firstPass=kFALSE;}
private:
  
  TObjString _do_gauss="FALSE";
  std::string _pyDir;
  Bool_t _dontTrain=kFALSE;
  Bool_t _firstPass=kTRUE;

};
