#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RDF/InterfaceUtils.hxx>
#include <TObject.h>
#include <TChain.h>

using std::string;

struct DataVariable{
  
  string name;
  Double_t min; //user defined min
  Double_t max; //user define max
  Double_t diffOffset; //DataLoader calculated offset in difference 
  Double_t diffRange;  //DataLoader calculated difference range


};

class DataLoader: public TObject{ //for TPython

  //Consider internal ROOT Tree with
  //branches tP1,tP2,tP3,rP1,rP2,rP3,Accepted,PDG
  using varnames_t = std::vector<std::string > ;
  using datavars_t = std::vector<DataVariable > ;
  
 public:
  DataLoader(std::string_view treename,std::string_view filename):_chain(treename.data()),_df{new ROOT::RDataFrame(_chain)},_currNode{(*_df.get())}{
    _chain.Add(filename.data());
  };

  ROOT::RDF::RNode  DataFrame(){return _currNode;}
  TChain& Chain(){return _chain;} //add more files etc

  
  void SetTruthVars(const varnames_t& vars){_truthVars=vars;}
  
  void DetailedTruthVars(const datavars_t& vars){
    //Set truth values here if you want to use plotter
    _dtruthVars=vars;
    Int_t index=0;
    for(const auto& var:_dtruthVars){
      _varIndex[var.name]=index++;
      _truthVars.push_back(var.name);
    }
  }

  void TakeDetailedVarsInfo( datavars_t vars){

    if(vars.size()!=_truthVars.size())
      Fatal("DataLoader::DetailedVarsInfo","Number of training vars do not match simulation vars.");

    for(UInt_t i=0;i<vars.size();++i){
      std::cout<< "DataLoader::TakeDetailedVarsInfo mapping new variable  = " <<_truthVars[i] <<" , to training variable " <<vars[i].name <<std::endl;
      vars[i].name = _truthVars[i];
    }
    //now set the detailed vars for use in simulation
    _truthVars.clear(); //they will get filled again.
    DetailedTruthVars(vars); 
  }
  
  void SetDiffVars(const varnames_t& vars){_diffVars=vars;}
  void SetReconVars(const varnames_t& vars){_reconVars=vars;}
  void SetAcceptVar(const string& var){_acceptVar=var;}
  void SetFastAcceptVar(const string& var){_fastAccVar=var;}
  void SetPDGVar(const string& var){_pdgVar=var;}
  
  varnames_t GetTruthVars()const {return _truthVars;}
  varnames_t GetReconVars()const {return _reconVars;}
  varnames_t GetDiffVars()const {return _diffVars;}
  varnames_t GetRandVars()const {return _randVars;}
  varnames_t GetNormTruthVars()const {return _normTruthVars;}
  varnames_t GetNormDiffVars()const {return _normDiffVars;}
  string GetAcceptVar()const {return _acceptVar;}
  string GetPDGVar()const {return _pdgVar;}

  const DataVariable& GetDataVar(const string& var) {return _dtruthVars[_varIndex[var]];}
  datavars_t GetDetailedVars()const {return _dtruthVars;}
  
  string GetAcceptCondition(Int_t val=1){return Form("%s==%d",_acceptVar.c_str(),val);}
  string GetInitAcceptCondition(Int_t val=1){
    //if initial accept defined filter on ==1, else always accept 
    return _fastAccVar.empty() ? "1" : Form("%s==%d",_fastAccVar.c_str(),val);
  }
  
  //Previously determined modelled fast weights
  string GetFastAcceptVar()const {return _fastAccVar;}
  string GetFastAcceptWeight()const {return _fastAccWeight;}
  string GetFastReweightVar()const {return _fastReWeight;}
  
  void Range(Long64_t min, Long64_t max){
    _currNode = DataFrame().Range(min,max);
    _rangeMin=min;
    _rangeMax=max;
  }
  
  ROOT::RDF::RNode  GetAcceptedFrame(){
    _currNode=DataFrame().Filter(Form("%s==1",GetAcceptVar().c_str()));
    return _currNode;
  }

