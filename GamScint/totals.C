#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TH1D.h"
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

void totalsPlot(const char* inputname, const char* savename, const bool totalsOnly = false,const bool batchMode=false){
  //using namespace std;
  if (batchMode){
    gROOT->SetBatch(kTRUE);
  }

  
  TFile  *f = TFile::Open(inputname);

  //  std::string saveFile = savename =".root";
  //  TFile fnew(saveFile,"RECREATE");
  TFile fnew(savename,"RECREATE");

  TTree *Tsing = (TTree*)f->Get("GSsingles");

  TH1D* NaITotal = new TH1D("NaITotal","NaI Energy (Totals)",8000.,0.,8000.);
  TH1D* NaITotalBG = new TH1D("NaITotalBG","NaI Energy Beta Gate (Totals)",8000.,0.,8000.);

  TH1D* SHTotal = new TH1D("SHTotal"," HAGRiD Energy (Totals)",8000.,0.,8000.);
  TH1D* SHTotalBG = new TH1D("SHTotalBG"," HAGRiD Energy Beta Gate (Totals)",8000.,0.,8000.);

  Tsing->Draw("GS_Energy>>SHTotal","GS_Type==\"smallhag\"","");
  Tsing->Draw("GS_Energy>>SHTotalBG","GS_Type==\"smallhag\" && GS_HasLowResBeta==1","");
  
  Tsing->Draw("GS_Energy>>NaITotal","GS_Type==\"nai\"","");
  Tsing->Draw("GS_Energy>>NaITotalBG","GS_Type==\"nai\" && GS_HasLowResBeta==1","");
  
  if (!totalsOnly){

    TH2D* longTimePlotSH = new TH2D("longTimePlotSH"," HAGRiD Long Time Plot (Energy vs BunchNum)",8000.,0.,8000.,2000.,0.,2000.);
    TH2D* longTimePlotNai = new TH2D("longTimePlotNai"," NaI Long Time Plot (Energy vs BunchNum)",8000.,0.,8000.,2000.,0.,2000.);

    Tsing->Draw("GS_BunchNum:GS_Energy>>longTimePlotSH","GS_Type==\"smallhag\"");
    Tsing->Draw("GS_BunchNum:GS_Energy>>longTimePlotNai","GS_Type==\"nai\"");

    TH1D *SHin[16]={NULL};
    TH1D *NaiIn[10]={NULL};

    for (auto it = 0; it<16;it++){
      std::stringstream ss;
      std::stringstream sss;
      string num = std::to_string(it);
      ss<<"SH"<<num;
      sss<<"Small Hagrid #"<<num<<" Energy";
      SHin[it] = new TH1D(ss.str().c_str(),sss.str().c_str(),8000.,0.,8000.);
    }

    for (auto it = 0; it<10;it++){
      std::stringstream ss;
      std::stringstream sss;
      string num = std::to_string(it);
      ss<<"NaI"<<num;
      sss<<"NaI #"<<num<<" Energy";
      NaiIn[it] = new TH1D(ss.str().c_str(),sss.str().c_str(),8000.,0.,8000.);
    }

    for (int it=0; it<16; it++){
      TH1D* tmp = (TH1D*) SHin[it];
      std::stringstream cut;
      cut<<"GS_Type==\"smallhag\" && GS_Num=="<< it;
      stringstream plotVar;
      plotVar<<"GS_Energy>>"<<"SH"<<it;
      Tsing->Draw(plotVar.str().c_str(),cut.str().c_str() ,"");
      SHin[it]=tmp;

    }

    for (int it=0; it <10; it++){
      TH1D* tmp = (TH1D*) NaiIn[it];
      std::stringstream cut;
      cut<<"GS_Type==\"nai\" && GS_Num=="<< it;
      stringstream plotVar;
      plotVar<<"GS_Energy>>"<<"NaI"<<it;
      Tsing->Draw(plotVar.str().c_str(),cut.str().c_str() ,"");
      NaiIn[it]=tmp;
    }
  }
  fnew.Write();
  fnew.Close();
  f->Close();
}
