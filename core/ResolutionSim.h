#pragma once

#include <TObjString.h>
#include <TSystem.h>
#include "DataLoader.h"
#include "ConfigureSimulation.h"

class ResolutionSim {

 public:

  ResolutionSim(const ConfigureSimulation& con):_organise{con}{
    gSystem->Exec(Form("mkdir -p  %s",con.SimulationDir().c_str() ));
    SetOutputDir(con.SimulationDir());
  }

  virtual void Track(DataLoader* df)=0;
  
  void SetModelDir(const string& dir){
    _modelSave=dir.c_str();
  }
  TObjString* ModelDir() {return &_modelSave;}
  
  const ConfigureSimulation& Organise(){return _organise;}

  void SetOutputDir(const string& mac){
    _outDir=((mac+'/')).c_str();
    gSystem->Exec(Form("mkdir -p %s",_outDir.GetName()));
  }
  TObjString& OutputDir(){return _outDir;}
  
private :
  
  TObjString _modelSave;
  TObjString _outDir;
  ConfigureSimulation _organise;

};
