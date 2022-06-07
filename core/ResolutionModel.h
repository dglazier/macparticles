#pragma once

#include <TObjString.h>
#include <TSystem.h>
#include "DataLoader.h"
#include "ConfigureSimulation.h"

class ResolutionModel {
  
 
 public:
  ResolutionModel(ConfigureSimulation& con):_organise{con}{
    SetModelDir(_organise.ResolutionDir());
  }

  ResolutionModel()=default;
  
  virtual void Train(DataLoader& df)=0;
  
  void SetModelDir(const string& dir){
    _modelSave=dir.c_str();
    gSystem->Exec(Form("mkdir %s",dir.c_str() ));
  }
  TObjString* ModelDir() {return &_modelSave;}
  const ConfigureSimulation& Organise(){return _organise;}


private :
  
  TObjString _modelSave;
  ConfigureSimulation _organise;
 
};
