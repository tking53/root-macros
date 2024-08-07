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

TH2D* cycleHistG(const char* filename) {

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  TCanvas *cTg = new TCanvas("cTobyGe","Ge vs Cycle",2000,2000);
  gStyle->SetOptLogz(1);
  
  cTg->Divide(2,2,0.001,0.001);
  
  TH2D* h1 = new TH2D("h1","Ge[0] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* h2 = new TH2D("h2","Ge[1] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* h3 = new TH2D("h3","Ge[2] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* h4 = new TH2D("h4","Ge[3] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  
  h1->SetMinimum(1);
  h2->SetMinimum(1);
  h3->SetMinimum(1);
  h4->SetMinimum(1);
  
  

  cTg->cd(1);
  //gPad->SetTickx(2);
  t->Draw("rawgam.cycle:rawgam.Ge[0]>>h1","rawgam.Ge[0]>0","COLZ");
  
  cTg->cd(2);
  // gPad->SetTickx(2);
  //  gPad->SetTicky(2);
  t->Draw("rawgam.cycle:rawgam.Ge[1]>>h2","rawgam.Ge[1]>0","COLZ");
  
  cTg->cd(3);
  t->Draw("rawgam.cycle:rawgam.Ge[2]>>h3","rawgam.Ge[2]>0","COLZ");
  
  cTg->cd(4);
  //gPad->SetTicky(2);
  t->Draw("rawgam.cycle:rawgam.Ge[3]>>h4","rawgam.Ge[3]>0","COLZ");

  
  
  
}
