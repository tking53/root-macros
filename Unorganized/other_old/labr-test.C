#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>



void labrtest(const char* savename){

  gROOT->SetBatch(kTRUE);
  
  TChain *A = new TChain("Taux");
  
  for (int i=1;i<11;i++){
      stringstream ss;
      if (i <10){
	ss<<"/home/tking/segment-94/094rb_14-seg-0"<<i<<".root";
      }
      else if (i >= 10){
	ss<<"/home/tking/segment-94/094rb_14-seg-"<<i<<".root";
      }
      
      A->Add(ss.str().c_str());
  }

  TFile *f = new TFile(savename,"RECREATE");
  
  TH1D* L0 = new TH1D("L0","LaBr3 #0",8000.,0.,8000.);  
  TH1D* L1 = new TH1D("L1","LaBr3 #1",8000.,0.,8000.);  
  TH1D* L8 = new TH1D("L8","LaBr3 #8",8000.,0.,8000.);  
  TH1D* L9 = new TH1D("L9","LaBr3 #9",8000.,0.,8000.);  
  TH1D* L4 = new TH1D("L4","LaBr3 #4 (at beamspot for reference)",8000.,0.,8000.);        

  A->Draw("sing.LaBr[0]>>L0","sing.beta>0");
  cout<<"labr 0 done"<<endl;
  A->Draw("sing.LaBr[1]>>L1","sing.beta>0");
  cout<<"labr 1 done"<<endl;
  A->Draw("sing.LaBr[4]>>L4","sing.beta>0");
  cout<<"labr 4 done"<<endl;
  A->Draw("sing.LaBr[8]>>L8","sing.beta>0");  
  cout<<"labr 8 done"<<endl;
  A->Draw("sing.LaBr[9]>>L9","sing.beta>0");
  
  f->Write();
  gApplication->Terminate(0);

}
