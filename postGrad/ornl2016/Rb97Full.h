//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 28 16:21:59 2019 by ROOT version 6.14/06
// from TChain PixTree/
//////////////////////////////////////////////////////////

#ifndef Rb_97_Full_h
#define Rb_97_Full_h

// Root Headers needed
#include <TROOT.h>  //this is unknown why it needs the full path even though all the ROOT  headers are in the same folder
#include <TSelector.h>
#include <TSystem.h>

// Data Input headers
#include <TApplication.h>
#include <TChain.h>
#include <TFile.h>
#include "ProcessorRootStruc.hpp"

// Reader Headers
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

//Visualization headers
#include <TH2.h>
#include <TH3.h>
#include <TH3F.h>
#include <THnSparse.h>
#include <TStyle.h>

//Analysis headers
#include <TCutG.h>
#include "VandleTCuts.h"

// System STD Headers
#include <sstream>
#include <string>

class Rb97Full : public TSelector {
   public:
    TTreeReader fReader;  //!the tree reader
    TTree *fChain = 0;    //!pointer to the analyzed TTree or TChain

    // Readers to access the data (delete the ones you do not need).
    TTreeReaderValue<Double_t> eventNum = {fReader, "eventNum"};
    TTreeReaderValue<string> fileName = {fReader, "fileName"};

    TTreeReaderArray<processor_struct::GAMMASCINT> gamscint = {fReader, "gamma_scint_vec_"};
    TTreeReaderArray<processor_struct::CLOVERS> clover = {fReader, "clover_vec_"};
    TTreeReaderArray<processor_struct::VANDLES> vandle = {fReader, "vandle_vec_"};
    TTreeReaderArray<processor_struct::LOGIC> logic = {fReader, "logic_vec_"};
    TTreeReaderArray<processor_struct::DOUBLEBETA> beta = {fReader, "doublebeta_vec_"};

    //declares
    TObjArray *GammaArray, *VandleArray;
    TFile *outFile;
    Double_t DSPclockInSeconds = 8e-9;
    Double_t ADCclockInSeconds = 4e-9;
    Double_t msConvert = 1e-6;
    Double_t timeBinning_;
    Int_t totalCycleTimeBins;
    string outputFilePrefix;
    Double_t subBinning;
    Int_t maxProj_;
    map<Int_t, pair<Double_t, Double_t>> cloverCals;
    map<Int_t, vector<Double_t>> hagCals;

    Int_t cnt;
    Int_t fcnt;
    Int_t entTotal_;
    Bool_t isChain;

    Double_t BinShift;
    Double_t gbdtBin;

    TCutG *valid_CBdT;
    TCutG *nGinV;

    vector<pair<Double_t,Double_t>> Valid_CloverBetaTdiff; //<low energy L,H >,<mid energy L , H>, <high energy L,H> in {0,500},{501,1000},{1001, end}
    Double_t Valid_HagHRBetaTdiff, Valid_HagLRBetaTdiff;
    Double_t Valid_NaiBetaTdiff;
    Double_t Valid_VandleTdiff;

    Rb97Full(TTree * /*tree*/ = 0) {}
    virtual ~Rb97Full() {}
    virtual Int_t Version() const { return 2; }
    virtual void Begin(TTree *tree);
    virtual void SlaveBegin(TTree *tree);
    virtual void Init(TTree *tree);
    virtual Bool_t Notify();
    virtual Bool_t Process(Long64_t entry);
    virtual Int_t GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
    virtual void SetOption(const char *option) { fOption = option; }
    virtual void SetObject(TObject *obj) { fObject = obj; }
    virtual void SetInputList(TList *input) { fInput = input; }
    virtual TList *GetOutputList() const { return fOutput; }
    virtual void SlaveTerminate();
    virtual void Terminate();
    void AddToOutputList(TObjArray *iArray);

    ClassDef(Rb97Full, 0);
};

#endif
