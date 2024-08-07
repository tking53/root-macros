#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TObjArrayWriter.hpp"
#include "TROOT.h"

using namespace std;
TH2* Slicer3D(Int_t axisToZoom, pair<Int_t, Int_t> axisToProj, pair<Int_t, Int_t> range, THnSparse* hist2slice, Int_t sliceNum, Bool_t verbose = false) {
    if (verbose) {
        cout << "Slicing from " << range.first << " to " << range.second << " on Axis " << axisToZoom << endl;
    }
    string rootTitle = hist2slice->GetTitle();
    string rootName = hist2slice->GetName();
    hist2slice->GetAxis(axisToZoom)->SetRangeUser(range.first, range.second);
    hist2slice->Projection(1, 0);
    string projectName = rootName + "_proj_" + to_string(axisToProj.first) + "_" + to_string(axisToProj.second);
    TH2* projection = ((TH2*)gDirectory->Get(projectName.c_str()));
    TH2F* SliceTemp = (TH2F*)projection->Clone();
    projection->Delete();
    string newTitle = rootTitle + ": Sliced with {" + to_string(range.first) + "," + to_string(range.second) + "}";
    string newName = rootName + "_slice_" + to_string(sliceNum);
    SliceTemp->SetTitle(newTitle.c_str());
    SliceTemp->SetName(newName.c_str());

    return SliceTemp;
}

TH1* SliceSubractor(TObjArray* h2s, pair<Int_t, Int_t> ids2Sub, pair<Int_t, Int_t> subRange, pair<Int_t, Int_t> normRange, Int_t sliceNum = 0) {
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
        name = "VS_" + to_string(subRange.first) + "_" + to_string(subRange.second) + "_" + to_string(sliceNum);
    }
    subd->SetNameTitle(name.c_str(), title.c_str());
    //delete tmp stuff
    h1P->Delete();
    h2P->Delete();

    return subd;
}

Int_t THn3SlicePlotter(const vector<pair<Int_t, Int_t>> ListOfSlices, string nSparse, Int_t axis2zoom, pair<Int_t, Int_t> axis2proj, Bool_t subtract = false, pair<Int_t, Int_t> subRange = {120, -1}, pair<Int_t, Int_t> normRange = {600, 800}, Bool_t subtractDisplay = false) {
    TObjArray* Sliced2ds = new TObjArray;
    TObjArray* Subtracted = new TObjArray;
    if (subtract && ListOfSlices.size() < 2) {
        cout << "Need more than 1 slice to subtract" << endl;
        return 2;
    }

    Sliced2ds->SetName("THn_Slices");
    Subtracted->SetName("THn_subd");
    if (!ListOfSlices.empty() && nSparse != "") {
        THnSparse* Hns = ((THnSparse*)gFile->Get(nSparse.c_str()));
        for (unsigned it = 0; it < ListOfSlices.size(); it++) {
            TH2* histTemp = Slicer3D(axis2zoom, axis2proj, ListOfSlices.at(it), Hns, it, true);

            Sliced2ds->Add(histTemp);
        }
        if (subtract) {
            gDirectory->Add(Sliced2ds);
            cout << "Adding THn_Slices TObjArray to gDirectory" << endl;
            cout << "Subtracting" << endl;
            for (Int_t sub = 0; sub <= (Int_t)(ListOfSlices.size() - 1); sub++) {
                Subtracted->Add(SliceSubractor(Sliced2ds, {0, sub}, subRange, normRange, sub));
            }
            gDirectory->Add(Subtracted);
            cout << "Adding THn_subd TObjArray to gDirectory" << endl;
        } else {
            gDirectory->Add(Sliced2ds);
        }

        stringstream fname;
        TDatime now;
        fname << "THn3Slice_output_subs_" << now.GetDate() << "_" << now.GetTime() << ".root";
        auto outFile = new TFile(filename.str().c_str(), "RECREATE");

        WriteTObjArray(Sliced2ds);
        WriteTObjArray(Subtracted);
        return 0;

    } else {
        cout << "Need List of Slice Ranges and a histogram to cut" << endl;
        cout << "THnSlicePlotter( vector <low,high> ) where low and high are ints" << endl;
        return 1;
    }
}