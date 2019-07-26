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
    totalCycleTimeBins = TimeBins_ / msPerBin_;

    HistList = new TObjArray();
    HistList->Add(new TH2D("dd_h_det_multi", "Hagrid Multiplicity per crystal", 20, 0, 20, 50, 0, 50));
    HistList->Add(new TH2D("dd_h_det_multi_dt", "Hagrid Multiplicity per crystal beta dt", 20, 0, 20, 50, 0, 50));

    //!-------------------------------------------------------------------------------
    //! 2D gamma beta dt plots
    //!-------------------------------------------------------------------------------
    HistList->Add(new TH2F("dd_h_e_hrbdt", "LaBr Energy vs LaBr- high res Beta dt", HagBins_, 0, HagBins_, DT_bins_, -1 * DT_bins_, DT_bins_));
    HistList->Add(new TH2F("dd_h_det_hrbdt", "LaBr- high res Beta dt vs det num", 2 * DT_bins_, -1 * DT_bins_, DT_bins_, 20, 0, 20));

    HistList->Add(new TH2F("dd_h_e_lrbdt", "LaBr Energy vs LaBr- low res Beta dt", HagBins_, 0, HagBins_, DT_bins_, -1 * DT_bins_, DT_bins_));
    HistList->Add(new TH2F("dd_h_det_lrbdt", "LaBr- low res Beta dt vs det num", 2 * DT_bins_, -1 * DT_bins_, DT_bins_, 20, 0, 20));

    HistList->Add(new TH2F("dd_n_e_lrbdt", "nai Energy vs nai- low res Beta dt", NaiBins_, 0, NaiBins_, DT_bins_, -1 * DT_bins_, DT_bins_));
    HistList->Add(new TH2F("dd_n_det_lrbdt", "nai- low res Beta dt vs det num", 2 * DT_bins_, -1 * DT_bins_, DT_bins_, 20, 0, 20));

    HistList->Add(new TH2F("dd_c_e_lrbdt", "Clover Energy vs LaBr- low res Beta dt", CloverBins_, 0, CloverBins_, DT_bins_, -1 * DT_bins_, DT_bins_));
    HistList->Add(new TH2F("dd_c_det_lrbdt", "Clover time - low res Beta dt vs det num", 2 * DT_bins_, -1 * DT_bins_, DT_bins_, 5, 0, 5));

    //!-------------------------------------------------------------------------------
    //! ADDBACK/TASBACK plots
    //!-------------------------------------------------------------------------------

    HistList->Add(new TH1F("d_h_TB", "TASBACK for HAGRiD", TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH1F("d_n_TB", "TASBACK for NaI", TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH1F("d_c_TB", "TASBACK for Clover", TB_Bins_, 0, TB_Bins_));

    HistList->Add(new TH1F("d_h_BG", "BetaDT gated HAGRiD", TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH1F("d_n_BG", "BetaDT gated NaI", TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH1F("d_c_BG", "BetaDT gated Clover", TB_Bins_, 0, TB_Bins_));

    HistList->Add(new TH1F("d_tas", "Pixie Event TASback", TB_Bins_, 0, TB_Bins_));

    HistList->Add(new TH2F("dd_h_tas", "Hagrid in beta DT vs Pixie TAS", HagBins_, 0, HagBins_, TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH2F("dd_n_tas", "Nai in beta DT vs Pixie TAS", NaiBins_, 0, NaiBins_, TB_Bins_, 0, TB_Bins_));
    HistList->Add(new TH2F("dd_c_tas", "Clover in beta DT vs Pixie TAS", CloverBins_, 0, CloverBins_, TB_Bins_, 0, TB_Bins_));

    AddToOutputList(HistList);

    //! Initalize addback stuff
    BuildTASParaMap();  // Build the Parameter "db" values for Thresh,subevent window are set in this function

    H_TASBACK_ = BuildTASbackMaps({"shg0", "shg1", "shg2", "shg3"}, "smallhag");
    N_TASBACK_ = BuildTASbackMaps({"naiL1", "naiTop"}, "nai");
    C_TASBACK_ = BuildTASbackMaps({"clover0"}, "clover");

    //! Load my various VANDLE TCUTS
    LoadTCuts();

    //!Initalize progress counters event count (cnt) and file count (fcnt)
    cnt = 0;
    fcnt = 1;
    cout << "Slave Begin() complete" << endl;
}

Bool_t New97Rb::Process(Long64_t entry) {
    fReader.SetLocalEntry(entry);

    // Reset the TASBACK
    ResetTASback();

    //individual hagrid multiplicities
    vector<Int_t> hagMulti(20, 0);
    vector<Int_t> hagMultiBG(20, 0);

    // Pixie Event TAS
    Double_t PixieTAS_ = 0;
    vector<Double_t> h_tas_singles_, n_tas_singles_, c_tas_singles_;

    // this shouldnt be needed
    h_tas_singles_.clear();
    n_tas_singles_.clear();
    c_tas_singles_.clear();

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
        vector<Double_t> *tas_singles_vec_;
        if (Type_ == "smallhag") {
            hagrid = true;
            tas_singles_vec_ = &h_tas_singles_;
        } else if (Type_ == "nai") {
            nai = true;
            tas_singles_vec_ = &n_tas_singles_;
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




        plotTAS_ = DoTASback(Type_, Group_, Energy_, Time_, tas_singles_vec_, false);
        PixieTAS_ += Energy_;
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
    for (auto itClo = clover.begin(); itClo != clover.end(); itClo++) {
        //! Get things from the stuct
        Double_t Energy_ = itClo->energy;
        Double_t Time_ = itClo->time;  //ns
        Int_t Num_ = itClo->detNum;
        string Type_ = "clover";
        string Group_ = Type_ + to_string(itClo->cloverNum);
        vector<Double_t> *tas_singles_vec_;
        tas_singles_vec_ = &c_tas_singles_;
        // Double_t dt = (Time_ - LastCT) * timeBinning_;  // binned time

        //! Init bools
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
        // if (inBetaGammaTCut) {
        plotTAS_ = DoTASback(Type_, Group_, Energy_, Time_, tas_singles_vec_, false);
        // c_tas_singles_.emplace_back(Energy_);  //  and the singles energy to the right vec
        // }
        PixieTAS_ += Energy_;  // add the summed energy to the pixie event total
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
            // PixieTAS_ += TB_Energy_;  // add the summed energy to the pixie event total
        }
    }
    for (auto it = N_TASBACK_.begin(); it != N_TASBACK_.end(); it++) {
        if (it->second.multiplicity == 1) {
            Double_t TB_Energy_ = it->second.energy;
            ((TH1 *)HistList->FindObject("d_n_TB"))->Fill(TB_Energy_);
            // PixieTAS_ += TB_Energy_;  // add the summed energy to the pixie event total
        }
    }
    for (auto it = C_TASBACK_.begin(); it != C_TASBACK_.end(); it++) {
        if (it->second.multiplicity == 1) {
            Double_t TB_Energy_ = it->second.energy;
            ((TH1 *)HistList->FindObject("d_c_TB"))->Fill(TB_Energy_);
            // PixieTAS_ += TB_Energy_;  // add the summed energy to the pixie event totals
        }
    }

    for (UInt_t it = 0; it < hagMulti.size(); it++) {
        ((TH2 *)HistList->FindObject("dd_h_det_multi"))->Fill(it, hagMulti.at(it));
    }

    //Fill the pixie event totals
    ((TH1 *)HistList->FindObject("d_tas"))->Fill(PixieTAS_);

    for (auto it = h_tas_singles_.begin(); it != h_tas_singles_.end(); it++) {
        ((TH2 *)HistList->FindObject("dd_h_tas"))->Fill((*it), PixieTAS_);
    }
    for (auto it = n_tas_singles_.begin(); it != n_tas_singles_.end(); it++) {
        ((TH2 *)HistList->FindObject("dd_n_tas"))->Fill((*it), PixieTAS_);
    }
    for (auto it = c_tas_singles_.begin(); it != c_tas_singles_.end(); it++) {
        ((TH2 *)HistList->FindObject("dd_c_tas"))->Fill((*it), PixieTAS_);
    }

    PrintRunStats();
    return kTRUE;
}

void New97Rb::SlaveTerminate() {
}

void New97Rb::Terminate() {
    gDirectory->Add(((TH2 *)HistList->FindObject("dd_h_det_multi")));
    // ((TH2 *)HistList->FindObject("dd_h_det_multi"))->Draw("colz");
    TIter next(GetOutputList());
    while (TObject *obj = next()) {
        gDirectory->Add(obj);
    }

    gDirectory->Add(betaCloverDT);
    gDirectory->Add(betaHagHrtDT);
    gDirectory->Add(betaHagLrtDT);
    gDirectory->Add(betaNaiDT);
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