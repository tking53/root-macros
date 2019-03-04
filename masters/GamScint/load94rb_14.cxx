#include <iostream>
#include <fstream>
#include <string>

#include "TSystem.h"
#include "TObject.h"
#include "TROOT.h"
#include "TProof.h"


TChain *DDchain;

TProof *pr;

int load94rb_14(){

  DDchain= new TChain("PixTree");

  pr = TProof::Open("lite://","workers=16");
  for (auto it=0; it<=99 ; it++){
    std::stringstream ss;
    ss <<"/SCRATCH/DRunScratch1/TK/rootFile/094rb_14_FILE"<<it<<"_DD.root";
    DDchain->Add(ss.str().c_str());
  }
  
  gDirectory->Add(DDchain);
  
  DDchain->SetProof();
  std::cout << "Number of TTrees added to DDchain: " << DDchain->GetNtrees() << std::endl;
  
  return 0;
}
