/*Plots the all of the individual channels of the respective detector 
 */

#include </opt/root/6.14.06/include/TROOT.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
enum EColor kDarkCyan = (enum EColor)(kCyan + 2);
vector<enum EColor> geCols = {kRed, kGreen, kBlue, kDarkCyan};

vector<enum EColor> hagCols = {
    kBlue, (enum EColor)(kBlue + 2),
    kRed, (enum EColor)(kRed + 1), (enum EColor)(kRed + 2), (enum EColor)(kRed + 3), (enum EColor)(kRed - 2), (enum EColor)(kRed - 7),
    (enum EColor)(kMagenta + 3), (enum EColor)(kMagenta - 2),
    kGreen, (enum EColor)(kGreen + 2), (enum EColor)(kGreen + 3), (enum EColor)(kGreen + 4), (enum EColor)(kGreen - 2), (enum EColor)(kGreen - 6)};

void CloverIndiviuals(string prefix = "g_E_", Int_t lowR = 0, Int_t highR = 8000) {
    auto c1 = new TCanvas("c1", "c1", 800, 600);
    stringstream ss;
    for (Int_t i = 0; i < 4; i++) {
        stringstream ss;
        ss << prefix.c_str() << i;
        TH2* hh = ((TH2*)gFile->Get(ss.str().c_str()));
        hh->SetLineColor(geCols.at(i));
        hh->GetXaxis()->SetRangeUser(lowR, highR);
        if (i == 0) {
            hh->Draw("");
        } else {
            hh->Draw("same");
        }
    }
}

void HagIndiviuals(string prefix = "h_E_", Int_t lowR = 0, Int_t highR = 8000) {
    auto c2 = new TCanvas("c2", "c2", 800, 600);
    stringstream ss;
    for (Int_t i = 0; i < 16; i++) {
        stringstream ss;
        ss << prefix.c_str() << i;
        TH2* hh = ((TH2*)gFile->Get(ss.str().c_str()));
        hh->SetLineColor(hagCols.at(i));
        hh->GetXaxis()->SetRangeUser(lowR, highR);
        if (i == 0) {
            hh->Draw("");
        } else {
            hh->Draw("same");
        }
    }
}
