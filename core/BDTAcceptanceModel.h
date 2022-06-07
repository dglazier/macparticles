#pragma once

#include <TObject.h>
#include "PyAcceptanceModel.h"

class TBDTConfig: public TObject{

  public:
  Int_t n_estimators = 200;
  Int_t max_depth=10;
  Double_t learning_rate=0.1;
  Double_t min_impurity_decrease=1;
  Int_t verbose=1;
  string bdt_name= "BDT";
  
  //Any TOject class used in py script needs ClassDef
  ClassDef(TBDTConfig,1);
};

class BDTAcceptanceModel : public PyAcceptanceModel {

 public :
  BDTAcceptanceModel(ConfigureSimulation& con, ProcessType type):PyAcceptanceModel{con,type}{
    if( type == ProcessType::ReWeight ){
      _config.bdt_name = con.reweight_model;
    }
    else if( type == ProcessType::Acceptance ){
      _config.bdt_name = con.acc_model;
    }
  }

  void Train(DataLoader& df){
    TPython::Bind( &_config, "bdtconf" );
    PyAcceptanceModel::Train(df);
  }
  
  void SetBDTName(const string& name){_config.bdt_name=name;}  //with default BDT
  void SetNEstimators(Int_t est){_config.n_estimators=est;}  //with default 100
  void SetMaxDepth(Int_t mx){_config.max_depth=mx;}  //with default 10
  void SetLearnRate(Double_t lr){_config.learning_rate=lr;} //with default 1E-2
  void SetMinImpurityDecrease(Double_t mn){_config.min_impurity_decrease=mn;} //with default 1
  void SetVerbose(Int_t ver){_config.verbose=1;}  //with default 1

  void SetPyMacro(string mac){_macro=mac;}

  const string& Macro() final {return _macro;}
  
 private :
  
  string _macro = "BDT.py";
  TBDTConfig _config;


};
