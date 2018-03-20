//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar  7 16:16:57 2018 by ROOT version 6.12/04
// from TTree GSsingles/Gamma Scint Singles
// found on file: CloverAddTest-GammaScint.root
//////////////////////////////////////////////////////////

#ifndef SinglesPlotter_h
#define SinglesPlotter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <string>



class SinglesPlotter : public TSelector {
public :
   TTreeReader     sReader;  //!the tree reader
   TTree          *sChain = 0;   //!pointer to the analyzed TTree or TChain


  // output file
  TFile * fOutputFile;
   
   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Double_t> GS_Energy = {sReader, "GS_Energy"};
   TTreeReaderValue<string> GS_Type = {sReader, "GS_Type"};
   TTreeReaderValue<string> GS_Group = {sReader, "GS_Group"};
   TTreeReaderValue<Int_t> GS_Num = {sReader, "GS_Num"};
   TTreeReaderValue<Double_t> GS_Time = {sReader, "GS_Time"};
   TTreeReaderValue<Bool_t> GS_HasLowResBeta = {sReader, "GS_HasLowResBeta"};
   TTreeReaderValue<Bool_t> GS_HasMedResBeta = {sReader, "GS_HasMedResBeta"};
   TTreeReaderValue<Double_t> GS_EvtStartTime = {sReader, "GS_EvtStartTime"};
   TTreeReaderValue<Double_t> GS_BetaGammaTDiff = {sReader, "GS_BetaGammaTDiff"};
   TTreeReaderValue<Double_t> GS_BetaEnergy = {sReader, "GS_BetaEnergy"};
   TTreeReaderValue<Double_t> GS_BetaTime = {sReader, "GS_BetaTime"};
   TTreeReaderValue<Double_t> GS_BetaMulti = {sReader, "GS_BetaMulti"};
   TTreeReaderValue<Double_t> GS_EvtNum = {sReader, "GS_EvtNum"};
   TTreeReaderValue<Double_t> GS_BunchNum = {sReader, "GS_BunchNum"};
   TTreeReaderValue<Double_t> GS_LastBunchTime = {sReader, "GS_LastBunchTime"};
   TTreeReaderValue<Double_t> GS_bighagmulti = {sReader, "GS_bighagmulti"};
   TTreeReaderValue<Double_t> GS_smallhagmulti = {sReader, "GS_smallhagmulti"};
   TTreeReaderValue<Double_t> GS_naimulti = {sReader, "GS_naimulti"};
   TTreeReaderValue<Double_t> GS_clovermulti = {sReader, "GS_clovermulti"};


   SinglesPlotter(TTree * /*tree*/ =0) { }
   virtual ~SinglesPlotter() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return sChain ? sChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   TObjArray *singHistArray;
   
   ClassDef(SinglesPlotter,0);

};

#endif

#ifdef SinglesPlotter_cxx
void SinglesPlotter::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   sReader.SetTree(tree);
}

Bool_t SinglesPlotter::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef SinglesPlotter_cxx
