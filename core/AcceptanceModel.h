#pragma once

#include <TObjString.h>
#include <TSystem.h>
#include "DataLoader.h"
#include "ConfigureSimulation.h"

enum class ProcessType{Acceptance,ReWeight};
  
class AcceptanceModel {

 public:
  AcceptanceModel()=default;
  AcceptanceModel(ConfigureSimulation& con, ProcessType type):_organise{con}{
    _myType=type;
    
    if( type == ProcessType::ReWeight ){
      _modelSave=_organise.ReWeightDir().c_str();
      con.use_reweights = true;
      _organise.use_reweights = true;
    }
    else if( type == ProcessType::Acceptance ){
      _modelSave=_organise.AcceptanceDir().c_str();
    }
    gSystem->Exec(Form("mkdir %s",_modelSave.GetName() ));
  }
  
  virtual void Train(DataLoader& df)=0;
  
  void SetModelDir(const string& dir){
    _modelSave=dir.c_str();
    gSystem->Exec(Form("mkdir %s",dir.c_str() ));
  }
  TObjString* ModelDir() {return &_modelSave;}
  
  string ModelName(){
    return _myType==ProcessType::Acceptance ? Organise().acc_model:Organise().reweight_model;
  }
  
  const ConfigureSimulation& Organise(){return _organise;}

  Bool_t IsAcceptance(){
    return _myType== ProcessType::Acceptance;
  }
private :
  
  TObjString _modelSave;
  ConfigureSimulation _organise;
  ProcessType _myType= ProcessType::Acceptance;
  
};