  void DefineColumn(std::string_view name, std::string_view expression){
    _currNode=DataFrame().Define(name,expression);
  }

  
  void AddUniformRandomColumn(std::string_view name,Double_t min=0,Double_t max=0){
    _randVars.push_back(std::string(name));
    DefineColumn(name,Form("gRandom->Uniform(%lf,%lf)",min,max));
  }

  
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
      auto range = 2*5*(*sigmas[i]);//+- 5 sigma
      auto off = *(means[i]);
      DefineColumn(normname,Form("(%s-(%lf))/%lf",diffname.c_str(),off,range));

      std::cout<<" AddDifferenceVars() using off "<<off<< " range "<<range <<" for "<<normname<<endl;
      _dtruthVars[i].diffRange=range;
      _dtruthVars[i].diffOffset=off;
    }
    
  }
  Double_t GetNormDiffRange(Int_t i) const {return  _dtruthVars[i].diffRange;}
  Double_t GetNormDiffOff(Int_t i) const {return _dtruthVars[i].diffOffset;}
  
  void AddNormalisedTruthVars(){
   
   if(_dtruthVars.empty())
       Fatal("DataLoader::NormaliseTruthVars()","No detailed truth variables defined, need ranges for normalisation");
   
   //convert truth values to range 0-1
    for(UInt_t i=0;i<_dtruthVars.size();++i){
      auto var=_dtruthVars[i];
      auto range = var.max-var.min;
      auto name=var.name +"_NORM";
      std::replace( name.begin(), name.end(), '.', '_');
      _normTruthVars.push_back(name);
      DefineColumn(name,Form("(%s-(%lf))/%lf",var.name.c_str(),var.min,range));
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
  void SetFastAccWeights(const string& treeName,const string& fileName,const string& accName="fast_accept",const string& weightName="fast_weight"){
    //For fine tuning, first run 1 classifier then use its weights here
    //Results of this training can reweight prior to get more accurate results
    _priorAccWeights.push_back(std::unique_ptr<TChain>{new TChain{treeName.c_str()} });
    
    _priorAccWeights.back()->Add(fileName.c_str());
    Chain().AddFriend(_priorAccWeights.back().get());
    SetFastAcceptVar(treeName+"."+accName); //mask
    _fastAccWeight=treeName+"."+weightName; //probability
 
    ResetWithChain();
  }
    void SetFastReWeights(const string& treeName,const string& fileName,const string& weightName="fast_weight"){
      _fastReWeight=treeName+"."+weightName;
      _priorAccWeights.push_back(std::unique_ptr<TChain>{new TChain{treeName.c_str()} });   
      _priorAccWeights.back()->Add(fileName.c_str());

      Chain().AddFriend(_priorAccWeights.back().get());
       
      ResetWithChain();
    }
  void AddFriend(const string& treeName,const string& fileName){
      _priorAccWeights.push_back(std::unique_ptr<TChain>{new TChain{treeName.c_str()} });   
      _priorAccWeights.back()->Add(fileName.c_str());

      Chain().AddFriend(_priorAccWeights.back().get());
       
      ResetWithChain();
 
  }
  
  void ResetWithChain(){
    //now we have a friend we need to recreate the dataframe and node
    _df.reset(new ROOT::RDataFrame(_chain));
    _currNode=*(_df.get());

    //Make sure to pass one any configurations
    if(_rangeMin>=0) Range(_rangeMin,_rangeMax);
  }

  
 private:

  TChain _chain;
  std::vector<std::unique_ptr<TChain>> _priorAccWeights;
  
  // ROOT::RDataFrame _df;
  std::unique_ptr<ROOT::RDataFrame> _df;
  ROOT::RDF::RNode _currNode;

  std::map<string,int> _varIndex;
  
  datavars_t _dtruthVars;
  varnames_t _truthVars;
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
  Long64_t _rangeMin=-1;
  Long64_t _rangeMax=-1;

  std::vector<Double_t> _diffNormRange;
  std::vector<Double_t> _diffNormOff;
  
  ClassDef(DataLoader,1);
 
};
