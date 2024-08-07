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



// inline string outputset(string num){ //histogram number
//   qqqq = baseH + num + filetypeH;
//   return (qqqq);
// }


TH2D* offlineCycEnHc(const char* filename) {
  gROOT->SetBatch(kTRUE);

  string qqqq;  
  string baseH="Hag-Output-EvC-Hag" ;
  string filetypeH=".png";

  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  
  stringstream ss;
  ss<<"FULLL-HagOutput-"<<filename;
  TFile f2(ss.str().c_str(),"RECREATE");

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

  TH2D* hHc0 = new TH2D("hHc0","Hag[0] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc1 = new TH2D("hHc1","Hag[1] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc2 = new TH2D("hHc2","Hag[2] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc3 = new TH2D("hHc3","Hag[3] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc4 = new TH2D("hHc4","Hag[4] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc5 = new TH2D("hHc5","Hag[5] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc6 = new TH2D("hHc6","Hag[6] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc7 = new TH2D("hHc7","Hag[7] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc8 = new TH2D("hHc8","Hag[8] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc9 = new TH2D("hHc9","Hag[9] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc10 = new TH2D("hHc10","Hag[10] vs cycle", 8000.,0.,8000.,1700.,0.,1700.); 
  TH2D* hHc11 = new TH2D("hHc11","Hag[11] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc12 = new TH2D("hHc12","Hag[12] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc13 = new TH2D("hHc13","Hag[13] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc14 = new TH2D("hHc14","Hag[14] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);
  TH2D* hHc15 = new TH2D("hHc15","Hag[15] vs cycle", 8000.,0.,8000.,1700.,0.,1700.);

  // Set min for LogZ
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

  // //Set defualt Zoom
  // hHc0->GetXaxis()->SetRangeUser(775,925);
  // hHc1->GetXaxis()->SetRangeUser(775,925);
  // hHc2->GetXaxis()->SetRangeUser(775,925);
  // hHc3->GetXaxis()->SetRangeUser(775,925);
  // hHc4->GetXaxis()->SetRangeUser(775,925);
  // hHc5->GetXaxis()->SetRangeUser(775,925);
  // hHc6->GetXaxis()->SetRangeUser(775,925);
  // hHc7->GetXaxis()->SetRangeUser(775,925);
  // hHc8->GetXaxis()->SetRangeUser(775,925);
  // hHc9->GetXaxis()->SetRangeUser(775,925);
  // hHc10->GetXaxis()->SetRangeUser(775,925);
  // hHc11->GetXaxis()->SetRangeUser(775,925);
  // hHc12->GetXaxis()->SetRangeUser(775,925);
  // hHc13->GetXaxis()->SetRangeUser(775,925);
  // hHc14->GetXaxis()->SetRangeUser(775,925);
  // hHc15->GetXaxis()->SetRangeUser(775,925);


  cout<<"Base name for pictures= "<<baseH<<endl<<"Filetype for Pictures= "<<filetypeH<<endl;

  cThc1->cd(1);
  t->Draw("calgam.cycle:calgam.Hag[0]>>hHc0","","COLZ");
  qqqq = baseH + "0" + filetypeH;
  //cThc1->cd(1)->SaveAs(qqqq.c_str());


  cThc1->cd(2);
  t->Draw("calgam.cycle:calgam.Hag[1]>>hHc1","","COLZ");
  qqqq = baseH + "1" + filetypeH;
  //cThc1->cd(2)->SaveAs(qqqq.c_str()); 

  cThc1->cd(3);
  t->Draw("calgam.cycle:calgam.Hag[2]>>hHc2","","COLZ");
  qqqq = baseH + "2" + filetypeH;
  //cThc1->cd(3)->SaveAs(qqqq.c_str()); 
 
  cThc1->cd(4);
  t->Draw("calgam.cycle:calgam.Hag[3]>>hHc3","","COLZ");
  qqqq = baseH + "3" + filetypeH;
  //  cThc1->cd(4)->SaveAs(qqqq.c_str()); 
 
  cout<<"First Canvas Filled"<<endl;
 
  cThc2->cd(1);
  t->Draw("calgam.cycle:calgam.Hag[4]>>hHc4","","COLZ");
  qqqq = baseH + "4" + filetypeH;
  ///cThc2->cd(1)->SaveAs(qqqq.c_str());   
 
  cThc2->cd(2);
  t->Draw("calgam.cycle:calgam.Hag[5]>>hHc5","","COLZ");
  qqqq = baseH + "5" + filetypeH;
  // cThc2->cd(2)->SaveAs(qqqq.c_str()); 
 
  cThc2->cd(3);
  t->Draw("calgam.cycle:calgam.Hag[6]>>hHc6","","COLZ");
  qqqq = baseH + "6" + filetypeH;
  //cThc2->cd(3)->SaveAs(qqqq.c_str()); 
 
  cThc2->cd(4);
  t->Draw("calgam.cycle:calgam.Hag[7]>>hHc7","","COLZ");
  qqqq = baseH + "7" + filetypeH;
  // cThc2->cd(4)->SaveAs(qqqq.c_str()); 
 
  cout<<"Second Canvas Filled"<<endl;
 
  cThc3->cd(1);
  t->Draw("calgam.cycle:calgam.Hag[8]>>hHc8","","COLZ");
  qqqq = baseH + "8" + filetypeH;
  //cThc3->cd(1)->SaveAs(qqqq.c_str()); 
 
  cThc3->cd(2);
  t->Draw("calgam.cycle:calgam.Hag[9]>>hHc9","","COLZ");
  qqqq = baseH + "9" + filetypeH; 
  //cThc3->cd(2)->SaveAs(qqqq.c_str()); 

  cThc3->cd(3);
  t->Draw("calgam.cycle:calgam.Hag[10]>>hHc10","","COLZ");
  qqqq = baseH + "10" + filetypeH;;
  //cThc3->cd(3)->SaveAs(qqqq.c_str()); 

  cThc3->cd(4);
  t->Draw("calgam.cycle:calgam.Hag[11]>>hHc11","","COLZ");
  qqqq = baseH + "11" + filetypeH;;
  //  cThc3->cd(4)->SaveAs(qqqq.c_str()); 

  cout<<"Third Canvas Filled"<<endl;
 
  cThc4->cd(1);
  t->Draw("calgam.cycle:calgam.Hag[12]>>hHc12","","COLZ");
  qqqq = baseH + "12" + filetypeH;;
  //  cThc4->cd(1)->SaveAs(qqqq.c_str()); 

  cThc4->cd(2);
  t->Draw("calgam.cycle:calgam.Hag[13]>>hHc13","","COLZ");
  qqqq = baseH + "13" + filetypeH;;
  ///cThc4->cd(2)->SaveAs(qqqq.c_str()); 
 
  cThc4->cd(3);
  t->Draw("calgam.cycle:calgam.Hag[14]>>hHc14","","COLZ");
  qqqq = baseH + "14" + filetypeH;;
  //cThc4->cd(3)->SaveAs(qqqq.c_str()); 

  cThc4->cd(4);
  t->Draw("calgam.cycle:calgam.Hag[15]>>hHc15","","COLZ");
  qqqq = baseH + "15" + filetypeH;;
  //  cThc4->cd(4)->SaveAs(qqqq.c_str()); 

  f2.Write();
  
  //  gApplication->Terminate();  
}
