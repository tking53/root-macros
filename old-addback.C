#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>



void ABplot(const char* savename){

  gROOT->SetBatch(kTRUE);
  
  TChain *A = new TChain("Taux");
  
  for (int i=1;i<11;i++){
    stringstream sss;
    if (i <10){
      sss<<"/home/tking/segment-94/094rb_14-seg-0"<<i<<".root";
    }
    else if (i >= 10){
      sss<<"/home/tking/segment-94/094rb_14-seg-"<<i<<".root";
    }
    
    A->Add(sss.str().c_str());

  }
  TFile *f = new TFile(savename,"RECREATE");
  
  TH2D* GvL = new TH2D("GvL","Ge addback (X) vs LaBr3 addback (Y)",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* GvN = new TH2D("GvN","Ge addback (X) vs NaI addback (Y)",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* NvL = new TH2D("NvL","NaI addback (X) vs LaBr3 addback (Y)",8000.,0.,8000.,8000.,0.,8000.);
  cout<<"three TH2D created"<<endl;
  A->Draw("Lpro.AbE:Gpro.AbE>>GvL");
  cout<<"GvL created (1 of 3)"<<endl;
  A->Draw("Npro.AbE:Gpro.AbE>>GvN");
  cout<<"GvN created (2 of 3)"<<endl;
  A->Draw("Lpro.AbE:Npro.AbE>>NvL");
  cout<<"NvL created (3 of 3)"<<endl;
  
  GvL->SetMinimum(3);
  GvN->SetMinimum(3);
  NvL->SetMinimum(3);
 

  f->Write();
  gApplication->Terminate(0);
  
}
