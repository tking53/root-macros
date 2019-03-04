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
#include "THStack.h"

TH1D* triComp97(const char* filename){
  TFile *f = TFile::Open(filename);
  TH1D *d6059 = (TH1D*) f->Get("d6059");
  TH1D *d6061 = (TH1D*) f->Get("d6061");
  TH1D *d6063 = (TH1D*) f->Get("d6063");

  TCanvas *TriComp = new TCanvas("TriComp","Comparison between the 3 detector types",1250,800);
  
  THStack *hs = new THStack("hs","Gamma-Ray Detector Type Comparison");


  // d6061->Draw();
  // d6059->Draw("same");
  // d6063->Draw("same");

  d6061->SetLineColor(kRed);
  d6063->SetLineColor(kBlack);


  hs->Add(d6059);
  hs->Add(d6061);
  hs->Add(d6063);

hs->Draw("nostack");

  hs->GetXaxis()->SetTitle("Energy (keV)");
  hs->GetYaxis()->SetTitle("Counts");
  hs->GetYaxis()->SetTitleOffset(1.4);
  hs->GetXaxis()->SetRangeUser(10,1500);
  hs->SetMaximum(5000);
 

  gStyle->SetOptDate(0);
  gStyle->SetOptStat(0);
  
  
  

  TriComp->Modified();


}

