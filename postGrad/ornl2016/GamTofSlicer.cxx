#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TROOT.h"
using namespace std;

Int_t GamTofSlicer(string nSparse, pair<Double_t, Double_t> qdcCut, vector<pair<Double_t, Double_t>> tapeDTCut, vector<pair<Int_t, Int_t>> slice2Sub = {{0, 0}}, string inFs = "_file0") {
    if (inFs == "_file0") {
        inFs = gROOT->GetFile()->GetName();
    }
    TFile* inF = new TFile(inFs.c_str());

    THnSparse* Hnsp = (THnSparse*)inF->Get(nSparse.c_str());

    if (!Hnsp && Hnsp->GetNdimensions() != 4) {
        cout << "ERROR::nSparse lookup failed or the nSparse that was found has the wrong number of DIMS" << endl;
        return 1;
    }

    string GammaType = nSparse.substr(0, 1);
    string GamDet;
    if (GammaType == "g") {
        GamDet = "Clover";
    } else if (GammaType == "h") {
        GamDet = "LaBr";
    } else if (GammaType == "n") {
        GamDet = "NaI";
    } else {
        cout << "ERROR::Unknown Gamma Det Type" << endl;
        return 2;
    }

    /// setting the max values, -1 defined as last bin
    if (qdcCut.second == -1) {
        qdcCut.second = Hnsp->GetAxis(2)->GetXmax();
    }
    for (UInt_t it = 0; it < tapeDTCut.size(); it++) {
        if (tapeDTCut.at(it).second == -1) {
            tapeDTCut.at(it).second = Hnsp->GetAxis(3)->GetXmax();
        }
    }

    TObjArray* output;
    string TOA_name = "GamTofSlices_" + GammaType;
    if ((TObjArray*)gDirectory->Get(TOA_name.c_str())) {
        output = (TObjArray*)gDirectory->Get(TOA_name.c_str());
        output->Clear();
    } else {
        output = new TObjArray;
        output->SetName(TOA_name.c_str());
    }

    //using the tapeCuts as number of projections to make
    for (UInt_t it = 0; it < tapeDTCut.size(); it++) {
        //set qdc range
        Hnsp->GetAxis(2)->SetRangeUser(qdcCut.first, qdcCut.second);
        //set dt range
        Hnsp->GetAxis(3)->SetRangeUser(tapeDTCut.at(it).first, tapeDTCut.at(it).second);

        TH1::AddDirectory(kFALSE);
        TH2* HHtmp = Hnsp->Projection(1, 0);

        string Name = "GamTof_" + to_string(it);
        string Title = GamDet + " vs Vandle CorTof:: QDC = <" + to_string((Int_t)qdcCut.first) + "," + to_string((Int_t)qdcCut.second) + "> and TapeCut = <" + to_string((Int_t)tapeDTCut.at(it).first) + "," + to_string((Int_t)tapeDTCut.at(it).second) + ">";

        HHtmp->SetName(Name.c_str());
        HHtmp->SetTitle(Title.c_str());

        output->Add(HHtmp->Clone());
        HHtmp->Delete();
    }

    if (tapeDTCut.size() >= 2 && slice2Sub.front().first != slice2Sub.front().second) {
        //Subtract all the things
        for (UInt_t it = 0; it < slice2Sub.size(); it++) {
            string id0 = "GamTof_" + to_string(slice2Sub.at(it).first);
            string id1 = "GamTof_" + to_string(slice2Sub.at(it).second);
            auto tmp1 = (TH2F*)((TH2F*)output->FindObject(id0.c_str()))->Clone();
            auto tmp2 = (TH2F*)((TH2F*)output->FindObject(id1.c_str()))->Clone();

            Double_t tmp1Flas, tmp2Flas;

            // tmp1->Projection()

            tmp1->Add(tmp2, -1);

            string Name = "SubGamTof_" + to_string(it);
            string title = "2D Subtracted: " + GamDet + " vs VCorTof:: Slice {" + to_string(slice2Sub.at(it).first) + " - " + to_string(slice2Sub.at(it).second) + "}";
            tmp1->SetName(Name.c_str());
            tmp1->SetTitle(title.c_str());
            output->Add(tmp1->Clone());
            tmp1->Delete();
            tmp2->Delete();
        }
    }

    gDirectory->Add(output);
    return 0;
}