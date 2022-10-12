#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <TObject.h>
#include <TChain.h>
#include <TLeafD.h>

using std::string;

namespace MACP{
  enum class Type{Combined,Accepted,Generated};
}

struct DataVariable{
  
  string acc_name;  //accepted truth variables
  string gen_name; //in case different in generated branches
  string rec_name; //in case different in reconstructed branches
  string title;
  Double_t min=0.; //user defined min
  Double_t max=0.; //user define max
  Double_t diffOffset=0.; //DataLoader calculated offset in difference 
  Double_t diffRange=1.;  //DataLoader calculated difference range
  // Double_t extraScaling=1.;

};
class DataFromTreeBranch{
   
 public:
  DataFromTreeBranch(){};
  DataFromTreeBranch(const string& treeName,const string& fileName,const string& colName){
    std::unique_ptr<TFile> treeFile{TFile::Open(fileName.data())};
    auto tree=treeFile->Get<TTree>(treeName.data());
    if(tree==nullptr){
      std::cerr<<"DataLoader::CopyColumnFromTree no tree found "<<treeName<<std::endl;
      return;
    }
    auto leaf = dynamic_cast<TLeafD*>(tree->GetLeaf(colName.data()));
    if(leaf==nullptr){
      std::cerr<<"DataFromTreeBranch no leaf found "<<colName<<std::endl;
      return;
    }
    Double_t val=0;
    tree->SetBranchAddress(colName.data(),&val);
 
    auto maxEntry = tree->GetEntries();
 
    //cache all the entries in the branch to a local vector
    _extraBranchData.resize(maxEntry);
    for( Long64_t count=0;count<maxEntry;++count){
      tree->GetEntry(count);
      _extraBranchData[count]=val;
    }
    std::cout<<"GOING to add branch "<< colName<<"  with "<< _extraBranchData.size()<<std::endl;
  }
 
  std::function<double()> CopyColumn(){
    return [this](){
      //return value of cached branch values
      if(_extraBranchEntry>=static_cast<Long64_t>(_extraBranchData.size()))
	_extraBranchEntry=0; 
     
      auto val= _extraBranchData[_extraBranchEntry];
    
      ++_extraBranchEntry;

      return val;};
  }

 private:
  std::vector<double> _extraBranchData;
  Long64_t _extraBranchEntry=0;


};

class DataLoader: public TObject{ //for TPython

  //Consider internal ROOT Tree with
  //branches tP1,tP2,tP3,rP1,rP2,rP3,Accepted,PDG
  using varnames_t = std::vector<std::string > ;
  using datavars_t = std::vector<DataVariable > ;
  
 public:
 
 DataLoader(std::string_view treename,std::string_view filename)
    :_chain(treename.data()),
    _df{new ROOT::RDataFrame(_chain)},
    _currNode{(*_df.get())}{

      Chain().Add(filename.data());
      _nEntriesToProcess=Chain().GetEntries(); 
       //////////////////////
    };
 

  virtual ROOT::RDF::RNode  GetAcceptedFrame(){
    //flagged accepted in input data
    std::cout<<"DataLoader::GetAcceptedFrame() cut on "<<GetAcceptVar()<<std::endl;
    return DataFrame().Filter(Form("%s==1",GetAcceptVar().c_str()));
  }
  virtual ROOT::RDF::RNode  GetGeneratedFrame(){
    //flagged accepted in input data
    //so return all events   
     return DataFrame();
  }
  virtual ROOT::RDF::RNode  GetPredictionsFrame(){
    //flagged accepted in input data
    // return GetAcceptedFrame();
    return DataFrame(); 
  }


  ROOT::RDF::RNode  DataFrame(){return _currNode;}
 
  virtual void LoadGenerated(){
    ////////////////////////////
  }
  void LoadDataFrame( ROOT::RDF::RNode df){
    _currNode = df;
  }
  void SetTruthVars(const varnames_t& vars){_truthVars=vars;}
  
  void ConfigVars(const datavars_t& vars){
    //Set truth values here if you want to use plotter
    _dtruthVars=vars;
    Int_t index=0;
    for(auto& var:_dtruthVars){
      if(var.acc_name.empty()==true) var.acc_name=var.gen_name;
      _varIndex[var.acc_name]=index++;
      _truthVars.push_back(var.acc_name);
      _genVars.push_back(var.gen_name);
      _reconVars.push_back(var.rec_name);
    }
  }
  void SimVars(const datavars_t& vars){
    //Set truth values here if you want to use plotter
    _dtruthVars=vars;
    Int_t index=0;
    for(auto& var:_dtruthVars){
      var.acc_name=var.gen_name;
      _varIndex[var.acc_name]=index++;
      _truthVars.push_back(var.acc_name);
      _genVars.push_back(var.gen_name);
      _reconVars.push_back(var.rec_name);
    }
  }

