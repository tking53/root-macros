//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar  7 16:17:12 2018 by ROOT version 6.12/04
// from TTree GSaddback/Gamma Scint Addback
// found on file: CloverAddTest-GammaScint.root
//////////////////////////////////////////////////////////

#ifndef TWaddbackPlotter_h
#define TWaddbackPlotter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <string>
#include <sstream>



class TWaddbackPlotter : public TSelector {
public :
   TTreeReader     abReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

  TFile *fOutputFile;

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Double_t> GS_ABEnergy = {abReader, "GS_ABEnergy"};
   TTreeReaderValue<Double_t> GS_ABEvtNum = {abReader, "GS_ABEvtNum"};
   TTreeReaderValue<Int_t> GS_ABMulti = {abReader, "GS_ABMulti"};
   TTreeReaderValue<string> GS_ABType = {abReader, "GS_ABType"};
   TTreeReaderValue<Double_t> GS_ABbunchNum = {abReader, "GS_ABbunchNum"};
   TTreeReaderValue<Bool_t> GS_ABhasLRBeta = {abReader, "GS_ABhasLRBeta"};
   TTreeReaderValue<Double_t> GS_ABTime = {abReader, "GS_ABTime"};
   TTreeReaderValue<Double_t> GS_EvtStartTime = {abReader, "GS_EvtStartTime"};


   TWaddbackPlotter(TTree * /*tree*/ =0) { }
   virtual ~TWaddbackPlotter() { }
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

   ClassDef(TWaddbackPlotter,0);

   TObjArray *abHistArray;
  

};

#endif

#ifdef TWaddbackPlotter_cxx
void TWaddbackPlotter::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   abReader.SetTree(tree);
}

Bool_t TWaddbackPlotter::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef TWaddbackPlotter_cxx
