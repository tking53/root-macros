#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TObjArray.h"
#include "TROOT.h"
#include "TTree.h"
#include <PaassRootStruct.hpp>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <ios>
#include <limits>
#include <vector>

using namespace std;

/* TChain* MakeTChain() */

void MakePSPMTDigaPlots(TFile *file, int fp_point, bool isVerbose) {

  const string pin_testString = "cross_pin_";

  TTreeReader PixTreeEvt("PixTree", file);

  TTreeReaderArray<processor_struct::ROOTDEV> rd = {PixTreeEvt, "rootdev_vec_"};

  string aLname, aHname, dLname, dHname;
  bool IsFP2 = false;
  bool IsFP1 = false;

  double dy_low_thresh, dy_high_thresh;

  if (fp_point == 2) {

    aLname = "anode_l";
    aHname = "anode_h";
    dLname = "dyn_l";
    dHname = "dyn_h";

    IsFP2 = true;

    dy_low_thresh = 200;
    dy_high_thresh = 500;
  } else if (fp_point == 1) {

    aLname = "anode_low";
    aHname = "anode_high";
    dLname = "dyn_low";
    dHname = "dyn_high";

    IsFP1 = true;

    dy_low_thresh = 200;
    dy_high_thresh = 500;
  } else {
    dy_high_thresh = 50;
    dy_low_thresh = 50;
  }

  TH2 *tqdcVSch =
      new TH2D("TQvCH", "TQDC vs ChanNum", 30e3, 0, 30e3, 32, 0, 31);
  TH2 *tmaxVSchan =
      new TH2D("TMvCH", "TMax vs ChanNum", 66000, 0, 66000, 32, 0, 31);
  TH2 *dlVSasuml = new TH2D("dlQvAs", "Dynode Low vs Anode Low Sum", 21e2, 0,
                            21e3, 21e2, 0, 21e3);
  TH2 *dhVSasumh = new TH2D("dhQvAs", "Dynode High vs Anode High Sum", 21e2, 0,
                            21e3, 21e2, 0, 21e3);

  TH2 *PinSingles =
      new TH2D("pinEn", "Pin Cal Energies", 4, 0, 4, 1000, 0,1000);

  TH1 *pileups = new TH1I("pileups", "PileUp tracker per chan", 210, 0, 209);
  TH1 *saturs = new TH1I("Saturs", "Saturation tracker per chan", 210, 0, 209);

  TH2 *ChanSingles =
      new TH2D("chanEn", "ChanLoc Cal Energies", 210, 0, 209, 65600, 0, 65600);

  TH2 *tof0 = new TH2D("tof0", "ToF: DB3ISL - xscint_T2 vs pin 0", 1000, 900, 1000, 10000, 0, 550);
  TH2 *tof1 = new TH2D("tof1", "ToF: DB3ISR - xscint_T2 vs pin 0", 1000, 900, 1000, 10000, 0, 550);
  TH2 *tof2 = new TH2D("tof2", "ToF: DB3ISL - xscint_T2 vs pin 1", 1000, 900, 1000, 10000, 0, 550);
  TH2 *tof3 = new TH2D("tof3", "ToF: DB3ISR - xscint_T2 vs pin 1", 1000, 900, 1000, 10000, 0, 550);

  TH2 *tof4 = new TH2D("tof4", "ToF: DB3UPA - xscint_T2 vs pin 0", 1000, 900, 1000, 10000, 0, 550);
  TH2 *tof5 = new TH2D("tof5", "ToF: DB3DPA - xscint_T2 vs pin 0", 1000, 900, 1000, 10000, 0, 550);
  TH2 *tof6 = new TH2D("tof6", "ToF: DB3UPA - xscint_T2 vs pin 1", 1000, 900, 1000, 10000, 0, 550);
  TH2 *tof7 = new TH2D("tof7", "ToF: DB3DPA - xscint_T2 vs pin 1", 1000, 900, 1000, 10000, 0, 550);

  cout << "Start Processing" << endl;
  long long TotalEvts = PixTreeEvt.GetEntries();
  /* map<Int_t, Double_t> eventsWithNoTrace; */
  cout<< "TotalEvts= " << TotalEvts << endl;
  long long CurEntryNum;
  while (PixTreeEvt.Next()) {
    CurEntryNum = PixTreeEvt.GetCurrentEntry();
    double anodeSumL = 0;
    double anodeSumH = 0;
    double dyL = 0;
    double dyH = 0;
    vector<bool> anodeTrackerL(64, false);
    vector<bool> anodeTrackerH(64, false);
    bool has_dyn_h = false;
    bool has_dyn_l = false;
    bool has_fit = false;
    bool has_rit = false;

    pair<double, double> PinsForTof[4] = {
        {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};

    pair<double, double> sepSigs[10] = {
        {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0},
        {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};

    pair<double, double> Xscint[2] = {{0.0, 0.0}, {0.0, 0.0}};

    for (auto itr = rd.begin(); itr != rd.end(); ++itr) {
      double mod = itr->modNum;
      double tmax = itr->maxVal;
      double tqdc = itr->tqdc;
      double chan = itr->chanNum;
      int chanloc = (mod * 16) + chan;

      //! Do chanloc plots before energy cut
      if (itr->pileup) {
        pileups->Fill(chanloc);
        continue;
      }

      if (itr->saturation) {
        saturs->Fill(chanloc);
        continue;
      }
      if (tmax > 0) {
        tmaxVSchan->Fill(tmax, chan);
      }

      if (tqdc > 0) {
        tqdcVSch->Fill(tqdc, chan);
      }

      ChanSingles->Fill(chanloc, itr->energy);

      //! Min energy for any signal to be counted as real
      if (itr->energy < 50 && itr->subtype != "cross_pin") {
        continue;
      }
      if (itr->type == "mtasimplantsipm" || itr->type == "pspmt") {
        if (itr->subtype == "aLname" && !anodeTrackerL.at(itr->detNum)) {
          anodeSumL += itr->energy;
          anodeTrackerL.at(itr->detNum) = true;
        } else if (itr->subtype == "aHname" && !anodeTrackerH.at(itr->detNum)) {
          anodeSumH += itr->energy;
          anodeTrackerH.at(itr->detNum) = true;
        } else if (itr->subtype == "dHname" && !has_dyn_h &&
                   itr->energy > dy_high_thresh) {
          dyH = itr->energy;
          has_dyn_h = true;
        } else if (itr->subtype == "dLname" && !has_dyn_l &&
                   itr->energy > dy_low_thresh) {
          dyL = itr->energy;
          has_dyn_l = true;
        }
      }
      /* if (itr->type.Data() == "mtas") { */
      /*   continue; */
      /* } */
      if (itr->type == "pid") {
        /* cout << "InsidePID:: subtype=\"" << itr->subtype.Data() << "\"" <<
         * endl; */

        if (itr->subtype == "cross_pin") {
          PinSingles->Fill(itr->detNum, itr->energy);
          if (itr->energy > PinsForTof[itr->detNum].first) {
            PinsForTof[itr->detNum] = {itr->energy, itr->time};
          }
        } else if (strcmp(itr->subtype, "ARIS") == 0) {
          //! map these in order of beam hitting
          int sepIndex = -1;
          if (itr->group == "db3ISL") {
            sepIndex = 0;
          } else if (itr->group == "db3ISR") {
            sepIndex = 1;
          } else if (itr->group == "db3UPA" && itr->energy >200 && itr->energy <350) {
            sepIndex = 2;
          } else if (itr->group == "db3DPA" && itr->energy >200 && itr->energy <350) {
            sepIndex = 3;
          } else if (itr->group == "db4PL") {
            sepIndex = 4;
          } else if (itr->group == "db4PR") {
            sepIndex = 5;
          } else if (itr->group == "db5PU") {
            sepIndex = 6;
          } else if (itr->group == "db5PD") {
            sepIndex = 7;
          } else if (itr->group == "db5PL") {
            sepIndex = 8;
          } else if (itr->group == "db5PR") {
            sepIndex = 9;
          } else if (itr->group == "FIT") {
            has_fit = true;
          } else if (itr->group == "RIT") {
            has_rit = true;
          }

          if (sepIndex != -1 && itr->energy > sepSigs[sepIndex].first) {
            sepSigs[sepIndex] = {itr->energy, itr->time};
          }
        }
      }
      if (itr->subtype == "xscint") {
        /* Xscint */
        /* cout<<"got to xscint"<<endl; */
        if (itr->group == "xscint_T2" || itr->group == "xscint_T1") {
          if (itr->energy > Xscint[0].first) {
            Xscint[0] = {itr->energy, itr->time};
          }
        } else if (itr->group == "xscint_B2" || itr->group == "xscint_B1") {
          if (itr->energy > Xscint[1].first) {
            Xscint[1] = {itr->energy, itr->time};
          }
        }
      } // end xscint

      //!
      if (!isVerbose && (CurEntryNum % 1000 == 0 || CurEntryNum == TotalEvts) ){
        cout << std::setprecision(1000000000) << "\rEvent::" << CurEntryNum
             << " of " << TotalEvts << " ( "
             << floor(((double)CurEntryNum / (double)TotalEvts) * 100) << " %)" << flush;
      } else if (CurEntryNum % 100 == 0) {
        cout << std::setprecision(1000000000) << "\rEvent::" << CurEntryNum
             << " of " << TotalEvts << " ( "
             << floor(((double)CurEntryNum / (double)TotalEvts) * 100) << " %)" << flush;
      }
    } //! end of for loop

    if (has_fit || has_rit) {
      cout << "fit or rit called" << endl;
    }
    double tof_subs_0 = sepSigs[0].second - Xscint[0].second ; // Xt2 - db3isl
    double tof_subs_1 = sepSigs[1].second - Xscint[0].second ; // Xt2 - db3isr
    double tof_subs_2 = sepSigs[2].second - Xscint[0].second ; // Xt2 - db3UPA
    double tof_subs_3 = sepSigs[3].second - Xscint[0].second ; // Xt2 - db3DPA

    /* cout<<"tof0= " << tof_subs_0 << " :: " */
    /*     <<"tof1= " << tof_subs_1 << " :: " */
    /*     <<"tof2= " << tof_subs_2 << " :: " */
    /*     <<"tof3= " << tof_subs_3 << " :: "  <<endl; */

    if (Xscint[0].second != 0 && sepSigs[0].second != 0) {
       /* cout<<endl<<"tof0= " << tof_subs_0 <<"  xscint=" << Xscint[1].second */
       /* << " sepSig[0]=" << sepSigs[0].second <<endl; */ 
      tof0->Fill(tof_subs_0, PinsForTof[0].first);
      tof2->Fill(tof_subs_0, PinsForTof[1].first);
    }
    if (Xscint[0].second != 0 && sepSigs[1].second != 0) {
      /* cout<<"tof1= " << tof_subs_1  <<endl; */
      tof1->Fill(tof_subs_1, PinsForTof[0].first);
      tof3->Fill(tof_subs_1, PinsForTof[1].first);
    }
    if (Xscint[0].second != 0 && sepSigs[2].second != 0) {
      /* cout<<"tof2= " << tof_subs_2  <<endl; */
      tof4->Fill(tof_subs_2, PinsForTof[0].first);
      tof6->Fill(tof_subs_2, PinsForTof[1].first);
    }
    if (Xscint[0].second != 0 && sepSigs[3].second != 0) {
      /* cout<<"tof3= " << tof_subs_3  <<endl; */
      tof5->Fill(tof_subs_3, PinsForTof[0].first);
      tof7->Fill(tof_subs_3, PinsForTof[1].first);
    }

    dlVSasuml->Fill(dyL, anodeSumL);
    dhVSasumh->Fill(dyH, anodeSumH);
  }
  cout << std::defaultfloat << endl;
}