  void TakeDetailedVarsInfo( datavars_t vars){

    if(vars.size()!=_truthVars.size())
      Fatal("DataLoader::DetailedVarsInfo","Number of training vars do not match simulation vars.");

    //keep given names, just want to change ranges etc.
    for(UInt_t i=0;i<vars.size();++i){
      std::cout<< "DataLoader::TakeDetailedVarsInfo mapping new variable  = " <<_truthVars[i] <<" , to training variable " <<vars[i].acc_name <<std::endl;
      vars[i].acc_name =  _truthVars[i];
      vars[i].gen_name =  _genVars[i];
      vars[i].rec_name =  _reconVars[i];
      //  vars[i].acc_name =vars[i].gen_name ;
    }
    //now set the detailed vars for use in simulation
    _truthVars.clear(); //they will get filled again.
    _genVars.clear(); //they will get filled again.
    _reconVars.clear(); //they will get filled again.
    ConfigVars(vars); 
  }
  
  void SetDiffVars(const varnames_t& vars){_diffVars=vars;}
  void SetReconVars(const varnames_t& vars){_reconVars=vars;}
  void SetAcceptVar(const string& var){_acceptVar=var;}
  void SetFastAcceptVar(const string& var){_fastAccVar=var;}
  void SetFastAcceptWgt(const string& var){_fastAccWeight=var;}
  void SetFastReWeight(const string& var){_fastReWeight=var;}
  void SetPDGVar(const string& var){_pdgVar=var;}
  
  varnames_t GetTruthVars()const {return _truthVars;}
  varnames_t GetGenVars()const {return _genVars;}
  varnames_t GetReconVars()const {return _reconVars;}
  varnames_t GetDiffVars()const {return _diffVars;}
  varnames_t GetRandVars()const {return _randVars;}
  varnames_t GetNormTruthVars()const {return _normTruthVars;}
  varnames_t GetNormDiffVars()const {return _normDiffVars;}
  string GetAcceptVar()const {return _acceptVar;}
  string GetPDGVar()const {return _pdgVar;}

  const DataVariable& GetDataVar(const string& var) {return _dtruthVars[_varIndex[var]];}
  const DataVariable& GetDataVar(Int_t index) {return _dtruthVars[index];}
  datavars_t GetDetailedVars()const {return _dtruthVars;}
  
  string GetAcceptCondition(Int_t val=1){return Form("%s==%d",_acceptVar.c_str(),val);}
  virtual string GetInitAcceptCondition(Int_t val=1){
    //if initial accept defined filter on ==1, else always accept 
    return _fastAccVar.empty() ? "1" : Form("%s==%d",_fastAccVar.c_str(),val);
  }
  
  //Previously determined modelled fast weights
  virtual string GetFastAcceptVar()const {return _fastAccVar;}
  virtual string GetFastAcceptWeight()const {return _fastAccWeight;}
  virtual string GetFastReweightVar()const {return _fastReWeight;}
  
   void Filter(std::string opt){
    //  _currNode = DataFrame().Filter(opt.data());
    if(std::find(_filters.begin(),_filters.end(),opt)==_filters.end())
      _filters.push_back(opt);

  }
  void SetFractionToProcess(Double_t frac){
    _processFraction= frac;
  }

  void DefineColumn(std::string_view name, std::string_view expression){
    _currNode=DataFrame().Define(name,expression);
  }

  
  void AddUniformRandomColumn(std::string_view name,Double_t min=0,Double_t max=0){
    _randVars.push_back(std::string(name));
    DefineColumn(name,Form("gRandom->Uniform(%lf,%lf)",min,max));
  }

  Int_t GetNRangeSigma()const {return _nRangeSigma;}

