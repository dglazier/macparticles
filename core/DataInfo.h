#pragma once

#include "DataLoader.h"
#include "DataWithAccAndGenFiles.h"
#include "DataWithAcceptanceVar.h"

class DataInfo : public TNamed{
  using datavars_t = std::vector<DataVariable > ;

  public:
  DataInfo()=default;
 DataInfo(const std::string& name):particle(name){}
 DataInfo(const std::string& name,const std::string& filename){
   std::cout<<"DataInfo  getting "<<name<<" from "<<filename<<std::endl;
     auto file = std::unique_ptr<TFile>(TFile::Open(filename.data()));
     *this = *file->Get<DataInfo>(name.data());
   }
   //   const std::string objectName{"DataInfo"}; 
   std::string particle;
   std::string treeName;
   std::string reconFile;
   std::string genFile;
   std::string acceptVar;
   std::string recFilter;
   std::string saveDir;

   Double_t recFrac{1.0};
   Double_t genFrac{1.0};
   
   datavars_t  variables;

   ///////////////////////////////////////////////////////////////////////////////
   std::unique_ptr<DataLoader>  TrainingData(){
     std::unique_ptr<DataLoader> dataLoader;

     if(genFile.empty()==false){
       dataLoader.reset(new DataWithAccAndGenFiles(treeName,reconFile,genFile));
       dynamic_cast<DataWithAccAndGenFiles*>(dataLoader.get())->SetAccFractionToProcess(recFrac);
       dynamic_cast<DataWithAccAndGenFiles*>(dataLoader.get())->SetGenFractionToProcess(genFrac);
     }
     else{
       dataLoader.reset(new DataWithAcceptanceVar(treeName,reconFile,acceptVar));
       dynamic_cast<DataWithAcceptanceVar*>(dataLoader.get())->SetFractionToProcess(recFrac);
     }
     dataLoader->ConfigVars(variables);
     dataLoader->Filter(recFilter);
     return std::move(dataLoader);
   }
   ///////////////////////////////////////////////////////////////////////////////
   void WriteTo(const std::string& filename){
     auto file = std::unique_ptr<TFile>(new TFile(filename.data(),"UPDATE"));
     if(file->IsOpen()==kFALSE)
       file.reset(new TFile(filename.data(),"recreate"));
     cout<<"file "<<file.get()<<" "<<file->IsOpen()<<endl;
     //     file->WriteTObject(this,particle.data());
     Write(particle.data());
   }

 private:


    ClassDef( DataInfo,1);
 };
 
