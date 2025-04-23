#pragma once

#include <TCanvas.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TBenchmark.h>
#include "DataLoader.h"
#include "ConfigureSimulation.h"

class AcceptancePlotter {
  
  using dfth1ds_t = std::vector<ROOT::RDF::RResultPtr<TH1D> >;
  
 public:
 AcceptancePlotter(DataLoader& data,ConfigureSimulation& conf):_data{&data},_conf{&conf}{
    _saveDir=_conf->AcceptanceDir()+"/acceptance_plots/";
    gSystem->Exec(Form("mkdir -p %s",_saveDir.data()));
  }

  AcceptancePlotter(DataLoader& data,ConfigureSimulation& conf, Double_t CanvasWidth, double_t CanvasHeight):_data{&data},_conf{&conf}{
    _saveDir=_conf->AcceptanceDir()+"/acceptance_plots/";
    gSystem->Exec(Form("mkdir -p %s",_saveDir.data()));
    _canvasWidth=CanvasWidth;
    _canvasHeight=CanvasHeight;
  }
  

  //  void DefinePlots(const std::string& varname,ROOT::RDF::RNode plotter){
  void DefinePlots(const std::string& varname,ROOT::RDF::RNode plotAcc,ROOT::RDF::RNode plotGen){
    auto& var = _data->GetDataVar(varname);
    auto isReweight=_data->GetFastReweightVar().empty()==false;

    auto hfast=plotGen.Filter(Form("gRandom->Uniform()<%s",_data->GetFastAcceptWeight().c_str())).Histo1D({"fast","fast",100,var.min,var.max},var.gen_name);
    //auto hacc=plotter.Filter("accepted").Histo1D({var.name.c_str(),var.name.c_str(),100,var.min,var.max},var.name);
    auto hacc=plotAcc.Histo1D({var.acc_name.c_str(),var.acc_name.c_str(),100,var.min,var.max},var.acc_name);
  
    _hfast.push_back(hfast);
    _hacc.push_back(hacc);
    if(isReweight){
      auto hrew=plotGen.Filter(Form("gRandom->Uniform()<(%s*%s)",_data->GetFastAcceptWeight().c_str(),_data->GetFastReweightVar().c_str())).Histo1D({"reweight","reweight",100,var.min,var.max},var.gen_name);
      _hrew.push_back(hrew);
    }

  }
  void RunPlots(Int_t nrows=1){

    Int_t nplots = _hfast.size();
    auto nvars=_data->GetTruthVars().size();

    _canvas=new TCanvas(Form("%d_of_%d_in_%s",_islice+1,_nslices,_sliceVar.data()),Form("%d_of_%d_in_%s",_islice+1,_nslices,_sliceVar.data()),100,100,_canvasWidth,_canvasHeight); //ROOT will delete this
    auto ncols=std::ceil(((float)nplots)/nrows/_nslices);
     _canvas->Divide(ncols,nrows);

    auto isReweight=_data->GetFastReweightVar().empty()==false;
   
    for(UInt_t i=0;i<nvars;++i){
      _canvas->cd(i+1);
    
      _hacc[_islice*nvars+i]->SetMinimum(0);
      _hacc[_islice*nvars+i]->SetLineWidth(2);
      _hacc[_islice*nvars+i]->DrawCopy();
      _hfast[_islice*nvars+i]->SetLineColor(2);
      _hfast[_islice*nvars+i]->SetLineWidth(2);
      _hfast[_islice*nvars+i]->DrawCopy("same");
      
      if(isReweight){
	_hrew[_islice*nvars+i]->SetLineColor(3);
	_hrew[_islice*nvars+i]->SetLineWidth(2);
	_hrew[_islice*nvars+i]->DrawCopy("same");
      }
    }
    _canvas->SaveAs((_saveDir+_canvas->GetName()+".png").data());
    //leave interactive root to delete canvas, so still visible
  }
  