  void AddDifferenceVars(){
    if(_truthVars.empty())
      Fatal("DataLoader::AddDifferenceVars","No truth variables defined");
    if(_truthVars.size()!=_reconVars.size())
      Fatal("DataLoader::AddDifferenceVars","Number Reconstructed variables do not match truth");

    using result_double_t = ROOT::RDF::RResultPtr< ROOT::RDF::RDFDetail::MinReturnType_t< Double_t > >;

    std::vector<result_double_t> means;
    std::vector<result_double_t> sigmas;

    for(UInt_t i=0;i<_truthVars.size();++i){
      auto diffname=_truthVars[i]+"_m_"+_reconVars[i];
      _diffVars.push_back(diffname);
      DefineColumn(diffname,_truthVars[i]+"-"+_reconVars[i]);
      auto resultMean = DataFrame().Mean(diffname);
      means.push_back(resultMean);
      auto resultStdDev = DataFrame().StdDev(diffname);
      sigmas.push_back(resultStdDev);
    }

    //Now(wait so can use lazy execution) we find the range and scale to that
    //Produce normalised diff vars
    for(UInt_t i=0;i<_truthVars.size();++i){
      auto diffname=_truthVars[i]+"_m_"+_reconVars[i];
      auto normname = diffname+"_NORM";
      _normDiffVars.push_back(normname);
      auto range = _nRangeSigma*(*sigmas[i]);//+- 5 sigma
      auto off = *(means[i]);
      DefineColumn(normname,Form("(%s-(%lf))/%lf",diffname.c_str(),off,range));

      std::cout<<" AddDifferenceVars() using off "<<off<< " range "<<range <<" for "<<normname<<endl;
      _dtruthVars[i].diffRange=range;
      _dtruthVars[i].diffOffset=off;
    }
    
  }
  Double_t GetNormDiffRange(Int_t i) const {return  _dtruthVars[i].diffRange;}
  Double_t GetNormDiffOff(Int_t i) const {return _dtruthVars[i].diffOffset;}
  //void SetExtraResolutionScaling(Int_t i,Double_t val){_dtruthVars[i].extraScaling=val;}

 
  void AddNormalisedTruthVars(){
   
   if(_dtruthVars.empty())
       Fatal("DataLoader::NormaliseTruthVars()","No detailed truth variables defined, need ranges for normalisation");
   
   //convert truth values to range 0-1
    for(UInt_t i=0;i<_dtruthVars.size();++i){
      auto var=_dtruthVars[i];
      auto range = var.max-var.min;
      auto name=var.acc_name +"_NORM";
      std::replace( name.begin(), name.end(), '.', '_');
      _normTruthVars.push_back(name);
      std::cout<<"AddNormalisedTruthVars() for variable "<<var.acc_name<<std::endl;
      DefineColumn(name,Form("(%s-(%lf))/%lf",var.acc_name.c_str(),var.min,range));
    }
  }


  
  ROOT::RDF::RNode ParticleData(Int_t pdg=0){
    //if single particle type can return whole DF
    //return (ROOT::RDF::RNode)_df;
    return _pdgVar.empty() ? DataFrame() : DataFrame().Filter([pdg](int y) { return y == pdg; }, {_pdgVar});
  }

  void SetAccWeights(const string& treeName,const string& fileName,const string& accName="fast_accept",const string& weightName="fast_weight"){
    //For example for after simulated acceptances
    _priorAccWeights.push_back(std::unique_ptr<TChain>{new TChain{treeName.c_str()} });
    
    _priorAccWeights.back()->Add(fileName.c_str());
    Chain().AddFriend(_priorAccWeights.back().get());
    SetAcceptVar(treeName+"."+accName); //mask
 
    ResetWithChain();
  }
  virtual void SetFastAccWeights(const string& treeName,const string& fileName,const string& accName="fast_accept",const string& weightName="fast_weight"){
    //For fine tuning, first run 1 classifier then use its weights here
    //Results of this training can reweight prior to get more accurate results
    _priorAccWeights.push_back(std::unique_ptr<TChain>{new TChain{treeName.c_str()} });
    
    _priorAccWeights.back()->Add(fileName.c_str());
    Chain().AddFriend(_priorAccWeights.back().get());
    SetFastAcceptVar(treeName+"."+accName); //mask
    _fastAccWeight=treeName+"."+weightName; //probability
 
    ResetWithChain();
  }
  virtual void SetFastReWeights(const string& treeName,const string& fileName,const string& weightName="fast_weight"){
    _fastReWeight=treeName+"."+weightName;
    _priorAccWeights.push_back(std::unique_ptr<TChain>{new TChain{treeName.c_str()} });   
    _priorAccWeights.back()->Add(fileName.c_str());

    Chain().AddFriend(_priorAccWeights.back().get());
    
    ResetWithChain();
  }

