//Combine the various simulated particle trees into 1 for reaction analysis
void Combine(std::vector<std::string> dirs){

  if(dirs.empty()==true) return;

  //get the first tree
  auto file = unique_ptr<TFile>(TFile::Open((dirs[0]+"/predictions.root").data()));
  auto tree = file->Get<TTree>("recon");

  //add the rest as friends
  for(UInt_t i=0;i<dirs.size();++i){
    if(i>0)tree->AddFriend("recon",(dirs[i]+"/predictions.root").data());
    tree->AddFriend("acceptance",(dirs[i]+"/simulation_acceptances.root").data());
  }

  std::vector<std::string> branchNames;
  //get branch names from first tree
  for(auto br:*tree->GetListOfBranches()){
    cout<<"Adding branch "<<	br->GetName()<<endl;
    branchNames.push_back(br->GetName());
  }
  //get branch names from friend trees
  for(auto fr:*tree->GetListOfFriends()){
    for(auto br:*dynamic_cast<TTree*>(dynamic_cast<TFriendElement*>(fr)->GetTree())->GetListOfBranches()){
      cout<<"Adding friend branch "<<	br->GetName()<<endl;
      branchNames.push_back(br->GetName());
    }
  }	

  gBenchmark->Start("writing");
  ROOT::RDataFrame df(*tree);
  df.Snapshot("fastsim","combined.root",branchNames);
  gBenchmark->Stop("writing");
  gBenchmark->Print("writing");

}

void CombineParticleTrees(){
  //give the names of the simulated particle files
  Combine({"/scratch/dglazier/fast_simulation/resultsFT/Pip/","/scratch/dglazier/fast_simulation/resultsFT/Km/","/scratch/dglazier/fast_simulation/resultsFT/Kp/","/scratch/dglazier/fast_simulation/resultsFT/El/"});

  // Combine({"/scratch/dglazier/fast_simulation/fast_nkkpiFTAll_results/all/"});

}
