#pragma once

#include "DataLoader.h"

class DataWithAccAndGenFiles: public DataLoader{

 public:

  //case one file each for generated and accepted
  // DataWithAccAndGenFiles(std::string_view treename,std::string_view accname,std::string_view genname,Long64_t NGenEvents=-1):DataLoader(treename,accname), _genChain{treename.data()},_genDf{new ROOT::RDataFrame(_genChain)}{
 DataWithAccAndGenFiles(std::string_view treename,std::string_view accname,
			std::string_view genname)
   :DataLoader(treename,accname), 
    _genData{treename.data(),genname}{
    
  }
  DataWithAccAndGenFiles(std::string_view treename,std::string_view accname)
   :DataLoader(treename,accname), 
    _genData{"",""}{
    _onlyAccepted=kTRUE;
  }

  ROOT::RDF::RNode  GetAcceptedFrame() override{
    return DataFrame();
  }
  ROOT::RDF::RNode  GetGeneratedFrame() override{
    return _genData.DataFrame();
  }
  ROOT::RDF::RNode  GetPredictionsFrame() override{
    //add dummy accpeted column to tree for masking (so it will not mask any)
    if(_addAcceptedCol==kFALSE){
      std::string accName= "macpAccepted";
      SetAcceptVar(accName);
      DefineColumn(accName,"1");
      _addAcceptedCol=kTRUE;
    }
    //flagged accepted in input data
    return GetAcceptedFrame();
  }

  void SetGenFractionToProcess(Double_t frac){
    _genData.SetFractionToProcess(frac);
  }
  void SetAccFractionToProcess(Double_t frac){
    SetFractionToProcess(frac);
  }

  //Previously determined modelled fast weights
  string GetFastAcceptVar()const  override{return _genData.GetFastAcceptVar();}
  string GetFastAcceptWeight()const  override{return _genData.GetFastAcceptWeight();}
  string GetFastReweightVar()const  override{return _genData.GetFastReweightVar();}
  string GetInitAcceptCondition(Int_t val=1)  override{
    return _genData.GetInitAcceptCondition(val);
  }
 
 /* void ResetWithChain() override{ */
 /*   DataLoader::ResetWithChain(); */
 /*   _genData.ResetWithChain(); */
 /*    //now we have a friend we need to recreate the dataframe and node */
 /*   cout<<"DataWithAccAndGenFiles::ResetWithChain()"<<endl; */
 /*   /\* if(_genrangeMax>-1)_genChain.SetEntries(_genrangeMax); *\/ */
 /*   /\* //  _genChain.Print(); *\/ */
 /*   /\* _genDf.reset(new ROOT::RDataFrame(_genChain)); *\/ */

 /*    //_currNode=*(_df.get()); */
 /*    //Make sure to pass one any configurations */
 /*    //if(_rangeMin>=0) Range(_rangeMin,_rangeMax); */
 /*    //for(auto& opt:_filters) */
 /*   // Filter(opt); */
 /*  } */
   void SetFastAccWeights(const string& treeName,const string& fileName,const string& accName="fast_accept",const string& weightName="fast_weight") override{
    //For fine tuning, first run 1 classifier then use its weights here
    //Results of this training can reweight prior to get more accurate results
    
     _genData.SetFastAccWeights(treeName,fileName,accName,weightName);
     /*
     _priorAccWeights.push_back(std::unique_ptr<TChain>{new TChain{treeName.c_str()} });
    
    _priorAccWeights.back()->Add(fileName.c_str());
    _genChain.AddFriend(_priorAccWeights.back().get());
    SetFastAcceptVar(treeName+"."+accName); //mask
    SetFastAcceptWgt(treeName+"."+weightName); //probability
 
    ResetWithChain();
     */
  }
    void SetFastReWeights(const string& treeName,const string& fileName,const string& weightName="fast_weight") override{
      _genData.SetFastReWeights(treeName,fileName,weightName);
      /*SetFastReWeight(treeName+"."+weightName);
      _priorAccWeights.push_back(std::unique_ptr<TChain>{new TChain{treeName.c_str()} });   
      _priorAccWeights.back()->Add(fileName.c_str());

      _genChain.AddFriend(_priorAccWeights.back().get());
       
      ResetWithChain();*/
    }

    Double_t ScaleFactor()const override{return DataLoader::ScaleFactor()/_genData.ScaleFactor();}
//   void SetScaleFactor(Double_t fact) override{
//      cout<<"DataWithAccAndGenFiles setscale factor "<<fact<<endl;
//     _genScaleFactor=fact;}


 public:
    void InitTrainingData() override{
      DataLoader::InitTrainingData();
      if(_onlyAccepted==kFALSE)_genData.InitTrainingData();
    }

    /* void Range(Long64_t min, Long64_t max) override{ */
    /*   auto fact = DataLoader::ScaleFactor(); */
    /*   if(fact==1){ */
    /* 	cout<<"DataWithAccAndGen::Range set scale factor to "<<fact*Chain().GetEntries()/(max-min)<<" from "<<fact<<endl; */
    /* 	DataLoader::SetScaleFactor(fact*Chain().GetEntries()/(max-min)); */
    /*   } */
    /*   DataLoader::Range(min,max); */
    /* } */

 private:

    DataLoader _genData;

    //  TChain _genChain;
    // std::unique_ptr<ROOT::RDataFrame> _genDf;
    Bool_t _addAcceptedCol=kFALSE;
    Bool_t _onlyAccepted=kFALSE;
    //Long64_t _genrangeMin=-1;
    //Long64_t _genrangeMax=-1;
    // Double_t _genScaleFactor=1.0;
 public:

  ClassDefOverride(DataWithAccAndGenFiles,1);

};

