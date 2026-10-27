#ifndef ROOT_HIST_MANIPULATION_H
#define ROOT_HIST_MANIPULATION_H

#include "RtypesCore.h"
#include "TString.h"
#include "TFile.h"

#include <iostream>


class TH1;
class TH2;
class TFile;
class TCanvas;

// Axis ranges
void xc(Double_t x1, Double_t x2, Bool_t update = true);
void yc(Double_t x1, Double_t x2, Bool_t update = true);

void dl(Double_t x1, Double_t x2, Bool_t update = true);
void dl(TH1* hist, Double_t x1, Double_t x2);

void xch(TH2* hist, Double_t x1, Double_t x2);
void ych(TH2* hist, Double_t x1, Double_t x2);

// Log axes
void lx(Int_t logx = 1, Bool_t update = true);
void ly(Int_t logy = 1, Bool_t update = true);
void lz(Int_t logz = 1, Bool_t update = true);

// Pad / canvas utilities
void cu();
void cuall();

void centertitle(Bool_t update = true);
void centertitleall(Bool_t update = true);

// Canvas creation
TCanvas* nc(TString name);

// Histogram appearance
void lincol(TH1* hist, Color_t color);
void lincol(Color_t color, Bool_t update = true);

// Histogram retrieval
template <typename HistT>
HistT* gethist(TFile* file, const char* histName) {
	// Retrieve a histogram with an explicit type from the given ROOT file.
	if (!file) {
		std::cout << "\033[1;31m[Error]\033[0m File pointer is null. Cannot retrieve histogram." << std::endl;
		return nullptr;
	}
	if (!file->Get(histName)) {
		std::cout << "\033[1;31m[Error]\033[0m Histogram '" << histName << "' not found in file '" << file->GetName() << "'." << std::endl;
		return nullptr;
	}
	return dynamic_cast<HistT*>(file->Get(histName));
}
#endif
