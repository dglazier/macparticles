#pragma once

#include <TCanvas.h>
#include <TSystem.h>
#include "DataLoader.h"
#include "ConfigureSimulation.h"

class ResolutionPlotter {
  
  using dfth1ds_t = std::vector<ROOT::RDF::RResultPtr<TH1D> >;
  using dfth2ds_t = std::vector<ROOT::RDF::RResultPtr<TH2D> >;
  using names_t = std::vector<std::string>;

 public:
 ResolutionPlotter(DataLoader& data,ConfigureSimulation& conf):_data{&data},_conf{&conf}{
    _saveDir=_conf->ResolutionDir()+"/resolution_plots/";
    gSystem->Exec(Form("mkdir -p %s",_saveDir.data()));
  }

  ResolutionPlotter(DataLoader& data,ConfigureSimulation& conf, Double_t CanvasWidth, double_t CanvasHeight, vector<Double_t> diffRanges):_data{&data},_conf{&conf}{
    _saveDir=_conf->ResolutionDir()+"/resolution_plots/";
    gSystem->Exec(Form("mkdir -p %s",_saveDir.data()));
    _canvasWidth=CanvasWidth;
    _canvasHeight=CanvasHeight;
    _diffRanges=diffRanges;
    //cout<<"Canvas Width "<<_canvasWidth<<" Height "<<_canvasHeight<<endl;
  }

  void AddReconstructedCols(const string& fileDir){
    auto vars=_data->GetDetailedVars();

    names_t cachevars;
    for(const auto& var:vars){
      //add fast resolution variable to dataframe
      auto fast_var = _data->CopyColumnFromTree("recon",fileDir+"predictions.root",var.rec_name);
      //define its difference column with truth variable
      std::string fastDiffName = var.rec_name+"Diff";
      _data->DefineColumn(fastDiffName,var.acc_name+"-"+ fast_var);
      //define original difference column with truth variable (from full simulation)
      std::string origDiffName = var.rec_name+"DiffReal";
      _data->DefineColumn(origDiffName,var.acc_name+"-"+ var.rec_name);

      _fullFastVars.push_back(fast_var);cachevars.push_back(fast_var);
      _fullOrigVars.push_back(var.rec_name);cachevars.push_back(var.rec_name);
      _diffFastVars.push_back(fastDiffName);cachevars.push_back(fastDiffName);
      _diffOrigVars.push_back(origDiffName);cachevars.push_back(origDiffName);
    }
    _data->Cache(cachevars);

  }
 
  void PlotTraining(){
    _saveDir=_conf->ResolutionDir();
    AddReconstructedCols(_saveDir);
    PlotResolutions();
  }
  void PlotSimulation(){
    _fastOnly=kTRUE;
    _saveDir=_conf->SimulationDir();
     AddReconstructedCols(_saveDir);
     PlotResolutions();

  }

