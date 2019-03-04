
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>


 void chainload94rb(string type,int fNum ,string tree){
  // if (argc !=3){
  // throw std::invalid_argument("Missing Argument(s). Need Type for load (seg or (indi)vidual), number for files to load, pointer for chain, which tree");
  // }

  TChain *C = new TChain(tree.c_str());
  
  if (type=="seg"){
    for (int i=1;i<11;i++){
      stringstream ss;
      if (i <10){
      ss<<"094rb_14-seg-0"<<i<<".root";
      }
      else if (i >= 10){
      ss<<"094rb_14-seg-"<<i<<".root";
      }
      
      C->Add(ss.str().c_str());
    };
  }

  if (type=="indi"){
    for (int i=0;i<100;i++){
      stringstream ss;
      if (i <10){
      ss<<"094rb_14-indi-0"<<i<<".root";
      }
      else if (i >= 10){
      ss<<"094rb_14-indi-"<<i<<".root";
      };
      C->Add(ss.str().c_str());
    };
  }
}

