#define New97Rb_cxx
// root> T->Process("New97Rb.C")
// root> T->Process("New97Rb.C","some options")
// root> T->Process("New97Rb.C+")
//

#include <TStyle.h>
#include "New97Rb.h"

#define VERBOSE true;

const Bool_t VERBOSE_ = VERBOSE;
void New97Rb::Begin(TTree * /*tree*/) {
    TString option = GetOption();
}
void New97Rb::Init(TTree *tree) {
    fReader.SetTree(tree);
    entTotal_ = fReader.GetEntries(kTRUE);  //! Get Total Entry count
    isChain = fReader.IsChain();            //! Am I a TChain or not?
}
Bool_t New97Rb::Notify() {
    if (isChain && cnt != 0) {
        cout << "Starting new tree in chain" << endl;
        fcnt++;
    }
    if (GetAbort()) {
        Abort("Aborting Check Earlier error", kAbortFile);
    }
    return kTRUE;
}
void New97Rb::SlaveBegin(TTree * /*tree*/) {
    TString option = GetOption();
    // Tape Cycle Gates
    beamOn_Gate_ = {1, 300};
    beamOff_Gate_ = {350, 650};

    //Bins for Gammas (kev)
    Int_t TB_Bins_ = 10000;
    Int_t RawEnergyBins_ = 32000;
    Int_t CloverBins_ = 6000;
    Int_t HagBins_ = 8000;
    Int_t NaiBins_ = 8000;

    //Vandle Plot bins
    Int_t VTofBins_ = 1500;
    Int_t VQdcBins_ = 20000;
    Int_t binNs_ = 2;

    //Curve timing
    Int_t TimeBins_ = 8000;
    Int_t msPerBin_ = 10;
    Int_t DT_bins_ = 2000;
    timeBinning_ = msConvert / msPerBin_;  //ns->desired time binning (1e-6 == 1 ms bins)
    totalCycleTimeBins_ = TimeBins_ / msPerBin_;

    //!-------------------------------------------------------------------------------
    //! vari bin maker
    //!-------------------------------------------------------------------------------

    Int_t tofRange_ = 155;
    Double_t TBins[155] = {};
    Double_t Tof = 10;
    for (int i = 0; i < tofRange_; i++) {
        Double_t HalfSigma = 0.000243432 * pow(Tof, 2) - 0.011635843 * (Tof) + 0.736628549;
        TBins[i] = (Tof) + HalfSigma;
        Tof += HalfSigma;
    }

    HistList = new TObjArray();

    //!-------------------------------------------------------------------------------
    //! 2D gamma beta dt plots
    //!-------------------------------------------------------------------------------
    HistList->Add(new TH2F("dd_h_e_hrbdt", "LaBr Energy vs LaBr- high res Beta dt", HagBins_, 0, HagBins_, DT_bins_, -1 * DT_bins_, DT_bins_));
    HistList->Add(new TH2F("dd_h_det_hrbdt", "LaBr- high res Beta dt vs det num", 2 * DT_bins_, -1 * DT_bins_, DT_bins_, 20, 0, 20));

    HistList->Add(new TH2F("dd_h_e_lrbdt", "LaBr Energy vs LaBr- low res Beta dt", HagBins_, 0, HagBins_, DT_bins_, -1 * DT_bins_, DT_bins_));
    HistList->Add(new TH2F("dd_h_det_lrbdt", "LaBr- low res Beta dt vs det num", 2 * DT_bins_, -1 * DT_bins_, DT_bins_, 20, 0, 20));

    HistList->Add(new TH2F("dd_n_e_lrbdt", "nai Energy vs nai- low res Beta dt", NaiBins_, 0, NaiBins_, DT_bins_, -1 * DT_bins_, DT_bins_));
    HistList->Add(new TH2F("dd_n_det_lrbdt", "nai- low res Beta dt vs det num", 2 * DT_bins_, -1 * DT_bins_, DT_bins_, 20, 0, 20));

    HistList->Add(new TH2F("dd_c_e_lrbdt", "Clover Energy vs Clover - low res Beta dt", CloverBins_, 0, CloverBins_, DT_bins_, -1 * DT_bins_, DT_bins_));
    HistList->Add(new TH2F("dd_c_det_lrbdt", "Clover time - low res Beta dt vs det num", 2 * DT_bins_, -1 * DT_bins_, DT_bins_, 5, 0, 5));

    //!-------------------------------------------------------------------------------
    //! ADDBACK/TASBACK plots
    //!-------------------------------------------------------------------------------

    HistList->Add(new TH1I("d_h_TB", "TASBACK for HAGRiD", TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH1I("d_n_TB", "TASBACK for NaI", TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH1I("d_c_TB", "TASBACK for Clover", TB_Bins_, 0, TB_Bins_));

    HistList->Add(new TH1I("d_h_BG", "BetaDT gated HAGRiD", TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH1I("d_n_BG", "BetaDT gated NaI", TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH1I("d_c_BG", "BetaDT gated Clover", TB_Bins_, 0, TB_Bins_));

    HistList->Add(new TH1I("d_tas", "Pixie Event TASback", TB_Bins_, 0, TB_Bins_));

    HistList->Add(new TH2F("dd_h_tas", "Hagrid in beta DT vs Pixie TAS", HagBins_, 0, HagBins_, TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH2F("dd_n_tas", "Nai in beta DT vs Pixie TAS", NaiBins_, 0, NaiBins_, TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH2F("dd_c_tas", "Clover in beta DT vs Pixie TAS", CloverBins_, 0, CloverBins_, TB_Bins_, 0, TB_Bins_));

    //!-------------------------------------------------------------------------------
    //! VANDLE plots
    //!-------------------------------------------------------------------------------
    TQ_BeamOn_key = "dd_tof_qdc_" + to_string((Int_t)beamOn_Gate_.first) + "_" + to_string((Int_t)beamOn_Gate_.second);
    TQ_BeamOff_key = "dd_tof_qdc_" + to_string((Int_t)beamOff_Gate_.first) + "_" + to_string((Int_t)beamOff_Gate_.second);

    HistList->Add(new TH2F("dd_qdc_tof_flash", "ToF vs QDC: Gamma Flash", 1000, 0, 50, VQdcBins_, 0, VQdcBins_));  //50ps bins

    //! Tof vs qdc vs Tape Timing in a Dim == 4 Sparse like the others so that it can be sliced with the same code
    HistList->Add(new THnSparseF("dd_tof_NA_qdc_tape", "Vandle corTof vs Vandle QDC vs empty Gamma  vs Time in Cycle", 4, new Int_t[4]{VTofBins_ * binNs_, VQdcBins_, 0, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(VTofBins_), static_cast<Double_t>(VQdcBins_), static_cast<Double_t>(0), static_cast<Double_t>(totalCycleTimeBins_)}));

    //! tof vs HAG energy vs Van qdc vs tape
    HistList->Add(new THnSparseF("1", "Vandle corTof vs HAGRiD Energy vs Vandle QDC vs Time in Cycle", 4, new Int_t[4]{VTofBins_ * binNs_, HagBins_, VQdcBins_, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(VTofBins_), static_cast<Double_t>(HagBins_), static_cast<Double_t>(VQdcBins_), static_cast<Double_t>(totalCycleTimeBins_)}));

    //! tof vs Nai energy vs Van qdc vs tape
    HistList->Add(new THnSparseF("dd_tof_n_qdc_tape", "Vandle corTof vs Nai Energy vs Vandle QDC vs Time in Cycle", 4, new Int_t[4]{VTofBins_ * binNs_, NaiBins_, VQdcBins_, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(VTofBins_), static_cast<Double_t>(NaiBins_), static_cast<Double_t>(VQdcBins_), static_cast<Double_t>(totalCycleTimeBins_)}));

    //! tof vs Clover energy vs Van qdc vs tape
    HistList->Add(new THnSparseF("dd_tof_c_qdc_tape", "Vandle corTof vs Clover Energy vs Vandle QDC vs Time in Cycle", 4, new Int_t[4]{VTofBins_ * binNs_, CloverBins_, VQdcBins_, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(VTofBins_), static_cast<Double_t>(CloverBins_), static_cast<Double_t>(VQdcBins_), static_cast<Double_t>(totalCycleTimeBins_)}));

    //! tof vs Pixie Event energy vs Van qdc vs tape
    HistList->Add(new THnSparseF("dd_tof_T_qdc_tape", "Vandle corTof vs Pixie Event Energy vs Vandle QDC vs Time in Cycle", 4, new Int_t[4]{VTofBins_ * binNs_, TB_Bins_, VQdcBins_, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(VTofBins_), static_cast<Double_t>(TB_Bins_), static_cast<Double_t>(VQdcBins_), static_cast<Double_t>(totalCycleTimeBins_)}));

    //! tof vs any gamma detector energy vs Van qdc vs tape
    HistList->Add(new THnSparseF("dd_tof_G_qdc_tape", "Vandle corTof vs All Gamma Dets Energy vs Vandle QDC vs Time in Cycle", 4, new Int_t[4]{VTofBins_ * binNs_, TB_Bins_, VQdcBins_, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(VTofBins_), static_cast<Double_t>(TB_Bins_), static_cast<Double_t>(VQdcBins_), static_cast<Double_t>(totalCycleTimeBins_)}));

    //! Hagrid vs Pixie Event energy vs 0 vs tape
    HistList->Add(new THnSparseF("dd_h_TAS_0_tape", "Hagrid vs Pixie Event energy vs EMPTY() vs Time in Cycle", 4, new Int_t[4]{HagBins_, TB_Bins_, 0, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(HagBins_), static_cast<Double_t>(TB_Bins_), static_cast<Double_t>(0), static_cast<Double_t>(totalCycleTimeBins_)}));

    //! Nai vs Pixie Event energy vs 0 vs tape
    HistList->Add(new THnSparseF("dd_n_TAS_0_tape", "Nai vs Pixie Event energy vs EMPTY() vs Time in Cycle", 4, new Int_t[4]{NaiBins_, TB_Bins_, 0, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(NaiBins_), static_cast<Double_t>(TB_Bins_), static_cast<Double_t>(0), static_cast<Double_t>(totalCycleTimeBins_)}));

    //! Clover vs Pixie Event energy vs 0 vs tape
    HistList->Add(new THnSparseF("dd_c_TAS_0_tape", "Clover vs Pixie Event energy vs EMPTY() vs Time in Cycle", 4, new Int_t[4]{CloverBins_, TB_Bins_, 0, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(CloverBins_), static_cast<Double_t>(TB_Bins_), static_cast<Double_t>(0), static_cast<Double_t>(totalCycleTimeBins_)}));

    //! Special tof vs clover/TAS debuggers
    HistList->Add(new TH2F("dd_tof_CLOVER_BON", "Tof vs CLOVER E beam on (no qdc cut)", VTofBins_, 0, VTofBins_, CloverBins_, 0, CloverBins_));
    HistList->Add(new TH2F("dd_tof_CLOVER_BOFF", "Tof vs CLOVER E beam off (no qdc cut)", VTofBins_, 0, VTofBins_, CloverBins_, 0, CloverBins_));
    HistList->Add(new TH2F("dd_tof_CLOVER_NOB", "Tof vs CLOVER E (no qdc cut or tape cut)", VTofBins_, 0, VTofBins_, CloverBins_, 0, CloverBins_));
    HistList->Add(new TH2F("dd_tof_TB_NOB", "Tof vs Pixie E (no qdc cut or tape cut)", VTofBins_, 0, VTofBins_, TB_Bins_, 0, TB_Bins_));

    HistList->Add(new TH2F("dd_h_ftof", "FUZZY Vandle corTof vs HAGRiD Energy ", TB_Bins_, 0, TB_Bins_, VTofBins_, 0, VTofBins_));

    HistList->Add(new TH2F("dd_cortof_ftof", "Vandle corTof vs FUZZY tof", VTofBins_, 0, VTofBins_, VTofBins_, 0, VTofBins_));
    HistList->Add(new TH2F("dd_cortof_qdc", "Vandle corTof vs qdc", VTofBins_, 0, VTofBins_, VQdcBins_, 0, VQdcBins_));
    HistList->Add(new TH2F("dd_ftof_qdc", "Vandle Fuzzy Tof vs qdc", VTofBins_, 0, VTofBins_, VQdcBins_, 0, VQdcBins_));

    HistList->Add(new TH2F("dd_h_ftof_var", "Hagrid Energy vs Vandle vari Tof gated on Bon, Vqdc>120", HagBins_, 0, HagBins_, tofRange_ - 1, TBins));

    HistList->Add(new TH2F("dd_h_tof", "Hagrid Energy vs Vandl corTof gated on Bon, Vqdc>120", HagBins_, 0, HagBins_, VTofBins_, 0, VTofBins_));

    //!-------------------------------------------------------------------------------------------
    //! fuzzy tof vs HAG energy vs Van qdc vs tape
    HistList->Add(new THnSparseF("dd_ftof_h_qdc_tape", "FUZZY Vandle corTof vs HAGRiD Energy vs Vandle QDC vs Time in Cycle", 4, new Int_t[4]{VTofBins_ * binNs_, HagBins_, VQdcBins_, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(VTofBins_), static_cast<Double_t>(HagBins_), static_cast<Double_t>(VQdcBins_), static_cast<Double_t>(totalCycleTimeBins_)}));

    HistList->Add(new THnSparseF("dd_ftof_c_qdc_tape", "FUZZY Vandle corTof vs Clover Energy vs Vandle QDC vs Time in Cycle", 4, new Int_t[4]{VTofBins_ * binNs_, HagBins_, VQdcBins_, totalCycleTimeBins_}, new Double_t[4]{0, 0, 0}, new Double_t[4]{static_cast<Double_t>(VTofBins_), static_cast<Double_t>(HagBins_), static_cast<Double_t>(VQdcBins_), static_cast<Double_t>(totalCycleTimeBins_)}));

    AddToOutputList(HistList);

    //! Initalize addback stuff
    BuildTASParaMap();  // Build the Parameter "db" values for Thresh,subevent window are set in this function

    H_TASBACK_ = BuildTASbackMaps({"shg0", "shg1", "shg2", "shg3"}, "smallhag");
    N_TASBACK_ = BuildTASbackMaps({"naiL1", "naiTop"}, "nai");
    C_TASBACK_ = BuildTASbackMaps({"clover0"}, "clover");

    //! Load my various VANDLE TCUTS & the VANDLE TDIFF
    LoadTCuts();
    Valid_VandleTdiff_ = 15;  // 1/2 the width of the TDiff spectra since its abs(tdiff)

    Fuzzer = new TRandom();

    //!Initalize progress counters event count (cnt) and file count (fcnt)
    cnt = 0;
    fcnt = 1;
    cout << "Slave Begin() complete" << endl;
}

Bool_t New97Rb::Process(Long64_t entry) {
    fReader.SetLocalEntry(entry);

    Double_t lastCycleStart_ = logic.At(0).lastTapeCycleStartTime;  // ns
    Int_t currentCycleNum_ = logic.At(0).cycleNum;

    // Reset the TASBACK
    ResetTASback();

    //individual hagrid multiplicities
    vector<Int_t> hagMulti(20, 0);
    vector<Int_t> hagMultiBG(20, 0);

    // Pixie Event TAS
    Double_t PixieTAS_ = 0, PixieTAS_BDT_ = 0;
    vector<Double_t> h_tas_singles_, n_tas_singles_, c_tas_singles_, h_good_singles_, n_good_singles_, c_good_singles_;

    //!-------------------------------------------------------------------------------
    //! DO BETA STUFF
    //!-------------------------------------------------------------------------------
    Bool_t LRbeta = false, HRbeta = false;
    vector<pair<Double_t, Double_t>> lowResBetaList, highResBetaList;
    for (auto itB = beta.begin(); itB != beta.end(); itB++) {
        if (itB->isLowResBeta) {
            LRbeta = true;
            lowResBetaList.emplace_back(itB->energy, itB->timeAvg);
        } else if (itB->isHighResBeta) {
            HRbeta = true;
            highResBetaList.emplace_back(itB->barQdc, itB->timeAvg);
        }
    }

    //!-------------------------------------------------------------------------------
    //!DO GAMMASCINT STUFF
    //!-------------------------------------------------------------------------------
    for (auto itGam = gamscint.begin(); itGam != gamscint.end(); itGam++) {
        //! Get things from the stuct
        Double_t Energy_ = itGam->energy;
        Double_t Time_ = itGam->time;  //ns
        Int_t Num_ = itGam->detNum;
        // Double_t dt = (Time_ - LastCT) * timeBinning_;  // binned time
        string Type_ = itGam->subtype.Data();
        string Group_ = itGam->group.Data();

        //! Init bools
        Bool_t hagrid = false, nai = false;
        Bool_t inBetaGammaTCut = false, plotTAS_ = false;

        //! Skip the bottom layer of NaI (no E cal or Eff)
        if (Type_ == "nai" && itGam->detNum < 4) {
            continue;
        }

        //! Set "what am i" bools and TAS vector pointer
        vector<Double_t> *tas_singles_vec_, *good_singles_vec_;
        if (Type_ == "smallhag") {
            hagrid = true;
            tas_singles_vec_ = &h_tas_singles_;
            good_singles_vec_ = &h_good_singles_;
        } else if (Type_ == "nai") {
            nai = true;
            tas_singles_vec_ = &n_tas_singles_;
            good_singles_vec_ = &n_good_singles_;
        } else {
            cout << "Unknown GamScintType" << endl;
            continue;
        }

        if (hagrid) {
            hagMulti.at(Num_) += 1;
            if (HRbeta) {
                for (auto itHRBlist = highResBetaList.begin(); itHRBlist != highResBetaList.end(); itHRBlist++) {
                    Double_t alignmentOffset = hagHRBdToffsets.at(Num_);
                    Double_t ScBdt = (Time_)-itHRBlist->second + alignmentOffset;
                    ((TH1 *)HistList->FindObject("dd_h_det_hrbdt"))->Fill(ScBdt, Num_);
                    ((TH2 *)HistList->FindObject("dd_h_e_hrbdt"))->Fill(Energy_, ScBdt);
                    if (betaHagHrtDT->IsInside(Energy_, ScBdt)) {
                        inBetaGammaTCut = true;
                        string hisKey = "d_" + (typeAbrev(Type_)).at(0) + "_BG";
                        ((TH1 *)HistList->FindObject(hisKey.c_str()))->Fill(Energy_); 
                    }
                }
            }
            if (LRbeta) {
                for (auto itLRBlist = lowResBetaList.begin(); itLRBlist != lowResBetaList.end(); itLRBlist++) {
                    Double_t alignmentOffset = hagHRBdToffsets.at(Num_) - hagridExternDelayOffset;  // 200 is the difference in ns between the ExternDelayLen of Mod 0 and Mod 9 (trig vs singles betas)
                    Double_t ScBdt = Time_ - itLRBlist->second + alignmentOffset;

                    ((TH2 *)HistList->FindObject("dd_h_det_lrbdt"))->Fill(ScBdt, Num_);
                    ((TH2 *)HistList->FindObject("dd_h_e_lrbdt"))->Fill(Energy_, ScBdt);

                    if (betaHagLrtDT->IsInside(Energy_, ScBdt)) {
                        inBetaGammaTCut = true;
                    }
                }
            }
        } else if (nai) {
            if (LRbeta) {
                for (auto itLRBlist = lowResBetaList.begin(); itLRBlist != lowResBetaList.end(); itLRBlist++) {
                    // Double_t alignmentOffset = hagDToffsets.at(Num_); //mostly aligned due to them being on the same mod as the singles betas
                    Double_t ScBdt = Time_ - itLRBlist->second;
                    ((TH1 *)HistList->FindObject("dd_n_det_lrbdt"))->Fill(ScBdt, Num_);
                    ((TH2 *)HistList->FindObject("dd_n_e_lrbdt"))->Fill(Energy_, ScBdt);
                    if (betaNaiDT->IsInside(Energy_, ScBdt)) {
                        inBetaGammaTCut = true;
                        string hisKey = "d_" + (typeAbrev(Type_)).at(0) + "_BG";
                        ((TH1 *)HistList->FindObject(hisKey.c_str()))->Fill(Energy_);
                    }
                }
            }
        }

        // if we are in the beta gamma tcut then add to the right "good" singles TCUT
        if (inBetaGammaTCut) {
            good_singles_vec_->emplace_back(Energy_);
            PixieTAS_BDT_ += Energy_;
        }

        //! Add the energy to the event total (cant beta tcut gate it, but a normal beta gate might work, but need to gate the tasback functions as well )
        PixieTAS_ += Energy_;

        // DO THE TAS back
        plotTAS_ = DoTASback(Type_, Group_, Energy_, Time_, tas_singles_vec_, false);

        //If we are out of the sub event window plot stuff but we need to run DoTASback again to catch THIS event since its the begining of a new TASback
        if (plotTAS_) {
            string hisKey = "d_" + (typeAbrev(Type_)).at(0) + "_TB";

            Double_t TB_Energy_ = GetTASback(Type_, Group_)->second.energy;
            ((TH1 *)HistList->FindObject(hisKey.c_str()))->Fill(TB_Energy_);
            // add the summed energy to the pixie event total

            DoTASback(Type_, Group_, Energy_, Time_, tas_singles_vec_, true);  // reset the tasback before math if outside to catch this detector event as the first in a new struct
        }
    }

    //!-------------------------------------------------------------------------------
    //!DO COLVER STUFF
    //!-------------------------------------------------------------------------------
    vector<Double_t> temp_singles_vec_;
    for (auto itClo = clover.begin(); itClo != clover.end(); itClo++) {
        //! Get things from the stuct
        Double_t Energy_ = itClo->energy;
        Double_t Time_ = itClo->time;  //ns
        Int_t Num_ = itClo->detNum;
        string Type_ = "clover";
        string Group_ = Type_ + to_string(itClo->cloverNum);

        //! these are unnecessary but they make the clover and gamma scint loops look the same which makes it easier to read
        vector<Double_t> *tas_singles_vec_, *good_singles_vec_;
        tas_singles_vec_ = &c_tas_singles_;
        good_singles_vec_ = &c_good_singles_;

        //! Init the bools
        Bool_t inBetaGammaTCut = false, plotTAS_ = false;

        if (LRbeta) {
            for (auto itLRBlist = lowResBetaList.begin(); itLRBlist != lowResBetaList.end(); itLRBlist++) {
                Double_t ScBdt = Time_ - itLRBlist->second;

                ((TH2 *)HistList->FindObject("dd_c_det_lrbdt"))->Fill(ScBdt, Num_);
                ((TH2 *)HistList->FindObject("dd_c_e_lrbdt"))->Fill(Energy_, ScBdt);

                if (betaCloverDT->IsInside(Energy_, ScBdt)) {
                    inBetaGammaTCut = true;
                    string hisKey = "d_" + (typeAbrev(Type_)).at(0) + "_BG";
                    ((TH1 *)HistList->FindObject(hisKey.c_str()))->Fill(Energy_);
                }
            }
        }

        // if we are in the beta gamma tcut then add to the right "good" singles TCUT
        if (inBetaGammaTCut) {
            good_singles_vec_->emplace_back(Energy_);
            PixieTAS_BDT_ += Energy_;
        }

        //! Add the energy to the event total (cant beta tcut gate it, but a normal beta gate might work, but need to gate the tasback functions as well )
        PixieTAS_ += Energy_;  // add the summed energy to the pixie event total
        temp_singles_vec_.emplace_back(Energy_);
        //Do the TAS BACK
        plotTAS_ = DoTASback(Type_, Group_, Energy_, Time_, tas_singles_vec_, false);

        //If we are out of the sub event window plot stuff but we need to run DoTASback again to catch THIS event since its the begining of a new TASback
        if (plotTAS_) {
            string hisKey = "d_" + (typeAbrev(Type_)).at(0) + "_TB";
            Double_t TB_Energy_ = GetTASback(Type_, Group_)->second.energy;
            ((TH1 *)HistList->FindObject(hisKey.c_str()))->Fill(TB_Energy_);

            DoTASback(Type_, Group_, Energy_, Time_, tas_singles_vec_, true);  // reset the tasback before math if outside to catch this detector event as the first in a new struct
        }
    }

    //!-------------------------------------------------------------------------------
    //!Catch SINGLE event per group TAS backs
    //!-------------------------------------------------------------------------------
    for (auto it = H_TASBACK_.begin(); it != H_TASBACK_.end(); it++) {
        if (it->second.multiplicity == 1) {
            Double_t TB_Energy_ = it->second.energy;
            ((TH1 *)HistList->FindObject("d_h_TB"))->Fill(TB_Energy_);
        }
    }
    for (auto it = N_TASBACK_.begin(); it != N_TASBACK_.end(); it++) {
        if (it->second.multiplicity == 1) {
            Double_t TB_Energy_ = it->second.energy;
            ((TH1 *)HistList->FindObject("d_n_TB"))->Fill(TB_Energy_);
        }
    }
    for (auto it = C_TASBACK_.begin(); it != C_TASBACK_.end(); it++) {
        if (it->second.multiplicity == 1) {
            Double_t TB_Energy_ = it->second.energy;
            ((TH1 *)HistList->FindObject("d_c_TB"))->Fill(TB_Energy_);
        }
    }

    //!Fill the pixie event totals and such
    ((TH1 *)HistList->FindObject("d_tas"))->Fill(PixieTAS_);

    //! Use the first HR or LR beta for the plcae in cycle typing. Low enough time binning that this isnt an issue. Also *_good_singles_ and PixieTAS_BDT_ are both already gated by the inBetaGammaTCut Bool
    Double_t TasTapeTime_ = -999;
    if (HRbeta) {
        TasTapeTime_ = (highResBetaList.front().second - lastCycleStart_) * timeBinning_;
    } else if (LRbeta) {
        TasTapeTime_ = (lowResBetaList.front().second - lastCycleStart_) * timeBinning_;
    }
    for (auto it = h_good_singles_.begin(); it != h_good_singles_.end(); it++) {
        ((TH2 *)HistList->FindObject("dd_h_tas"))->Fill((*it), PixieTAS_BDT_);
        ((THnSparse *)HistList->FindObject("dd_h_TAS_0_tape"))->Fill(new Double_t[4]{(*it), PixieTAS_BDT_, 0, TasTapeTime_});
    }
    for (auto it = n_good_singles_.begin(); it != n_good_singles_.end(); it++) {
        ((TH2 *)HistList->FindObject("dd_n_tas"))->Fill((*it), PixieTAS_BDT_);
        ((THnSparse *)HistList->FindObject("dd_n_TAS_0_tape"))->Fill(new Double_t[4]{(*it), PixieTAS_BDT_, 0, TasTapeTime_});
    }
    for (auto it = c_good_singles_.begin(); it != c_good_singles_.end(); it++) {
        ((TH2 *)HistList->FindObject("dd_c_tas"))->Fill((*it), PixieTAS_BDT_);
        ((THnSparse *)HistList->FindObject("dd_c_TAS_0_tape"))->Fill(new Double_t[4]{(*it), PixieTAS_BDT_, 0, TasTapeTime_});
    }

    //!-------------------------------------------------------------------------------
    //!DO VANDLE STUFF
    //!-------------------------------------------------------------------------------

    for (auto itVan = vandle.begin(); itVan != vandle.end(); itVan++) {
        Double_t tof_ = itVan->tof;
        Double_t cortof_ = itVan->corTof;
        Double_t qdc_ = itVan->qdc;
        Double_t tdiff_ = itVan->tdiff;
        Double_t barnum_ = itVan->barNum;
        Double_t tapeTime_ = (itVan->sTime - lastCycleStart_) * timeBinning_;  //10 ms bins
        Bool_t BeamOn_ = false, BeamOff_ = false;

        // throw out bar 18 and events outside of the good TDIFF
        if (barnum_ == 18 || abs(tdiff_) > Valid_VandleTdiff_) {
            continue;
        }

        // Tof Vs QDC with beam gates
        if (tapeTime_ >= beamOn_Gate_.first && tapeTime_ <= beamOn_Gate_.second) {
            // ((TH2 *)HistList->FindObject(TQ_BeamOn_key.c_str()))->Fill(cortof_, qdc_);
            BeamOn_ = true;
        } else if (tapeTime_ >= beamOff_Gate_.first && tapeTime_ <= beamOff_Gate_.second) {
            // ((TH2 *)HistList->FindObject(TQ_BeamOff_key.c_str()))->Fill(cortof_, qdc_);
            BeamOff_ = true;
        }

        //ONLY plot into the high binned one if we are near the gamma flash
        if (cortof_ < 50 && cortof_ > 0) {
            ((TH2 *)HistList->FindObject("dd_qdc_tof_flash"))->Fill(cortof_, qdc_);
        }

        string BeamState_;
        if (BeamOn_) {
            BeamState_ = "_Bon";
        } else if (BeamOff_) {
            BeamState_ = "_Boff";
        }
        Double_t fuzCortof_;
        if (cortof_ > 25) {
            fuzCortof_ = FuzzTheToF(cortof_);
        } else {
            fuzCortof_ = -999;
        }

        if (BeamOn_) {
            ((TH2 *)HistList->FindObject("dd_cortof_ftof"))->Fill(cortof_, fuzCortof_);
            ((TH2 *)HistList->FindObject("dd_cortof_qdc"))->Fill(cortof_, qdc_);
            ((TH2 *)HistList->FindObject("dd_ftof_qdc"))->Fill(fuzCortof_, qdc_);
        }

        //! Tof vs qdc vs Tape Timing in a Dim == 4
        ((THnSparse *)HistList->FindObject("dd_tof_NA_qdc_tape"))->Fill(new Double_t[4]{cortof_, 0, qdc_, tapeTime_});

        //! Diagnostic TAS vs TOF
        ((TH2 *)HistList->FindObject("dd_tof_TB_NOB"))->Fill(cortof_, PixieTAS_);

        //! TOF vs TAS vs Van QDC vs Tape
        ((THnSparse *)HistList->FindObject("dd_tof_T_qdc_tape"))->Fill(new Double_t[4]{cortof_, PixieTAS_, qdc_, tapeTime_});

        // plot tof vs gamma
        //! already has betaGammaTDiff check in the creation of the *_good_singles_ vectors
        // if (BeamOn_ || BeamOff_) {
        if (!h_good_singles_.empty()) {
            for (auto it = h_good_singles_.begin(); it != h_good_singles_.end(); it++) {
                //! cortof vs specific gammas
                ((THnSparse *)HistList->FindObject("dd_tof_h_qdc_tape"))->Fill(new Double_t[4]{cortof_, (*it), qdc_, tapeTime_});

                ((THnSparse *)HistList->FindObject("dd_ftof_h_qdc_tape"))->Fill(new Double_t[4]{fuzCortof_, (*it), qdc_, tapeTime_});

                if (BeamOn_) {
                    ((TH2 *)HistList->FindObject("dd_h_ftof"))->Fill((*it), fuzCortof_);
                    ((TH2 *)HistList->FindObject("dd_h_tof"))->Fill((*it), cortof_);
                    ((TH2 *)HistList->FindObject("dd_h_ftof_var"))->Fill((*it), fuzCortof_);
                }
                //!corTof vs ANY Gamma
                ((THnSparse *)HistList->FindObject("dd_tof_G_qdc_tape"))->Fill(new Double_t[4]{cortof_, (*it), qdc_, tapeTime_});
            }  //end loop over good singles
        }
        if (!n_good_singles_.empty()) {
            for (auto it = n_good_singles_.begin(); it != n_good_singles_.end(); it++) {
                //! cortof vs specific gammas
                ((THnSparse *)HistList->FindObject("dd_tof_n_qdc_tape"))->Fill(new Double_t[4]{cortof_, (*it), qdc_, tapeTime_});

                //!corTof vs ANY Gamma
                ((THnSparse *)HistList->FindObject("dd_tof_G_qdc_tape"))->Fill(new Double_t[4]{cortof_, (*it), qdc_, tapeTime_});
            }  //end loop over good singles
        }
        if (!c_good_singles_.empty()) {
            for (auto it = c_good_singles_.begin(); it != c_good_singles_.end(); it++) {
                //! cortof vs specific gammas
                ((THnSparse *)HistList->FindObject("dd_tof_c_qdc_tape"))->Fill(new Double_t[4]{cortof_, (*it), qdc_, tapeTime_});

                ((THnSparse *)HistList->FindObject("dd_ftof_c_qdc_tape"))->Fill(new Double_t[4]{fuzCortof_, (*it), qdc_, tapeTime_});

                //!corTof vs ANY Gamma
                ((THnSparse *)HistList->FindObject("dd_tof_G_qdc_tape"))->Fill(new Double_t[4]{cortof_, (*it), qdc_, tapeTime_});

                //! Clover gets Special diagnostic TH2s
                ((TH2 *)HistList->FindObject("dd_tof_CLOVER_NOB"))->Fill(cortof_, (*it));
                if (BeamOn_) {
                    ((TH2 *)HistList->FindObject("dd_tof_CLOVER_BON"))->Fill(cortof_, (*it));
                } else if (BeamOff_) {
                    ((TH2 *)HistList->FindObject("dd_tof_CLOVER_BOFF"))->Fill(cortof_, (*it));
                }

            }  //end loop over good singles
        }
        // }  //end BeamOn_ or BeamOff_ check
    }  //end vandle loop

    PrintRunStats();
    return kTRUE;
}

void New97Rb::SlaveTerminate() {
}

void New97Rb::Terminate() {
    TFile *outFile = new TFile("New97Rb_test.root", "RECREATE");

    TIter next(GetOutputList());
    while (TObject *obj = next()) {
        obj->Write();
    }
    outFile->Close();
    gApplication->Terminate();
}

void New97Rb::LoadTCuts() {
    vanBanana = new TCutG("VTQ_fullQ", VandleTCuts::VTQ_fullQ_vec.size());
    for (UInt_t it = 0; it < VandleTCuts::VTQ_fullQ_vec.size(); it++) {
        vanBanana->SetPoint(it, VandleTCuts::VTQ_fullQ_vec.at(it).first, VandleTCuts::VTQ_fullQ_vec.at(it).second);
    }

    betaCloverDT = new TCutG("Clover_LRB_dt", VandleTCuts::CloBetaTDiff_cut.size());
    for (UInt_t it = 0; it < VandleTCuts::CloBetaTDiff_cut.size(); it++) {
        betaCloverDT->SetPoint(it, VandleTCuts::CloBetaTDiff_cut.at(it).first, VandleTCuts::CloBetaTDiff_cut.at(it).second);
    }

    betaHagHrtDT = new TCutG("Hag_HRB_dt", VandleTCuts::newHagHRBdt.size());
    for (UInt_t it = 0; it < VandleTCuts::newHagHRBdt.size(); it++) {
        betaHagHrtDT->SetPoint(it, VandleTCuts::newHagHRBdt.at(it).first, VandleTCuts::newHagHRBdt.at(it).second);
    }

    betaHagLrtDT = new TCutG("Hag_LRB_dt", VandleTCuts::newHagLRBdt.size());
    for (UInt_t it = 0; it < VandleTCuts::newHagLRBdt.size(); it++) {
        betaHagLrtDT->SetPoint(it, VandleTCuts::newHagLRBdt.at(it).first, VandleTCuts::newHagLRBdt.at(it).second);
    }

    betaNaiDT = new TCutG("Nai_LRB_dt", VandleTCuts::newNaiLRBdt.size());
    for (UInt_t it = 0; it < VandleTCuts::newNaiLRBdt.size(); it++) {
        betaNaiDT->SetPoint(it, VandleTCuts::newNaiLRBdt.at(it).first, VandleTCuts::newNaiLRBdt.at(it).second);
    }

    tasCut = new TCutG("TasCut", VandleTCuts::tasCut.size());
    for (UInt_t it = 0; it < VandleTCuts::tasCut.size(); it++) {
        tasCut->SetPoint(it, VandleTCuts::tasCut.at(it).first, VandleTCuts::tasCut.at(it).second);
    }
}

void New97Rb::AddToOutputList(TObjArray *iArray) {
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

void New97Rb::PrintRunStats() {
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
}

map<string, GSAddback> New97Rb::BuildTASbackMaps(vector<string> groups_, string type_) {
    map<string, GSAddback> tmp;
    Double_t curSEW = GetTASbackPara(type_, "subEvtWin");  // NOTE: SEWs must be in ns
    for (auto it = groups_.begin(); it != groups_.end(); it++) {
        tmp.emplace((*it), GSAddback(0., -2.0 * curSEW, 0., 0));
    }
    return (tmp);
}

map<string, GSAddback>::iterator New97Rb::GetTASback(const string &subtype_, const string &group_) {
    map<string, GSAddback> *curTMap;
    if (subtype_ == "nai")
        curTMap = &N_TASBACK_;
    else if (subtype_ == "smallhag")
        curTMap = &H_TASBACK_;
    else if (subtype_ == "clover")
        curTMap = &C_TASBACK_;

    auto ITinner = curTMap->find(group_);
    if (ITinner == curTMap->end()) {
        stringstream ss;
        ss << "Error in looking up TAS-back class for " << subtype_ << ":" << group_;
        Abort(ss.str().c_str(), kAbortFile);
        return curTMap->begin();
    } else {
        return ITinner;
    }
}

Double_t New97Rb::GetTASbackPara(const std::string &subtype, const std::string &option) {
    auto ito = TASParaMap.find(subtype);
    if (ito == TASParaMap.end()) {
        stringstream ss;
        ss << "Error in looking up TAS-back Parameters for " << subtype << " (Unknown Type)";
        Abort(ss.str().c_str(), kAbortFile);
        return numeric_limits<double>::max();
    } else {
        auto iti = ito->second.find(option);
        if (iti == ito->second.end()) {
            stringstream ss;
            ss << "Error in looking up TAS-back Parameter for " << subtype << " (Unknown Parameter)";
            Abort(ss.str().c_str(), kAbortFile);
            return numeric_limits<double>::max();
        } else {
            return iti->second;
        }
    }
}

Bool_t New97Rb::DoTASback(string &subtype_, string &group_, Double_t &energy_, Double_t &time_, vector<Double_t> *singlesVec_, Bool_t reset_) {
    auto curTAS_ = GetTASback(subtype_, group_);
    Double_t TDIFF_ = abs(time_ - curTAS_->second.time);

    //Reset if asked ONLY if already plotted the tasback etc. (i,e, previous call returned true)
    if (reset_) {
        Double_t SeW_init_ = GetTASbackPara(subtype_, "subEvtWin");
        curTAS_->second = GSAddback(0., -2.0 * SeW_init_, 0., 0);
    }

    //only do math if we are still inside of the sub event TAS window
    if (TDIFF_ > GetTASbackPara(subtype_, "subEvtWin") && curTAS_->second.multiplicity != 0) {
        // we are outside of the loop and its not the first one so no math just report for histo filling etc
        return true;
    }
    //If energy less than threshold skip
    if (energy_ < GetTASbackPara(subtype_, "thresh")) {
        return false;
    }
    // if multiplicity 0 set first time
    if (curTAS_->second.multiplicity == 0) {
        curTAS_->second.ftime = time_;
    }
    curTAS_->second.energy += energy_;
    curTAS_->second.multiplicity += 1;
    curTAS_->second.time = time_;

    // add this event to the valid single energy list
    singlesVec_->emplace_back(energy_);

    return false;
}
//Reset TAS-back maps at the begining of the Event
void New97Rb::ResetTASback() {
    Double_t H_SEW = GetTASbackPara("smallhag", "subEvtWin");
    Double_t N_SEW = GetTASbackPara("nai", "subEvtWin");
    Double_t C_SEW = GetTASbackPara("clover", "subEvtWin");
    for (auto it = H_TASBACK_.begin(); it != H_TASBACK_.end(); it++) {
        it->second = GSAddback(0., -2.0 * H_SEW, 0., 0);
    }
    for (auto it = N_TASBACK_.begin(); it != N_TASBACK_.end(); it++) {
        it->second = GSAddback(0., -2.0 * N_SEW, 0., 0);
    }
    for (auto it = C_TASBACK_.begin(); it != C_TASBACK_.end(); it++) {
        it->second = GSAddback(0., -2.0 * C_SEW, 0., 0);
    }
};

void New97Rb::BuildTASParaMap() {
    std::map<std::string, double> paraData;
    //HAGRID:  thresh = kev, subevent window = ns
    Double_t HgammaThreshold_ = 10, HsubEventWin_ = 750;
    paraData.emplace("thresh", HgammaThreshold_);
    paraData.emplace("subEvtWin", HsubEventWin_);  //convert the to ns
    TASParaMap.emplace("smallhag", paraData);
    paraData.clear();

    //NAI: thresh = kev, subevent window = ns
    Double_t NgammaThreshold_ = 10, NsubEventWin_ = 750;
    paraData.emplace("thresh", NgammaThreshold_);
    paraData.emplace("subEvtWin", NsubEventWin_);  //convert the to ns
    TASParaMap.emplace("nai", paraData);
    paraData.clear();

    //CLOVER: thresh = kev, subevent window = ns
    Double_t CgammaThreshold_ = 10, CsubEventWin_ = 750;
    paraData.emplace("thresh", CgammaThreshold_);
    paraData.emplace("subEvtWin", CsubEventWin_);  //convert the to ns
    TASParaMap.emplace("clover", paraData);
    paraData.clear();
}

vector<string> New97Rb::typeAbrev(string &subtype_) {
    vector<string> typeAbrev_;
    if (subtype_ == "smallhag") {
        typeAbrev_.emplace_back("h");
    } else if (subtype_ == "nai") {
        typeAbrev_.emplace_back("n");
    } else if (subtype_ == "clover") {
        typeAbrev_.emplace_back("c");
    } else {
        typeAbrev_.emplace_back("");
    }
    return typeAbrev_;
}

Double_t New97Rb::FuzzTheToF(Double_t TOF_) {
    Double_t LFHWM = 243432 * pow(10, -9) * pow(TOF_, 2) - 11635843 * pow(10, -9) * TOF_ + 0.736628549;
    Double_t sigma_ = LFHWM / 2.335;
    Double_t FuzzyTof_ = Fuzzer->Gaus(TOF_, sigma_);
    // cout<<"TOF= "<<TOF_ << "     Sigma_= "<<sigma_<<"     FuzzyTof_="<<FuzzyTof_<<endl;
    return (FuzzyTof_);
}
