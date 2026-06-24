#include "TList.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"
#include "TColor.h"
#include "TFile.h"
#include "TROOT.h"
#include "TPaveText.h"
#include "TVirtualPad.h"

#include "iostream"


void xc(Double_t x1, Double_t x2, Bool_t update=true){
	// x axis zoom in on the current pad. If update is true, the pad will be modified and updated after setting the range.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to set x-axis range." << std::endl;
		return;
	}
	TH1* hist = dynamic_cast<TH1*>(gPad->GetPrimitive("htemp"));
	if (!hist) {
		TList* primitives = gPad->GetListOfPrimitives();
		TObject* obj = primitives ? primitives->First() : nullptr;
		while (obj) {
			hist = dynamic_cast<TH1*>(obj);
			if (hist) {
				break;
			}
			obj = primitives->After(obj);
		}
	}
	if (!hist) {
		return;
	}
	hist->GetXaxis()->SetRangeUser(x1, x2);
	if (update) {
		gPad->Modified();
		gPad->Update();
	}
	
}

void yc(Double_t x1, Double_t x2, Bool_t update=true){
	// y axis zoom in on the current pad. If update is true, the pad will be modified and updated after setting the range.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to set y-axis range." << std::endl;
		return;
	}
	TH2* hist = dynamic_cast<TH2*>(gPad->GetPrimitive("htemp"));
	if (!hist) {
		TList* primitives = gPad->GetListOfPrimitives();
		TObject* obj = primitives ? primitives->First() : nullptr;
		while (obj) {
			hist = dynamic_cast<TH2*>(obj);
			if (hist) {
				break;
			}
			obj = primitives->After(obj);
		}
	}
	if (!hist) {
		return;
	}
	hist->GetYaxis()->SetRangeUser(x1, x2);
	if (update) {
		gPad->Modified();
		gPad->Update();
	}
}

void dl(Double_t x1, Double_t x2, Bool_t update=true){
	// y axis zoom in on the current pad. If update is true, the pad will be modified and updated after setting the range.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to set y-axis range." << std::endl;
		return;
	}
	TH1* hist = dynamic_cast<TH1*>(gPad->GetPrimitive("htemp"));
	if (!hist) {
		TList* primitives = gPad->GetListOfPrimitives();
		TObject* obj = primitives ? primitives->First() : nullptr;
		while (obj) {
			hist = dynamic_cast<TH1*>(obj);
			if (hist) {
				break;
			}
			obj = primitives->After(obj);
		}
	}
	if (!hist) {
		return;
	}
	hist->GetXaxis()->SetRangeUser(x1, x2);
	if (update) {
		gPad->Modified();
		gPad->Update();
	}
}

void dl(TH1* hist, Double_t x1, Double_t x2){
	// x axis zoom in on the given histogram. If update is true, the pad will be modified and updated after setting the range.
	
	if (!hist) {
		std::cout << "\033[1;31m[Error]\033[0m Histogram pointer is null. Cannot set x-axis range." << std::endl;
		return;
	}
	hist->GetXaxis()->SetRangeUser(x1, x2);
}

void xch(TH2* hist, Double_t x1, Double_t x2){
	// x axis zoom in on the given histogram. If update is true, the pad will be modified and updated after setting the range.
	
	if (!hist) {
		std::cout << "\033[1;31m[Error]\033[0m Histogram pointer is null. Cannot set x-axis range." << std::endl;
		return;
	}
	hist->GetXaxis()->SetRangeUser(x1, x2);
}

void ych(TH2* hist, Double_t x1, Double_t x2){
	// y axis zoom in on the given histogram. If update is true, the pad will be modified and updated after setting the range.
	
	if (!hist) {
		std::cout << "\033[1;31m[Error]\033[0m Histogram pointer is null. Cannot set y-axis range." << std::endl;
		return;
	}
	hist->GetYaxis()->SetRangeUser(x1, x2);
}


void lx(Int_t logx=1, Bool_t update=true){
	// Set the log scale for the x-axis of the current pad. If update is true, the pad will be modified and updated after setting the log scale.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to set log scale." << std::endl;
		return;
	}
	gPad->SetLogx(logx);
	if (update) {
		gPad->Modified();
		gPad->Update();
	}
}
void ly(Int_t logy=1, Bool_t update=true){
	// Set the log scale for the y-axis of the current pad. If update is true, the pad will be modified and updated after setting the log scale.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to set log scale." << std::endl;
		return;
	}
	gPad->SetLogy(logy);
	if (update) {
		gPad->Modified();
		gPad->Update();
	}
}
void lz(Int_t logz=1, Bool_t update=true){
	// Set the log scale for the z-axis of the current pad. If update is true, the pad will be modified and updated after setting the log scale.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to set log scale." << std::endl;
		return;
	}
	gPad->SetLogz(logz);
	if (update) {
		gPad->Modified();
		gPad->Update();
	}
}

