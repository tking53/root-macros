#include <iostream>
#include <fstream>
#include <string>

#include "TSystem.h"
#include "TObject.h"
#include "TROOT.h"
#include "TProof.h"


TChain *chain1;
TChain *chain2;
Bool_t abfile;

TProof *pr;

int loadGStchain(){

    pr = TProof::Open("lite://","workers=7");
    chain1 = new TChain("GSsingles");
    chain2 = new TChain("GSaddback");
  
    for (auto it=1; it<=5 ; it++){
      
      std::stringstream ss;
      ss << "/mnt/Data/Paass-Scans/94rb/mk4_94rb/94rb_14_set"<<it<<"/094rb_14_5scan_set"<<it<<"-GammaScint.root";
      chain1->Add(ss.str().c_str());
      chain2->Add(ss.str().c_str());
      
    }
    gDirectory->Add(chain1);
    gDirectory->Add(chain2);
    //    chain1->AddFriend(chain2);

    chain1->SetProof();
    chain2->SetProof();
    std::cout << "Number of TTrees added to chain1: " << chain1->GetNtrees() << std::endl;
    std::cout << "Number of TTrees added to chain2: " << chain2->GetNtrees() << std::endl;
    
    return 0;
}
