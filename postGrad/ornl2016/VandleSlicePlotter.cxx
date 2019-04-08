
#include </opt/root/6.14.06/include/TROOT.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void VandleSlicePlotter() {
    //47 is too many plots so we are splitting onto multiple canvas'

    Int_t MaxPerCan = 5;
    TCanvas* c1 = new TCanvas("c1", "c1", 1920, 1080);
    c1->SetLogz();
    c1->Divide(3, 2);
    for (int it = 1; it < MaxPerCan; it++) {
        stringstream ss;
        ss << "v_tq_g" << it << ";1" << endl;
        TH2* hh = ((TH2*)gFile->Get(ss.str().c_str()));
        hh->GetXaxis()->SetRangeUser(20, 1000);
        hh->GetYaxis()->SetRangeUser(0, 8000);
        hh->SetMaximum(5);
        hh->SetMinimum(1);
        // std::cout<<"Can1 t="<<it<<"      IT="<<it<<endl;
        c1->cd((Int_t)it);
        gPad->SetLogz();
        hh->Draw("COLZ");
    }

    TCanvas* c2 = new TCanvas("c2", "c2", 1920, 1080);
    c2->SetLogz();
    c2->Divide(3, 2);
    c2->cd();
    for (int it = MaxPerCan; it < MaxPerCan * 2; it++) {
        stringstream ss;
        ss << "v_tq_g" << it << ";1" << endl;
        TH2* hh = ((TH2*)gFile->Get(ss.str().c_str()));
        hh->GetXaxis()->SetRangeUser(20, 1000);
        hh->GetYaxis()->SetRangeUser(0, 8000);
        hh->SetMaximum(5);
        hh->SetMinimum(1);
        Int_t t = it - (MaxPerCan - 1);
        // std::cout<<"Can2 t="<<t<<"      IT="<<it<<endl;
        c2->cd((Int_t)t);
        gPad->SetLogz();
        hh->Draw("COLZ");
    }

    TCanvas* c3 = new TCanvas("c3", "c3", 1920, 1080);
    c3->SetLogz();
    c3->Divide(3, 2);
    c3->cd();
    for (int it = MaxPerCan * 2; it < MaxPerCan *3; it++) {
        stringstream ss;
        ss << "v_tq_g" << it << ";1" << endl;
        TH2* hh = ((TH2*)gFile->Get(ss.str().c_str()));
        hh->GetXaxis()->SetRangeUser(20, 1000);
        hh->GetYaxis()->SetRangeUser(0, 8000);
        hh->SetMaximum(5);
        hh->SetMinimum(1);
        Int_t t = it - (MaxPerCan * 2 - 1);
        // std::cout<<"Can3 t="<<t<<"      IT="<<it<<endl;
        c3->cd((Int_t)t);
        gPad->SetLogz();
        hh->Draw("COLZ");
    }

    TCanvas* c4 = new TCanvas("c4", "c4", 1920, 1080);
    c4->SetLogz();
    c4->Divide(3, 2);
    c4->cd();
    for (int it = MaxPerCan * 3; it < 21; it++) {
        stringstream ss;
        ss << "v_tq_g" << it << ";1" << endl;
        TH2* hh = ((TH2*)gFile->Get(ss.str().c_str()));
        hh->GetXaxis()->SetRangeUser(20, 1000);
        hh->GetYaxis()->SetRangeUser(0, 8000);
        hh->SetMaximum(5);
        hh->SetMinimum(1);
        Int_t t = it - (MaxPerCan * 3 - 1);
        // std::cout<<"Can3 t="<<t<<"      IT="<<it<<endl;
        c4->cd((Int_t)t);
        gPad->SetLogz();
        hh->Draw("COLZ");
    }
}
