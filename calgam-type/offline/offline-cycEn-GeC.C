#include "TSystem.h"
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
#include <string>
#include "/home/tking/programs/root-macros/styles/Style_Toby_1.C"

// struct RAY { 
//     double Hag[16];
//     double NaI[10];
//     double Ge[4];
//     double beta;
//     int cycle;
//  };



// inline string outputset(string numG){
//   baseG="HPGe-Output-EvC-Ge";
//   filetypeG=".eps";
//   qqq = baseG + numG + filetypeG;
  
//   return qqq;
// }


TH2D* offlineCycEnGc(const char* filename, int batchmode) {
 
  if (batchmode ==1){
  gROOT->SetBatch(kTRUE);
  }

  string qqq;  
  string baseG= "HPGe-Output-EvC-Ge";
  string filetypeG=".png";
 

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
 

  stringstream sss;
  sss<<"FULL-HPGeOutput-"<<filename;
  TFile f1(sss.str().c_str(),"RECREATE");

 
  

  TCanvas *cTgc = new TCanvas("cTobyGe","Ge vs Cycle",2000,2000);
  gStyle->SetOptLogz(1);
  
  cTgc->Divide(2,2,0.001,0.001);
  
  TH2D* hGc0 = new TH2D("hGc0","Ge[0] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hGc1 = new TH2D("hGc1","Ge[1] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hGc2 = new TH2D("hGc2","Ge[2] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hGc3 = new TH2D("hGc3","Ge[3] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  
  hGc0->SetMinimum(1);
  hGc1->SetMinimum(1);
  hGc2->SetMinimum(1);
  hGc3->SetMinimum(1);
  
  hGc0->GetXaxis()->SetRangeUser(775,925);
  hGc1->GetXaxis()->SetRangeUser(775,925);  
  hGc2->GetXaxis()->SetRangeUser(775,925);
  hGc3->GetXaxis()->SetRangeUser(775,925);


  cout<<"Base name for pictures= "<<baseG<<endl<<"Filetype for Pictures= "<<filetypeG<<endl;

  cTgc->cd(1);
  qqq = baseG + "0" + filetypeG;
  t->Draw("calgam.cycle:calgam.Ge[0]>>hGc0","","COLZ");
  //   cTgc->cd(1)->SaveAs(qqq.c_str());
  //  hGc1->Draw("COLZ");

  cTgc->cd(2);
   qqq = baseG + "1" + filetypeG;
  t->Draw("calgam.cycle:calgam.Ge[1]>>hGc1","","COLZ");
  //cTgc->cd(2)->SaveAs(qqq.c_str());  

  cTgc->cd(3);
   qqq = baseG + "2" + filetypeG;
  t->Draw("calgam.cycle:calgam.Ge[2]>>hGc2","","COLZ");
  // cTgc->cd(3)->SaveAs(qqq.c_str());  
 
  cTgc->cd(4);
   qqq = baseG + "3" + filetypeG;
  t->Draw("calgam.cycle:calgam.Ge[3]>>hGc3","","COLZ");
  //cTgc->cd(4)->SaveAs(qqq.c_str());


  f1.Write();

  //  gApplication->Terminate();
}
