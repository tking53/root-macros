#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

TH2D* cycleHistGcH(const char* filename) {

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  TCanvas *cTgc = new TCanvas("cTobyGe","Ge vs Cycle",2000,2000);
  gStyle->SetOptLogz(1);
  
  cTgc->Divide(2,2,0.001,0.001);
  
  TH2D* hGc1 = new TH2D("hGc1","Ge[0] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hGc2 = new TH2D("hGc2","Ge[1] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hGc3 = new TH2D("hGc3","Ge[2] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hGc4 = new TH2D("hGc4","Ge[3] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  
  hGc1->SetMinimum(1);
  hGc2->SetMinimum(1);
  hGc3->SetMinimum(1);
  hGc4->SetMinimum(1);
  
  

  cTgc->cd(1);
  t->Draw("calgam.cycle:(calgam.Ge[0])/2>>hGc1","calgam.Ge[0]>0","COLZ");
  
  cTgc->cd(2);
  t->Draw("calgam.cycle:(calgam.Ge[1])/2>>hGc2","calgam.Ge[1]>0","COLZ");
  
  cTgc->cd(3);
  t->Draw("calgam.cycle:(calgam.Ge[2])/2>>hGc3","calgam.Ge[2]>0","COLZ");
  
  cTgc->cd(4);
  t->Draw("calgam.cycle:(calgam.Ge[3])/2>>hGc4","calgam.Ge[3]>0","COLZ");

  
  
  
}
