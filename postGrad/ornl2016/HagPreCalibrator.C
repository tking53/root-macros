#include "HagCalibrator.hpp"
using namespace std;

vector<pair<TH1*, TH1*>> HagProjector(TFile* F) {
    vector<pair<TH1*, TH1*>> ProjTemp;

    //pull the 2ds from the file and project them
    for (auto it = 0; it < numHag; it++) {
        string s = "h_re_dt_" + to_string(it);
        auto ht = (TH2*)F->Get(s.c_str());
        inputs.emplace_back(ht);

        TH1* fastProj = 0;
        TH1* slowProj = 0;
        stringstream SK, FK;

        FK << s.c_str() << "_FP";
        ht->ProjectionX(FK.str().c_str(), projFast.first, projFast.second);

        SK << s.c_str() << "_SP";
        ht->ProjectionX(SK.str().c_str(), projSlow.first, projSlow.second);

        ProjTemp.emplace_back((TH1*)gDirectory->Get(FK.str().c_str()), (TH1*)gDirectory->Get(SK.str().c_str()));
    }

    return ProjTemp;
}

pair<Int_t, Double_t*> PeakFinder(TH1* hist, Int_t nPeaks = 5, Int_t RL = 10, Int_t RH = 32000) {
    auto TS = new TSpectrum(nPeaks);
    hist->GetXaxis()->SetRangeUser(RL, RH);
    Int_t nFound = TS->Search(hist, 20, "", 0.01);
    printf("Found %d candidate peaks to fit\n", nFound);
    Double_t* peaks = TS->GetPositionX();
    return make_pair(nFound, peaks);
}

void HagCalibrator(TFile* F, Int_t nPeaks = 5) {
    cout << "Projecting the 2d historgrams" << endl;
    projections = HagProjector(F);
    auto c1 = new TCanvas("c1", "c1", 600, 800);
    c1->Divide(4, 4);
    for (auto HAG = 0; HAG < numHag; HAG++) {
        printf("\nSearching in HAGRiD #%d\n", HAG);
        pair<Int_t, Double_t*> Peaks;
        Int_t pad = HAG + 1;
        c1->cd(pad);
        gPad->SetLogy();
        Peaks = PeakFinder(projections.at(HAG).second, nPeaks);

        for (auto it = 0; it < Peaks.first; it++) {
            printf("PeakX= %d\n", (Int_t)Peaks.second[it]);
        }
    }
}