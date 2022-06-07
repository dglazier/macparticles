#pragma once

#include <TCanvas.h>
#include <TRandom.h>
#include "DataLoader.h"

class AcceptancePlotter {
  
  using dfth1ds_t = std::vector<ROOT::RDF::RResultPtr<TH1D> >;
  
 public:
  AcceptancePlotter(DataLoader& data):_data{&data}{}
  

  void DefinePlots(const std::string& varname,ROOT::RDF::RNode plotter){
    auto& var = _data->GetDataVar(varname);
    auto isReweight=_data->GetFastReweightVar().empty()==false;
    //    auto plotter=_data->DataFrame();
    //plotter = plotter.Define("random1", [](){return gRandom->Uniform();});
    //auto hfast=plotter.Filter("random1<kerasDNN.fast_weight").Histo1D({"fast","fast",100,var.min,var.max},var.name);
    //auto hrew=plotter.Filter("random1<BDT.fast_weight*kerasDNN.fast_weight").Histo1D({"reweight","reweight",100,var.min,var.max},var.name);
    auto hfast=plotter.Filter(Form("gRandom->Uniform()<%s",_data->GetFastAcceptWeight().c_str())).Histo1D({"fast","fast",100,var.min,var.max},var.name);
    auto hacc=plotter.Filter("accepted").Histo1D({var.name.c_str(),var.name.c_str(),100,var.min,var.max},var.name);
    // auto hacc=plotter.Filter([](bool acc){return acc;},{_data->GetAcceptVar()}).Histo1D({"sim","sim",100,var.min,var.max},var.name);
    // auto hfast=plotter.Filter([](float weight,double rand) { return rand<weight; }, {"kerasDNN.fast_weight","random1"}).Histo1D({"fast","fast",100,var.min,var.max},var.name);
    // auto hrew=plotter.Filter([](float weight1,float weight2,double rand) { return rand<(weight1*weight2); }, {"kerasDNN.fast_weight","BDT.fast_weight","random1"}).Histo1D({"reweight","reweight",100,var.min,var.max},var.name);
 
    _hfast.push_back(hfast);
    _hacc.push_back(hacc);
    if(isReweight){
      auto hrew=plotter.Filter(Form("gRandom->Uniform()<(%s*%s)",_data->GetFastAcceptWeight().c_str(),_data->GetFastReweightVar().c_str())).Histo1D({"reweight","reweight",100,var.min,var.max},var.name);
      _hrew.push_back(hrew);
    }

  }
  void RunPlots(Int_t nrows=1){

    Int_t nplots = _hfast.size();
    auto nvars=_data->GetTruthVars().size();

    _canvas=new TCanvas(); //ROOT will delete this
    auto ncols=std::ceil(((float)nplots)/nrows/_nslices);
     _canvas->Divide(ncols,nrows);

    auto isReweight=_data->GetFastReweightVar().empty()==false;
   
    for(UInt_t i=0;i<nvars;++i){
      _canvas->cd(i+1);
    
      _hacc[_islice*nvars+i]->SetMinimum(0);
      _hacc[_islice*nvars+i]->DrawCopy();
      _hfast[_islice*nvars+i]->SetLineColor(2);
      _hfast[_islice*nvars+i]->DrawCopy("same");
      
      if(isReweight){
       _hrew[_islice*nvars+i]->SetLineColor(3);
      _hrew[_islice*nvars+i]->DrawCopy("same");
      }
    }
  }
  
  void Plot(const std::string& varname){
    auto& var = _data->GetDataVar(varname);
    auto plotter=_data->DataFrame();
    //plotter.Define("random1", [](){return gRandom->Uniform();});
    auto hfast=plotter.Filter("gRandom->Uniform()<kerasDNN.fast_weight").Histo1D({"fast","fast",100,var.min,var.max},var.name);
    auto hrew=plotter.Filter("gRandom->Uniform()<BDT.fast_weight*kerasDNN.fast_weight").Histo1D({"reweight","reweight",100,var.min,var.max},var.name);
    auto hacc=plotter.Filter(_data->GetAcceptVar()).Histo1D({"sim","sim",100,var.min,var.max},var.name);
   
    hacc->DrawCopy();
    hacc->SetMinimum(0);
    hfast->SetLineColor(2);
    hfast->DrawCopy("same");
    hrew->SetLineColor(3);
    hrew->DrawCopy("same");
  }

  void PlotTruthVars(Int_t nrows=1){
    cout<<"Pot "<<Form("gRandom->Uniform()<(%s*%s)",_data->GetFastAcceptWeight().c_str(),_data->GetFastReweightVar().c_str())<<endl;
    //take advantage of lazy execution
    //define all histograms first
    _nslices=1;
    _islice=0;

    auto plotter=_data->DataFrame();
    
    auto vars=_data->GetTruthVars();
    for(const auto& var:vars)
      DefinePlots(var,plotter);
    //now process data
    RunPlots(nrows);
  }
  
  void PlotSliceVars(const string& sliceVar,Int_t nrows=1){
    //take advantage of lazy execution
    //define all histograms first
   _nslices=10;
   auto var = _data->GetDataVar(sliceVar);
  
   auto sliceWidth = (var.max-var.min)/_nslices;
   
   for(_islice=0;_islice<_nslices;++_islice){
     
     auto plotter=_data->DataFrame();
     plotter = plotter.Filter(Form("%s>%lf&&%s<%lf",sliceVar.c_str(),var.min+_islice*sliceWidth,sliceVar.c_str(),var.min+(_islice+1)*sliceWidth));

     
     auto vars=_data->GetTruthVars();
     for(const auto& var:vars)
       DefinePlots(var,plotter);
    }
    //now process data
    for(_islice=0;_islice<_nslices;++_islice){
     RunPlots(nrows);
   }
 }

  
  // void PlotTruthVars(Int_t nrows=1){
  //   _canvas=new TCanvas(); //ROOT will delete this
  //   auto vars=_data->GetTruthVars();
  //   auto nvars=vars.size();
  //   auto ncols=std::ceil(((float)nvars)/nrows);
  //   _canvas->Divide(ncols,nrows);
  //   Int_t ipad=1;
  //   for(const auto& var:vars){
  //     _canvas->cd(ipad++);
  //     Plot(var);
  //   }
  // }
  
private :
  
  DataLoader* _data=nullptr;
  TCanvas* _canvas=nullptr;
  dfth1ds_t _hfast;
  dfth1ds_t _hacc;
  dfth1ds_t _hrew;

  Int_t _islice=0;
  Int_t _nslices=1;
};
