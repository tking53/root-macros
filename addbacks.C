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



void symplot(const char* savename){

  gROOT->SetBatch(kTRUE);
  
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

  TFile *f = new TFile(savename,"RECREATE");

  TH2D* GvL = new TH2D("GvL","Ge addback (X) vs LaBr3 addback (Y)",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* GvN = new TH2D("GvN","Ge addback (X) vs NaI addback (Y)",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* NvL = new TH2D("NvL","NaI addback (X) vs LaBr3 addback (Y)",8000.,0.,8000.,8000.,0.,8000.);

  A->Draw("Lpro.AbE:Gpro.AbE>>GvL");
  A->Draw("Npro.AbE:Gpro.AbE>>GvN");
  A->Draw("Lpro.AbE:Npro.AbE>>NvL");

  GvL->SetMinimum(3);
  GvN->SetMinimum(3);
  NvL->SetMinimum(3);


  f->Write();
  gApplication->Terminate(0);
  
}


