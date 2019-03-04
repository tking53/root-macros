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

void Rb97Full::Begin(TTree * /*tree*/)
{
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).
    
    TString option = GetOption();

    outputFilePrefix = "Rb97Full_output";

}

void Rb97Full::SlaveBegin(TTree * /*tree*/)
{
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();

    Double_t GammaBins = 8000;
    Double_t TimeBins = 10000;

    GammaArray = new TObjArray;
    
    for (auto i = 0; i < 4; i++) {
        stringstream ss, s;
        ss << "cloverChan" << i;
        s << "Clover Channel #" << i;
        GammaArray->Add(new TH1D(ss.str().c_str(), s.str().c_str(), GammaBins, 0, GammaBins));
    }

    GammaArray->Add(new TH2F("e_dt", "Decay curves for clover", GammaBins, 0, GammaBins, TimeBins, 0, TimeBins));
    GammaArray->Add(new TH2F("e_cycle", "Energy vs Cycle", GammaBins, 0, GammaBins, 500, 0, 500));


// Loop over the TObjArray and add the his to the output list.
// needs 2 while loops for 1d and 2d (per TObjArray)
    TIter next(GammaArray);
    
    while(TH1* hist=(TH1*)next()){
        GetOutputList()->Add(hist);
    }
    
    while(TH2* hist=(TH2*)next()){
        GetOutputList()->Add(hist);
    }
    
}

// void Rb97Full::Notify(){
//     if (fReader.GetCurrentEntry() == 0)
//         ;

// }

Bool_t Rb97Full::Process(Long64_t entry)
{
    fReader.SetLocalEntry(entry);

    Double_t LastCT = logic.At(0).lastTapeCycleStartTime;

    // Int_t currentTreeNum=fReader.GetChainEntryNumber();

    Int_t currentCycleNum = logic.At(0).cycleNum;

    for (auto itC = clover.begin(); itC != clover.end(); itC++)
    {
        Double_t cEnergy_ = itC->energy;
        Double_t cTime_ = itC->time * DSPclockInSeconds * 1e9;
        Double_t dt = abs(LastCT - cTime_);

        stringstream s;
        s << "cloverChan" << itC->detNum;
        ((TH1*)GammaArray->FindObject(s.str().c_str()))->Fill(cEnergy_, dt);
        ((TH2*)GammaArray->FindObject("e_dt"))->Fill(cEnergy_, dt);
        ((TH2*)GammaArray->FindObject("e_cycle"))->Fill(cEnergy_, currentCycleNum);
    }

    return kTRUE;
}

void Rb97Full::SlaveTerminate()
{
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.
}

void Rb97Full::Terminate()
{
    stringstream filename;
    TDatime now;
    filename <<outputFilePrefix <<"_" << now.GetDate() << "_" << now.GetTime() << ".root";
    outFile = new TFile(filename.str().c_str(),"RECREATE");
    
    TIter next(GetOutputList());\
    while( TObject* obj = next() ){ 
        obj->Write();
        //    delete (obj);
    }
    outFile->Close();
}

