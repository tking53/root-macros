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

int loadGStchain(const std::string paassFile="sing"){



  if (paassFile == "ab"){
    chain1 = new TChain("GSsinglesAB");
    abfile = true;
  } else{
    pr = TProof::Open("lite://","workers=6");
    chain1 = new TChain("GSsingles");
    chain2 = new TChain("GSaddback");
    abfile=false;
}

  
    for (auto it=1; it<=5 ; it++){
      if (abfile){
        std::stringstream ss;
        ss <<"/home/hanayo/research/ornl2016/thesis/interHolding/mk3_94rb/94rb_14_set"<<it<<"/094rb_14_5scan_set"<<it<<"-GammaScintPAB.root";
        chain1->Add(ss.str().c_str());
      }else{
        std::stringstream ss;
        ss << "/home/hanayo/research/ornl2016/thesis/interHolding/mk3_94rb/94rb_14_set"<<it<<"/094rb_14_5scan_set"<<it<<"-GammaScint.root";
        chain1->Add(ss.str().c_str());
        chain2->Add(ss.str().c_str());
      }
    }

    if (abfile){
      gDirectory->Add(chain1);
    }else {
      gDirectory->Add(chain1);
      gDirectory->Add(chain2);
      chain1->AddFriend(chain2);
    }

    if (abfile){
      // chain1->SetProof();
      std::cout << "Number of TTrees added to chain1: " << chain1->GetNtrees() << std::endl;
    } else {
      chain1->SetProof();
      chain2->SetProof();
      std::cout << "Number of TTrees added to chain1: " << chain1->GetNtrees() << std::endl;
      std::cout << "Number of TTrees added to chain2: " << chain2->GetNtrees() << std::endl;
    }

    //  chain->Add("/home/hanayo/research/ornl2016/thesis/rootFiles/94rbSets/94rb_14_set1/094rb_14_5scan_set1-GammaScintPAB.root");


  return 0;
}
