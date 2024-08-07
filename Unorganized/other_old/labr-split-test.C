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


void Splitplot(const char* savename){
  gROOT->SetBatch(kTRUE);
  
  TChain *A = new TChain("Taux");
  
  for (int i=1;i<11;i++){
    stringstream ss;
    if (i <10){
      ss<<"~/segment-94/094rb_14-seg-0"<<i<<".root";
    }
    else if (i >= 10){
      ss<<"~/segment-94/094rb_14-seg-"<<i<<".root";
    }
    
    A->Add(ss.str().c_str());
  }
  
  A->Print();

  TFile *f = new TFile(savename,"RECREATE");
  
  TH1D* Ll = new TH1D("Ll","Beam Left LaBr beta-gated singles",8000.,0.,8000.);
  TH1D* Lr = new TH1D("Lr","Beam right LaBr beta-gated singles",8000.,0.,8000.);

  cout<<"Hist made:----: starting Left Draw"<<endl;

  A->Draw("sing.LaBr[]>>Ll","sing.beta>0 && Iteration$>=1 && Iteration$<=8","");
  
  cout<<"Left filled"<<endl<<"Filling Right now. more complicated conditions on draw)"<<endl;
  
  A->Draw("sing.LaBr[]>>Ll","sing.beta>0 && (Iteration$>=9 && Iteration$<=15 || Iteration$=0)","");
 

  f->Write();
  gApplication->Terminate(0);
  
}