  void Plot(const std::string& varname){
    auto& var = _data->GetDataVar(varname);
    // _data->LoadGenerated();
    auto plotter=_data->DataFrame();
    //plotter.Define("random1", [](){return gRandom->Uniform();});
    auto hfast=plotter.Filter("gRandom->Uniform()<kerasDNN.fast_weight").Histo1D({"fast","fast",100,var.min,var.max},var.gen_name);
    auto hrew=plotter.Filter("gRandom->Uniform()<BDT.fast_weight*kerasDNN.fast_weight").Histo1D({"reweight","reweight",100,var.min,var.max},var.gen_name);
    auto hacc=plotter.Filter(_data->GetAcceptVar()).Histo1D({"sim","sim",100,var.min,var.max},var.acc_name);
   
    hacc->DrawCopy();
    hacc->SetMinimum(0);
    hfast->SetLineColor(2);
    hfast->DrawCopy("same");
    hrew->SetLineColor(3);
    hrew->DrawCopy("same");
  }

  void PlotTruthVars(Int_t nrows=1){
    cout<<"AccpetancePlotter::PlotTruthVars "<<Form("gRandom->Uniform()<(%s*%s)",_data->GetFastAcceptWeight().c_str(),_data->GetFastReweightVar().c_str())<<endl;
    //take advantage of lazy execution
    //define all histograms first
    _nslices=1;
    _islice=0;

    gBenchmark->Start("plotting");
    //auto plotter=_data->DataFrame();
    auto plotAcc=_data->GetAcceptedFrame(); //original simulation
    auto plotGen=_data->GetGeneratedFrame();

    auto vars=_data->GetTruthVars();
    for(const auto& var:vars)
      DefinePlots(var,plotAcc,plotGen);
    //now process data
    RunPlots(nrows);

    gBenchmark->Stop("plotting");
    gBenchmark->Print("plotting");

  }
  
  //  void PlotSliceVars(const string& sliceVar,Int_t nrows=1){
  void PlotSliceVars(Int_t index,Int_t nrows=1){
    //take advantage of lazy execution
    //define all histograms first
    _nslices=10;
    gBenchmark->Start(Form("slice plotting %s",_sliceVar.data()));

    auto var = _data->GetDataVar(index);
    _sliceVar = var.acc_name;
    auto sliceWidth = (var.max-var.min)/_nslices;
   
    for(_islice=0;_islice<_nslices;++_islice){
     
      auto plotterAcc=_data->GetAcceptedFrame();
      plotterAcc = plotterAcc.Filter(Form("%s>%lf&&%s<%lf",var.acc_name.c_str(),var.min+_islice*sliceWidth,var.acc_name.c_str(),var.min+(_islice+1)*sliceWidth));
      auto plotterGen=_data->GetGeneratedFrame();
      plotterGen = plotterGen.Filter(Form("%s>%lf&&%s<%lf",var.gen_name.c_str(),var.min+_islice*sliceWidth,var.gen_name.c_str(),var.min+(_islice+1)*sliceWidth));

     
      auto vars=_data->GetTruthVars();
      for(const auto& var2:vars)
	DefinePlots(var2,plotterAcc,plotterGen);
    }
    //now process data
    for(_islice=0;_islice<_nslices;++_islice){
      RunPlots(nrows);
    }
    gBenchmark->Stop(Form("slice plotting %s",_sliceVar.data()));
    gBenchmark->Print(Form("slice plotting %s",_sliceVar.data()));
  }


private :
  
  DataLoader* _data=nullptr;
  ConfigureSimulation* _conf=nullptr;
  TCanvas* _canvas=nullptr;
  dfth1ds_t _hfast;
  dfth1ds_t _hacc;
  dfth1ds_t _hrew;

  Int_t _islice=0;
  Int_t _nslices=1;
  std::string _sliceVar;
  std::string _saveDir;
  Double_t _canvasWidth{700};
  Double_t _canvasHeight{500};
};
