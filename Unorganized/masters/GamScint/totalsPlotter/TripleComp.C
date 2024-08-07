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

void TripleComp(){

  TFile *_file1=TFile::Open("94rbSets/94rb_14_set1/094rb_14_5scan_set1.root");
  TFile *_file2=TFile::Open("94rbSets/94rb_14_set2/094rb_14_5scan_set2.root");
  TFile *_file3=TFile::Open("94rbSets/94rb_14_set3/094rb_14_5scan_set3.root");
  TFile *_file4=TFile::Open("94rbSets/94rb_14_set4/094rb_14_5scan_set4.root");
  TFile *_file5=TFile::Open("94rbSets/94rb_14_set5/094rb_14_5scan_set5.root");
  TFile *GStotal=TFile::Open("94rbFullset_totals.root");

  // TCanvas *TriComp = new TCanvas("TriComp","Comparison between the 3 detector types",800,450);
  TCanvas *TriCompBG = new TCanvas("TriCompBG","Beta-Gated comparison between the 3 detector types",800,450);
  
  THStack *hs = new THStack("hs","Gamma-Ray Detector Type Comparison");
  THStack *hsbg = new THStack("hsbg","Gamma-Ray Detector Type Beta-Gated Comparison");

  TH1D* HPGeT = new TH1D("HPGeT","Clover Energy (Totals)",8192.,0,8192.);
  TH1D* HPGeTbg = new TH1D("HPGeTbg","Clover Energy (Beta-Gated Totals)",8192.,0,8192.);

  TH1D* d2500_1 = (TH1D*) _file1->Get("d2500");  
  TH1D* d2510_1 = (TH1D*) _file1->Get("d2510");

  TH1D* d2500_2 = (TH1D*) _file2->Get("d2500");  
  TH1D* d2510_2 = (TH1D*) _file2->Get("d2510");

  TH1D* d2500_3 = (TH1D*) _file3->Get("d2500");
  TH1D* d2510_3 = (TH1D*) _file3->Get("d2510");

  TH1D* d2500_4 = (TH1D*) _file4->Get("d2500");  
  TH1D* d2510_4 = (TH1D*) _file4->Get("d2510");

  TH1D* d2500_5 = (TH1D*) _file5->Get("d2500");  
  TH1D* d2510_5 = (TH1D*) _file5->Get("d2510");

  TH1D* naiT = (TH1D*) GStotal->Get("NaITotal");  
  TH1D* naiTbg = (TH1D*) GStotal->Get("NaITotalBG");

  TH1D* shT = (TH1D*) GStotal->Get("SHTotal");  
  TH1D* shTbg = (TH1D*) GStotal->Get("SHTotalBG");

  HPGeT->Add(d2500_1);
  HPGeT->Add(d2500_2);
  HPGeT->Add(d2500_3);
  HPGeT->Add(d2500_4);
  HPGeT->Add(d2500_5);

  HPGeTbg->Add(d2510_1);
  HPGeTbg->Add(d2510_2);
  HPGeTbg->Add(d2510_3);
  HPGeTbg->Add(d2510_4);
  HPGeTbg->Add(d2510_5);

  naiT->SetLineColor(kRed);
  shT->SetLineColor(kBlack);

  naiTbg->SetLineColor(kRed);
  shTbg->SetLineColor(kBlack);

  hs->Add(HPGeT);
  hs->Add(naiT);
  hs->Add(shT);

  hsbg->Add(HPGeTbg);
  naiTbg->SetTitle("NaI Energy (Beta-Gated Totals)");
  shTbg->SetTitle("2\" HAGRiD Energy (Beta-Gated Totals)");
  hsbg->Add(naiTbg);
  hsbg->Add(shTbg);

  /* TriComp->cd();
   hs->Draw("nostack");
  
  hs->GetXaxis()->SetTitle("Energy (keV)");
  hs->GetYaxis()->SetTitle("Counts");
  hs->GetYaxis()->SetTitleOffset(1.4);
  //hs->GetXaxis()->SetRangeUser(10,1500);
  hs->SetMaximum(2000000);
  */

  TriCompBG->cd();
  hsbg->Draw("nostack");
  

  hsbg->GetYaxis()->SetTitle("Counts");
  hsbg->GetYaxis()->SetTitleOffset(1);
  
  hsbg->GetXaxis()->SetTitle("Energy (keV)");
  hsbg->GetXaxis()->SetRangeUser(0,4000);

  

  gStyle->SetOptDate(0);
  gStyle->SetOptStat(0);
  
  //  TriComp->SetLogy();
  TriCompBG->SetLogy();
  
  
  //TriComp->Modified();
  TriCompBG->Modified();
  hsbg->GetYaxis()->SetRangeUser(100,7000000);
  gPad->RedrawAxis();

}

