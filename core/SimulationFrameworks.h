#include "ConfigureSimulation.h"

void SimWithKerasAccDTRes(ConfigureSimulation& conf,DataLoader& data){


  //Check acceptances
  PyAcceptanceSim accSim(conf);
  accSim.Track(data);
  
  //Add the new weights to the data
  string accVar = "acceptance.accept";
  data.SetAccWeights("acceptance",(conf.SimulationDir()+"/simulation_acceptances.root"),"accept");

  //apply tracking resolutions
  DTResolutionSim resSim(conf);
  resSim.Track(data);
  
}
