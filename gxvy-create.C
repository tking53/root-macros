#include "TSystem.h"
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

  TH2D* Gxvy = new TH2D("Gxvy","GsymX (x) vs GsymY (y)",8000.,0.,8000.,8000.,0.,8000.);

  A->Draw("Gpro.SymY:Gpro.SymX>>Gxvy");

  f->Write();
  gApplication->Terminate(0)
  
}


