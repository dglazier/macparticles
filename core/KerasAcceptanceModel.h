#pragma once

#include <TObject.h>
#include "PyAcceptanceModel.h"

class TKerasConfig: public TObject{

  public:

  Double_t learn_rate=1E-3;
  Int_t max_epochs=30;
  Int_t batch_size=1024;
  std::vector<Int_t > layout= {1024, 512 , 128, 64, 16, 8, 4};
  string activation="relu";
  string dnn_name= "kerasDNN";
  
  //Any TOject class used in py script needs ClassDef
  ClassDef(TKerasConfig,1);
};

class KerasAcceptanceModel : public PyAcceptanceModel {

 public :
  KerasAcceptanceModel(ConfigureSimulation& con, ProcessType type):PyAcceptanceModel{con,type}{
    if( type == ProcessType::ReWeight ){
      _config.dnn_name = con.reweight_model;
    }
    else if( type == ProcessType::Acceptance ){
      _config.dnn_name = con.acc_model;
    }
  }

  void Train(DataLoader* dl){
    TPython::Bind( &_config, "kerasconf" );
    PyAcceptanceModel::Train(dl);
  }
  
  void SetDNNName(const string& name){_config.dnn_name=name;}  //with default kerasDNN
  void SetMaxEpochs(Int_t mx){_config.max_epochs=mx;}  //with default 30
  void SetLearnRate(Double_t lr){_config.learn_rate=lr;} //with default 1E-3
  void SetNBatch(Int_t batch){_config.batch_size=batch;} //with default 1024
  void SetNetwork(std::vector<Int_t > layout){_config.layout=layout;} //with default ? 1024, 512 , 128, 64, 16, 8, 4
  void SetActivation(string act){_config.activation=act;} //with default relu

  void SetPyMacro(string mac){_macro=mac;}

  const string& Macro() final {return _macro;}
  
 private :
  
  string _macro = "KerasWithGaus.py";
  TKerasConfig _config;


};
