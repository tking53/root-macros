#define TWaddbackPlotter_cxx
// The class definition in TWaddbackPlotter.h has been generated automatically
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
// root> T->Process("TWaddbackPlotter.C")
// root> T->Process("TWaddbackPlotter.C","some options")
// root> T->Process("TWaddbackPlotter.C+")
//


#include "TWaddbackPlotter.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>

void TWaddbackPlotter::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void TWaddbackPlotter::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   abHistArray = new TObjArray;

   abHistArray->Add(new TH1D("naiAB","NaI Addback Energy",8000.,0.,8000.));
   abHistArray->Add(new TH1D("smallhagAB","2\" HAGRiD Addback Energy",8000.,0.,8000.));
   abHistArray->Add(new TH1D("cloverAB","HPGe Clover Addback Energy",8000.,0.,8000.));

   abHistArray->Add(new TH1D("naiABbg","NaI BetaGated Addback Energy",8000.,0.,8000.));
   abHistArray->Add(new TH1D("smallhagABbg","2\" HAGRiD BetaGated Addback Energy",8000.,0.,8000.));
   abHistArray->Add(new TH1D("cloverABbg","HPGe Clover BetaGated Addback Energy",8000.,0.,8000.));

   abHistArray->Add(new TH2D("naiABbunch","NaI Addback Energy vs Bunch Number",8000.,0.,8000.,400.,0.,400.));
   abHistArray->Add(new TH2D("smallhagABbunch","2\" HAGRiD Addback Energy vs Bunch Number",8000.,0.,8000.,400.,0.,400.));
   abHistArray->Add(new TH2D("cloverABbunch","HPGe Clover Addback Energy vs Bunch Number",8000.,0.,8000.,400.,0.,400.));

   abHistArray->Add(new TH2D("naiABbunchBG","NaI BG Addback Energy vs Bunch Number",8000.,0.,8000.,400.,0.,400.));
   abHistArray->Add(new TH2D("smallhagABbunchBG","2\" HAGRiD BG Addback Energy vs Bunch Number",8000.,0.,8000.,400.,0.,400.));
   abHistArray->Add(new TH2D("cloverABbunchBG","HPGe Clover BG Addback Energy vs Bunch Number",8000.,0.,8000.,400.,0.,400.));

   abHistArray->Add(new TH1D("naiABtdiff","NaI Event Start->First subevt (ns)",8000.,0.,8000.));
   abHistArray->Add(new TH1D("naiABtdiffBG","NaI (BG) Event Start->First subevt (ns)",8000.,0.,8000.));
   abHistArray->Add(new TH1D("smallhagABtdiff","2\" HAGRiD Event Start->First subevt (ns)",8000.,0.,8000.));
   abHistArray->Add(new TH1D("smallhagABtdiffBG","2\" HAGRiD (BG) Event Start->First subevt (ns)",8000.,0.,8000.));
   abHistArray->Add(new TH1D("cloverABtdiff","HPGe Clover Event Start->First subevt (ns)",8000.,0.,8000.));
   abHistArray->Add(new TH1D("cloverABtdiffBG","HPGe Clover (BG) Event Start->First subevt (ns)",8000.,0.,8000.));

   //add to output list
   TIter next(abHistArray);
   //adding 1D hists
   while (TH1D* hist = (TH1D*)next() ){
     GetOutputList()->Add(hist);
   }
   //add the 2Ds as well
   while (TH2D* hist = (TH2D*)next() ){
     GetOutputList()->Add(hist);
   }
}

Bool_t TWaddbackPlotter::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
  
  abReader.SetLocalEntry(entry);
  
  std::stringstream hist2Fill;
  
  hist2Fill<<(*GS_ABType)<<"AB";
  ((TH1D*)abHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_ABEnergy));
  
  hist2Fill.str("");
  hist2Fill<<(*GS_ABType)<<"ABtdiff";
  ((TH1D*)abHistArray->FindObject(hist2Fill.str().c_str()))->Fill(((*GS_ABTime)-(*GS_EvtStartTime)+100));
  
  hist2Fill.str("");
  hist2Fill<<(*GS_ABType)<<"ABbunch";
  ((TH2D*)abHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_ABEnergy),(*GS_ABbunchNum));
  
  
  if ((*GS_ABhasLRBeta)){
    hist2Fill.str("");
    hist2Fill<<(*GS_ABType)<<"ABbg";
    ((TH1D*)abHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_ABEnergy));
    
    hist2Fill.str("");
    hist2Fill<<(*GS_ABType)<<"ABtdiffBG";
    ((TH1D*)abHistArray->FindObject(hist2Fill.str().c_str()))->Fill(((*GS_ABTime)-(*GS_EvtStartTime)+100));
    
    hist2Fill.str("");
    hist2Fill<<(*GS_ABType)<<"ABbunchBG";
    ((TH2D*)abHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_ABEnergy),(*GS_ABbunchNum));
    }
    
  return kTRUE;
}

  void TWaddbackPlotter::SlaveTerminate()
  {
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void TWaddbackPlotter::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.


  // write the histograms
  const std::string fNew = "TWaddbackPlotter_output.root";
  fOutputFile = new TFile(fNew.c_str(),"recreate");
  TIter next(GetOutputList());
  while( TObject* obj = next() ){
    obj->Write();
  }

}
