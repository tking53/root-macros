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
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include <vector>
#include "TChain.h"
#include "TProof.h"

//TChain* sysChain
TFile *fOut;
TTreeReader fRead;

TChain *chain1;
TChain *chain2;

TProof *pr;

TTreeReaderValue<double> GS_ABEnergy = {fRead, "GS_ABEnergy"};
TTreeReaderValue<double> GS_ABEvtNum = {fRead, "GS_ABEvtNum"};
TTreeReaderValue<std::string> GS_ABType = {fRead, "GS_ABType"};
TTreeReaderValue<bool> GS_ABhasLRBeta = {fRead, "GS_ABhasLRBeta"};


TTreeReaderValue<double> GS_Energy = {fRead, "GS_Energy"};
TTreeReaderValue<std::string> GS_Type = {fRead, "GS_Type"};
TTreeReaderValue<std::string> GS_Group = {fRead, "GS_Group"};
TTreeReaderValue<bool> GS_HasLowResBeta = {fRead, "GS_HasLowResBeta"};
TTreeReaderValue<double> GS_EvtNum = {fRead, "GS_EvtNum"};

std::vector<double> SingTrack;
double ABTotal = 0;
double evtLast = 0;
bool LRBG;

void Cplotter(){  
  fOut = new TFile("CPlotterOut.root","RECREATE");
  pr = TProof::Open("lite://","workers=8");
  chain1 = new TChain("GSsingles");
  chain2 = new TChain("GSaddback");
  
  for (auto it=1; it<=5 ; it++){
    std::stringstream ss;
    ss << "/home/hanayo/research/ornl2016/thesis/interHolding/mk2_94rb/94rb_14_set"<<it<<"/094rb_14_5scan_set"<<it<<"-GammaScint.root";
    chain1->Add(ss.str().c_str());
    chain2->Add(ss.str().c_str());
  }
  gDirectory->Add(chain1);
  gDirectory->Add(chain2);
  chain1->AddFriend(chain2);
  
  
  chain1->SetProof();
  chain2->SetProof();
  std::cout << "Number of TTrees added to chain1: " << chain1->GetNtrees() << std::endl;
  std::cout << "Number of TTrees added to chain2: " << chain2->GetNtrees() << std::endl;
 
  fRead.SetTree( chain1 );
  
  gROOT->SetBatch();
  cout<<"canvas-start"<<endl;
  TCanvas *c1 = new TCanvas("c1","c1",1920.,1200.);
  
  TH2D* CPlot = new TH2D("CPlot","CPlot",10000.,0.,10000.,10000.,0.,10000.);
  TH2D* CPlotBG = new TH2D("CPlotBG","CPlotBG",10000.,0.,10000.,10000.,0.,10000.);
  while (fRead.Next()){
  
    double singEn = (*GS_Energy);
    double ABEn = (*GS_ABEnergy);

    double gsNum = (*GS_EvtNum);
    double abNum = (*GS_ABEvtNum);

    bool gs_hlrb = (*GS_HasLowResBeta);

    std::string gsType = (*GS_Type);
    std::string gsABType = (*GS_ABType);

    if (gsNum != abNum){
      evtLast = gsNum;
      continue;
    }
    
    if (!(evtLast == gsNum)){

      for (auto it=SingTrack.begin(); it != SingTrack.end(); it++){
        CPlot->Fill(ABTotal,(*it));
      }

      SingTrack.clear();
      ABTotal = 0;
      evtLast=gsNum;
      continue;

    }

    //    if ((evtLast == gsNum == abNum)){
    
      if (gsType == "clover"){
        SingTrack.push_back(singEn);
      }
      ABTotal += ABEn;
      LRBG = gs_hlrb; 
      evtLast = gsNum;
      //}
    
  }
  
  /* CPlot1->SetMinimum(1);
  CPlot2->SetMinimum(1);

  CPlot1->GetXaxis()->SetRangeUser(0,6500);
  CPlot1->GetYaxis()->SetRangeUser(0,6500);

  CPlot2->GetXaxis()->SetRangeUser(0,6500);
  CPlot2->GetYaxis()->SetRangeUser(0,6500);

  c1->Modified();
  c2->Modified();

  */
  //gDirectory->Add(CPlot);
  //gDirectory->Add(CPlotBG);
  
  fOut->Write();
  

}
