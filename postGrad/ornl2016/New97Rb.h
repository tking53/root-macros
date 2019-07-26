//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul  3 16:45:09 2019 by ROOT version 6.14/06
// from TTree PixTree/Pixie Event Tree
// found on file: 097rb_53-0_DD.root
//////////////////////////////////////////////////////////

#ifndef New97Rb_h
#define New97Rb_h

#include <TChain.h>
#include <TFile.h>
#include <TROOT.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

// Headers needed by this particular selector
#include "GSaddback.hpp"
#include "ProcessorRootStruc.hpp"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THnSparse.h"

//Analysis headers
#include <TCutG.h>
#include "VandleTCuts.h"

class New97Rb : public TSelector {
   public:
    TTreeReader fReader;  //!the tree reader
    TTree *fChain = 0;    //!pointer to the analyzed TTree or TChain

    TTreeReaderValue<Double_t> eventNum = {fReader, "eventNum"};
    TTreeReaderValue<string> fileName = {fReader, "fileName"};

    TTreeReaderArray<processor_struct::GAMMASCINT> gamscint = {fReader, "gamma_scint_vec_"};
    TTreeReaderArray<processor_struct::CLOVERS> clover = {fReader, "clover_vec_"};
    TTreeReaderArray<processor_struct::VANDLES> vandle = {fReader, "vandle_vec_"};
    TTreeReaderArray<processor_struct::LOGIC> logic = {fReader, "logic_vec_"};
    TTreeReaderArray<processor_struct::DOUBLEBETA> beta = {fReader, "doublebeta_vec_"};

    TCutG *betaCloverDT, *vanBanana, *betaHagHrtDT, *betaHagLrtDT, *betaNaiDT;

    Int_t cnt;
    Int_t fcnt;
    Int_t entTotal_;
    Bool_t isChain;
    Double_t DSPclockInSeconds = 8e-9;
    Double_t ADCclockInSeconds = 4e-9;
    Double_t msConvert = 1e-6;
    Double_t timeBinning_;
    Int_t totalCycleTimeBins;

    vector<Double_t> hagHRBdToffsets = {149, 158, 160, 159, 159, 168, 160, 161, 357, 348, 358, 365, 357, 356, 359, 359};
    static constexpr Double_t hagridExternDelayOffset = 200;

    //Vectors of Events for addback
    map<string, GSAddback> H_TASBACK_;  //!<structure for Small HAGRiD addback
    map<string, GSAddback> N_TASBACK_;  //!<structure for NaI addback
    map<string, GSAddback> C_TASBACK_;  //!<structure for Clover addback

    /** The nested map structure is <subtype , < parameter , value > >.
     *    Parameters are "thresh" , "subEvtWin"
     *    SubEvtWin (SEW) need to be in ns    */
    std::map<std::string, std::map<std::string, double> > TASParaMap;  //!<Map of TASback parameters

    New97Rb(TTree * /*tree*/ = 0) {
    }
    virtual ~New97Rb() {}
    virtual Int_t Version() const { return 2; }
    virtual void Begin(TTree *tree);
    virtual void SlaveBegin(TTree *tree);
    virtual void Init(TTree *tree);
    virtual Bool_t Notify();
    virtual Bool_t Process(Long64_t entry);
    virtual Int_t GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
    virtual void SetOption(const char *option) { fOption = option; }
    virtual void SetObject(TObject *obj) { fObject = obj; }
    virtual void SetInputList(TList *input) { fInput = input; }
    virtual TList *GetOutputList() const { return fOutput; }
    virtual void SlaveTerminate();
    virtual void Terminate();

    void LoadTCuts();
    void AddToOutputList(TObjArray *iArray);
    void PrintRunStats();

    //Build and Init the map of groups, GSaddback stuct
    map<string, GSAddback> BuildTASbackMaps(vector<string> groups_, string type_);

    //Build and Init the Parameter Map
    void BuildTASParaMap();

    //Lookup the Specific Parameter for a given subtype
    Double_t GetTASbackPara(const std::string &subtype, const std::string &option);

    /* Get an iterator to the right map location for the subtype and group
    Access with ITER->second.{energy,time etc}    */
    map<string, GSAddback>::iterator GetTASback(const string &subtype_, const string &group_);

    //Reset Everything
    void ResetTASback();

    /* \brief Do the actual math for TAS/Add back
    \param [in] subtype_  is the current subtype. I.E. which TASback struct to operate on
    \param [in] group_ is the current group. I.E. which TASback struct to operate on
    \param [in] energy_ is the current event energy to add
    \param [in] time_ is the current event time
    \param [in] reset_ is should we reset the struct before the math. Used at the end of the event loop to catch the current event into the TASback after we plot things
    \return true if we are outside of our sub event window for this subtype:group combo
     */
    Bool_t DoTASback(string &subtype_, string &group_, Double_t &energy_, Double_t &time_, vector<Double_t>* singlesVec, Bool_t reset_);

    //Return the histograming abrevations for a given type
    vector<string> typeAbrev(string &subtype_);

    ClassDef(New97Rb, 0);

    TObjArray *HistList;
};

#endif
