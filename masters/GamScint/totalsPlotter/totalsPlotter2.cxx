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
#include "TTreeReader.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TTreeReaderValue.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>

void totalsPlotter2(const char* savename,const bool batchMode=true,  const bool totalsOnly = false){
  //using namespace std;
  if (batchMode){
    gROOT->SetBatch(kTRUE);
  }

  TTreeReader SingReader;
  TTreeReader ABReader;

  TFile fnew(savename,"RECREATE");

  TChain *GS = (TChain*)gROOT->FindObject("GSsingles");
  TChain *AB = (TChain*)gROOT->FindObject("GSaddback");

  TH1D* NaITotal = new TH1D("NaITotal","NaI Energy (Totals)",8000.,0.,8000.);
  TH1D* NaITotalBG = new TH1D("NaITotalBG","NaI Energy Beta Gate (Totals)",8000.,0.,8000.);
  TH1D* NaITWAB  = new TH1D("NaITypeWideAB","NaI AddBack Energy (TypeWide)",8000.,0.,8000.);
  TH1D* NaITWABBG  = new TH1D("NaITypeWideABBG","NaI BG AddBack Energy (TypeWide)",8000.,0.,8000.);

  TH1D* SHTotal = new TH1D("SHTotal"," HAGRiD Energy (Totals)",8000.,0.,8000.);
  TH1D* SHTotalBG = new TH1D("SHTotalBG","HAGRiD Energy Beta Gate (Totals)",8000.,0.,8000.);
  TH1D* SHTWAB = new TH1D("SHTypeWideAB","HAGRiD AddBack Energy (TypeWide)",8000.,0.,8000.);
  TH1D* SHTWABBG = new TH1D("SHTypeWideABBG","HAGRiD BG AddBack Energy (TypeWide)",8000.,0.,8000.);

  TH1D* ClovTotal = new TH1D("ClovTotal","HPGe Clover Energy (Totals)",8000.,0.,8000.);
  TH1D* ClovTotalBG = new TH1D("ClovTotalBG","HPGe Clover Energy Beta Gate (Totals)",8000.,0.,8000.);
  TH1D* ClovTWAB = new TH1D("ClovTypeWideAB","HPGe Clover AddBack Energy (TypeWide)",8000.,0.,8000.);
  TH1D* ClovTWABBG = new TH1D("ClovTypeWideABBG","HPGe Clover BG AddBack Energy (TypeWide)",8000.,0.,8000.);

  TH1D* SHin[16]={NULL};
  TH1D* NaiIn[10]={NULL};
  TH1D* HPGen[4]={NULL};

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
  for (auto it = 0; it<4;it++){
    std::stringstream ss;
    std::stringstream sss;
    string num = std::to_string(it);
    ss<<"Clover"<<num;
    sss<<"Leaf #"<<num<<" Energy";
    HPGen[it] = new TH1D(ss.str().c_str(),sss.str().c_str(),8000.,0.,8000.);
  }
  SingReader.SetTree( GSsingles );
  ABReader.SetTree( GSaddback );

  while (SingReader.Next()){





  }//end singles tree while



  fnew.Write();
  fnew.Close();
  //f->Close();
}
