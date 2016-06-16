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

TH1D* triDecComp(const char* filename, int batchmode){

  if (batchmode ==1){
  gROOT->SetBatch(kTRUE);
  }

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");

  stringstream sss;
  sss<<"triCompare-"<<filename;
  TFile f1(sss.str().c_str(),"RECREATE");

  TCanvas *tDc = new TCanvas("tDc","Gamma Dec Comparison",2000.,2000.);
  tDc->Divide(2,2,0.001,0.001);
  

  TH1D* hGs = new TH1D("hGs", "Ge[Sum] Energy non Beta-Gated",5000.,0,5000.);
  TH1D* hNs = new TH1D("hNs", "NaI[Sum] Energy non Beta-Gated",5000.,0,5000.);
  TH1D* hHs = new TH1D("hHs", "Hag[Sum] Energy non Beta-Gated",5000.,0,5000.);
  
 

  // hGs->SetXTitle("Energy keV");
  // hNs->SetXTitle("Energy keV");
  // hHs->SetXTitle("Energy keV");
 

  cout<<"Starting Fill of the 3 Sums"<<endl;

  tDc->cd(1);
  t->Draw("calgam.Ge[]>>hGs","calgam.Ge[]>0","");
  hGs->Draw();

  tDc->cd(2);
  t->Draw("calgam.NaI[]>>hNs","calgam.NaI[]>0","");

  tDc->cd(3);
  t->Draw("calgam.Hag[]>>hHs","calgam.Hag[]>0","");


  tDc->cd(4);
  hGs->GetYaxis()->SetRangeUser(0,20000);
  hGs->Draw();
  hGs->SetLineColor(kBlack);
  hNs->Draw("same");
  hNs->SetLineColor(kRed);
  hHs->Draw("same");
  hHs->SetLineColor(kBlue);

  f1.Write();

}
