#pragma once

#include "DataLoader.h"

class DataWithAcceptanceVar: public DataLoader{


 public:

  //case single file with generated and flagged accepted
 DataWithAcceptanceVar(std::string_view treename,std::string_view filename,std::string_view accVar):DataLoader(treename,filename){
    SetAcceptVar(accVar.data());	
  };
  /*
  ROOT::RDF::RNode  GetAcceptedFrame() override{
    //flagged accepted in input data
    return DataFrame().Filter(Form("%s==1",GetAcceptVar().c_str()));
  }
  ROOT::RDF::RNode  GetGeneratedFrame() override{
    //flagged accepted in input data
    //so return all events   
     return DataFrame();
  }
  ROOT::RDF::RNode  GetPredictionsFrame() override{
    //flagged accepted in input data
    return GetGeneratedFrame();
  }
  */

  ClassDefOverride(DataWithAcceptanceVar,1);

};

