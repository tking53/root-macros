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



TH2D* singleNcVe(const char* filename,int batchmode=0) {

  if (batchmode ==1){
    gROOT->SetBatch(kTRUE);
  }

  string qq;  
  string baseN = "SingleNaI-";
  string filetypeN = ".png";


  TFile *f = new TFile(filename);
  TTree *t = (TTree*) f->Get("gammas");
  
  stringstream ssss;
  ssss<<"SingleNaI-"<<filename;
  TFile f3(ssss.str().c_str(),"UPDATE");
  
  TCanvas *cTn = new TCanvas("cTn","NaI vs Cycle ",2000,2000);

  
  TH2D* hNc = new TH2D("hNc","NaI vs cycle", 8000.,0.,8000.,1700.,0.,1700.);

  
  // cout<<"Base name for pictures= "<<baseN<<endl<<"Filetype for Pictures= "<<filetypeN<<endl;

  t->Draw("calgam.cycle:calgam.NaI[8]>>hNc","calgam.NaI[8]>0","COLZ");
  qq = baseN + "8" + filetypeN;
  //cTnc3->cd(3)->SaveAs(qq.c_str());
 

  f3.Write(); 

}
