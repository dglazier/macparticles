#pragma once

#include "DataLoader.h"
#include "DataWithAccAndGenFiles.h"
#include "DataWithAcceptanceVar.h"

class TrainingInfo : public TNamed{
  using datavars_t = std::vector<DataVariable > ;

  public:
  TrainingInfo()=default;
 TrainingInfo(const std::string& name):particle(name){}
 TrainingInfo(const std::string& name,const std::string& filename){
   std::cout<<"TrainingInfo:  getting "<<name<<" from "<<filename<<std::endl;
   auto file = std::unique_ptr<TFile>(TFile::Open(filename.data()));
   *this = *file->Get<TrainingInfo>(name.data());
 }
  
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
     if(recFilter.empty()==false)dataLoader->Filter(recFilter);
     return std::move(dataLoader);
   }
   ///////////////////////////////////////////////////////////////////////////////
   void WriteTo(const std::string& filename){
     auto file = std::unique_ptr<TFile>(new TFile(filename.data(),"UPDATE"));
     if(file->IsOpen()==kFALSE)
       file.reset(new TFile(filename.data(),"recreate"));
     Write(particle.data());
     std::cout<<"TrainingInfo:  wrote info for "<<particle <<" to "<<filename<<std::endl;
   }
   ///////////////////////////////////////////////////////////////////////////////
 
 private:


    ClassDef( TrainingInfo,1);
 };
 
