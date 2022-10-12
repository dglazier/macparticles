{
  ROOT::RDataFrame df("simtree", "toy_training.root");
  df.Range(0,1E6);
  auto acc = df.Filter("accepted==1");
  acc.Snapshot("simtree", "acc_training.root");
  df.Snapshot("simtree", "gen_training.root");

}
