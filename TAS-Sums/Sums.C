#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TCutG.h"
#include "TChain.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>


void Sums(const char* savename, int batchmode=1){
  
  if (batchmode ==1){
      gROOT->SetBatch(kTRUE);
  }
  //TChain* V;
  // gROOT->ProcessLine(".x dual-seg-chainload.C");
  TChain *V = new TChain("Tab");
 
   for (int i=1;i<11;i++){
       stringstream ss;
       if (i <10){
  	ss<<"094rb_14-seg-0"<<i<<".root";
       }
       else if (i >= 10){
  	ss<<"094rb_14-seg-"<<i<<".root";
       }
       V->Add(ss.str().c_str());
   }
  


      
  TFile *f = new TFile(savename,"RECREATE");
  
  TH2D* NvG = new TH2D("NvG"," NaI (x) vs Ge (y) Paddbacks",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* LvG = new TH2D("HvG"," LaBr3 (x) vs Ge (y) Paddbacks",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* NvL = new TH2D("NvL"," NaI (x) vs LaBr3 (y) Paddbacks",8000.,0.,8000.,8000.,0.,8000.);

  NvG->SetMinimum(1);
  LvG->SetMinimum(1);
  NvL->SetMinimum(1);

  V->Draw("pab.GabE:pab.NabE>>NvG");
  V->Draw("pab.GabE:pab.LabE>>LvG");
  V->Draw("pab.LabE:pab.NabE>>NvL");

  f->Write();
}
  
