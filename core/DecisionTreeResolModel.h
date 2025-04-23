#pragma once

#include <string>
#include <TPython.h>
#include <TBenchmark.h>
#include "ResolutionModel.h"

class TDTResConfig: public TObject{
  using datavars_t = std::vector<DataVariable > ;
 
  public:


  UInt_t n_rand_in=1;
  UInt_t n_trees=1;
  UInt_t n_regs=10;
  std::string model_name= "dt";
  std::vector<std::string> x_vars; //default 1
  datavars_t detailVars; //contains variable scaling info
  Bool_t applyExtraScale=false;
  //Any TOject class used in py script needs ClassDef
  ClassDef(TDTResConfig,1);
};

class DecisionTreeResolModel : public ResolutionModel {
 
 public:
  
  DecisionTreeResolModel(ConfigureSimulation& con):ResolutionModel{con}{
    _pyDir = std::string(gSystem->Getenv("MACP"));
    if(_pyDir.empty()==false) _pyDir+="/python/";

  }

  void AddFilter(std::string opt){
    //  _currNode = DataFrame().Filter(opt.data());
    if(std::find(_filters.begin(),_filters.end(),opt)==_filters.end())
      _filters.push_back(opt);
  }
  

  void Train(DataLoader* dl) override {
      
    if(_filters.size()!=0){
      for(string filter: _filters){
        dl->Filter(filter);
      }
    }

    dl->InitTrainingData();

    gBenchmark->Start("resolution");

    
    //use rdataframe to create required columns
    dl->AddNormalisedTruthVars(); //onto range 0-1
    dl->AddDifferenceVars();  //truth-rec
    _config.detailVars=dl->GetDetailedVars(); //copy for use in simulation
     
    TPython::Bind( &_config, "dtconf" );
    TPython::Bind( dl, "df" );
    TPython::Bind( ModelDir(), "save_dir" );
    TPython::Bind( ModelDir(), "out_dir" );

    //Do the training
    TPython::LoadMacro((_pyDir+ _trainPy).c_str() );
    
    gBenchmark->Stop("resolution");
    gBenchmark->Print("resolution");
    
    gBenchmark->Start("predictions");

    //Make predictions for the training data
    TPython::LoadMacro( (_pyDir+_predictionPy).c_str() );
    
    gBenchmark->Stop("predictions");
    gBenchmark->Print("predictions");

    //Now done training save configuration for use in simulations.
    auto file=std::unique_ptr<TFile>{ TFile::Open(ModelDir()->String()+"config.root","recreate") };
    _config.Write();

 
  }
  
  void AddRandomInputs(DataLoader* dl){
    for(UInt_t i=0;i<_config.n_rand_in;++i){
      auto rname=Form("rand_in%d",i);
      dl->AddUniformRandomColumn(rname,0,1);
      _config.x_vars.push_back(rname);
    }
  }
  
  void SetNTrees(Int_t mx){_config.n_trees=mx;}  //with default 1
  void SetNRegressors(Int_t mx){_config.n_regs=mx;}  //with default 10
  void SetNRandInputs(Int_t mx){_config.n_rand_in=mx;}  //with default 1

  void SetTrainPy(const std::string& py){_trainPy=py;}
  void SetPredictPy(const std::string& py){_predictionPy=py;}
  
  void SetApplyExtraScaling(){_config.applyExtraScale=true;}


private:
  
  std::string _trainPy = "DTRes.py";
  std::string _predictionPy = "DTResPred.py";
  std::string _pyDir;
  std::vector<std::string> _filters;

  TDTResConfig _config;
  
};
