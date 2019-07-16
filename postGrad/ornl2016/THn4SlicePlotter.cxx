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

TH2* Slicer4D(vector<Int_t> axisToZoom, pair<Int_t, Int_t> axisToProj, vector<pair<Int_t, Int_t>> range, THnSparse* hist2slice, Int_t sliceNum, Bool_t verbose = false) {
    if (verbose) {
        cout << "Slicing # " << sliceNum << endl;
    }
    string rootTitle = hist2slice->GetTitle();
    string rootName = hist2slice->GetName();
    for (Int_t it = 0; it < (Int_t)axisToZoom.size(); it++) {
        hist2slice->GetAxis(axisToZoom.at(it))->SetRangeUser(range.at(it).first, range.at(it).second);
    }
    hist2slice->Projection(axisToProj.second, axisToProj.first);
    string projectName = rootName + "_proj_" + to_string(axisToProj.first) + "_" + to_string(axisToProj.second);
    TH2* projection = ((TH2*)gDirectory->Get(projectName.c_str()));
    TH2F* SliceTemp = (TH2F*)projection->Clone();
    projection->Delete();
    string newTitle = rootTitle + ": ";
    for (Int_t it = 0; it < (Int_t)axisToZoom.size(); it++) {
        newTitle += " A" + to_string(axisToZoom.at(it)) + " {" + to_string(range.at(it).first) + "," + to_string(range.at(it).second);
        if (axisToZoom.at(it) == axisToZoom.back()) {
            newTitle += "}";
        } else {
            newTitle += "},";
        }
    }
    string newName = rootName + "_slice_" + to_string(sliceNum);
    SliceTemp->SetTitle(newTitle.c_str());
    SliceTemp->SetName(newName.c_str());

    return SliceTemp;
}

TH1* Slice4Subractor(TObjArray* h2s, pair<Int_t, Int_t> ids2Sub, pair<Int_t, Int_t> subRange, pair<Int_t, Int_t> normRange, Int_t sliceNum = 0) {
    TH2* h1 = (TH2*)h2s->At(ids2Sub.first);
    TH2* h2 = (TH2*)h2s->At(ids2Sub.second);

    // cout << "subracting: " << h2->GetTitle() << " from " << h1->GetTitle() << endl;

    h1->ProjectionX("h1Proj", subRange.first, subRange.second);
    h2->ProjectionX("h2Proj", subRange.first, subRange.second);

    TH1D* h1P = (TH1D*)gDirectory->Get("h1Proj");
    TH1D* h2P = (TH1D*)gDirectory->Get("h2Proj");

    Double_t h1PInt = h1P->Integral(normRange.first, normRange.second);  // integrate late tof (600ns @1m~ 14keV )
    Double_t h2PInt = h2P->Integral(normRange.first, normRange.second);  // integrate late tof (600ns @1m~ 14keV )

    Double_t NormConstant = -1 * h1PInt / h2PInt;

    TH1D* subd = (TH1D*)h1P->Clone();
    subd->Add(h2P, NormConstant);
    string name, title;
    if (subRange.second == -1) {
        title = "Projection: A {" + to_string(subRange.first) + ", last} sliceNum=" + to_string(sliceNum);
        name = "VS_" + to_string(subRange.first) + "_L_" + to_string(sliceNum);
    } else {
        title = "Projection: A {" + to_string(subRange.first) + "," + to_string(subRange.second) + "} sliceNum=" + to_string(sliceNum);
        name = "THnSub_" + to_string(subRange.first) + "_" + to_string(subRange.second) + "_" + to_string(sliceNum);
    }
    subd->SetNameTitle(name.c_str(), title.c_str());
    //delete tmp stuff
    h1P->Delete();
    h2P->Delete();

    return subd;
}

Int_t THn4SlicePlotter(const vector<vector<pair<Int_t, Int_t>>> ListOfSlices, string nSparse, vector<Int_t> axis2zoom, pair<Int_t, Int_t> axis2proj, Bool_t subtract = false, pair<Int_t, Int_t> subRange = {120, -1}, pair<Int_t, Int_t> normRange = {600, 800}, Bool_t subtractDisplay = false, string OutFilePre = "date") {
    TObjArray* Sliced2ds = new TObjArray;
    TObjArray* Subtracted = new TObjArray;
    if (subtract && ListOfSlices.size() < 2) {
        cout << "Need more than 1 slice to subtract" << endl;
        return 2;
    }
    Sliced2ds->SetName("THn4_Slices");
    Subtracted->SetName("THn4_subd");
    if (!ListOfSlices.empty() && nSparse != "") {
        THnSparse* Hns = ((THnSparse*)gFile->Get(nSparse.c_str()));
        if (Hns == 0) {
            cout << "ERROR: Failed to find specified THnSparse. Check spelling" << endl;
            return 3;
        }
        for (unsigned it = 0; it < ListOfSlices.size(); it++) {
            TH2* histTemp = Slicer4D(axis2zoom, axis2proj, ListOfSlices.at(it), Hns, it, true);

            Sliced2ds->Add(histTemp);
        }
        if (subtract) {
            gDirectory->Add(Sliced2ds);
            cout << "Subtracting" << endl;
            for (Int_t sub = 1; sub <= (Int_t)(ListOfSlices.size() - 1); sub++) {
                Subtracted->Add(Slice4Subractor(Sliced2ds, {0, sub}, subRange, normRange, sub));  //600,800 for tof
            }

            gDirectory->Add(Subtracted);
            // TH1* sub1 = SliceSubractor(Sliced2ds, {0, 1}, {120, -1}, 0);
            // TH1* sub2 = SliceSubractor(Sliced2ds, {0, 2}, {120, -1}, 1);

            // c1->Divide(1,2);
            // c1->cd(1);
            // // sub1->Draw()
            // if (subtractDisplay) {
            //     TCanvas* c1 = new TCanvas("C1", "C1");
            //     sub2->SetLineColor(kRed);

            //     sub1->Draw();
            //     sub2->Draw("SAME");
            //     // gDirectory->Add(sub1);
            // }
        } else {
            gDirectory->Add(Sliced2ds);
        }

        // stringstream OutFileName;
        // if (strcmp(OutFilePre.c_str(), "date") == 0) {
        //     TDatime now;
        //     OutFileName << "SlicedOutput_" << now.GetDate() << "_" << now.GetTime() << ".root";
        // } else if (OutFilePre.find(".root") != string::npos) {
        //     OutFileName << OutFilePre;
        // } else {
        //     OutFileName << OutFilePre <<".root";
        // }

        // TFile* outFile = new TFile(OutFileName.str().c_str(), "RECREATE");
        // Sliced2ds->Write();
        // if(subtract){
        //     Subtracted->Write();
        // }
        // outFile->Close();
        return 0;
    } else {
        cout << "Need List of Slice Ranges and a histogram to cut" << endl;
        cout << "THnSlicePlotter( vector <low,high> ) where low and high are ints" << endl;
        return 1;
    }
}