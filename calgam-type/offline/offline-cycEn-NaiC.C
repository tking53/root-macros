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



TH2D* offlineCycEnNc(const char* filename) {
  gROOT->SetBatch(kTRUE);

  string qq;  
  string baseN = "NaI-Output-EvC-NaI";
  string filetypeN = ".png";


  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  
  stringstream ssss;
  ssss<<"FULL-NaIOutput-"<<filename;
  TFile f3(ssss.str().c_str(),"UPDATE");
  
  TCanvas *cTnc1 = new TCanvas("cTobyN1","NaI vs Cycle (1 of 3)",2000,2000);
  TCanvas *cTnc2 = new TCanvas("cTobyN2","NaI vs Cycle (2 of 3)",2000,2000);
  TCanvas *cTnc3 = new TCanvas("cTobyN3","NaI vs Cycle (3 of 3)",2000,2000);
  cout <<"Opening 3 Canvases, with 3,3,4 for the NaI crystals"<<endl;
  gStyle->SetOptLogz(1);
  
  cTnc1->Divide(2,2,0.001,0.001);
  cTnc2->Divide(2,2,0.001,0.001);
  cTnc3->Divide(2,2,0.001,0.001);
  
  TH2D* hNc0 = new TH2D("hNc0","NaI[0] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hNc1 = new TH2D("hNc1","NaI[1] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hNc2 = new TH2D("hNc2","NaI[2] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hNc3 = new TH2D("hNc3","NaI[3] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hNc4 = new TH2D("hNc4","NaI[4] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hNc5 = new TH2D("hNc5","NaI[5] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hNc6 = new TH2D("hNc6","NaI[6] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hNc7 = new TH2D("hNc7","NaI[7] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hNc8 = new TH2D("hNc8","NaI[8] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hNc9 = new TH2D("hNc9","NaI[9] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  
  //set minimum for logz  
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
  
  //set zoom range
  // hNc0->GetXaxis()->SetRangeUser(1200,1600);
  // hNc1->GetXaxis()->SetRangeUser(1300,1600);
  // hNc2->GetXaxis()->SetRangeUser(1300,1600);
  // hNc3->GetXaxis()->SetRangeUser(1300,1600);
  // hNc4->GetXaxis()->SetRangeUser(1300,1600);
  // hNc5->GetXaxis()->SetRangeUser(1300,1600);
  // hNc6->GetXaxis()->SetRangeUser(1300,1600);
  // hNc7->GetXaxis()->SetRangeUser(1300,1600);
  // hNc8->GetXaxis()->SetRangeUser(1300,1600);
  // hNc9->GetXaxis()->SetRangeUser(1300,1600);

  cout<<"Base name for pictures= "<<baseN<<endl<<"Filetype for Pictures= "<<filetypeN<<endl;

  cTnc1->cd(1);
  t->Draw("calgam.cycle:calgam.NaI[0]>>hNc0","","COLZ");
  qq = baseN + "0" + filetypeN;
  //  cTnc1->cd(1)->SaveAs(qq.c_str());

  cTnc1->cd(2);
  t->Draw("calgam.cycle:calgam.NaI[1]>>hNc1","","COLZ");
  qq = baseN + "1" + filetypeN;
  //cTnc1->cd(2)->SaveAs(qq.c_str());
 
  cTnc1->cd(3);
  t->Draw("calgam.cycle:calgam.NaI[2]>>hNc2","","COLZ");
  qq = baseN + "2" + filetypeN;
  //cTnc1->cd(3)->SaveAs(qq.c_str());
 
  cout<<"   First canvas fill complete. Moving on"<<endl;

  cTnc2->cd(1);
  t->Draw("calgam.cycle:calgam.NaI[3]>>hNc3","","COLZ");
  qq = baseN + "3" + filetypeN;
  //cTnc2->cd(1)->SaveAs(qq.c_str());

  cTnc2->cd(2);
  t->Draw("calgam.cycle:calgam.NaI[4]>>hNc4","","COLZ");
  qq = baseN + "4" + filetypeN;
  //cTnc2->cd(2)->SaveAs(qq.c_str());

  cTnc2->cd(3);
  t->Draw("calgam.cycle:calgam.NaI[5]>>hNc5","","COLZ");
  qq = baseN + "5" + filetypeN;
  //  cTnc2->cd(3)->SaveAs(qq.c_str());

  cout<<"   Second canvas fill complete. Moving on"<<endl;

  cTnc3->cd(1);
  t->Draw("calgam.cycle:calgam.NaI[6]>>hNc6","","COLZ");
  qq = baseN + "6" + filetypeN;
  //cTnc3->cd(1)->SaveAs(qq.c_str());

  cTnc3->cd(2);
  t->Draw("calgam.cycle:calgam.NaI[7]>>hNc7","","COLZ");
  qq = baseN + "7" + filetypeN;
  //cTnc3->cd(2)->SaveAs(qq.c_str());

  cTnc3->cd(3);
  t->Draw("calgam.cycle:calgam.NaI[8]>>hNc8","","COLZ");
  qq = baseN + "8" + filetypeN;
  //cTnc3->cd(3)->SaveAs(qq.c_str());
 
  cTnc3->cd(4);
  t->Draw("calgam.cycle:calgam.NaI[9]>>hNc9","","COLZ");
  qq = baseN + "9" + filetypeN;
  //cTnc3->cd(4)->SaveAs(qq.c_str());

  f3.Write(); 

  //  gApplication->Terminate();
}
