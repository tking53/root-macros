#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TROOT.h"
TH2D* MakeHist(const char* filename) {

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  TH2D* h = new TH2D("h1","Ge[0] vs cycle", 8000.,0.,8000.,2000.,0.,2000.);
  //  TH2D* h = (TH2D*) gROOT->FindObject("histName");
  t->Draw("rawgam.cycle:rawgam.Ge[0]>>h1","rawgam.Ge[0]>0","COLZ");


  return h;
}
