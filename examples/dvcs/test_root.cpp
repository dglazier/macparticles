#include <iostream>
#include <TFile.h>
#include <TTree.h>

using namespace std;

void test_root() {

    //string treeLoc="/w/work/clas12/singh/HighLevelAN/outputs/simulations/rga/rga_spring2018/dvcsgen/new_inbeding_0na_10594/fulldata/Gen_Spring2018_dvcs_0na_inbend_full_data.root";
    string treeLoc="/w/work/clas12/singh/HighLevelAN/outputs/simulations/rga/rga_spring2018/dvcsgen/new_inbeding_0na_10594/example_files/Gen_Spring2018_dvcs_0na_inbend_example.root";

    TFile *outputFile= TFile::Open((treeLoc).c_str());
    TTree *outputTree = (TTree *) outputFile->Get("PhysicsEvents");//els

    int NEvents=outputTree->GetEntries();

    cout<<NEvents<<endl;

}