  std::string CopyColumnFromTree(const string& treeName,const string& fileName,const string& colName){
 
    _extraBranches[_iExtra]=DataFromTreeBranch(treeName,fileName,colName);
    std::string newName=treeName+"__"+colName;
    std::cout<<"DataLoader::CopyColumnFromTree add column "<<newName<<std::endl;
    _currNode=DataFrame().Define(newName, _extraBranches[_iExtra].CopyColumn());
   
  //make sure we execute while tree is alive
    auto sumCheck = _currNode.Sum(newName);
  
    std::cout<<"DataLoader::CopyColumnFromTree check new column sum "<<*sumCheck<<std::endl;
    _iExtra++;
    return newName;
  }

  void AddFriend(const string& treeName,const string& fileName){
      _priorAccWeights.push_back(std::unique_ptr<TChain>{new TChain{treeName.c_str()} });   
      _priorAccWeights.back()->Add(fileName.c_str());

      Chain().AddFriend(_priorAccWeights.back().get());
      
      DataLoader::ResetWithChain();
 
  }
  void Cache(varnames_t cvars){
    cvars.push_back(GetAcceptVar());
    _currNode=_currNode.Cache(cvars);

  }
  virtual void ResetWithChain(){
    //now we have a friend we need to recreaste the dataframe and node
    _df.reset(new ROOT::RDataFrame(_chain));
    _currNode=*(_df.get());

    //Make sure to pass one any configurations
    /* if(_rangeMin>=0) Range(_rangeMin,_rangeMax); */
    /* for(auto& opt:_filters) */
    /*   Filter(opt); */
    _doneInitTraining=kFALSE;
    InitTrainingData();
  }

  virtual Double_t ScaleFactor()const {return _scaleFactor;}
  virtual void SetScaleFactor(Double_t fact){_scaleFactor=fact;}

   virtual void InitTrainingData(){
     if(_doneInitTraining) return;
     _doneInitTraining=kTRUE;
    //First apply any filters
    for(const auto& filt :_filters){
      _currNode = DataFrame().Filter(filt.data());	
    }
    //and scale by fraction to process
    _nEntriesToProcess=(*(_currNode.Count()))*_processFraction;
    SetScaleFactor(1./_processFraction);
    //then set the number to process
    Range(0,_nEntriesToProcess);
    std::cout<<"DataLoader::Init() will process "<<_nEntriesToProcess<<" event, from  "<<Chain().GetEntries()<<" in original tree"<<std::endl;

  }

protected:


  TChain& Chain(){return _chain;} //add more files etc
  std::vector<std::unique_ptr<TChain>> _priorAccWeights;
 
 virtual void Range(Long64_t min, Long64_t max){
    _currNode = DataFrame().Range(min,max);
    _rangeMin=min;
    _rangeMax=max;
  }

 private:
 
  TChain _chain;
  
  // ROOT::RDataFrame _df;
  std::unique_ptr<ROOT::RDataFrame> _df;
  ROOT::RDF::RNode _currNode;

  std::map<string,int> _varIndex;
  
  datavars_t _dtruthVars;
  varnames_t _truthVars;
  varnames_t _genVars;
  varnames_t _reconVars;
  varnames_t _diffVars;
  varnames_t _randVars;
  varnames_t _normTruthVars;
  varnames_t _normDiffVars;
  string _acceptVar;
  string _fastAccVar;
  string _fastAccWeight;
  string _fastReWeight;
  string _pdgVar;
  Double_t _scaleFactor=1.0;
  Long64_t _rangeMin=-1;
  Long64_t _rangeMax=-1;
  Long64_t _nEntriesToProcess=-1;
  Double_t _processFraction=1.0;
  std::vector<std::string> _filters;
 
  std::vector<Double_t> _diffNormRange;
  std::vector<Double_t> _diffNormOff;
  Bool_t _doneInitTraining=kFALSE;

  std::function<double(std::vector<double>&,Long64_t&)> _copyColumn;
  //std::vector<double> _extraBranchData;
  //Long64_t _extraBranchEntry=0;
  std::vector<DataFromTreeBranch> _extraBranches{3};
  Int_t _iExtra=0;
  Int_t _nRangeSigma=10; //2 * 5 sigma

  ClassDef(DataLoader,1);
 
};
