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


TH2D* cycleHistNc(const char* filename) {

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  TCanvas *cTnc1 = new TCanvas("cTobyN1","NaI vs Cycle (1 of 3)",2000,2000);
  TCanvas *cTnc2 = new TCanvas("cTobyN2","NaI vs Cycle (2 of 3)",2000,2000);
  TCanvas *cTnc3 = new TCanvas("cTobyN3","NaI vs Cycle (3 of 3)",2000,2000);
  cout <<"Opening 3 Canvases, with 3,3,4 for the NaI crystals"<<endl;
  gStyle->SetOptLogz(1);
  
  cTnc1->Divide(2,2,0.001,0.001);
  cTnc2->Divide(2,2,0.001,0.001);
  cTnc3->Divide(2,2,0.001,0.001);
  
  TH2D* hNc0 = new TH2D("hNc0","NaI[0] vs cycle", 8000.,0.,8000,200.,0.,200.);
  TH2D* hNc1 = new TH2D("hNc1","NaI[1] vs cycle", 8000.,0.,8000,200.,0.,200.);
  TH2D* hNc2 = new TH2D("hNc2","NaI[2] vs cycle", 8000.,0.,8000,200.,0.,200.);
  TH2D* hNc3 = new TH2D("hNc3","NaI[3] vs cycle", 8000.,0.,8000,200.,0.,200.);
  TH2D* hNc4 = new TH2D("hNc4","NaI[4] vs cycle", 8000.,0.,8000,200.,0.,200.);
  TH2D* hNc5 = new TH2D("hNc5","NaI[5] vs cycle", 8000.,0.,8000,200.,0.,200.);
  TH2D* hNc6 = new TH2D("hNc6","NaI[6] vs cycle", 8000.,0.,8000,200.,0.,200.);
  TH2D* hNc7 = new TH2D("hNc7","NaI[7] vs cycle", 8000.,0.,8000,200.,0.,200.);
  TH2D* hNc8 = new TH2D("hNc8","NaI[8] vs cycle", 8000.,0.,8000,200.,0.,200.);
  TH2D* hNc9 = new TH2D("hNc9","NaI[9] vs cycle", 8000.,0.,8000,200.,0.,200.);
  
  
  hNc0->SetMinimum(1);
  hNc1->SetMinimum(1);
  hNc2->SetMinimum(1);
  hNc3->SetMinimum(1);
  hNc4->SetMinimum(1);
  hNc5->SetMinimum(1);
  hNc6->SetMinimum(1);
  hNc7->SetMinimum(1);
  hNc8->SetMinimum(1);
  hNc9->SetMinimum(1);
  
  hNc0->GetXaxis()->SetRange(775,925);
  hNc1->GetXaxis()->SetRange(775,925);
  hNc2->GetXaxis()->SetRange(775,925);
  hNc3->GetXaxis()->SetRange(775,925);
  hNc4->GetXaxis()->SetRange(775,925);
  hNc5->GetXaxis()->SetRange(775,925);
  hNc6->GetXaxis()->SetRange(775,925);
  hNc7->GetXaxis()->SetRange(775,925);
  hNc8->GetXaxis()->SetRange(775,925);
  hNc9->GetXaxis()->SetRange(775,925);


  cTnc1->cd(1);
  t->Draw("calgam.cycle:calgam.NaI[0]>>hNc0","calgam.NaI[0]>800 && calgam.NaI[0]<900","COLZ");
  
  cTnc1->cd(2);
  t->Draw("calgam.cycle:calgam.NaI[1]>>hNc1","calgam.NaI[1]>800 && calgam.NaI[1]<900","COLZ");
  
  cTnc1->cd(3);
  t->Draw("calgam.cycle:calgam.NaI[2]>>hNc2","calgam.NaI[2]>800 && calgam.NaI[2]<900","COLZ");
  
  cout<<"   First canvas fill complete. Moving on"<<endl;
  cTnc2->cd(1);
  t->Draw("calgam.cycle:calgam.NaI[3]>>hNc3","calgam.NaI[3]>800 && calgam.NaI[3]<900","COLZ");

  cTnc2->cd(2);
  t->Draw("calgam.cycle:calgam.NaI[4]>>hNc4","calgam.NaI[4]>800 && calgam.NaI[4]<900","COLZ");

  cTnc2->cd(3);
  t->Draw("calgam.cycle:calgam.NaI[5]>>hNc5","calgam.NaI[5]>800 && calgam.NaI[5]<900","COLZ");

  cout<<"   Second canvas fill complete. Moving on"<<endl;
  cTnc3->cd(1);
  t->Draw("calgam.cycle:calgam.NaI[6]>>hNc6","calgam.NaI[6]>800 && calgam.NaI[6]<900","COLZ");

  cTnc3->cd(2);
  t->Draw("calgam.cycle:calgam.NaI[7]>>hNc8","calgam.NaI[7]>800 && calgam.NaI[7]<900","COLZ");

  cTnc3->cd(3);
  t->Draw("calgam.cycle:calgam.NaI[8]>>hNc8","calgam.NaI[8]>800 && calgam.NaI[8]<900","COLZ");
  
  cTnc3->cd(4);
  t->Draw("calgam.cycle:calgam.NaI[9]>>hNc9","calgam.NaI[9]>800 && calgam.NaI[9]<900","COLZ");

  
}
