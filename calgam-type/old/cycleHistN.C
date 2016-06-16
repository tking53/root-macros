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

TH2D* cycleHistN(const char* filename) {

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  TCanvas *cTn1 = new TCanvas("cTobyN1","NaI vs Cycle (1 of 3)",2000,2000);
  TCanvas *cTn2 = new TCanvas("cTobyN2","NaI vs Cycle (2 of 3)",2000,2000);
  TCanvas *cTn3 = new TCanvas("cTobyN3","NaI vs Cycle (3 of 3)",2000,2000);
  cout <<"Opening 3 Canvases, with 3,3,4 for the NaI crystals"<<endl;
  gStyle->SetOptLogz(1);
  
  cTn1->Divide(2,2,0.001,0.001);
  cTn2->Divide(2,2,0.001,0.001);
  cTn3->Divide(2,2,0.001,0.001);
  
  TH2D* hN0 = new TH2D("hN0","NaI[0] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hN1 = new TH2D("hN1","NaI[1] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hN2 = new TH2D("hN2","NaI[2] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hN3 = new TH2D("hN3","NaI[3] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hN4 = new TH2D("hN4","NaI[4] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hN5 = new TH2D("hN5","NaI[5] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hN6 = new TH2D("hN6","NaI[6] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hN7 = new TH2D("hN7","NaI[7] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hN8 = new TH2D("hN8","NaI[8] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hN9 = new TH2D("hN9","NaI[9] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  
  
  hN0->SetMinimum(1);
  hN1->SetMinimum(1);
  hN2->SetMinimum(1);
  hN3->SetMinimum(1);
  hN4->SetMinimum(1);
  hN5->SetMinimum(1);
  hN6->SetMinimum(1);
  hN7->SetMinimum(1);
  hN8->SetMinimum(1);
  hN9->SetMinimum(1);
  
  

  cTn1->cd(1);
  t->Draw("rawgam.cycle:rawgam.NaI[0]>>hN0","rawgam.NaI[0]>0","COLZ");
  
  cTn1->cd(2);
  t->Draw("rawgam.cycle:rawgam.NaI[1]>>hN1","rawgam.NaI[1]>0","COLZ");
  
  cTn1->cd(3);
  t->Draw("rawgam.cycle:rawgam.NaI[2]>>hN2","rawgam.NaI[2]>0","COLZ");
  
  cout<<"First canvas fill complete. Moving on"<<endl;
  cTn2->cd(1);
  t->Draw("rawgam.cycle:rawgam.NaI[3]>>hN3","rawgam.NaI[3]>0","COLZ");

  cTn2->cd(2);
  t->Draw("rawgam.cycle:rawgam.NaI[4]>>hN4","rawgam.NaI[4]>0","COLZ");

  cTn2->cd(3);
  t->Draw("rawgam.cycle:rawgam.NaI[5]>>hN5","rawgam.NaI[5]>0","COLZ");

  cout<<"Second canvas fill complete. Moving on"<<endl;
  cTn3->cd(1);
  t->Draw("rawgam.cycle:rawgam.NaI[6]>>hN6","rawgam.NaI[6]>0","COLZ");

  cTn3->cd(2);
  t->Draw("rawgam.cycle:rawgam.NaI[7]>>hN8","rawgam.NaI[8]>0","COLZ");

  cTn3->cd(3);
  t->Draw("rawgam.cycle:rawgam.NaI[8]>>hN8","rawgam.NaI[8]>0","COLZ");
  
  cTn3->cd(4);
  t->Draw("rawgam.cycle:rawgam.NaI[9]>>hN9","rawgam.NaI[9]>0","COLZ");

  
}
