{
  ConfigureSimulation config;
  config.Load("fast_simulation/");
  //output directory for simulated data
  config.Simulate("results/");

  //Soret the pid
  auto pid = GetPID();
  if(config.ContainsPDG(pid.Data()))
    config.UsePdg(pid.Data());
  else{
    config.UsePdg("all");
    config.PretendPdg(pid.Data());
  }
  //configure the data loader for requested particle
  DataLoader  dl1("tree", "toy_reaction.root");
   if(pid=="pi-"){
    //variable ranges should match those from training
    dl1.DetailedTruthVars({{"truth.pimP",0,10},
			   {"truth.pimTheta",0,TMath::Pi()},
			   {"truth.pimPhi",-TMath::Pi(),TMath::Pi()}});
  
    //these branches willl be created
    dl1.SetReconVars({"pimP","pimTheta","pimPhi"});

  }
  else if(pid=="pi+"){

    //variable ranges should match those from training
    dl1.DetailedTruthVars({{"truth.pipP",0,10},
			   {"truth.pipTheta",0,TMath::Pi()},
			   {"truth.pipPhi",-TMath::Pi(),TMath::Pi()}});
  
    //these branches willl be created
    dl1.SetReconVars({"pipP","pipTheta","pipPhi"});

  }
  else if(pid=="proton"){
    //variable ranges should match those from training
    dl1.DetailedTruthVars({{"truth.pP",0,10},
			   {"truth.pTheta",0,TMath::Pi()},
			   {"truth.pPhi",-TMath::Pi(),TMath::Pi()}});
    
    //these branches willl be created
    dl1.SetReconVars({"pP","pTheta","pPhi"});
    
  }
  else{
    Fatal("RunSimulation","This particle %s is not recgonised ",pid.Data());
  }

   //and run fast simulation
   SimWithKerasAccDTRes(config,dl1);

 
   
}
