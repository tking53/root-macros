#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>

class GSAddback {
public:
  /** Default constructor setting things to zero */
  GSAddback() {
    energy = time = multiplicity = 0;
  }

  /** Default constructor setting default values
   * \param [in] ienergy : the initial energy
   * \param [in] itime : the initial time
   * \param [in] imultiplicity : multiplicity of the event */
  GSAddback(Double_t ienergy, Double_t itime, unsigned imultiplicity) {
    energy = ienergy;
    time = itime;
    multiplicity = imultiplicity;
  }

  Double_t energy;//!< Energy of the addback event
  Double_t time;//!< time of the addback event
  unsigned multiplicity;//!< multiplicity of the event
};

class addbackCalculator {
public:
  addbackCalculator(const char* inputname, const char* savename,const Bool_t batchMode=false);

  ~addbackCalculator();

  inline std::string CheckFileName(const std::string& FileName,const int position) //<! position 0 is filename position 1 is the file extension
  {
    if(FileName.find_last_of(".") != std::string::npos)
      return FileName.substr(FileName.find_last_of("."));
    return "";
  }

private:
  TFile *fin;  //!< Pointer to the input file
  TFile *fout; //!< Pointer to the output file
  TTree *Gsing; //!< Pointer to the input singles tree
  TTree *Gsrab;  //!< Pointer to the AddBack tree that we will make

  const Double_t shsubwin = 0;
  const Double_t shthresh = 0;

  const Double_t naithresh = 0;
  const Double_t naisubwin = 0;

  

  Double_t GS_ABEnergy = 0; //!<Energy of the addback event
  Double_t GS_ABEvtNum = 0; //!< Pixie Event number of the addback event
  Int_t GS_ABMulti = 0; //!< Number of Detectors used in the Addback event
  std::string GS_ABType = "";//!< Detector subtype of the addback event
  Double_t GS_ABbunchNum = 0; //!<Bunch number of addback event
  Bool_t GS_ABhasLRBeta =false ; //!< Low Res Beta flag for addback
};
