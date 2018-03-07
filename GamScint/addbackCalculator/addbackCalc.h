//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Feb  6 09:12:23 2018 by ROOT version 6.12/04
// from TTree GSsingles/Gamma Scint Singles
//////////////////////////////////////////////////////////

#ifndef addbackCalc_h
#define addbackCalc_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <string>
#include "GSaddback.hpp"


class addbackCalc : public TSelector {
public :
  TTreeReader     singReader;  //!the tree reader
  TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

  // Readers to access the data (delete the ones you do not need).
  TTreeReaderValue<Double_t> GS_LastBunchTime = {singReader, "GS_LastBunchTime"};
  TTreeReaderValue<Double_t> GS_Energy = {singReader, "GS_Energy"};
  TTreeReaderValue<Double_t> GS_BunchNum = {singReader, "GS_BunchNum"};
  TTreeReaderValue<string> GS_Type = {singReader, "GS_Type"};
  TTreeReaderValue<string> GS_Group = {singReader, "GS_Group"};
  TTreeReaderValue<Int_t> GS_Num = {singReader, "GS_Num"};
  TTreeReaderValue<Double_t> GS_Time = {singReader, "GS_Time"};
  TTreeReaderValue<Bool_t> GS_HasLowResBeta = {singReader, "GS_HasLowResBeta"};
  TTreeReaderValue<Bool_t> GS_HasMedResBeta = {singReader, "GS_HasMedResBeta"};
  TTreeReaderValue<Double_t> GS_BetaGammaTDiff = {singReader, "GS_BetaGammaTDiff"};
  TTreeReaderValue<Double_t> GS_BetaEnergy = {singReader, "GS_BetaEnergy"};
  TTreeReaderValue<Double_t> GS_BetaMulti = {singReader, "GS_BetaMulti"};
  TTreeReaderValue<Double_t> GS_EvtNum = {singReader, "GS_EvtNum"};
  
  Double_t EvtLast = 0; //evt last counter
  std::vector<std::vector<Double_t>> pixieEvtVec;
  std::set<std::string> groupList;

  addbackCalc(TTree * /*tree*/ =0) { }
  virtual ~addbackCalc() { }
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();

  ClassDef(addbackCalc,0);

};

#endif

#ifdef addbackCalc_cxx
void addbackCalc::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the reader is initialized.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  singReader.SetTree(tree);
}

Bool_t addbackCalc::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}


#endif // #ifdef addbackCalc_cxx
