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

TH2D* cycleHistHc(const char* filename) {

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  TCanvas *cThc1 = new TCanvas("cTobyH1","Hag vs Cycle (1 of 4)",2000,2000);
  TCanvas *cThc2 = new TCanvas("cTobyH2","Hag vs Cycle (2 of 4)",2000,2000);
  TCanvas *cThc3 = new TCanvas("cTobyH3","Hag vs Cycle (3 of 4)",2000,2000);
  TCanvas *cThc4 = new TCanvas("cTobyH4","Hag vs Cycle (4 of 4)",2000,2000);
  cout <<"Opening 4 Canvases, with 4 each for the Hagrid crystals"<<endl;
  gStyle->SetOptLogz(1);

  cThc1->Divide(2,2,0.001,0.001);
  cThc2->Divide(2,2,0.001,0.001);
  cThc3->Divide(2,2,0.001,0.001);
  cThc4->Divide(2,2,0.001,0.001);  

  TH2D* hHc0 = new TH2D("hHc0","Hag[1] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc1 = new TH2D("hHc1","Hag[2] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc2 = new TH2D("hHc2","Hag[3] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc3 = new TH2D("hHc3","Hag[4] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc4 = new TH2D("hHc4","Hag[5] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc5 = new TH2D("hHc5","Hag[6] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc6 = new TH2D("hHc6","Hag[7] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc7 = new TH2D("hHc7","Hag[8] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc8 = new TH2D("hHc8","Hag[9] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc9 = new TH2D("hHc9","Hag[10] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc10 = new TH2D("hHc10","Hag[11] vs cycle", 8000.,0.,8000.,200.,0.,200.); 
  TH2D* hHc11 = new TH2D("hHc11","Hag[12] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc12 = new TH2D("hHc12","Hag[13] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc13 = new TH2D("hHc13","Hag[14] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc14 = new TH2D("hHc14","Hag[15] vs cycle", 8000.,0.,8000.,200.,0.,200.);
  TH2D* hHc15 = new TH2D("hHc15","Hag[16] vs cycle", 8000.,0.,8000.,200.,0.,200.);

  //Set Min for logZ
  hHc0->SetMinimum(1);
  hHc1->SetMinimum(1);
  hHc2->SetMinimum(1);
  hHc3->SetMinimum(1);
  hHc4->SetMinimum(1);
  hHc5->SetMinimum(1);
  hHc6->SetMinimum(1);
  hHc7->SetMinimum(1);
  hHc8->SetMinimum(1);
  hHc9->SetMinimum(1);
  hHc10->SetMinimum(1);
  hHc11->SetMinimum(1);
  hHc12->SetMinimum(1);
  hHc13->SetMinimum(1);
  hHc14->SetMinimum(1);
  hHc15->SetMinimum(1);
 
  //Set Axis Zoom
  hHc0->GetXaxis()->SetRange(775,925);
  hHc1->GetXaxis()->SetRange(775,925);
  hHc2->GetXaxis()->SetRange(775,925);
  hHc3->GetXaxis()->SetRange(775,925);
  hHc4->GetXaxis()->SetRange(775,925);
  hHc5->GetXaxis()->SetRange(775,925);  
  hHc6->GetXaxis()->SetRange(775,925);  
  hHc7->GetXaxis()->SetRange(775,925);  
  hHc8->GetXaxis()->SetRange(775,925);  
  hHc9->GetXaxis()->SetRange(775,925);  
  hHc10->GetXaxis()->SetRange(775,925);  
  hHc11->GetXaxis()->SetRange(775,925);  
  hHc12->GetXaxis()->SetRange(775,925);  
  hHc13->GetXaxis()->SetRange(775,925);  
  hHc14->GetXaxis()->SetRange(775,925);  
  hHc15->GetXaxis()->SetRange(775,925);  



  cThc1->cd(1);
  t->Draw("calgam.cycle:calgam.Hag[0]>>hHc0","calgam.Hag[0]>800 && calgam.Hag[0]<900","COLZ");

   // cThc1->cd(2);
   // t->Draw("calgam.cycle:calgam.Hag[1]>>hHc1","calgam.Hag[1]>800 && calgam.Hag[1]<900","COLZ");

   // cThc1->cd(3);
   // t->Draw("calgam.cycle:calgam.Hag[2]>>hHc2","calgam.Hag[2]>800 && calgam.Hag[2]<900","COLZ");

   // cThc1->cd(4);
   // t->Draw("calgam.cycle:calgam.Hag[3]>>hHc3","calgam.Hag[3]>800 && calgam.Hag[3]<900","COLZ");

   //  cout<<"First Canvas Filled"<<endl;

   // cThc2->cd(1);
   // t->Draw("calgam.cycle:calgam.Hag[4]>>hHc4","calgam.Hag[4]>800 && calgam.Hag[4]<900","COLZ");

   // cThc2->cd(2);
   // t->Draw("calgam.cycle:calgam.Hag[5]>>hHc5","calgam.Hag[5]>800 && calgam.Hag[5]<900","COLZ");

   // cThc2->cd(3);
   // t->Draw("calgam.cycle:calgam.Hag[6]>>hHc6","calgam.Hag[6]>800 && calgam.Hag[6]<900","COLZ");

   // cThc2->cd(4);
   // t->Draw("calgam.cycle:calgam.Hag[7]>>hHc7","calgam.Hag[7]>800 && calgam.Hag[7]<900","COLZ");

   // cout<<"Second Canvas Filled"<<endl;

   // cThc3->cd(1);
   // t->Draw("calgam.cycle:calgam.Hag[8]>>hHc8","calgam.Hag[8]>800 && calgam.Hag[8]<900","COLZ");

   // cThc3->cd(2);
   // t->Draw("calgam.cycle:calgam.Hag[9]>>hHc9","calgam.Hag[9]>800 && calgam.Hag[9]<900","COLZ");

   // cThc3->cd(3);
   // t->Draw("calgam.cycle:calgam.Hag[10]>>hHc10","calgam.Hag[10]>800 && calgam.Hag[10]<900","COLZ");

   // cThc3->cd(4);
   // t->Draw("calgam.cycle:calgam.Hag[11]>>hHc11","calgam.Hag[11]>800 && calgam.Hag[11]<900","COLZ");

   // cout<<"Third Canvas Filled"<<endl;

   // cThc4->cd(1);
   // t->Draw("calgam.cycle:calgam.Hag[12]>>hHc12","calgam.Hag[12]>800 && calgam.Hag[12]<900","COLZ");

   // cThc4->cd(2);
   // t->Draw("calgam.cycle:calgam.Hag[13]>>hHc13","calgam.Hag[13]>800 && calgam.Hag[13]<900","COLZ");

   // cThc4->cd(3);
   // t->Draw("calgam.cycle:calgam.Hag[14]>>hHc14","calgam.Hag[14]>800 && calgam.Hag[14]<900","COLZ");

   // cThc4->cd(4);
   // t->Draw("calgam.cycle:calgam.Hag[15]>>hHc15","calgam.Hag[15]>800 && calgam.Hag[15]<900","COLZ");

  
  
  
}