  void PlotResolutions(){
    // auto accEvents = _data->GetPredictionsFrame();
   auto accEvents = _data->GetAcceptedFrame();


   auto vars=_data->GetDetailedVars();

   std::vector<Double_t> valStdev;
   for(UInt_t i=0;i<vars.size();++i){
     auto& var = vars[i];
     auto stdev= accEvents.StdDev(_diffFastVars[i].data());
     valStdev.push_back(*stdev);
     cout<<"resolution "<<_diffFastVars[i]<< *stdev<<endl;
   }
  
  for(UInt_t i=0;i<vars.size();++i){
     auto& var = vars[i];
      // plot histrograms via RDataFrame
     //1D full distributions
     auto h1d = accEvents.Histo1D({_fullFastVars[i].data(),vars[i].title.data(),200,var.min,var.max},_fullFastVars[i].data());
     _vhistFullFast.push_back(h1d);
     if(_fastOnly==kFALSE) {
       h1d = accEvents.Histo1D({_fullOrigVars[i].data(),vars[i].title.data(),200,var.min,var.max},_fullOrigVars[i].data());
       _vhistFullOrig.push_back(h1d);
     }

      Double_t drange=0.5*valStdev[i];
      if(_diffRanges.size()!=0){
        drange=_diffRanges[i];
      }

     //1D resolutions
     //need to find max and min values
     h1d = accEvents.Histo1D({_diffFastVars[i].data(),(string("Fast #Delta")+vars[i].title).data(),100,-drange,drange},_diffFastVars[i].data());
     _vhistDiffFast.push_back(h1d);
     if(_fastOnly==kFALSE) {
       h1d = accEvents.Histo1D({_diffOrigVars[i].data(),(string("Original #Delta")+vars[i].title).data(),100,-drange,drange},_diffOrigVars[i].data());
       _vhistDiffOrig.push_back(h1d);
     }
     //2D resolutions versus variable
     auto h2d = accEvents.Histo2D<Double_t, Double_t >({(_diffFastVars[i]+"2D").data(),(string("Fast #Delta")+vars[i].title+" v "+vars[i].title).data(),50,var.min,var.max,50,-drange,drange}, _fullFastVars[i].data() ,_diffFastVars[i].data());
     _vhist2DDiffFast.push_back(h2d);
     if(_fastOnly==kFALSE) {
       h2d = accEvents.Histo2D({(_diffOrigVars[i]+"2D").data(),(string("Original #Delta")+vars[i].title+" v "+vars[i].title).data(),50,var.min,var.max,50,-drange,drange}, _fullOrigVars[i].data(), _diffOrigVars[i].data());
       _vhist2DDiffOrig.push_back(h2d);
     }

  }

  _canvas=new TCanvas(Form("FullVariables"),Form("FullVariables"),100, 100, _canvasWidth,_canvasHeight); //ROOT will delete this  
  auto ncols=std::ceil(((float)vars.size())/3); //max 3 columns
  _canvas->Divide(3,ncols);

  //Plot the full variables
   for(UInt_t i=0;i<vars.size();++i){
     _canvas->cd(i+1);
     if(_fastOnly==kFALSE) {
       _vhistFullOrig[i]->SetMinimum(0.1);
       _vhistFullOrig[i]->SetLineWidth(2);
       _vhistFullOrig[i]->DrawCopy();
       _vhistFullFast[i]->SetLineColor(2);
       _vhistFullFast[i]->SetLineWidth(2);
       _vhistFullFast[i]->DrawCopy("same");
     }
     else{
       _vhistFullFast[i]->SetMinimum(0.1);
       _vhistFullFast[i]->SetLineColor(2);
       _vhistFullFast[i]->SetLineWidth(2);
       _vhistFullFast[i]->DrawCopy("");

     }
   }
   _canvas->SaveAs((_saveDir+_canvas->GetName()+".png").data());
  
   _canvas=new TCanvas(Form("DeltaVariables"),Form("DeltaVariables"),100, 100, _canvasWidth,_canvasHeight); //ROOT will delete this  
   _canvas->Divide(3,ncols);
  //Plot the resolution variables
   for(UInt_t i=0;i<vars.size();++i){
     _canvas->cd(i+1);
     if(_fastOnly==kFALSE) {
       _vhistDiffOrig[i]->SetMinimum(0.1);
       _vhistDiffOrig[i]->SetLineWidth(2);
       _vhistDiffOrig[i]->DrawCopy();
       _vhistDiffFast[i]->SetLineColor(2);
       _vhistDiffFast[i]->SetLineWidth(2);
       _vhistDiffFast[i]->DrawCopy("same");
     }
     else{
       _vhistDiffFast[i]->SetMinimum(0.1);
       _vhistDiffFast[i]->SetLineColor(2);
       _vhistDiffFast[i]->SetLineWidth(2);
       _vhistDiffFast[i]->DrawCopy("");
     }
   }
  
   _canvas->SaveAs((_saveDir+_canvas->GetName()+".png").data());
  

   _canvas=new TCanvas(Form("FastVariables2D"),Form("FastVariables2D"),100, 100, _canvasWidth,_canvasHeight); //ROOT will delete this  
   auto _canvas2=new TCanvas(Form("OrigVariables2D"),Form("OrigVariables2D"),100, 100, _canvasWidth,_canvasHeight); //ROOT will delete this  

   _canvas->Divide(3,ncols);
   _canvas2->Divide(3,ncols);
  //Plot the 2Dresolution variables
   for(UInt_t i=0;i<vars.size();++i){
     _canvas->cd(i+1);
     if(_fastOnly==kFALSE) {
       _vhist2DDiffFast[i]->SetMinimum(0);
       _vhist2DDiffOrig[i]->SetMinimum(0);
       _vhist2DDiffOrig[i]->DrawCopy("col");
       _canvas2->cd(i+1);
       _vhist2DDiffFast[i]->DrawCopy("col");
     }
     else{
       _vhist2DDiffFast[i]->SetMinimum(0);
       _vhist2DDiffFast[i]->DrawCopy("col");
     }
   }
  
   _canvas->SaveAs((_saveDir+_canvas->GetName()+".png").data());

  }
 private:
  
  DataLoader* _data=nullptr;
  ConfigureSimulation* _conf=nullptr;
  TCanvas* _canvas=nullptr;
  std::string _saveDir;
  names_t _fullFastVars;
  names_t _fullOrigVars;
  names_t _diffFastVars;
  names_t _diffOrigVars;
  dfth1ds_t _vhistFullFast;
  dfth1ds_t _vhistFullOrig;
  dfth1ds_t _vhistDiffFast;
  dfth1ds_t _vhistDiffOrig;
  dfth2ds_t _vhist2DDiffFast;
  dfth2ds_t _vhist2DDiffOrig;
  Double_t _canvasWidth{700};
  Double_t _canvasHeight{500};
  vector<Double_t> _diffRanges;
  
Bool_t _fastOnly=kFALSE;
};
