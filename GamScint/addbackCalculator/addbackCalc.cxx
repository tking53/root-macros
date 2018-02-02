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
#include <string.h>
#include <stdio.h>

//#include "addbackCalc.hpp"
#include "GSaddback.hpp"

inline std::string CheckFileName(const std::string& FileName,const int position) //<! position 0 is filename position 1 is the file extension
{
  if(FileName.find_last_of(".") != std::string::npos)
    return FileName.substr(FileName.find_last_of("."));
  return "";
}


void addbackCalculator(const char* inputname, const char* savename,const Bool_t batchMode){
  TFile *fin;  //!< Pointer to the input file
  TFile *fout; //!< Pointer to the output file
  TTree *Gsing; //!< Pointer to the input singles tree
  TTree *Gsrab;  //!< Pointer to the AddBack tree that we will make

  const Double_t shthresh = 10;
  const Double_t shSEW = 1e6;
  Double_t shRefTime=-2*shSEW;

  const Double_t naithresh = 10;
  const Double_t naiSEW = 1e6;
  Double_t naiRefTime=-2*naiSEW;
  /*
  std::vector<GSAddback> naiABvec_;
  std::vector<GSAddback> shABvec_;

  naiABvec_.emplace_back(GSAddback(0,0,0));
  shABvec_.emplace_back(GSAddback(0,0,0));
  */
  Double_t ABEnergy = 0; //!<Energy of the addback event
  Double_t ABEvtNum = 0; //!< Pixie Event number of the addback event
  Int_t ABMulti = 0; //!< Number of Detectors used in the Addback event
  std::string ABType = "";//!< Detector subtype of the addback event
  Double_t ABbunchNum = 0; //!<Bunch number of addback event
  Bool_t ABhasLRBeta =false ; //!< Low Res Beta flag for addback
  std::string ABgroup = ""; //!< group for addback

  if(batchMode){
    gROOT->SetBatch(kTRUE);
  }

  // std::string inExt = CheckFileName(inputname,1);
  // std::string inFile = CheckFileName(inputname,0);

  // if (savename == ""){
  //   if(inExt == ""){
  //     savename =inFile+"_Rootaddback.root";
  //   } else {
  //     savename = 
  //   }
  // }
  // if (inExt == "" ){
  //   inputname = +".root"
  //     }

  fin = TFile::Open(inputname);
  fout = new TFile(savename,"RECREATE");

  Gsing = (TTree*)fin->Get("GSsingles");
  Gsrab = new TTree("RootAddback","Tree of addback events created with root");

  Gsrab->SetAutoFlush(3000);
  Gsrab->Branch("ABEnergy",&ABEnergy);
  Gsrab->Branch("ABEvtNum",&ABEvtNum);
  Gsrab->Branch("ABMulti",&ABMulti);
  Gsrab->Branch("ABType",&ABType);
  Gsrab->Branch("ABbunchNum",&ABbunchNum);
  Gsrab->Branch("ABhasLRBeta",&ABhasLRBeta);
  Gsrab->Branch("ABgroup",&ABgroup);


  std::set<std::string> groupList;

  Double_t *GS_EvtNum;
  Double_t *GS_Energy;
  Double_t *GS_BunchNum;
  Double_t *GS_Num;
  Double_t *GS_Time;
  Bool_t *GS_HasLowResBeta;
  std::string *GS_Type;
  std::string *GS_Group;

  std::map<std::string,std::vector<GSAddback>> shAddbackMap;
  std::map<std::string,std::vector<GSAddback>> naiAddbackMap;

  Gsing->SetBranchAddress("GS_Energy",&GS_Energy);
  Gsing->SetBranchAddress("GS_EvtNum",&GS_EvtNum);
  Gsing->SetBranchAddress("GS_BunchNum",&GS_BunchNum);
  Gsing->SetBranchAddress("GS_Num",&GS_Num);
  Gsing->SetBranchAddress("GS_Time",&GS_Time);
  Gsing->SetBranchAddress("GS_HasLowResBeta",&GS_HasLowResBeta);
  Gsing->SetBranchAddress("GS_Type",&GS_Type);
  Gsing->SetBranchAddress("GS_Group",&GS_Group);

  for (auto it=0; it<Gsing->GetEntries();it++){
    Gsing->GetEntry(it);
    groupList.emplace((*GS_Group));
    }

  for (auto it=groupList.begin(); it != groupList.end();it++){
    if (strncmp((*it).c_str(),"sh",2) == 0){
      std::vector<GSAddback> *tmp1 = new std::vector<GSAddback>;
      shAddbackMap.emplace((*it),(*tmp1));
    } else if (strncmp((*it).c_str(),"nai",2) == 0){
      //      naiAddbackMap.emplace((*it),GSAddback(0,0,0));
    }
  }

  for (auto i=0;i<Gsing->GetEntries(); i++){
    Gsing->GetEntry(i);
    //if (GS_EvtNum != EvtLast){
      
    //}
    

  
  }
}
