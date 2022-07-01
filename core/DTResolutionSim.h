#pragma once

#include <string>
#include <TPython.h>
#include "ResolutionSim.h"
#include "DecisionTreeResolModel.h"


class DTResolutionSim : public ResolutionSim {

 public:

  DTResolutionSim(const ConfigureSimulation& con):ResolutionSim{con}{
    
    SetModelName(con.res_model);
    _resDir = con.ResolutionDir().c_str();
 
    _pyDir = std::string(gSystem->Getenv("MACP"));
    if(_pyDir.empty()==false) _pyDir+="/python/";
  }

  
  void Track(DataLoader& dl) override {
    gBenchmark->Start("resolution tracking");
    //cout<<"Going to add friend "<<OutputDir().String()<<endl;
    if(_macro.empty()){
      std::cerr<<"DTResolutionSimSim::Track need py macro"<<std::endl;
      exit(0);
    }
    //Get saved configuration info from training

    auto file=std::unique_ptr<TFile>{ TFile::Open(_resDir.String()+"config.root") };
    auto conf = file->Get<TDTResConfig>("TDTResConfig");
    dl.TakeDetailedVarsInfo(conf->detailVars); //do not copy new names just ranges

    //use rdataframe to create required columns
    dl.AddNormalisedTruthVars(); //onto range 0-1

     
    TPython::Bind( conf, "dtconf" );
    TPython::Bind( &dl, "df" );
    TPython::Bind( &_modelName, "model_name" );
    TPython::Bind( &_resDir, "save_dir" );
    TPython::Bind( &OutputDir(), "out_dir" );
     
    TPython::LoadMacro( (_pyDir+Macro()).data() );
    gBenchmark->Stop("resolution tracking");
    gBenchmark->Print("resolution tracking");

    //Add as friend so can make plots in this session
    dl.AddFriend("recon",(OutputDir().String()+"predictions.root").Data());
  }

  const std::string& Macro(){return _macro;}
  void SetModelName(const std::string& mac){_modelName=mac.c_str();}

 

private:
  
  TObjString _modelName = "";
  TObjString _resDir = "";
  std::string _macro = "DTResPred.py";
  std::string _pyDir;

};
