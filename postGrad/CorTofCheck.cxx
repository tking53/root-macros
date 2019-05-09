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

#include "ornl2016/VandleTCuts.h"

using namespace std;
void CorTofCheck(TFile* file, Double_t Qcut = 0, Double_t Tcut = 0, Double_t TDiffCut = 15) {
    TTree* PTree = (TTree*)file->Get("PixTree");
    TTreeReader PixTreeEvt;

    TCutG* nGinV = new TCutG("VTQ_fullQ", 12);
    for (Int_t it = 0; it < VandleTCuts::VTQ_fullQ_vec.size(); it++) {
        nGinV->SetPoint(it, VandleTCuts::VTQ_fullQ_vec.at(it).first, VandleTCuts::VTQ_fullQ_vec.at(it).second);
    }

    PixTreeEvt.SetTree(PTree);
    TTreeReaderArray<processor_struct::VANDLES> vandle = {PixTreeEvt, "vandle_vec_"};

    Int_t entTotal_ = PixTreeEvt.GetEntries(kTRUE);
    Int_t cnt = 0;

    TH1D* hh = new TH1D("corTofCheck", "tdiff between tof and corTof", 2000, -500, 500);

    TH1D* hh2 = new TH1D("corTofCheck2", "tdiff between tof and corTof in bannana", 2000, -500, 500);

    TH2D* hh3 = new TH2D("ctq", "corTof vs QDC if abs(ct - t ) <60", 4000, 0, 2000, 10000, 0, 10000);

    cout << endl
         << "Using Tof<=" << Tcut << "    Qdc>=" << Qcut << "    abs(TDIFF)<=" << TDiffCut << endl
         << endl;

    while (PixTreeEvt.Next()) {
        for (auto itC = vandle.begin(); itC != vandle.end(); itC++) {
            Double_t corTof = itC->corTof;
            Double_t tof = itC->tof;
            Double_t qdc = itC->qdc;
            Double_t tdiff = itC->tdiff;

            if (corTof == -999 || tof == -999 || qdc == -999 || tdiff == -999) {
                continue;
            }

            if (abs(tdiff) <= TDiffCut && corTof - tof <= tof*0.2 ) {
                hh3->Fill(tof, qdc);
            }

            if (abs(tdiff) <= TDiffCut && qdc >= Qcut && tof <= Tcut && tof >= 0) {
                hh->Fill(corTof - tof);
            }
            if (nGinV->IsInside(tof, qdc) && abs(tdiff) <= TDiffCut && qdc >= Qcut && tof <= Tcut ) {
                hh2->Fill(corTof - tof);
            }
        }

        //Run Stats
        cnt++;
        if (cnt % 1000000 == 0) {
            int per = ((Double_t)cnt / (Double_t)entTotal_) * 100;
            cout << "Processed entry " << cnt << " of " << entTotal_ << " (" << per << "%)" << endl;
        }

    }  //end while
    hh->Draw();
    hh2->SetLineColor(kRed);
    hh2->Draw("same");
    // TCanvas* c2 = new TCanvas("c2", "c2");
    // c2->cd();
    // hh3->SetMinimum(1);
    // c2->SetLogz();
    // hh3->Draw("colz");
}
