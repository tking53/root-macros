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

void naiPull(const char* filename, int batchmode){

  if (batchmode ==1){
  gROOT->SetBatch(kTRUE);
  }

  TFile *f = new TFile(filename);
  TCanvas* temp2= new TCanvas("temp2","",100.,100.);
  temp2->cd();
 unsigned  int numNai=1;

 for (unsigned int i=0;i<numNai;i++){
   stringstream name;
   name<<"hNc"<<i;
   TH2D* temp = (TH2D*)f->Get(name.str().c_str());
   name<<".pdf";
   temp->GetXaxis()->SetRangeUser(750,850);
   temp->SetMinimum(20);
   temp->Draw("colz");
   temp2->SaveAs(name.str().c_str());
 }
 return;
}
