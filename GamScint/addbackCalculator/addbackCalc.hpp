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
  addbackCalculator(const char* inputname, const char* savename, const bool totalsOnly = false,const bool batchMode=false)

    ~addbackCalculator();

private:
  TFile *fin;
  TFile *fout;
  TTree
  const Double_t naisubwin = 0;
  const Double_t shsubwin = 0;
  const Double_t naithresh = 0;
  const Double_t shthresh = 0;
}
