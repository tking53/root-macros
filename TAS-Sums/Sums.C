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


void Sums(const char* savename,unsigned batchmode=0){
  
  if (batchmode ==1){
    gROOT->SetBatch(kTRUE); //set batch mode to suppress canvas output
     }
  //TChain* V;
  // gROOT->ProcessLine(".x dual-seg-chainload.C");
  TChain *P = new TChain("Tab");
 
   for (int i=1;i<11;i++){
       stringstream ss;
       if (i <10){
  	ss<<"094rb_14-seg-0"<<i<<".root";
       }
       else if (i >= 10){
  	ss<<"094rb_14-seg-"<<i<<".root";
       }
       P->Add(ss.str().c_str());
   }
  

   cout<<"chain loaded"<<endl;
      
  TFile *f = new TFile(savename,"RECREATE");

  
  TH2D* NvG = new TH2D("NvG"," NaI (x) vs Ge (y) Paddbacks",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* LvG = new TH2D("LvG"," LaBr3 (x) vs Ge (y) Paddbacks",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* NvL = new TH2D("NvL"," NaI (x) vs LaBr3 (y) Paddbacks",8000.,0.,8000.,8000.,0.,8000.);

  cout<<"Hist + files made"<<endl;
    
  NvG->SetMinimum(1);
  LvG->SetMinimum(1);
  NvL->SetMinimum(1);
  cout<<"minimum set"<<endl;
  P->Draw("pab.GabE:pab.NabE>>NvG");
  cout<<"NvG done"<<endl; 
  P->Draw("pab.GabE:pab.LabE>>LvG");
  cout<<"LvG done"<<endl; 
  P->Draw("pab.LabE:pab.NabE>>NvL");
  
  f->Write();
}
  
