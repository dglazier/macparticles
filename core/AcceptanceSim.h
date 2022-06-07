#pragma once

#include <TObjString.h>
#include <TSystem.h>
#include "DataLoader.h"
#include "ConfigureSimulation.h"

class AcceptanceSim {

 public:
  AcceptanceSim()=default;

  AcceptanceSim(const ConfigureSimulation& con):_organise{con}{
    SetOutputDir(con.SimulationDir());
  }

  virtual void Track(DataLoader& df)=0;

  void SetOutputDir(const string& mac){
    _outDir=(mac+'/').c_str();
    gSystem->Exec(Form("mkdir -p %s",_outDir.GetName() ));
  }

  TObjString& OutputDir(){return _outDir;}
  
private :
  
  ConfigureSimulation _organise;
  TObjString _outDir;

};
