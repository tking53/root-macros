#include <iostream>
#include <fstream>
#include <string>

TChain * chain;
TProof * pr;

int createTChain(std::string iMergerListName = "mergerList.txt")
{
  pr = TProof::Open("lite://","workers=16");
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
  
  return 0;
}
