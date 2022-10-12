{
using namespace ROOT;
using namespace ROOT::RDF;

 auto df = RDataFrame("tree","toy_reaction.root");

 //create dataset when pi+ detected
 auto pipdata = df.Filter("accepted_pip==1");
 //create pi+ histograms
 auto hisTruPip = pipdata.Histo1D( TH1DModel{"PipThetat","#pi+ #theta",100,0,1},{"truth.pipTheta"} );
 auto hisRecPip = pipdata.Histo1D( TH1DModel{"PipThetar","#pi+ #theta",100,0,1},{"recon.pipTheta"} );


 Double_t m1 = 0.13957000; //pion mass
 auto Px= [&m1](double pmag1, double theta1, double phi1){
   auto px = pmag1*sin(theta1)*cos(phi1);
   auto py = pmag1*sin(theta1)*sin(phi1);
   auto pz = pmag1*cos(theta1);
   ROOT::Math::PxPyPzMVector p1(px, py, pz, m1);
   return p1.Px(); //obviously I could just return px, but this is just an example.
 };

 auto pxdata = pipdata.Define("pipPx",Px,{"recon.pipP","recon.pipTheta","recon.pipPhi"});
 auto hisRecPx	 = pxdata.Histo1D( TH1DModel{"PipPx","#pi+ Px",100,0,10},{"pipPx"} );



 //Do all plotting at the end to take advantage 
 //of "lazy" execution
 hisTruPip->Draw();
 hisRecPip->Draw("same");

 new TCanvas;
 hisRecPx->Draw();

} 
