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
    dl1.SetTruthVars({"truth.pimP","truth.pimTheta","truth.pimPhi"});
    //these branches willl be created
    dl1.SetReconVars({"pimP","pimTheta","pimPhi"});

  }
  else if(pid=="pi+"){
    dl1.SetTruthVars({"truth.pipP","truth.pipTheta","truth.pipPhi"}); 
    //these branches willl be created
    dl1.SetReconVars({"pipP","pipTheta","pipPhi"});

  }
  else if(pid=="proton"){
    dl1.SetTruthVars({"truth.pP","truth.pTheta","truth.pPhi"});  
    //these branches willl be created
    dl1.SetReconVars({"pP","pTheta","pPhi"});
  }
  else{
    Fatal("RunSimulation","This particle %s is not recgonised ",pid.Data());
  }
  
  //and run fast simulation
  SimWithKerasAccDTRes(config,dl1);

 
   
}
