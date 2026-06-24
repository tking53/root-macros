#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TMath.h"
#include "TROOT.h"


TH1F* BKGsub(TFile* DataFile, TFile* BKGFile, Double_t Scale, Bool_t draw=true){
	
	TH1F* Data = (TH1F*)DataFile->Get("EBSS_2");
	TH1F* BKG = (TH1F*)BKGFile->Get("EBSS_2");
	
	
	if (!Data || !BKG) {
		return nullptr;
	}

	if (gROOT->FindObject("bkgsub")) {
		delete gROOT->FindObject("bkgsub");
	}
	TH1F* Subtracted = (TH1F*)Data->Clone("bkgsub");
	Subtracted->Add(BKG, Scale);
	Subtracted->SetLineColor(kRed);
	if (draw) {
		Subtracted->Draw();
	}
	return Subtracted;
	
}

Double_t gPeakInt(TF1 *fit){
	return (fit->GetParameter(0) * fit->GetParameter(2) * TMath::Sqrt(2*TMath::Pi()));	
}

