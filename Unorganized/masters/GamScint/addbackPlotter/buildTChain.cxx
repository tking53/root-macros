#include <iostream>
#include <fstream>
#include <string>

#include "TSystem.h"
#include "TObject.h"
#include "TROOT.h"
#include "TProof.h"

TChain * chain;
TProof * pr;

//int buildTChain(std::string iChainListName="/home/hanayo/research/ornl2016/thesis/rootFiles/94rbSets/94rb_14_set1/094rb_14_5scan_set1-GammaScintPAB.root", Double_t NGT=0, Double_t SHGT=0,Double_t NSeW=0,Double_t SHSeW=0)

int buildTChain()
{
  // pr = TProof::Open("lite://","workers=6");
  //chain = new TChain("tree");
  chain = new TChain("GSsinglesAB");
  chain->Add("/home/hanayo/research/ornl2016/thesis/rootFiles/testFiles/single94file-GammaScintPAB.root");
  /*
  std::ifstream chainListFiles( iChainListName.c_str() );
  std::string tempName("");
  
  while ( getline (chainListFiles,tempName) ) 
    {
      chain->AddFile( tempName.c_str() );
      std::cout << tempName.c_str() << std::endl;
    }
  
    chainListFiles.close();

    std::cout << "Number of TTrees added to the chain: " << chain->GetNtrees() << std::endl;
*/
  /*
  chain->SetProof();
  gProof->AddInput(new TParameter<Double_t>("NGT",30));
  gProof->AddInput(new TParameter<Double_t>("NSeW",1.e-6));
  gProof->AddInput(new TParameter<Double_t>("SHGT",30));
  gProof->AddInput(new TParameter<Double_t>("SHSeW",1.e-6));
  
  //chain->Process("root-macrosAddbackCalculator.C+");
  */
  return 0;
}
