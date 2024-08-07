#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include <PaassRootStruct.hpp>
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

using namespace std;
void HighResTimeCheck(TFile* file, string Wtype) {
    TTree* PTree = (TTree*)file->Get("PixTree");
    TTreeReader PixTreeEvt;

    PixTreeEvt.SetTree(PTree);
    TTreeReaderArray<processor_struct::ROOTDEV> rd = {PixTreeEvt, "root_dev_vec_"};

    Double_t adcTickNs = 4;
    Double_t dspTickNs = 8;

    TH1D* hh = new TH1D("hrt_check", "tdiff between HRTns and computed HRT", 2000, -1000, 1000);

    while (PixTreeEvt.Next()) {
        for (auto itC = rd.begin(); itC != rd.end(); itC++) {
            string dtype = itC->subtype.Data();
            if (Wtype == dtype) {
                Double_t HRTns = itC->highResTime;
                Double_t myHRT = 0;

                Double_t EVT = itC->timeSansCfd /(8e-9);
                Double_t MaxPos = itC->maxPos;
                Double_t Phase = itC->phase;
                Double_t corPhase = (Phase) * adcTickNs;

                myHRT = (EVT * dspTickNs) + corPhase;

                cout<<std::setprecision(10)<<"EVT="<<EVT<<"   MaxP="<<MaxPos<<"   phase="<<Phase <<"    Phase-Max="<<Phase-MaxPos<<"      HRTns="<<HRTns<<"      tdiff=" << myHRT - HRTns << endl;
                // hh->Fill(myHRT);
            }
        }
    }

    // hh->Draw();
    
}
