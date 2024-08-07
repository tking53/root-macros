#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>


void tracePlotter(){
  TFile *_file0 = TFile::Open("testFiles/single94file-GammaScint.root");
  TTree *GS = (TTree*)_file0->Get("GSsingles");
  TTreeReader singe;
  cout<<"readermade"<<endl;
  singe.SetTree( GS );
  TTreeReaderValue<std::vector<unsigned int>> trace = {singe,"GS_Trace"};
  TTreeReaderValue<Double_t> tdiff = {singe,"GS_BetaGammaTDiff"};
  TTreeReaderValue<std::string> type = {singe,"GS_Type"};

  TCanvas* c1 = new TCanvas("c1","c1",1200,600);
  TCanvas* c2 = new TCanvas("c2","c2",1200,600);
  c1->SetLogz();
  c2->SetLogz();
  TH2D* hagTrace1=new TH2D("hagTrace1","hagTraces1",200.,0.,200.,5000,0.,5000.);
  TH2D* hagTrace2=new TH2D("hagTrace2","hagTraces2",200.,0.,200.,5000,0.,5000.);

  while(singe.Next()){
    //cout<<"while Loop"<<endl;
    std::vector<unsigned int> traceV = *trace;
    std::string dtype = *type;
    double bgTDiff = *tdiff;
    if (dtype == "smallhag"){
      for (unsigned int it=0,itend = traceV.size();it<itend;it++){
	hagTrace1->Fill(it,traceV.at(it));
	if (bgTDiff > 150 && bgTDiff <250){
	  hagTrace2->Fill(it,traceV.at(it));
	}
      }
    }
  }
  c1->cd();
  hagTrace1->Draw("colz");
  
  c2->cd();
  hagTrace2->Draw("colz");
}
