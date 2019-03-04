//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 28 16:21:59 2019 by ROOT version 6.14/06
// from TChain PixTree/
//////////////////////////////////////////////////////////

#ifndef Rb_97_Full_h
#define Rb_97_Full_h

// Root Headers needed 
#include </opt/root/6.14.06/include/TROOT.h> //this is unkowned why it needs the full path even though all the ROOT  headers are in the same folder
#include <TSystem.h>
#include <TSelector.h>

// Data Input headers
#include <TFile.h>
#include <TChain.h>
#include "ProcessorRootStruc.hpp"

// Reader Headers
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

//Visualization headers
#include <TH2.h>
#include <TStyle.h>

// System STD Headers
#include <sstream>
#include <string>


class Rb97Full : public TSelector {
public :

    TTreeReader     fReader;  //!the tree reader
    TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

    // Readers to access the data (delete the ones you do not need).
    TTreeReaderValue<Double_t> eventNum = {fReader, "eventNum"};
    TTreeReaderValue<string> fileName = {fReader, "fileName"};

    TTreeReaderArray<processor_struct::GAMMASCINT> gamscint = {fReader,"gamma_scint_vec_"};
    TTreeReaderArray<processor_struct::CLOVERS> clover = {fReader,"clover_vec_"};
    TTreeReaderArray<processor_struct::VANDLES> vandle = {fReader,"vandle_vec_"};
    TTreeReaderArray<processor_struct::LOGIC> logic = {fReader,"logic_vec_"};
    TTreeReaderArray<processor_struct::DOUBLEBETA> doublebeta = {fReader,"doublebeta_vec_"};

    //declares
    TObjArray* GammaArray;
    TFile* outFile;
    Double_t DSPclockInSeconds = 8e-9;
    Double_t ADCclockInSeconds = 4e-9;
    string outputFilePrefix;

    Rb97Full(TTree * /*tree*/ =0) { }
    virtual ~Rb97Full() { }
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

    ClassDef(Rb97Full,0);

};

#endif

#ifdef Rb97Full_cxx
void Rb97Full::Init(TTree *tree)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the reader is initialized.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    fReader.SetTree(tree);
}

Bool_t Rb97Full::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}


#endif // #ifdef Rb97Full_cxx
