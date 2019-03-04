{
#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"

  TFile *f = TFile::Open("094rb_14_full-gammaSing.root");
  TFile *da = TFile::Open("094rb_14_full.root");
    
  gStyle->SetOptStat(0);
 
  
  TCanvas *LComp = new TCanvas("LComp","Comparison between Different Beta Gates",1250,800);
  TCanvas *NComp = new TCanvas("NComp","Comparison between Different Beta Gates",1250,800);
  //LComp->SetLogy();
  //NComp->SetLogy();
  
  TH1D* SupNe = new TH1D("SupNe","NaI Energy Totals, Event Gate",5000.,0,5000.);
  TH1D* SupLe = new TH1D("SupLe","LaBr Energy Totals, Event Gate",5000.,0,5000.);
  TH1D* SupL8 = new TH1D("SupL8","LaBr Energy Totals, 8 Pixie Clock Ticks",5000.,0,5000.);
  TH1D* SupN8 = new TH1D("SupN8","NaI Energy Totals, 8 Pixie Clock Ticks",5000.,0,5000.);
  TH1D* SupN5 = new TH1D("SupN5","NaI Energy Totals, 5 Pixie Clock Ticks",5000.,0,5000.);
  TH1D* SupL5 = new TH1D("SupL5","LaBr Energy Totals, 5 Pixie Clock Ticks",5000.,0,5000.);
  TH1D* SupL2 = new TH1D("SupL2","LaBr Energy Totals, 2 Pixie Clock Tick",5000.,0,5000.);
  TH1D* SupN2 = new TH1D("SupN2","NaI Energy Totals, 2 Pixie Clock Tick",5000.,0,5000.);

  TTree *TauxL = (TTree*)f->Get("TauxL");
  TTree *TauxN = (TTree*)f->Get("TauxN");

  LComp->cd();
 
  d6058->SetLineColor(858); //kAzure -2 = 858
  d6058->Draw();

  SupLe->SetLineColor(kRed);
  TauxL->Draw("aux_LaBrEn>>SupLe","aux_BetaEn>0","same");

  SupL8->SetLineColor(kGreen);
  TauxL->Draw("aux_LaBrEn>>SupL8","aux_BetaEn>0 && aux_LaBrTime-aux_BetaTime<=8 ","same");
  
  SupL5->SetLineColor(807); //kOrange +7
  TauxL->Draw("aux_LaBrEn>>SupL5","aux_BetaEn>0  && aux_LaBrTime-aux_BetaTime<=5","same");
  
  SupL2->SetLineColor(kBlack);
  TauxL->Draw("aux_LaBrEn>>SupL2","aux_BetaEn>0  && aux_LaBrTime-aux_BetaTime<=2","same");


  NComp->cd();
  //NaiSet
  d6062->SetLineColor(858); //kAzure -2 = 858
  d6062->Draw();

  SupNe->SetLineColor(kRed);
  TauxN->Draw("aux_NaIEn>>SupNe","aux_BetaEn>0","same");

  SupN8->SetLineColor(kGreen);
  TauxN->Draw("aux_NaIEn>>SupN8","aux_BetaEn>0 && aux_NaITime-aux_BetaTime<=8 ","same");
  
  SupN5->SetLineColor(807); //kOrange +7
  TauxN->Draw("aux_NaIEn>>SupN5","aux_BetaEn>0  && aux_NaITime-aux_BetaTime<=5","same");
  
  SupN2->SetLineColor(kBlack);
  TauxN->Draw("aux_NaIEn>>SupN2","aux_BetaEn>0  && aux_NaITime-aux_BetaTime<=2","same");
  
  
}
