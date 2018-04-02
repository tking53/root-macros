#include <iostream>
#include <fstream>
#include <string>

#include "TSystem.h"
#include "TObject.h"
#include "TROOT.h"
#include "TProof.h"


TChain *DDchain;

TProof *pr;

int loadDDchain(){

  DDchain= new TChain("PixTree");

      for (auto it=1; it<=5 ; it++){
        std::stringstream ss;
        ss <<"/home/hanayo/research/ornl2016/thesis/interHolding/94rb/094rb_14_5scan_set"<<it<<"_DD.root";
        DDchain->Add(ss.str().c_str());
      }

      gDirectory->Add(DDchain);

      // DDchain->SetProof();
      std::cout << "Number of TTrees added to DDchain: " << DDchain->GetNtrees() << std::endl;

  return 0;
}
