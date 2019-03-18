#define Rb_97_Full_cxx
// The class definition in 097rbFull.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("097rbFull.C")
// root> T->Process("097rbFull.C","some options")
// root> T->Process("097rbFull.C+")
//

#include "Rb97Full.hpp"
//All includes are in hpp

using namespace std;

void Rb97Full::Begin(TTree * /*tree*/) {
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
    outputFilePrefix = "Rb97Full_output";

    cloverCals.emplace(0, make_pair(0.340173, -0.115387));
    cloverCals.emplace(1, make_pair(0.346233, -0.743541));
    cloverCals.emplace(2, make_pair(0.329557, -0.235071));
    cloverCals.emplace(3, make_pair(0.367216, -0.223755));
}

void Rb97Full::SlaveBegin(TTree * /*PixTree*/) {
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    cout << "Slave Start" << endl;

    TString option = GetOption();

    Double_t CloverBins = 6000;
    Double_t HagBins = 8000;
    Double_t NaiBins = 8000;
    Double_t TimeBins = 8000;
    Double_t msPerBin = 10;

    timeBinning_ = msConvert / msPerBin;  //ns->desired time binning (1e-6 == 1 ms bins)

    totalCycleTimeBins = TimeBins / msPerBin;
    //tape cycle options
    maxProj_ = 5;
    subBinning = 150;  // how large should the projections be in plotted bins

    //create the arrays to store the Histos
    GammaArray = new TObjArray;
    VandleArray = new TObjArray;

    cout << "Creating clover" << endl;
    //Create the clover histos
    for (auto i = 0; i < 4; i++) {
        stringstream ss, s, sss, g, gg;
        ss << "cloverChan_" << i;
        s << "Clover Channel #" << i;
        sss << "g_e_dt_" << i;
        GammaArray->Add(new TH1D(ss.str().c_str(), s.str().c_str(), CloverBins * 4, 0, CloverBins));
        s << "Decay Curves";
        GammaArray->Add(new TH2F(sss.str().c_str(), s.str().c_str(), CloverBins * 4, 0, CloverBins, totalCycleTimeBins, 0, totalCycleTimeBins));
        g << "corrCloverChan_" << i;
        gg << "Corrected Clover Channel #" << i;
        GammaArray->Add(new TH1D(g.str().c_str(), gg.str().c_str(), CloverBins * 4, 0, CloverBins));
    }

    cout << "Creating smallHag" << endl;
    //Create the smallHag histos
    for (auto i = 0; i < 16; i++) {
        stringstream ss, s;
        ss << "smaHag_" << i;
        s << "Hargid Channel #" << i;
        GammaArray->Add(new TH1D(ss.str().c_str(), s.str().c_str(), HagBins * 2, 0, HagBins));
    }

    cout << "Creating nai" << endl;
    //Create the nai histos; only doing nai>3 (middle and top layers) adjusting numbering for clarity
    for (auto i = 4; i < 10; i++) {
        stringstream ss, s;
        ss << "nai_" << i;
        s << "NaI Channel #" << i;
        GammaArray->Add(new TH1D(ss.str().c_str(), s.str().c_str(), NaiBins, 0, NaiBins));
    }

    cout << "Creating Time Plots" << endl;
    //Clover time plots
    GammaArray->Add(new TH1F("g_e_total", "Clover Totals", CloverBins * 4, 0, CloverBins));
    GammaArray->Add(new TH2F("g_e_dt", "Decay curves for clover", CloverBins * 4, 0, CloverBins, totalCycleTimeBins, 0, totalCycleTimeBins));
    GammaArray->Add(new TH2F("g_e_cycle", "Clover Energy vs Cycle", CloverBins * 4, 0, CloverBins, 500, 0, 500));

    //Hag time plots
    GammaArray->Add(new TH1F("h_e_total", "HAGRiD Totals", HagBins * 2, 0, HagBins));
    GammaArray->Add(new TH2F("h_e_dt", "Decay curves for HAGRiD", HagBins * 2, 0, HagBins, totalCycleTimeBins, 0, totalCycleTimeBins));
    GammaArray->Add(new TH2F("h_e_cycle", "HAGRiD Energy vs Cycle", HagBins * 2, 0, HagBins, 500, 0, 500));

    //NaI time plots
    GammaArray->Add(new TH1F("n_e_total", "NaI Totals", NaiBins, 0, NaiBins));
    GammaArray->Add(new TH2F("n_e_dt", "Decay curves for NaI", NaiBins, 0, NaiBins, totalCycleTimeBins, 0, totalCycleTimeBins));
    GammaArray->Add(new TH2F("n_e_cycle", "NaI Energy vs Cycle", NaiBins, 0, NaiBins, 500, 0, 500));

    cout << "Creating Vandle and Slice plots" << endl;
    //create the vandle histograms
    VandleArray->Add(new TH2F("t_q_ug", "ToF vs QDC Ungated", 2500, 0, 2500, 10000, 0, 10000));
    VandleArray->Add(new TH2F("t_bn", "Tof vs Bar Num", 2500, 0, 2500, 42, 0, 42));
    VandleArray->Add(new TH2F("bn_td", "Bar Num vs TDiff", 42, 0, 42, 2000, -1000, 1000));

    //Make the gamma/vandle - tape cycle projections/histos
    for (auto i = 0; i < maxProj_; i++) {
        stringstream gs, gss, shs, shss, vs, vss;
        gs << "g_e_tpro_" << i + 1;
        gss << "Clover Total Gated with slice #" << i + 1;
        GammaArray->Add(new TH1D(gs.str().c_str(), gss.str().c_str(), CloverBins * 2, 0, CloverBins));

        shs << "h_e_tpro_" << i + 1;
        shss << "HAGRiD Total Gated with slice #" << i + 1;
        GammaArray->Add(new TH1D(shs.str().c_str(), shss.str().c_str(), HagBins * 2, 0, HagBins));

        vs << "v_tq_g" << i + 1;
        vss << "Vandle Tof vs QDC Gated with slice #" << i + 1;
        VandleArray->Add(new TH2F(vs.str().c_str(), vss.str().c_str(), 1500, 0, 1500, 10000, 0, 10000));
        vs.str("");
        vss.str("");
        vs << "v_tbn_g" << i + 1;
        vss << "Vandle Tof vs BarNum Gated with slice #" << i + 1;
        VandleArray->Add(new TH2F(vs.str().c_str(), vss.str().c_str(), 1500, 0, 1500, 42, 0, 42));
    }

    //Add the TObjArrays to the output list
    AddToOutputList(GammaArray);
    AddToOutputList(VandleArray);

    //Initalize progress counters event count (cnt) and file count (fcnt)
    cnt = 0;
    fcnt = 1;
    cout << "Slave Begin() complete" << endl;
}

