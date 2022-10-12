#pragma once

#include <TObject.h>
#include "PyAcceptanceModel.h"
#include "TBDTConfig.h"


class BDTAcceptanceModel : public PyAcceptanceModel {

 public :
  BDTAcceptanceModel(ConfigureSimulation& con, ProcessType type):PyAcceptanceModel{con,type}{
    if( type == ProcessType::ReWeight ){
      // _config.bdt_name = con.reweight_model;
    }
    else if( type == ProcessType::Acceptance ){
      //  _config.bdt_name = con.acc_model;
    }
  }

  void Train(DataLoader* df){
  
    //TPython::Bind
    PythonBinding::Instance().PythonBindOnce( &_config, "bdtconf" );
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
  TBDTConfig _config={};

  string _macro = "BDT.py";
 

};
