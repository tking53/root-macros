#include <iostream>
#include <fstream>
#include <string>

#include "TSystem.h"
#include "TObject.h"
#include "TROOT.h"
//#include "TProof.h"


TChain *chain1;
TChain *chain2;
Bool_t abfile;

int loadGStchain(const std::string paassFile="sing"){
  if (paassFile == "ab"){
    chain1 = new TChain("GSsinglesAB");
    abfile = true;
  } else{
    chain1 = new TChain("GSsingles");
    chain2 = new TChain("GSaddback");
    abfile=false;
}

  
    for (auto it=1; it<=5 ; it++){
      if (abfile){
        std::stringstream ss;
        ss <<"/home/hanayo/research/ornl2016/thesis/rootFiles/94rbSets/94rb_14_set"<<it<<"/094rb_14_5scan_set"<<it<<"-GammaScintPAB.root";
        chain1->Add(ss.str().c_str());
      }else{
        std::stringstream ss;
        ss << "/home/hanayo/research/ornl2016/thesis/rootFiles/94rbSets/94rb_14_set"<<it<<"/094rb_14_5scan_set"<<it<<"-GammaScint.root";
        chain1->Add(ss.str().c_str());
        chain2->Add(ss.str().c_str());
      }
    }

    if (abfile){
      gDirectory->Add(chain1);
    }else {
      gDirectory->Add(chain1);
      gDirectory->Add(chain2);
    }
    //  chain->Add("/home/hanayo/research/ornl2016/thesis/rootFiles/94rbSets/94rb_14_set1/094rb_14_5scan_set1-GammaScintPAB.root");


  return 0;
}
