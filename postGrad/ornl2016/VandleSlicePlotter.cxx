
#include </opt/root/6.14.06/include/TROOT.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <sstream>
#include <string>
#include <vector>

void VandleSlicePlotter() {
    //47 is too many plots so we are splitting onto 2 canvas'
    TCanvas* c1 = new TCanvas("c1","c1");
    c1->SetLogz();
    c1->Divide(4, 6);
    for (int it = 1; it < 24; it++) {
        stringstream ss;
        ss << "v_tq_g" << it << ";1" << endl;
        TH2* hh = ((TH2*)gFile->Get(ss.str().c_str()));
        hh->GetXaxis()->SetRangeUser(0, 1500);
        hh->GetYaxis()->SetRangeUser(0, 1500);
        if (it == 5)
            hh->SetMinimum(1);
        else
            hh->SetMinimum(2);

        c1->cd((Int_t)it);
        gPad->SetLogz();
        hh->Draw("COLZ");
    }

    TCanvas* c2 = new TCanvas("c2","c2");
    c2->SetLogz();
    c2->Divide(4, 6);
    for (int it = 24; it < 48; it++) {
        stringstream ss;
        ss << "v_tq_g" << it << ";1" << endl;
        TH2* hh = ((TH2*)gFile->Get(ss.str().c_str()));
        hh->GetXaxis()->SetRangeUser(0, 1500);
        hh->GetYaxis()->SetRangeUser(0, 1500);
        if (it == 5)
            hh->SetMinimum(1);
        else
            hh->SetMinimum(2);

        c1->cd((Int_t)it);
        gPad->SetLogz();
        hh->Draw("COLZ");
    }
}
