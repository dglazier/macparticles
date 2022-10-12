#pragma once

#include <TObject.h>

class TBDTConfig : public TObject{


  public:

  Int_t n_estimators = 200;
  Int_t max_depth=10;
  Double_t learning_rate=0.1;
  Double_t min_impurity_decrease=1;
  Int_t verbose=1;
  string bdt_name= "BDT";
 
  //Any TOject class used in py script needs ClassDef
  ClassDef(TBDTConfig,0);
};
