#include <iostream>
#include <fstream>
#include <string>

TChain * chain;
TProof * pr;

int createTChain(std::string iMergerListName = "mergerList5.txt")
{
  pr = TProof::Open("lite://","workers=32");
  //chain = new TChain("tree");
  chain = new TChain("mergedData");
  std::ifstream mergerListFiles( iMergerListName.c_str() );
  std::string tempName("");
  
  while ( getline (mergerListFiles,tempName) ) 
  {
    chain->AddFile( tempName.c_str() );
    std::cout << tempName.c_str() << std::endl;
  }
  
  mergerListFiles.close();
  std::cout << "Number of TTrees added to the chain: " << chain->GetNtrees() << std::endl;

  chain->SetProof();
  gProof->AddInput(new TParameter<Double_t>("test",30));
  gProof->AddInput(new TParameter<std::string>("teststr",std::string("test")));
  
  return 0;
}
