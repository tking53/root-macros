// AnamergerSelector for PROOF created by Rin Yokoyama on 7/21/2017

#ifndef ANAMERGER_SELECTOR_H
#define ANAMERGER_SELECTOR_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <unistd.h>
#include <map>

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TStopwatch.h"
#include "TSelector.h"
#include "TProofServ.h"
#include "Rtypes.h"
#include "TMath.h"

#define WHAT_COMPUTER 6
//#include "WhatComputer.txt"

#if (WHAT_COMPUTER == 3)
#define AM_PATH "/home/ryokoyam/briken/ur17a/scripts/merger/tmp/anamerger/"
#include "/home/ryokoyam/briken/ur17a/scripts/merger/tmp/anamerger/hIsotope.cxx"
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

  int loadCUTG(std::string icutname);

protected:
  std::vector <hIsotope> vectorIsotopes;
  const Double_t tini = 0;
  const Double_t tfin = 1e15;

  TTreeReader aReader;
  TTreeReaderValue <brData2TTree>    bigrips;
  TTreeReaderValue <impData2TTree>   implant;
  TTreeReaderValue <betaData2TTree>  beta; 
  TTreeReaderValue <neuData2TTree>   neutron;
  TTreeReaderValue <gammaData2TTree> gamma;  
  TTreeReaderValue <ancData2TTree>   ancillary;

  // array for histograms
  TObjArray* fHistArray;
  // output file
  TFile * fOutputFile;

  ClassDef(AnamergerSelector,0)
};

#endif