void Rb97Full::Init(TTree *tree) {
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the reader is initialized.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    fReader.SetTree(tree);
    entTotal_ = fReader.GetEntries(kTRUE);
    isChain = fReader.IsChain();
}

Bool_t Rb97Full::Notify() {
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    if (isChain && cnt != 0) {
        cout << "Starting new tree in chain" << endl;
        fcnt++;
    }

    return kTRUE;
}

Bool_t Rb97Full::Process(Long64_t entry) {
    fReader.SetLocalEntry(entry);
    Double_t LastCT = 0;
    Int_t currentCycleNum = 0;

    LastCT = logic.At(0).lastTapeCycleStartTime;  // ns
    currentCycleNum = logic.At(0).cycleNum;

    //get sizes of vectors 
    Bool_t cloEvt, gsEvt, vanEvt;
    cloEvt=gsEvt=vanEvt=false;
    if (clover.GetSize() != 0) {
        cloEvt = true;
    }
    if (gamscint.GetSize() != 0) {
        gsEvt = true;
    }
    if (vandle.GetSize() != 0) {
        vanEvt = true;
    }

    
    //clover only
    if (cloEvt) {
        for (auto itC = clover.begin(); itC != clover.end(); itC++) {
            Double_t cEnergy_ = itC->energy;
            Double_t cTime_ = itC->time * DSPclockInSeconds * 1e9;  //ns after convert
            Double_t dt = (cTime_ - LastCT) * timeBinning_;         // binned time
            
            Double_t slope = cloverCals.find(itC->detNum)->second.first;
            Double_t inter = cloverCals.find(itC->detNum)->second.second;
            Double_t cCorEnergy_ = slope * itC->rawEnergy + inter;
            
            stringstream s, ss, sss;
            s << "cloverChan_" << itC->detNum;
            ss << "g_e_dt_" << itC->detNum;
            sss << "corrCloverChan_" << itC->detNum;
            ((TH1 *)GammaArray->FindObject(s.str().c_str()))->Fill(cEnergy_);
            ((TH1 *)GammaArray->FindObject(sss.str().c_str()))->Fill(cCorEnergy_);
            ((TH1 *)GammaArray->FindObject("g_e_total"))->Fill(cEnergy_);
            ((TH2 *)GammaArray->FindObject("g_e_dt"))->Fill(cEnergy_, dt);
            ((TH2 *)GammaArray->FindObject(ss.str().c_str()))->Fill(cEnergy_, dt);
            ((TH2 *)GammaArray->FindObject("g_e_cycle"))->Fill(cEnergy_, currentCycleNum);

            //sliced 1d
            for (auto ind = 0; ind < maxProj_; ind++) {
                ss.str("");
                ss << "g_e_tpro_" << ind + 1;

                if (dt > subBinning * ind && dt <= subBinning * (ind + 1)) {
                    ((TH1 *)GammaArray->FindObject(ss.str().c_str()))->Fill(cEnergy_);
                }
            }
        }
    }
   
    //hag and nai
    if (gsEvt) {
        for (auto itGS = gamscint.begin(); itGS != gamscint.end(); itGS++) {
            Double_t GSEnergy_ = itGS->energy;
            Double_t GSTime_ = itGS->time * DSPclockInSeconds * 1e9;  //ns after convert
            Double_t dt = (GSTime_ - LastCT) * timeBinning_;          // binned time
            string type = itGS->subtype.Data();
            string sst = itGS->group.Data();
            string timeType, singleType;
            stringstream S, SET, SEDT, SEC, SS;
            Bool_t hagrid = false;
            if (type == "nai" && itGS->detNum < 4) {
                continue;
            }
            if (type == "smallhag") {
                timeType = "h";
                singleType = "smaHag_";
                hagrid = true;
            } else if (type == "nai") {
                timeType = "n";
                singleType = "nai_";
            } else {
                cout << "UNKNOWN TYPE IN GAMMASCINT LOOP" << endl;
                continue;
            }

            S << singleType << itGS->detNum;
            SET << timeType << "_e_total";
            SEDT << timeType << "_e_dt";
            SEC << timeType << "_e_cycle";
           
            ((TH1 *)GammaArray->FindObject(S.str().c_str()))->Fill(GSEnergy_);
            ((TH1 *)GammaArray->FindObject(SET.str().c_str()))->Fill(GSEnergy_);
            ((TH2 *)GammaArray->FindObject(SEDT.str().c_str()))->Fill(GSEnergy_, dt);
            ((TH2 *)GammaArray->FindObject(SEC.str().c_str()))->Fill(GSEnergy_, currentCycleNum);
            
            //sliced
            if (hagrid) {
                for (auto ind = 0; ind < maxProj_; ind++) {
                    SS.str("");
                    SS << timeType << "_e_tpro_" << ind + 1;
                    if (dt > subBinning * ind && dt <= subBinning * (ind + 1)) {
                        ((TH1 *)GammaArray->FindObject(SS.str().c_str()))->Fill(GSEnergy_);
                    }
                }
            }
            
        }
    }
    
    //vandle
    if (vanEvt) {
        for (auto itV = vandle.begin(); itV != vandle.end(); itV++) {
            Double_t tof = itV->tof;
            Double_t qdc = itV->qdc;
            Double_t tdiff = itV->tdiff;
            Double_t barnum = itV->barNum;
            if (barnum == 18) {
                continue;
            }
            Double_t curTime = (itV->sTime - LastCT) * timeBinning_;
            //no tape cut
            ((TH2 *)VandleArray->FindObject("t_q_ug"))->Fill(tof, qdc);
            ((TH2 *)VandleArray->FindObject("t_bn"))->Fill(tof, barnum);
            ((TH2 *)VandleArray->FindObject("bn_td"))->Fill(barnum, tdiff);

            stringstream ss, vss;  //totalCycleTimeBins
            for (auto ind = 0; ind < maxProj_; ind++) {
                ss.str("");
                vss.str("");
                ss << "v_tq_g" << ind + 1;
                vss << "v_tbn_g" << ind + 1;

                if (curTime > subBinning * ind && curTime <= subBinning * (ind + 1)) {
                    ((TH2 *)VandleArray->FindObject(ss.str().c_str()))->Fill(tof, qdc);
                    ((TH2 *)VandleArray->FindObject(vss.str().c_str()))->Fill(tof, barnum);
                }
            }
        }
    }

    //Run Stats
    cnt++;
    if (!gROOT->IsProofServ()) {
        if (cnt % 1000000 == 0) {
            int per = ((Double_t)cnt / (Double_t)entTotal_) * 100;
            if (isChain) {
                cout << "Processed entry " << cnt << " of " << entTotal_ << " (" << per << "%; file# " << fcnt << ")" << endl;
            } else {
                cout << "Processed entry " << cnt << " of " << entTotal_ << " (" << per << "%)" << endl;
            }
        }
    }

    return kTRUE;
}

void Rb97Full::SlaveTerminate() {
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.
}

void Rb97Full::Terminate() {
    stringstream filename;
    TDatime now;
    filename << outputFilePrefix << "_" << now.GetDate() << "_" << now.GetTime() << ".root";
    outFile = new TFile(filename.str().c_str(), "RECREATE");

    TIter next(GetOutputList());
    while (TObject *obj = next()) {
        obj->Write();
        //    delete (obj);
    }
    outFile->Close();
}

void Rb97Full::AddToOutputList(TObjArray *iArray) {
    // Loop over the TObjArrays and add the histos to the output list.
    // needs 2 while loops for 1d and 2d (per TObjArray)

    TIter next(iArray);
    while (TH1 *hist = (TH1 *)next()) {
        GetOutputList()->Add(hist);
    }
    while (TH2 *hist = (TH2 *)next()) {
        GetOutputList()->Add(hist);
    }
}