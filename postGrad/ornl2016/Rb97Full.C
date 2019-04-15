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

#include "Rb97Full.h"
//All includes are in hpp

using namespace std;

void Rb97Full::Begin(TTree * /*tree*/) {
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
    outputFilePrefix = "Rb97Full_output";
}

void Rb97Full::SlaveBegin(TTree * /*PixTree*/) {
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    cout << "Slave Start" << endl;

    valid_CBdT = new TCutG("clover_beta_tdiff", 20);
    valid_CBdT->SetVarX("Clover Energy vs Clover-Beta dt");
    valid_CBdT->SetVarY("");
    valid_CBdT->SetTitle("Graph");
    valid_CBdT->SetFillStyle(1000);
    valid_CBdT->SetPoint(0, 67.2575, 935.835);
    valid_CBdT->SetPoint(1, 125.912, 691.18);
    valid_CBdT->SetPoint(2, 297.967, 512.45);
    valid_CBdT->SetPoint(3, 599.061, 417.225);
    valid_CBdT->SetPoint(4, 1920.75, 348.37);
    valid_CBdT->SetPoint(5, 3582.64, 314.675);
    valid_CBdT->SetPoint(6, 5549.53, 291.235);
    valid_CBdT->SetPoint(7, 5690.3, 232.635);
    valid_CBdT->SetPoint(8, 5338.37, 179.895);
    valid_CBdT->SetPoint(9, 3402.76, 200.405);
    valid_CBdT->SetPoint(10, 1005.74, 200.405);
    valid_CBdT->SetPoint(11, 638.165, 204.8);
    valid_CBdT->SetPoint(12, 137.643, 247.285);
    valid_CBdT->SetPoint(13, 55.5265, 399.645);
    valid_CBdT->SetPoint(14, 32.0646, 595.955);
    valid_CBdT->SetPoint(15, 32.0646, 921.185);
    valid_CBdT->SetPoint(16, 67.2575, 927.045);

    TString option = GetOption();

    //global things
    gbdtBin = 1;                                                   // gamma beta dT Bin factor
    BinShift = 200;                                                // offset for Gamma Beta dT
    Valid_CloverBetaTdiff = {{250, 600}, {225, 400}, {225, 330}};  //max tdiff (including shift) between clover and beta to be good
    Valid_HagLRBetaTdiff = 300;                                    //max tdiff (including shift) between Hag and beta to be good
    Valid_HagHRBetaTdiff = 300;                                    //max tdiff (including shift) between Hag and HR beta to be good
    Valid_NaiBetaTdiff = 275;                                      //max tdiff (including shift) between Nai and beta to be good
    Valid_VandleTdiff = 15;                                        // +/- tdiff Vandle bar to be good

    //Bins for His
    Int_t CloverBins = 6000;
    Int_t HagBins = 8000;
    Int_t NaiBins = 8000;
    Int_t VTofBins = 1500;
    Int_t VQdcBins = 20000;
    Int_t TimeBins = 8000;
    Int_t msPerBin = 10;
    Int_t RawEnergyBins = 32000;
    Int_t DT_bins = 2000 / gbdtBin;

    timeBinning_ = msConvert / msPerBin;  //ns->desired time binning (1e-6 == 1 ms bins)

    totalCycleTimeBins = TimeBins / msPerBin;
    //tape cycle options
    // maxProj_ = 20;
    // subBinning = ceil(700 / maxProj_);  // how large should the projections be in plotted bins

    //create the arrays to store the Histos
    GammaArray = new TObjArray;
    VandleArray = new TObjArray;

    VandleArray->Add(new TH2F("beta_td_q", "High res beta tdiff vs qdc ", 2000, -100, 100, 8000, 0, 8000));
    VandleArray->Add(new TH2F("beta_multi", "Beta Multiplicy 0=LR 1=HR ", 50, 0, 50, 3, 0, 3));

    //-------------------------------------------------------------------------------------------------------
    cout << "Creating clover" << endl;
    //Create the clover histos
    for (auto i = 0; i < 4; i++) {
        stringstream ss, s;
        ss << "g_E_" << i;
        s << "Clover Channel #" << i;
        GammaArray->Add(new TH1D(ss.str().c_str(), s.str().c_str(), CloverBins, 0, CloverBins));

        // s.str("");
        // ss.str("");
        // s << "RAW: Clover Channel #" << i << " Decay Curves";
        // ss << "g_re_dt_" << i;
        // GammaArray->Add(new TH2F(ss.str().c_str(), s.str().c_str(), RawEnergyBins, 0, RawEnergyBins, totalCycleTimeBins, 0, totalCycleTimeBins));

        // s.str("");
        // ss.str("");
        // s << "Cal: Clover Channel #" << i << " Decay Curves";
        // ss << "g_e_dt_" << i;
        // GammaArray->Add(new TH2F(ss.str().c_str(), s.str().c_str(), RawEnergyBins, 0, RawEnergyBins, totalCycleTimeBins, 0, totalCycleTimeBins));
    }
    //Clover time plots
    GammaArray->Add(new TH1D("g_e_BG", "Clover Totals: Beta Gated", CloverBins, 0, CloverBins));

    GammaArray->Add(new TH1F("g_e_total", "Clover Totals", CloverBins, 0, CloverBins));
    GammaArray->Add(new TH2F("g_e_dt_cut", "Clover Totals in tdiff tcut vs time in cycle", CloverBins, 0, CloverBins, DT_bins, 0, DT_bins));
    GammaArray->Add(new TH2F("g_e_dt", "Decay curves for clover", CloverBins, 0, CloverBins, totalCycleTimeBins, 0, totalCycleTimeBins));
    GammaArray->Add(new TH2F("g_e_cycle", "Clover Energy vs Cycle", CloverBins, 0, CloverBins, 500, 0, 500));

    GammaArray->Add(new TH2F("g_e_b_dt", "Clover Energy vs Clover-Beta dt", CloverBins, 0, CloverBins, DT_bins, 0, DT_bins));

    GammaArray->Add(new TH2F("g_e_b_dt_cut", "CUT CHECK:Clover Energy vs Clover-Beta dt", CloverBins, 0, CloverBins, DT_bins, 0, DT_bins));

    //-------------------------------------------------------------------------------------------------------
    cout << "Creating smallHag" << endl;
    //Create the smallHag histos
    for (auto i = 0; i < 16; i++) {
        stringstream ss, s;
        ss << "h_E_" << i;
        s << "Hargid Channel #" << i;
        GammaArray->Add(new TH1D(ss.str().c_str(), s.str().c_str(), HagBins, 0, HagBins));
    }

    //Hag time plots
    GammaArray->Add(new TH1D("h_e_BG", "LaBr Totals: Beta Gated", HagBins, 0, HagBins));
    GammaArray->Add(new TH1F("h_e_total", "HAGRiD Totals", HagBins, 0, HagBins));
    GammaArray->Add(new TH2F("h_e_dt", "Decay curves for HAGRiD", HagBins, 0, HagBins, totalCycleTimeBins, 0, totalCycleTimeBins));
    GammaArray->Add(new TH2F("h_e_cycle", "HAGRiD Energy vs Cycle", HagBins, 0, HagBins, 500, 0, 500));

    GammaArray->Add(new TH2F("h_e_b_dt", "LaBr Energy vs LaBr-Beta dt", HagBins, 0, HagBins, DT_bins, 0, DT_bins));

    GammaArray->Add(new TH2F("h_e_hrb_dt", "LaBr Energy vs LaBr- high res Beta dt", HagBins, 0, HagBins, DT_bins, 0, DT_bins));

    GammaArray->Add(new TH2F("h_num_b_dt", "LaBr DetNum vs LaBr-Beta dt", 20, 0, 20, DT_bins, 0, DT_bins));

    //-------------------------------------------------------------------------------------------------------
    cout << "Creating nai" << endl;
    //Create the nai histos; only doing nai>3 (middle and top layers) adjusting numbering for clarity
    for (auto i = 4; i < 10; i++) {
        stringstream ss, s;
        ss << "n_E_" << i;
        s << "NaI Channel #" << i;
        GammaArray->Add(new TH1D(ss.str().c_str(), s.str().c_str(), NaiBins, 0, NaiBins));
    }

    //NaI time plots
    GammaArray->Add(new TH1D("n_e_BG", "Nai Totals: Beta Gated", NaiBins, 0, NaiBins));
    GammaArray->Add(new TH1F("n_e_total", "NaI Totals", NaiBins, 0, NaiBins));
    GammaArray->Add(new TH2F("n_e_dt", "Decay curves for NaI", NaiBins, 0, NaiBins, totalCycleTimeBins, 0, totalCycleTimeBins));
    GammaArray->Add(new TH2F("n_e_cycle", "NaI Energy vs Cycle", NaiBins, 0, NaiBins, 500, 0, 500));

    GammaArray->Add(new TH2F("n_e_b_dt", "nai Energy vs NaI-Beta dt", NaiBins, 0, NaiBins, DT_bins, 0, DT_bins));
    GammaArray->Add(new TH2F("n_num_b_dt", "Nai DetNum vs Nai-Beta dt", 20, 0, 20, DT_bins, 0, DT_bins));

    //-------------------------------------------------------------------------------------------------------
    cout << "Creating Vandle and Slice plots" << endl;
    //create the vandle histograms
    VandleArray->Add(new TH2F("t_q_ug", "ToF vs QDC Ungated", 2500, 0, 2500, 10000, 0, 10000));
    VandleArray->Add(new TH2F("t_bn", "Tof vs Bar Num", 2500, 0, 2500, 42, 0, 42));
    VandleArray->Add(new TH2F("bn_td", "Bar Num vs TDiff", 42, 0, 42, 2000, -1000, 1000));

    //------------------------------------------------------------------------------------------------------
    VandleArray->Add(new TH2F("g_tof", "Clover Energy vs Vandle Tof (with cuts on the appropriate TDIFF) ", CloverBins, 0, CloverBins, 2000, 0, 2000));
    VandleArray->Add(new TH2F("h_tof", "LaBr Energy vs Vandle Tof (with cuts on the appropriate TDIFF) ", HagBins, 0, HagBins, 2000, 0, 2000));
    VandleArray->Add(new TH2F("n_tof", "Nai Energy vs Vandle Tof (with cuts on the appropriate TDIFF) ", NaiBins, 0, NaiBins, 2000, 0, 2000));

    // //Make the gamma/vandle - tape cycle projections/histos
    // for (auto i = 0; i < maxProj_; i++) {
    //     stringstream gs, gss, vs, vss;
    //     vs << "v_tq_g" << i + 1;
    //     vss << "Vandle Tof vs QDC Gated with slice #" << i + 1;
    //     VandleArray->Add(new TH2F(vs.str().c_str(), vss.str().c_str(), 1500, 0, 1500, 10000, 0, 10000));
    //     vs.str("");
    //     vss.str("");
    //     vs << "v_tbn_g" << i + 1;
    //     vss << "Vandle Tof vs BarNum Gated with slice #" << i + 1;
    //     VandleArray->Add(new TH2F(vs.str().c_str(), vss.str().c_str(), 1500, 0, 1500, 42, 0, 42));

    //     gss.str("");
    //     gs.str("");
    //     gs << "n_e_b_dt_" << i + 1;
    //     gss << "Nai Energy vs NaI-Beta dt: Slice #" << i + 1;
    //     GammaArray->Add(new TH2F(gs.str().c_str(), gss.str().c_str(), NaiBins, 0, NaiBins, DT_bins, 0, DT_bins));
    //     gss.str("");
    //     gs.str("");
    //     gs << "h_e_b_dt_" << i + 1;
    //     gss << "LaBr Energy vs NaI-Beta dt: Slice #" << i + 1;
    //     GammaArray->Add(new TH2F(gs.str().c_str(), gss.str().c_str(), HagBins, 0, HagBins, DT_bins, 0, DT_bins));
    //     gss.str("");
    //     gs.str("");
    //     gs << "g_e_b_dt_" << i + 1;
    //     gss << "Clover Energy vs NaI-Beta dt: Slice #" << i + 1;
    //     GammaArray->Add(new TH2F(gs.str().c_str(), gss.str().c_str(), CloverBins, 0, CloverBins, DT_bins, 0, DT_bins));
    // }
    // for (auto ind = 0; ind < maxProj_; ind++) {
    //     stringstream ss, vss;
    //     ss.str("");
    //     ss << shortType << "_e_b_dt_" << ind + 1;
    //     if (dt > subBinning * ind && dt <= subBinning * (ind + 1)) {
    //         ((TH2 *)GammaArray->FindObject(ss.str().c_str()))->Fill(GSEnergy_, (ScBdt + BinShift) / gbdtBin);
    //     }
    // }

    //----------------------------------------------------------------------------------------------------------------
    //3Ds

    //VandleArray->Add( new TH3F("v_t_q_dt", "Vandle Tof vs QDC vs Time in Cycle", 1500, 0, 1500, 4000, 0, 12000, 700, 0, 700));

    VandleArray->Add(new THnSparseF("v_t_q_dt", "Vandle Tof vs QDC vs Time in Cycle", 3, new Int_t[3]{1500, 12000, totalCycleTimeBins}, new Double_t[3]{0, 0, 0}, new Double_t[3]{1500, 12000, static_cast<Double_t>(totalCycleTimeBins)}));

    VandleArray->Add(new THnSparseF("v_t_bn_dt", "Vandle Tof vs Bar Num vs Time in Cycle", 3, new Int_t[3]{1500, 43, totalCycleTimeBins}, new Double_t[3]{0, 0, 0}, new Double_t[3]{1500, 43, static_cast<Double_t>(totalCycleTimeBins)}));

    GammaArray->Add(new THnSparseF("g_e_bdt_dt", "Clover Energy vs Clover-Beta dt vs Time in Cycle", 3, new Int_t[3]{CloverBins, DT_bins, totalCycleTimeBins}, new Double_t[3]{0, 0, 0}, new Double_t[3]{static_cast<Double_t>(CloverBins), static_cast<Double_t>(DT_bins), static_cast<Double_t>(totalCycleTimeBins)}));
    GammaArray->Add(new THnSparseF("h_e_bdt_dt", "LaBr Energy vs LaBr-Beta dt vs Time in Cycle", 3, new Int_t[3]{HagBins, DT_bins, totalCycleTimeBins}, new Double_t[3]{0, 0, 0}, new Double_t[3]{static_cast<Double_t>(HagBins), static_cast<Double_t>(DT_bins), static_cast<Double_t>(totalCycleTimeBins)}));
    GammaArray->Add(new THnSparseF("h_e_hrbdt_dt", "LaBr Energy vs LaBr - High Res Beta dt vs Time in Cycle", 3, new Int_t[3]{HagBins, DT_bins, totalCycleTimeBins}, new Double_t[3]{0, 0, 0}, new Double_t[3]{static_cast<Double_t>(HagBins), static_cast<Double_t>(DT_bins), static_cast<Double_t>(totalCycleTimeBins)}));
    GammaArray->Add(new THnSparseF("n_e_bdt_dt", "Nai Energy vs NaI-Beta dt vs Time in Cycle", 3, new Int_t[3]{NaiBins, DT_bins, totalCycleTimeBins}, new Double_t[3]{0, 0, 0}, new Double_t[3]{static_cast<Double_t>(NaiBins), static_cast<Double_t>(DT_bins), static_cast<Double_t>(totalCycleTimeBins)}));

    ///---------------VANDLE GAMMA HISTS-------------------------------------------
    VandleArray->Add(new THnSparseF("g_t_q_dt", "Clover Energy vs Vandle Tof vs Vandle QDC vs Time in Cycle", 4, new Int_t[4]{CloverBins, VTofBins, VQdcBins, totalCycleTimeBins}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(CloverBins), static_cast<Double_t>(VTofBins), static_cast<Double_t>(VQdcBins), static_cast<Double_t>(totalCycleTimeBins)}));

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
    Bool_t cloEvt, gsEvt, vanEvt, LRbeta, HRbeta;
    cloEvt = gsEvt = vanEvt = LRbeta = HRbeta = false;
    if (clover.GetSize() != 0) {
        cloEvt = true;
    }
    if (gamscint.GetSize() != 0) {
        gsEvt = true;
    }
    if (vandle.GetSize() != 0) {
        vanEvt = true;
    }

    vector<pair<Double_t, Double_t>> lowResBetaList, highResBetaList;

    Bool_t goodCloverTdiff = false, goodHagTdiff = false, goodNaiTdiff = false, goodVanTdiff = false;
    vector<Double_t> goodClovEn = {}, goodHagEn = {}, goodNaiEn = {};

    for (auto itB = beta.begin(); itB != beta.end(); itB++) {
        if (itB->isLowResBeta) {
            LRbeta = true;
            lowResBetaList.emplace_back(itB->energy, itB->timeAvg);
        } else if (itB->isHighResBeta) {
            HRbeta = true;
            highResBetaList.emplace_back(itB->barQdc, itB->timeAvg);
            ((TH2 *)VandleArray->FindObject("beta_td_q"))->Fill(itB->timeDiff, itB->barQdc);
        }
    }
    ((TH2 *)VandleArray->FindObject("beta_multi"))->Fill(lowResBetaList.size(), 0);
    ((TH2 *)VandleArray->FindObject("beta_multi"))->Fill(highResBetaList.size(), 1);

    //clover only
    for (auto itC = clover.begin(); itC != clover.end(); itC++) {
        Double_t cEnergy_ = itC->energy;
        Double_t cRawEnergy_ = itC->rawEnergy;
        Double_t cTime_ = itC->time;                     //ns
        Double_t dt = (cTime_ - LastCT) * timeBinning_;  // binned time

        stringstream s, ss, gg;
        s << "g_E_" << itC->detNum;

        ((TH1 *)GammaArray->FindObject(s.str().c_str()))->Fill(cEnergy_);
        ((TH1 *)GammaArray->FindObject("g_e_total"))->Fill(cEnergy_);
        ((TH2 *)GammaArray->FindObject("g_e_dt"))->Fill(cEnergy_, dt);
        ((TH2 *)GammaArray->FindObject("g_e_cycle"))->Fill(cEnergy_, currentCycleNum);

        if (LRbeta) {
            ((TH1 *)GammaArray->FindObject("g_e_BG"))->Fill(cEnergy_);
            for (auto itCB = lowResBetaList.begin(); itCB != lowResBetaList.end(); itCB++) {
                Double_t cbdt = itC->time - itCB->second;
                if (cbdt + BinShift >= 0) {
                    if (valid_CBdT->IsInside(cEnergy_, (cbdt + BinShift))) {
                        ((TH2 *)GammaArray->FindObject("g_e_dt_cut"))->Fill(cEnergy_, dt);
                        ((TH2 *)GammaArray->FindObject("g_e_b_dt_cut"))->Fill(cEnergy_, (cbdt + BinShift) / gbdtBin);  //replot with IsInside to check that it works
                        goodCloverTdiff = true;
                        goodClovEn.emplace_back(cEnergy_);
                    }
                    ((TH2 *)GammaArray->FindObject("g_e_b_dt"))->Fill(cEnergy_, (cbdt + BinShift) / gbdtBin);

                    ((THnSparse *)GammaArray->FindObject("g_e_bdt_dt"))->Fill(new Double_t[3]{cEnergy_, (cbdt + BinShift) / gbdtBin, dt});
                    // for (auto ind = 0; ind < maxProj_; ind++) {
                    //     stringstream ss, vss;
                    //     ss.str("");
                    //     ss << "g_e_b_dt_" << ind + 1;
                    //     if (dt > subBinning * ind && dt <= subBinning * (ind + 1)) {
                    //         ((TH2 *)GammaArray->FindObject(ss.str().c_str()))->Fill(cEnergy_, (cbdt + BinShift) / gbdtBin);
                    //     }
                    // }
                    break;
                }
            }
        }

        // stringstream v;
        // v << "g_re_dt_" << itC->detNum;
        // ((TH2 *)GammaArray->FindObject(v.str().c_str()))->Fill(cRawEnergy_, dt);
        // v.str("");
        // v << "g_e_dt_" << itC->detNum;
        // ((TH2 *)GammaArray->FindObject(v.str().c_str()))->Fill(cEnergy_, dt);
    }

    //hag and nai

    for (auto itGS = gamscint.begin(); itGS != gamscint.end(); itGS++) {
        Double_t GSEnergy_ = itGS->energy;
        Double_t GSRawEnergy_ = itGS->rawEnergy;
        Double_t GSTime_ = itGS->time;  //ns
        Int_t GSNum_ = itGS->detNum;
        Double_t dt = (GSTime_ - LastCT) * timeBinning_;  // binned time
        string type = itGS->subtype.Data();
        string sst = itGS->group.Data();
        string shortType;
        stringstream S, SET, SEDT, SEC, SS, SEDTS, HREDT;
        Bool_t hagrid = false, nai = false;
        if (type == "nai" && itGS->detNum < 4) {
            continue;
        }
        if (type == "smallhag") {
            shortType = "h";
            hagrid = true;
        } else if (type == "nai") {
            shortType = "n";
            nai = true;
        } else {
            cout << "UNKNOWN TYPE IN GAMMASCINT LOOP" << endl;
            continue;
        }

        S << shortType << "_E_" << itGS->detNum;
        SET << shortType << "_e_total";
        SEDT << shortType << "_e_dt";
        SEC << shortType << "_e_cycle";

        ((TH1 *)GammaArray->FindObject(S.str().c_str()))->Fill(GSEnergy_);                     //Cal En: Single Det
        ((TH1 *)GammaArray->FindObject(SET.str().c_str()))->Fill(GSEnergy_);                   //Cal En: type totals
        ((TH2 *)GammaArray->FindObject(SEDT.str().c_str()))->Fill(GSEnergy_, dt);              //Cal En: type total vs dt
        ((TH2 *)GammaArray->FindObject(SEC.str().c_str()))->Fill(GSEnergy_, currentCycleNum);  //Cal En: type total vs cycle num
                                                                                               //h_e_bdt_dt
        if (HRbeta) {
            if (hagrid) {
                for (auto itScB = highResBetaList.begin(); itScB != highResBetaList.end(); itScB++) {
                    Double_t alignmentOffset = 0;
                    if (hagrid && GSNum_ >= 8) {
                        alignmentOffset = 0;
                    } else if (hagrid && GSNum_ < 8) {
                        alignmentOffset = -200;
                    }
                    Double_t ScBdt = (GSTime_ + alignmentOffset) - itScB->second;
                    if (ScBdt + Valid_HagHRBetaTdiff >= 0) {
                        ((TH2 *)GammaArray->FindObject("h_e_hrb_dt"))->Fill(GSEnergy_, (ScBdt + Valid_HagHRBetaTdiff));
                        ((THnSparse *)GammaArray->FindObject("h_e_hrbdt_dt"))->Fill(new Double_t[3]{GSEnergy_, (ScBdt + Valid_HagHRBetaTdiff), dt});
                    }
                }
            }
        }
        if (LRbeta) {
            stringstream sbdt, sbg;
            sbg << shortType << "_e_BG";
            sbdt << shortType << "_e_b_dt";
            ((TH1 *)GammaArray->FindObject(sbg.str().c_str()))->Fill(GSEnergy_);

            //--------------------------------------------
            for (auto itScB = lowResBetaList.begin(); itScB != lowResBetaList.end(); itScB++) {
                Double_t alignmentOffset = 0;
                if (hagrid && GSNum_ >= 8) {
                    alignmentOffset = 0;
                } else if (hagrid && GSNum_ < 8) {
                    alignmentOffset = -200;
                } else if (nai) {
                    alignmentOffset = 0;
                }
                Double_t ScBdt = (GSTime_ + alignmentOffset) - itScB->second;
                if (ScBdt + BinShift >= 0) {
                    if (hagrid && (ScBdt + BinShift) <= Valid_HagLRBetaTdiff) {
                        goodHagTdiff = true;
                        goodHagEn.emplace_back(GSEnergy_);
                    }
                    if (nai && (ScBdt + BinShift) <= Valid_NaiBetaTdiff) {
                        goodNaiTdiff = true;
                        goodNaiEn.emplace_back(GSEnergy_);
                    }

                    //scint vs beta tdif
                    ((TH2 *)GammaArray->FindObject(sbdt.str().c_str()))->Fill(GSEnergy_, (ScBdt + BinShift) / gbdtBin);

                    string tmp = shortType + "_e_bdt_dt";
                    ((THnSparse *)GammaArray->FindObject(tmp.c_str()))->Fill(new Double_t[3]{GSEnergy_, (ScBdt + BinShift) / gbdtBin, dt});

                    // scint num vs beta tdiff
                    string numDT = shortType + "_num_b_dt";
                    ((TH2 *)GammaArray->FindObject(numDT.c_str()))->Fill(GSNum_, (ScBdt + BinShift) / gbdtBin);

                    break;
                }
            }
        }
    }

    //vandle

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

        if (abs(tdiff) <= Valid_VandleTdiff) {
            goodVanTdiff = true;
            ((THnSparse *)VandleArray->FindObject("v_t_q_dt"))->Fill(new Double_t[3]{tof, qdc, curTime});
            ((THnSparse *)VandleArray->FindObject("v_t_bn_dt"))->Fill(new Double_t[3]{tof, barnum, curTime});
        }

        if (goodVanTdiff) {
            for (auto it = goodClovEn.begin(); it != goodClovEn.end(); it++) {
                ((TH2D *)VandleArray->FindObject("g_tof"))->Fill((*it), itV->tof);
                ((THnSparse *)VandleArray->FindObject("g_t_q_dt"))->Fill(new Double_t[4]{(*it), tof, qdc, curTime});
            }
            for (auto it = goodHagEn.begin(); it != goodHagEn.end(); it++) {
                ((TH2D *)VandleArray->FindObject("h_tof"))->Fill((*it), itV->tof);
            }
            for (auto it = goodNaiEn.begin(); it != goodNaiEn.end(); it++) {
                ((TH2D *)VandleArray->FindObject("n_tof"))->Fill((*it), itV->tof);
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
    gApplication->Terminate();
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
    while (TH3 *hist = (TH3 *)next()) {
        GetOutputList()->Add(hist);
    }
    while (THnSparse *hist = (THnSparse *)next()) {
        GetOutputList()->Add(hist);
    }
}