#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include <ProcessorRootStruc.hpp>
#include <vector>
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TObjArray.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"

TObjArray* tracePlots;

using namespace std;
void TracePlotter(TFile* file, vector<string> chanTrace, Int_t traceLength = 500) {
    TTree* PTree = (TTree*)file->Get("PixTree");
    TTreeReader PixTreeEvt;

    PixTreeEvt.SetTree(PTree);
    TTreeReaderArray<processor_struct::ROOTDEV> rd = {PixTreeEvt, "root_dev_vec_"};

    tracePlots = new TObjArray;
    tracePlots->SetName("tracePlots");
    Int_t TraceXBins = traceLength;
    Int_t TraceYBins = 5000;
    for (unsigned it = 0; it < chanTrace.size(); it++) {
        string combo = chanTrace.at(it);
        string title = chanTrace.at(it) + " Traces";
        tracePlots->Add(new TH2D(combo.c_str(), title.c_str(), TraceXBins,0,TraceXBins,TraceYBins,0,TraceYBins));
    }

    while (PixTreeEvt.Next()) {
        for (auto itC = rd.begin(); itC != rd.end(); itC++) {
            vector<unsigned> trace = itC->trace;
            double energy = itC->energy;
            string dtype = itC->subtype.Data();
            string dgroup = itC->group.Data();
            string key = dtype + ":" + dgroup;
            for (unsigned int it = 0, itend = trace.size(); it < itend; it++) {
                ((TH2*)tracePlots->FindObject(key.c_str()))->Fill(it, trace.at(it));
            }
        }
    }

    gDirectory->Add(tracePlots);
}
