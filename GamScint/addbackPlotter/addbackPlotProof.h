#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TH1.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TStopwatch.h"
#include "TSelector.h"
#include "TProofServ.h"
#include "TMath.h"


#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>

class addbackSelector : public TSelector {
public:
  addbackSelector(TTree *=0);
  virtual ~addbackSelector();

  virtual Int_t   Version() const { return 0.5; }
  virtual void    Init(TTree* mergedData);
  virtual void    Begin(TTree *mergedData);
  virtual void    SubBegin(TTree *mergedData);
  virtual Bool_t  Notify() { return kTRUE;}
  virtual Bool_t  Process(Long64_t entry);
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SubTerminate(){}
  virtual void    Terminate();

protected:
  const Double_t naisubwin = 0;
  const Double_t shsubwin = 0;
  const Double_t naithresh = 0;
  const Double_t shthresh = 0;

  TTreeReader gsReader;

  //his array
  TObjArray* fHistos;

  TFile* fout;
  TFile* fin;

}
