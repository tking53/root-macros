#include </opt/root/6.14.06/include/TROOT.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <THnSparse.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

TH2* Slicer(pair<Int_t, Int_t> range, THnSparse* hist2slice, Int_t sliceNum, Bool_t verbose = false) {
    if (verbose) {
        cout << "Slicing from " << range.first << " to " << range.second << endl;
    }
    string rootTitle = hist2slice->GetTitle();
    string rootName = hist2slice->GetName();
    hist2slice->GetAxis(2)->SetRangeUser(range.first, range.second);
    hist2slice->Projection(1, 0);
    TH2* projection = ((TH2*)gDirectory->Get("v_t_q_dt_proj_0_1"));
    TH2F* SliceTemp = (TH2F*)projection->Clone();
    projection->Delete();
    string newTitle = rootTitle + ": Sliced with {" + to_string(range.first) + "," + to_string(range.second) + "}";
    string newName = rootName + "_slice_" + to_string(sliceNum);
    SliceTemp->SetTitle(newTitle.c_str());
    SliceTemp->SetName(newName.c_str());

    return SliceTemp;
}

TH1* SliceSubractor(TObjArray* h2s, pair<Int_t, Int_t> ids2Sub, pair<Int_t, Int_t> subRange, Int_t sliceNum = 0) {
    TH2* h1 = (TH2*)h2s->At(ids2Sub.first);
    TH2* h2 = (TH2*)h2s->At(ids2Sub.second);

    // cout << "subracting: " << h2->GetTitle() << " from " << h1->GetTitle() << endl;

    h1->ProjectionX("h1Proj", subRange.first, subRange.second);
    h2->ProjectionX("h2Proj", subRange.first, subRange.second);

    TH1D* h1P = (TH1D*)gDirectory->Get("h1Proj");
    TH1D* h2P = (TH1D*)gDirectory->Get("h2Proj");

    Double_t h1PInt = h1P->Integral(600, 800);  // integrate late tof (600ns @1m~ 14keV )
    Double_t h2PInt = h2P->Integral(600, 800);  // integrate late tof (600ns @1m~ 14keV )

    Double_t NormConstant = -1 * h1PInt / h2PInt;

    TH1D* subd = (TH1D*)h1P->Clone();
    subd->Add(h2P, NormConstant);
    string name, title;
    if (subRange.second == -1) {
        title = "Vandle Tof projection: QDC cut {" + to_string(subRange.first) + ", last} sliceNum=" + to_string(sliceNum);
        name = "VS_" + to_string(subRange.first) + "_L_" + to_string(sliceNum);
    } else {
        title = "Vandle Tof projection: QDC cut {" + to_string(subRange.first) + "," + to_string(subRange.second) + "} sliceNum=" + to_string(sliceNum);
        name = "VS_" + to_string(subRange.first) + "_" + to_string(subRange.second) + "_" + to_string(sliceNum);
    }
    subd->SetNameTitle(name.c_str(), title.c_str());
    //delete tmp stuff
    h1P->Delete();
    h2P->Delete();

    return subd;
}

Int_t VandleSlicePlotter(const vector<pair<Int_t, Int_t>> ListOfSlices, Bool_t subtract = false) {
    TObjArray* Sliced2ds = new TObjArray;
    if (subtract) {
        TObjArray* Subtraced = new TObjArray;
    }
    Sliced2ds->SetName("VTQ_Slices");
    if (!ListOfSlices.empty()) {
        string nSparse = "v_t_q_dt";
        THnSparse* Hns = ((THnSparse*)gFile->Get(nSparse.c_str()));
        for (unsigned it = 0; it < ListOfSlices.size(); it++) {
            TH2* histTemp = Slicer(ListOfSlices.at(it), Hns, it, true);
            Sliced2ds->Add(histTemp);
        }
        if (!subtract) {
            gDirectory->Add(Sliced2ds);
        } else {
            gDirectory->Add(Sliced2ds);
            cout << "Subtracting" << endl;
            TCanvas* c1 = new TCanvas("C1", "C1");
            TH1* sub1 = SliceSubractor(Sliced2ds, {0, 1}, {120, -1}, 0);
            TH1* sub2 = SliceSubractor(Sliced2ds, {0, 2}, {120, -1}, 1);

            // c1->Divide(1,2);
            // c1->cd(1);
            // sub1->Draw()

            sub2->SetLineColor(kRed);

            sub1->Draw();
            sub2->Draw("SAME");
            // gDirectory->Add(sub1);
        }

        return 0;
    } else {
        cout << "Need List of Slice Ranges" << endl;
        cout << "VandleSlicePlotter( vector <low,high> ) where low and high are ints" << endl;
        return 1;
    }
}