namespace {
void markPadTreeModified(TPad* pad) {
	if (!pad) {
		return;
	}
	pad->Modified();

	TList* primitives = pad->GetListOfPrimitives();
	TObject* obj = primitives ? primitives->First() : nullptr;
	while (obj) {
		TPad* subPad = dynamic_cast<TPad*>(obj);
		if (subPad && subPad != pad) {
			markPadTreeModified(subPad);
		}
		obj = primitives->After(obj);
	}
}

Bool_t centerTitleOnPad(TVirtualPad* pad, Bool_t updatePad) {
	if (!pad) {
		return kFALSE;
	}

	// Ensure ROOT has created the title object after recent SetTitle/Draw calls.
	pad->Modified();
	pad->Update();

	TPaveText* title = dynamic_cast<TPaveText*>(pad->GetPrimitive("title"));
	if (!title) {
		return kFALSE;
	}

	const Double_t width = title->GetX2NDC() - title->GetX1NDC();
	Double_t newX1 = 0.5 - (0.5 * width);
	Double_t newX2 = 0.5 + (0.5 * width);

	if (newX1 < 0.0) {
		newX1 = 0.0;
		newX2 = width;
	}
	if (newX2 > 1.0) {
		newX2 = 1.0;
		newX1 = 1.0 - width;
	}

	title->SetX1NDC(newX1);
	title->SetX2NDC(newX2);

	if (updatePad) {
		pad->Modified();
		pad->Update();
	}

	return kTRUE;
}

void centerTitlesInPadTree(TPad* pad, Int_t& centeredCount) {
	if (!pad) {
		return;
	}

	if (centerTitleOnPad(pad, kFALSE)) {
		++centeredCount;
	}

	TList* primitives = pad->GetListOfPrimitives();
	TObject* obj = primitives ? primitives->First() : nullptr;
	while (obj) {
		TPad* subPad = dynamic_cast<TPad*>(obj);
		if (subPad && subPad != pad) {
			centerTitlesInPadTree(subPad, centeredCount);
		}
		obj = primitives->After(obj);
	}
}
}

void cu(){
	// Update the current pad and all its subpads.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to update." << std::endl;
		return;
	}
	gPad->Modified();
	gPad->Update();
}

void cuall(){
	// Update all pads in the current canvas so non-active subpads refresh too.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to update." << std::endl;
		return;
	}

	TVirtualPad* activePad = gPad;
	TCanvas* canvas = activePad->GetCanvas();
	if (!canvas) {
		activePad->Modified();
		activePad->Update();
		return;
	}

	markPadTreeModified(canvas);
	canvas->Modified();
	canvas->Update();

	if (activePad) {
		activePad->cd();
	}
}

void centertitle(Bool_t update=true){
	// Center the histogram title box on the current active pad.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to center title on." << std::endl;
		return;
	}

	if (!centerTitleOnPad(gPad, update)) {
		std::cout << "\033[1;31m[Error]\033[0m No title object found on current pad." << std::endl;
	}

	if (update) {
		gPad->Modified();
		gPad->Update();
	}
}

void centertitleall(Bool_t update=true){
	// Center title boxes on all pads in the current canvas.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to center titles on." << std::endl;
		return;
	}

	TVirtualPad* activePad = gPad;
	TCanvas* canvas = activePad->GetCanvas();
	if (!canvas) {
		std::cout << "\033[1;31m[Error]\033[0m No canvas found for current pad." << std::endl;
		return;
	}

	Int_t centeredCount = 0;
	centerTitlesInPadTree(canvas, centeredCount);

	if (update) {
		markPadTreeModified(canvas);
		canvas->Modified();
		canvas->Update();
	}

	if (activePad) {
		activePad->cd();
	}

	if (centeredCount == 0) {
		std::cout << "\033[1;31m[Error]\033[0m No title objects found on canvas pads." << std::endl;
	}
}

TCanvas* nc(TString name) {
	TCanvas* tmp = new TCanvas(name,name);
	return tmp;
}

void lincol(TH1* hist, Color_t color){
	// Set the line color of the given histogram.
	if (!hist) {
		std::cout << "\033[1;31m[Error]\033[0m Histogram pointer is null. Cannot set line color." << std::endl;
		return;
	}
	hist->SetLineColor(color);
}

void lincol(Color_t color, Bool_t update=true){
	// Set the line color of the histogram in the current pad.
	if (!gPad) {
		std::cout << "\033[1;31m[Error]\033[0m No current pad to set x-axis range." << std::endl;
		return;
	}
	TH1* hist = dynamic_cast<TH1*>(gPad->GetPrimitive("htemp"));
	if (!hist) {
		TList* primitives = gPad->GetListOfPrimitives();
		TObject* obj = primitives ? primitives->First() : nullptr;
		while (obj) {
			hist = dynamic_cast<TH1*>(obj);
			if (hist) {
				break;
			}
			obj = primitives->After(obj);
		}
	}
	if (!hist) {
		return;
	}
	hist->SetLineColor(color);
	if (update) {
		gPad->Modified();
		gPad->Update();
	}
	
}

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