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

  TChain *C = new TChain("Taux");
  
  for (int i=1;i<11;i++){
      stringstream ss;
      if (i <10){
	ss<<"/home/tking/segment-94/094rb_14-seg-0"<<i<<".root";
      }
      else if (i >= 10){
	ss<<"/home/tking/segment-94/094rb_14-seg-"<<i<<".root";
      }
      
      C->Add(ss.str().c_str());
  }

  TChain *V = new TChain("Tab");
  
  for (int i=1;i<11;i++){
      stringstream ss;
      if (i <10){
	ss<<"/home/tking/segment-94/094rb_14-seg-0"<<i<<".root";
      }
      else if (i >= 10){
	ss<<"/home/tking/segment-94/094rb_14-seg-"<<i<<".root";
      }
      
      V->Add(ss.str().c_str());
  }

  //  C->Print();
  V->Print();
 
}


