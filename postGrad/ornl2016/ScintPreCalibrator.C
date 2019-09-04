#include <sstream>
#include <utility>
#include <vector>

#include "TROOT.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TSpectrum.h"
pair<Int_t, Int_t> projSlow = {350, 800}, projFast = {0, 300}, numNai = {4, 9}, numHag = {0, 15}, numScint, numGe={0,3};

vector<TH2*> inputs;
vector<vector<TH1*>> projections;

using namespace std;

vector<vector<TH1*>> ScintProjector(TFile* F, string prefix = "h", Bool_t raw = true) {
    vector<vector<TH1*>> ProjTemp;

    if (prefix == "n") {
        numScint = numNai;
    } else if (prefix == "h") {
        numScint = numHag;
    } else if (prefix == "g") {
        numScint = numGe;
    } else {
        cout << "Unknown prefix" << endl;
        return ProjTemp;
    }
    string EType;
    if (raw) {
        EType = "re";
    } else {
        EType = "e";
    }
    //pull the 2ds from the file and project them
    for (auto it = numScint.first; it <= numScint.second; it++) {
        cout << "Projecting histogram triplet #" << it << endl;
        string s = prefix + "_" + EType + "_dt_" + to_string(it);
        auto ht = (TH2*)F->Get(s.c_str());
        vector<TH1*> innerTemp;
        stringstream SK, FK, TK;

        TK << "TP_" << s.c_str();
        ht->ProjectionX(TK.str().c_str(), 0, -1);

        FK << "FP_" << s.c_str();
        ht->ProjectionX(FK.str().c_str(), projFast.first, projFast.second);

        SK << "SP_" << s.c_str();
        ht->ProjectionX(SK.str().c_str(), projSlow.first, projSlow.second);

        auto TH = (TH1*)gDirectory->Get(TK.str().c_str());
        auto FH = (TH1*)gDirectory->Get(FK.str().c_str());
        auto SH = (TH1*)gDirectory->Get(SK.str().c_str());

        string TH_Title = (string)ht->GetTitle() + (string) " full projection;Energy(Channels)";
        string FH_Title = (string)ht->GetTitle() + (string) " early projection;Energy(Channels)";
        string SH_Title = (string)ht->GetTitle() + (string) " late projection;Energy(Channels)";

        TH->SetTitle(TH_Title.c_str());
        FH->SetTitle(FH_Title.c_str());
        SH->SetTitle(SH_Title.c_str());

        innerTemp.emplace_back(TH);
        innerTemp.emplace_back(FH);
        innerTemp.emplace_back(SH);
        ProjTemp.emplace_back(innerTemp);
    }

    return ProjTemp;
}

Int_t ScintPreCalibrator(TFile* F, string prefix = "h", Bool_t sub = false, Bool_t raw = true, string outFile = "ScintPreCalibrator_") {
    string EType;
    if (raw) {
        EType = "RAW_";
        cout << "Using Raw Energy Histograms" << endl;
    } else {
        EType = "CAL_";
        cout << "Using Cal Energy Histograms" << endl;
    }
    cout << "Projecting the 2d historgrams and saving to file" << endl;
    projections = ScintProjector(F, prefix, raw);

    if (projections.size() == 0) {
        cout << "Something broke in the projector() method" << endl;
        return 1;
    }

    stringstream ouf;
    string originalFileName = F->GetName();
    string originalDate = originalFileName.substr(originalFileName.find("_", 12), 16);
    ouf << EType << outFile << "for_file_" << originalDate << ".root";
    auto f1 = new TFile(ouf.str().c_str(), "RECREATE");
    string projSlowStr = "Slow Projection Range = {" + to_string(projSlow.first) + "," + to_string(projSlow.second) + "}";
    string projFastStr = "Fast Projection Range = {" + to_string(projFast.first) + "," + to_string(projFast.second) + "}";
    TNamed SlowTN("SlowRange", projSlowStr);
    TNamed FastTN("FastRange", projFastStr);

    for (auto it = 0; it < (Int_t)projections.size(); it++) {
        for (int IT = 0; IT < 3; IT++) {
            projections.at(it).at(IT)->Write();
        }
        if (sub) {
            cout << "Subtracting the 2 projections for triplet #" << to_string(it) << endl;
            string subKey = "sub_" + to_string(it);
            string subName = EType + "Subtracted:" + to_string(it) + " Type Set by key";
            Int_t nBin = projections.at(it).at(1)->GetXaxis()->GetNbins();
            Int_t xmin = projections.at(it).at(1)->GetXaxis()->GetXmax();
            Int_t xmax = projections.at(it).at(1)->GetXaxis()->GetXmax();
            auto subtracted = new TH1D(subKey.c_str(), subName.c_str(), nBin, xmin, xmax);

            subtracted->Add(projections.at(it).at(1));
            cout << "1" << endl;
            subtracted->Add(projections.at(it).at(2), -1);
        }
    }
    SlowTN.Write();
    FastTN.Write();
    f1->Write();
    f1->Close();
    return 0;
}