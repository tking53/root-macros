// AnamergerSelector for PROOF created by Rin Yokoyama on 7/21/2017

#ifndef ANAMERGER_SELECTOR_H
#define ANAMERGER_SELECTOR_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <map>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TStopwatch.h"
#include "TSelector.h"
#include "TProofServ.h"
#include "Rtypes.h"
#include "TMath.h"
#include "TMarker.h"
#include "TProof.h"
#include "TParameter.h"

#define WHAT_COMPUTER 3
//#include "WhatComputer.txt"

#if (WHAT_COMPUTER == 3)
#define AM_PATH "/home/ryokoyam/briken/ur17b/ana/test/"
#include "/home/ryokoyam/opt/BrikenTools/Merger/MergerSoft/data2Tree.cxx"
//#include "/home/ryokoyam/briken/ur17b/utk/anamerger/hIsotope.cxx"
#endif

#if (WHAT_COMPUTER == 4)
#define AM_PATH "/home/ntbrewer/scripts/anamerger/"
#include "/home/ntbrewer/scripts/anamerger/hIsotope.cxx"
#endif

#if (WHAT_COMPUTER == 5)
#define AM_PATH "/home/bcrasco/BRIKEN_Computing/scripts/anamerger/"
#include "/home/bcrasco/BRIKEN_Computing/scripts/anamerger/hIsotope.cxx"
#endif

#if (WHAT_COMPUTER == 6)
#define AM_PATH "/home/briken/utk/anamerger/"
#include "/home/briken/utk/anamerger/hIsotope.cxx"
#endif


const string ANAMERGER_PATH = AM_PATH;

class AnamergerSelector : public TSelector {
public :

  AnamergerSelector(TTree * =0);
  virtual ~AnamergerSelector();

  virtual Int_t   Version() const { return 2; }
  virtual void    Init(TTree* mergedData);
  virtual void    Begin(TTree *mergedData);
  virtual void    SlaveBegin(TTree *mergedData);
  virtual Bool_t  Notify() { return kTRUE;}
  virtual Bool_t  Process(Long64_t entry);
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate(){}
  virtual void    Terminate();

protected:
  const Double_t tini = 0;
  const Double_t tfin = 1e15;

  TTreeReader aReader;
  TTreeReaderValue <brData2TTree>    bigrips;
  TTreeReaderValue <impData2TTree>   implant;
  TTreeReaderValue <betaData2TTree>  beta; 
  TTreeReaderValue <neuData2TTree>   neutron;
  TTreeReaderValue <gammaData2TTree> gamma;  
  TTreeReaderValue <ancData2TTree>   ancillary;

  static const Int_t kNDivE = 1;

  // array for histograms
  TObjArray* fHistArray;
  Long64_t event_id;
  Double_t proc_id;
  TTree* fTree;
  Double_t test_val;
  static Double_t test_val2;
  TParameter<Double_t> *test_param;
  TProof* fTProof;
  // output file
  TFile * fOutputFile;

  ClassDef(AnamergerSelector,0)
};

#endif
