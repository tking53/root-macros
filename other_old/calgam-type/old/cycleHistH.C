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

TH2D* cycleHistH(const char* filename) {

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  TCanvas *cTh1 = new TCanvas("cTobyH1","Hag vs Cycle (1 of 4)",2000,2000);
  TCanvas *cTh2 = new TCanvas("cTobyH2","Hag vs Cycle (2 of 4)",2000,2000);
  TCanvas *cTh3 = new TCanvas("cTobyH3","Hag vs Cycle (3 of 4)",2000,2000);
  TCanvas *cTh4 = new TCanvas("cTobyH4","Hag vs Cycle (4 of 4)",2000,2000);
  cout <<"Opening 4 Canvases, with 4 each for the Hagrid crystals"<<endl;
  gStyle->SetOptLogz(1);
  
  cTh1->Divide(2,2,0.001,0.001);
  cTh2->Divide(2,2,0.001,0.001);
  cTh3->Divide(2,2,0.001,0.001);
  cTh4->Divide(2,2,0.001,0.001);  

  TH2D* hH0 = new TH2D("hH0","Hag[1] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH1 = new TH2D("hH1","Hag[2] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH2 = new TH2D("hH2","Hag[3] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH3 = new TH2D("hH3","Hag[4] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH4 = new TH2D("hH4","Hag[5] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH5 = new TH2D("hH5","Hag[6] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH6 = new TH2D("hH6","Hag[7] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH7 = new TH2D("hH7","Hag[8] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH8 = new TH2D("hH8","Hag[9] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH9 = new TH2D("hH9","Hag[10] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH10 = new TH2D("hH10","Hag[11] vs cycle", 8000.,0.,8000.,200.,0.,200.); 
  TH2D* hH11 = new TH2D("hH11","Hag[12] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH12 = new TH2D("hH12","Hag[13] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH13 = new TH2D("hH13","Hag[14] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH14 = new TH2D("hH14","Hag[15] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hH15 = new TH2D("hH15","Hag[16] vs cycle", 8000.,0.,8000.,200.,0.,200.);

  hH0->SetMinimum(1);
  hH1->SetMinimum(1);
  hH2->SetMinimum(1);
  hH3->SetMinimum(1);
  hH4->SetMinimum(1);
  hH5->SetMinimum(1);
  hH6->SetMinimum(1);
  hH7->SetMinimum(1);
  hH8->SetMinimum(1);
  hH9->SetMinimum(1);
  hH10->SetMinimum(1);
  hH11->SetMinimum(1);
  hH12->SetMinimum(1);
  hH13->SetMinimum(1);
  hH14->SetMinimum(1);
  hH15->SetMinimum(1);


  
  

  cTh1->cd(1);
  t->Draw("rawgam.cycle:rawgam.Hag[0]>>hH0","rawgam.Hag[0]>0","COLZ");

  cTh1->cd(2);
  t->Draw("rawgam.cycle:rawgam.Hag[1]>>hH1","rawgam.Hag[1]>0","COLZ");

  cTh1->cd(3);
  t->Draw("rawgam.cycle:rawgam.Hag[2]>>hH2","rawgam.Hag[2]>0","COLZ");

  cTh1->cd(4);
  t->Draw("rawgam.cycle:rawgam.Hag[3]>>hH3","rawgam.Hag[3]>0","COLZ");

   cout<<"First Canvas Filled"<<endl;

  cTh2->cd(1);
  t->Draw("rawgam.cycle:rawgam.Hag[4]>>hH4","rawgam.Hag[4]>0","COLZ");

  cTh2->cd(2);
  t->Draw("rawgam.cycle:rawgam.Hag[5]>>hH5","rawgam.Hag[5]>0","COLZ");

  cTh2->cd(3);
  t->Draw("rawgam.cycle:rawgam.Hag[6]>>hH6","rawgam.Hag[6]>0","COLZ");

  cTh2->cd(4);
  t->Draw("rawgam.cycle:rawgam.Hag[7]>>hH7","rawgam.Hag[7]>0","COLZ");

  cout<<"Second Canvas Filled"<<endl;

  cTh3->cd(1);
  t->Draw("rawgam.cycle:rawgam.Hag[8]>>hH8","rawgam.Hag[8]>0","COLZ");

  cTh3->cd(2);
  t->Draw("rawgam.cycle:rawgam.Hag[9]>>hH9","rawgam.Hag[9]>0","COLZ");

  cTh3->cd(3);
  t->Draw("rawgam.cycle:rawgam.Hag[10]>>hH10","rawgam.Hag[10]>0","COLZ");

  cTh3->cd(4);
  t->Draw("rawgam.cycle:rawgam.Hag[11]>>hH11","rawgam.Hag[11]>0","COLZ");

  cout<<"Third Canvas Filled"<<endl;

  cTh4->cd(1);
  t->Draw("rawgam.cycle:rawgam.Hag[12]>>hH12","rawgam.Hag[12]>0","COLZ");

  cTh4->cd(2);
  t->Draw("rawgam.cycle:rawgam.Hag[13]>>hH13","rawgam.Hag[13]>0","COLZ");

  cTh4->cd(3);
  t->Draw("rawgam.cycle:rawgam.Hag[14]>>hH14","rawgam.Hag[14]>0","COLZ");

  cTh4->cd(4);
  t->Draw("rawgam.cycle:rawgam.Hag[15]>>hH15","rawgam.Hag[15]>0","COLZ");

  
  
  
}
