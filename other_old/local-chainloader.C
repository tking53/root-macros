{
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"

  TChain *A = new TChain("Taux");
  
  for (int i=1;i<11;i++){
      stringstream ss;
      if (i <10){
	ss<<"094rb_14-seg-0"<<i<<".root";
      }
      else if (i >= 10){
	ss<<"094rb_14-seg-"<<i<<".root";
      }
      
      A->Add(ss.str().c_str());
  }

 
}


