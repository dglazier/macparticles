#pragma once

#include <TObject.h>
#include <TPython.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class PythonBinding
{
 public:
  static PythonBinding& Instance()
  {
    static PythonBinding   instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
  }
 private:
  PythonBinding() {}                    // Constructor? (the {} brackets) are needed here.


 public:
  PythonBinding(PythonBinding const&)   = delete;
  void operator=(PythonBinding const&)  = delete;

  void PythonBindOnce(TObject* ptr,const std::string& label){
    if((std::find(_pythonLabels.begin(),_pythonLabels.end(),label))!=_pythonLabels.end()){
      std::cout<<"AcceptanceModel::PythonBind already loaded "<<label<<std::endl;
      return;
    }
    TPython::Bind( ptr, label.data() );
    _pythonLabels.push_back(label);

  }
  void PythonBindReload(TObject* ptr,const std::string& label){
    TPython::Bind( ptr, label.data() );
    _pythonLabels.push_back(label);

  }

 private:

  std::vector<std::string >_pythonLabels;

};
