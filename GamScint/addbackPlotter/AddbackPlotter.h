#ifndef AddbackPlotter_h
#define AddbackPlotter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TStreamerElement.h>
#include <TProofOutputFile.h>

// Headers needed by this particular selector
#include <TParameter.h>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <TProofServ.h>

#define WHAT_COMPUTER 0

#if (WHAT_COMPUTER == 0)
#define PPATH "/home/hanayo/programs/root-macros/GamScint/addbackCalculator"
#define OUTPATH "/home/hanayo/research/ornl2016/thesis/rootFiles"
#include "/home/hanayo/programs/root-macros/GamScint/GSaddback.hpp"
#include "/home/hanayo/programs/root-macros/GamScint/GammaScintStruc.hpp"
#endif

#if (WHAT_COMPUTER == 1)
#define PPATH "/home/setsuna/programs/root-macros/GamScint/addbackCalculator"
#define OUTPATH "/home/setsuna/programs/paass/testingRuns/"
#include "/home/setsuna/programs/root-macros/GamScint/GSaddback.hpp"
#endif

class AddbackPlotter : public TSelector {
public :
  TTreeReader     singReader;  //!the tree reader
  TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

  TTree *GabR; //output tree
  TFile *outFile; //output file
  TProofOutputFile *fProofFile; //intermediate proof outputfile

  // Readers to access the data (delete the ones you do not need).

  // TTreeReaderArray<Bool_t> PEsing_HasLowResBeta = {singReader, "PEsing.HasLowResBeta"};
  // //  TTreeReaderArray<Bool_t> PEsing_HasMedResBeta = {singReader, "PEsing.HasMedResBeta"};
  // TTreeReaderArray<Double_t> PEsing_Energy = {singReader, "PEsing.Energy"};
  // //TTreeReaderArray<std::string> PEsing_Type = {singReader, "PEsing.Type"};
  // TTreeReaderArray<Int_t> PEsing_DetNum = {singReader, "PEsing.DetNum"};
  // TTreeReaderArray<Double_t> PEsing_Time = {singReader, "PEsing.Time"};
  // TTreeReaderArray<Double_t> PEsing_BetaGammaTDiff = {singReader, "PEsing.BetaGammaTDiff"};
  // TTreeReaderArray<Double_t> PEsing_BetaEnergy = {singReader, "PEsing.BetaEnergy"};
  // TTreeReaderArray<Double_t> PEsing_BetaMulti = {singReader, "PEsing.BetaMulti"};
  // TTreeReaderArray<Double_t> PEsing_EvtNum = {singReader, "PEsing.EvtNum"};
  // TTreeReaderArray<Double_t> PEsing_BunchNum = {singReader, "PEsing.BunchNum"};
  // TTreeReaderArray<Double_t> PEsing_LastBunchTime = {singReader, "PEsing.LastBunchTime"};
  // // TTreeReaderArray<std::string> PEsing_Group = {singReader, "PEsing.Group"};

  // //TTreeReaderArray<SINGLES> singVec = {singReader,"PEsing"};
  // TTreeReaderArray<Double_t> singEvt = {singReader,"PEsing.EvtNum"};

  TTreeReaderArray<SINGLES> singVec = {singReader,"PEsing"};

  Double_t EvtLast = 0; //evt last counter
  std::set<std::string> groupList, typeList;
  std::set<Double_t> PEvt;

  Double_t ABEnergy, ABEvtNum, ABMulti,ABbunchNum,ABevtTotal;
  std::string ABType,ABGroup;


  Double_t PevtNum,PbunchNum,PbetaEnergy;
  Bool_t ABhasLRBeta;

  TObjArray *abHists;

  std::map<std::string,std::map<std::string,double> > ParameterMap;
  //see begin() method in AddbackPlotter.C for the defaults 
  Double_t NgammaThresh; //threshold (in keV) for addback (10 default from clover processor)
  Double_t SHgammaThresh; //threshold (in keV) for addback (10 default from clover processor)
  Double_t NSeW; //sub event window in sec (1e-6 default from clover processor) 
  Double_t SHSeW ;  //sub event window in sec (1e-6 default from clover processor)
  Double_t GegammaThresh; //threshold (in keV) for addback (10 default from clover processor)
  Double_t GeSeW; //sub event window in sec (1e-6 default from clover processor) 
  
  AddbackPlotter(TTree * /*tree*/ =0) { }
  virtual ~AddbackPlotter() { }
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

  ClassDef(AddbackPlotter,0);

};

#endif

#ifdef AddbackPlotter_cxx
void AddbackPlotter::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the reader is initialized.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  singReader.SetTree(tree);
}

Bool_t AddbackPlotter::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}


#endif // #ifdef AddbackPlotter_cxx
