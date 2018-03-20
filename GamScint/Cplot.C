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
#include "TObject.h"

void Cplot(){  
  TFile *fOut = new TFile("CPlotOut.root","RECREATE");
  
  //  gROOT->SetBatch();
  TCanvas *c1 = new TCanvas("c1","c1",1920.,1200.);
  TCanvas *c2 = new TCanvas("c2","c2",1920.,1200.);

  TH2D* CPlot1 = new TH2D("CPlot1","CPlot1",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* CPlot1bg = new TH2D("CPlot1bg","CPlot1bg",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* CPlot2 = new TH2D("CPlot2","CPlot2",8000.,0.,8000.,8000.,0.,8000.);
  TH2D* CPlot2bg = new TH2D("CPlot2bg","CPlot2bg",8000.,0.,8000.,8000.,0.,8000.);
    
  
  c1->SetLogz();
  c2->SetLogz();
  
  c1->cd();
  chain1->Draw("GS_Energy:GS_ABEnergy>>CPlot1","GS_Type == \"clover\" && GS_ABType ==\"smallhag\" && GS_HasLowResBeta ","COLZ");
  c2->cd();
  chain1->Draw("GS_Energy:GS_ABEnergy>>CPlot2","GS_Type == \"clover\" && GS_ABType ==\"nai\" && GS_HasLowResBeta","COLZ");

  c2->cd();
    chain1->Draw("GS_Energy:GS_ABEnergy>>CPlot1bg","GS_Type == \"clover\" && GS_ABType ==\"smallhag\"","COLZ");
  c2->cd();
  chain1->Draw("GS_Energy:GS_ABEnergy>>CPlot2bg","GS_Type == \"clover\" && GS_ABType ==\"nai\" ","COLZ");

  
  CPlot1->SetMinimum(1);
  CPlot2->SetMinimum(1);

  CPlot1->GetXaxis()->SetRangeUser(0,6500);
  CPlot1->GetYaxis()->SetRangeUser(0,6500);

  CPlot2->GetXaxis()->SetRangeUser(0,6500);
  CPlot2->GetYaxis()->SetRangeUser(0,6500);

  c1->Modified();
  c2->Modified();


  gDirectory->Add(CPlot1);
  gDirectory->Add(CPlot1bg);
  gDirectory->Add(CPlot2);
  gDirectory->Add(CPlot2bg);
  
  // c1->SaveAs("CPlot1-2.png");
  // c2->SaveAs("CPlot2-2.png");
  

  
  // fOut->Write();
  

}
