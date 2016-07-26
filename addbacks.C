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

  TH1D* Ll = new TH1D("Ll","Beam Left LaBr beta-gated singles",8000.,0.,8000.);
  TH1D* Lr = new TH1D("Lr","Beam right LaBr beta-gated singles",8000.,0.,8000.);

  TList *left = new TList;
  TList *right = new TList;

  unsigned  int leftLabr=8;
  
  for (unsigned int i=1;i<=leftLabr;i++){
    stringstream ss;
    stringstream sss;
    ss<<"Ll"<<leftLabr;
    sss<<"Left LaBr3 (number "<<leftLabr<<")";
    TH1D* temp = new TH1D(ss.str().c_str(),sss.str().c_str(),8000.,0.,8000.);
    A->Draw("sing.LaBr[i]>>temp","sing.beta>0");
    left->Add(temp);
  }
  
  
  unsigned  int rightLabr=15;

  TH1D* Lr0 = new TH1D("Lr0","Right LaBr3 (number 0)",8000.,0.,8000.);
  A->Draw("sing.LaBr[0]>>Lr0","sing.beta>0");
  right->Add(Lr0);
		       
  
  for (unsigned int i=9;i<=rightLabr;i++){
    stringstream ss;
    stringstream sss;
    ss<<"Lr"<<rightLabr;
    sss<<"right LaBr3 (number "<<rightLabr<<")";
    TH1D* temp2 = new TH1D(ss.str().c_str(),sss.str().c_str(),8000.,0.,8000.);
    A->Draw("sing.LaBr[i]>>temp2","sing.beta>0");
    right->Add(temp2);
  }

  Ll->Merge(left);
  Lr->Merge(right);

  

  f->Write();
  gApplication->Terminate(0);
  
}


