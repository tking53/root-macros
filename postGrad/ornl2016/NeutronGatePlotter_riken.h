//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep  5 16:16:20 2018 by ROOT version 6.14/02
// from TChain PixTree/
//////////////////////////////////////////////////////////

#ifndef NeutronGatePlotter_riken_h
#define NeutronGatePlotter_riken_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TCutG.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include "ProcessorRootStruc.hpp"
#include <vector>




class NeutronGatePlotter_riken : public TSelector {
 public :
  TTreeReader     nReader;  //!the tree reader
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain

  // Readers to access the data (delete the ones you do not need).

  TTreeReaderArray<processor_struct::GAMMASCINT> gamscint = {nReader,"gamma_scint_vec_"};
  TTreeReaderArray<processor_struct::CLOVERS> clover = {nReader,"clover_vec_"};
  TTreeReaderArray<processor_struct::VANDLES> vandle = {nReader,"vandle_vec_"};
  
  TCutG* cutg;
  //Gating Declares
  Double_t Gmin;
  Double_t TofMin;
  Double_t QDCmin;
  Double_t HagMinGammaNum;
  std::string isotope ;

  std::pair<Double_t,Double_t>qdcCut;
  std::pair<Double_t,Double_t>PromptTof;
  std::pair<Double_t,Double_t>DelayTof;
  Double_t CloverGateWidth,HagGateWidth;
  std::vector<Double_t>gammaLines;
  std::pair<Double_t,Double_t>vTDiff;

  Double_t  CloverGoodTDiff;
  Double_t  HagGoodTDiff;

  //other declares
  std::string outputFilePrefix;
  TObjArray *neutronHists;
  TFile* outFile;

  NeutronGatePlotter_riken(TTree * /*tree*/ =0) { }
  virtual ~NeutronGatePlotter_riken() { }
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

  ClassDef(NeutronGatePlotter_riken,0);

};

#endif

#ifdef NeutronGatePlotter_riken_cxx
void NeutronGatePlotter_riken::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the reader is initialized.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  nReader.SetTree(tree);
}

Bool_t NeutronGatePlotter_riken::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}


inline std::vector<std::string> SplitString(const std::string &str, const std::string &delimiter) {
  std::vector<std::string> tokenized;
  size_t current, next = -1;
  do {
    current = next + 1;
    next = str.find_first_of(delimiter, current);
    tokenized.push_back(str.substr(current, next - current));
  } while (next != std::string::npos);
  return tokenized;
}

#endif // #ifdef NeutronGatePlotter_riken_cxx
