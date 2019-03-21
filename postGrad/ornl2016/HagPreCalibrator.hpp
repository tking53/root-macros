#ifndef HAG_CALIBRATOR_HPP
#define HAG_CALIBRATOR_HPP

#include <sstream>
#include <utility>
#include <vector>

#include "/opt/root/6.14.06/include/TROOT.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TSpectrum.h"
#include "TCanvas.h"

Int_t numHag = 16;
pair<Int_t, Int_t> projFast = {0, 40}, projSlow = {400, 700};

vector<TH2*> inputs;
vector<pair<TH1*, TH1*>> projections;


#endif