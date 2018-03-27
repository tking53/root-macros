#define SinglesPlotter_cxx
// The class definition in SinglesPlotter.h has been generated automatically
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
// root> T->Process("SinglesPlotter.C")
// root> T->Process("SinglesPlotter.C","some options")
// root> T->Process("SinglesPlotter.C+")
//


#include "SinglesPlotter.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>

void SinglesPlotter::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void SinglesPlotter::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   singHistArray = new TObjArray();

   singHistArray->Add(new TH1D("naiMulti","NaI Multiplicity",50,0.,50.));
   singHistArray->Add(new TH1D("naiTotal","NaI Energy (Totals)",8000.,0.,8000.));
   singHistArray->Add(new TH1D("naiTotalBG","NaI Energy Beta Gate (Totals)",8000.,0.,8000.));
   singHistArray->Add(new TH1D("naiBetaTDiff","Beta->Gamma TDiff (NaI) ",8000.,0.,8000.));
   singHistArray->Add(new TH1D("naiEvtStartTDiff","Pixie Event Start->Gamma TDiff (100 offset) (NaI) ",8000.,0.,8000.));
   singHistArray->Add(new TH1D("naiSuperBG","NaI Super BetaGate (B->G Tdiff <150ns) ",8000.,0.,8000.));
   singHistArray->Add(new TH1D("naiScalNum","NaI Counts per Detector Number ",20.,0.,20.));
   singHistArray->Add(new TH2D("naiDriftTrac","NaI Energy vs Cycle Num (Drift Tracker) ",8000.,0.,8000.,400.,0.,400.));
   singHistArray->Add(new TH2D("naiInBun","NaI Energy in Bunch (0.01s/bin)",8000.,0.,8000.,4000.,0.,4000.));
   singHistArray->Add(new TH2D("naiInBunBG","NaI Energy (Evt BetaGated) in Bunch (0.01s/bin)",8000.,0.,8000.,4000.,0.,4000.));

   singHistArray->Add(new TH1D("smallhagMulti","2\" HAGRiD Multiplicity",50,0.,50.));
   singHistArray->Add(new TH1D("smallhagTotal","2\" HAGRiD Energy (Totals)",8000.,0.,8000.));
   singHistArray->Add(new TH1D("smallhagTotalBG","2\" HAGRiD Energy Beta Gate (Totals)",8000.,0.,8000.));
   singHistArray->Add(new TH1D("smallhagBetaTDiff","Beta->Gamma TDiff (2\" HAGRiD) ",8000.,0.,8000.));
   singHistArray->Add(new TH1D("smallhagEvtStartTDiff","Pixie Event Start->Gamma TDiff (100 offset) (2\" HAGRiD) ",8000.,0.,8000.));
   singHistArray->Add(new TH1D("smallhagSuperBG","2\" HAGRiD Super BetaGate (B->G Tdiff <150ns) ",8000.,0.,8000.));
   singHistArray->Add(new TH1D("smallhagScalNum","2\" HAGRiD Counts per Detector Number ",20.,0.,20.));
   singHistArray->Add(new TH2D("smallhagDriftTrac","2\" HAGRiD Energy vs Cycle Num (Drift Tracker) ",8000.,0.,8000.,400.,0.,400.));
   singHistArray->Add(new TH2D("smallhagInBun","2\" HAGRiD Energy in Bunch (0.01s/bin)",8000.,0.,8000.,4000.,0.,4000.));
   singHistArray->Add(new TH2D("smallhagInBunBG","2\" HAGRiD Energy (Evt BetaGated) in Bunch (0.01s/bin)",8000.,0.,8000.,4000.,0.,4000.));

   singHistArray->Add(new TH1D("cloverMulti","HPGe Clover Multiplicity",50,0.,50.));
   singHistArray->Add(new TH1D("cloverTotal","HPGe Clover Energy (Totals)",8000.,0.,8000.));
   singHistArray->Add(new TH1D("cloverTotalBG","HPGe Clover Energy Beta Gate (Totals)",8000.,0.,8000.));
   singHistArray->Add(new TH1D("cloverBetaTDiff","Beta->Gamma TDiff (HPGe Clover) ",8000.,0.,8000.));
   singHistArray->Add(new TH1D("cloverEvtStartTDiff","Pixie Event Start->Gamma TDiff (100 offset) (HPGe Clover) ",8000.,0.,8000.));
   singHistArray->Add(new TH1D("cloverSuperBG","HPGe Clover Super BetaGate (B->G Tdiff <150ns) ",8000.,0.,8000.));
   singHistArray->Add(new TH1D("cloverScalNum","HPGe Clover Counts per Detector Number ",20.,0.,20.));
   singHistArray->Add(new TH2D("cloverDriftTrac","HPGe Clover Energy vs Cycle Num (Drift Tracker) ",8000.,0.,8000.,400.,0.,400.));
   singHistArray->Add(new TH2D("cloverInBun","HPGe Clover Energy in Bunch (0.01s/bin)",8000.,0.,8000.,4000.,0.,4000.));
   singHistArray->Add(new TH2D("cloverInBunBG","HPGe Clover Energy (Evt BetaGated) in Bunch (0.01s/bin)",8000.,0.,8000.,4000.,0.,4000.));

   singHistArray->Add(new TH2D("fullwalkcheck","Gamma - Beta Time vs Beta QDC",2000.,0.,2000.,8000.,0.,8000.));
   singHistArray->Add(new TH2D("cloverwalkcheck","Gamma (Clover) Time (+100 offset) - Beta Time vs Beta QDC",2000.,0.,2000.,8000.,0.,8000.));
   singHistArray->Add(new TH2D("naiwalkcheck","Gamma (NaI) Time (+100 offset) - Beta Time vs Beta QDC",2000.,0.,2000.,8000.,0.,8000.));
   singHistArray->Add(new TH2D("smallhagwalkcheck","Gamma (2\" HAGRID) Time (+100 offset) - Beta Time vs Beta QDC",2000.,0.,2000.,8000.,0.,8000.));

   singHistArray->Add(new TH2D("smallhagwalkcheckself","2\" HAGRID Time (+100 offset) - Beta Time vs 2\" HAGRiD Energy",2000.,0.,2000.,8000.,0.,8000.));
   singHistArray->Add(new TH2D("cloverwalkcheckself","Clover Time (+100 offset) - Beta Time vs Clover Energy",2000.,0.,2000.,8000.,0.,8000.));
   singHistArray->Add(new TH2D("naiwalkcheckself","NaI Time (+100 offset) - Beta Time vs NaI Energy",2000.,0.,2000.,8000.,0.,8000.));

   singHistArray->Add(new TH1D("betaMulti","Beta Detector Multiplicity",50,0.,50.));

   for (auto it = 0; it<16;it++){
     std::stringstream ss;
     std::stringstream sss;
     string num = std::to_string(it);
     ss<<"smallhag"<<num;
     sss<<"Small Hagrid #"<<num<<" Energy";
     singHistArray->Add(new TH1D(ss.str().c_str(),sss.str().c_str(),8000.,0.,8000.));
   }

   for (auto it = 0; it<10;it++){
     std::stringstream ss;
     std::stringstream sss;
     string num = std::to_string(it);
     ss<<"nai"<<num;
     sss<<"NaI #"<<num<<" Energy";
     singHistArray->Add(new TH1D(ss.str().c_str(),sss.str().c_str(),8000.,0.,8000.));
   }
   for (auto it = 0; it<4;it++){
     std::stringstream ss;
     std::stringstream sss;
     string num = std::to_string(it);
     ss<<"clover"<<num;
     sss<<"Clover Leaf #"<<num<<" Energy";
     singHistArray->Add(new TH1D(ss.str().c_str(),sss.str().c_str(),8000.,0.,8000.));
   }

   
   //add to output list
   TIter next(singHistArray);
   //add the 1Ds 
   while (TH1D* hist = (TH1D*)next() ){
     GetOutputList()->Add(hist);
   }
   //add the 2Ds
   while (TH2D* hist = (TH2D*)next() ){
     GetOutputList()->Add(hist);
   }

}

Bool_t SinglesPlotter::Process(Long64_t entry)
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

   sReader.SetLocalEntry(entry);

   std::stringstream hist2Fill;

   //filling Totals and BG Totals
   hist2Fill<<(*GS_Type)<<"Total";
   ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_Energy));

   if ((*GS_HasLowResBeta)){
     hist2Fill.str("");
     hist2Fill<<(*GS_Type)<<"TotalBG";
     ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_Energy));
   }

   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<(*GS_Num);
   ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_Energy));
   
   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<"BetaTDiff";
   ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_BetaGammaTDiff));
                                                                     
   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<"DriftTrac";
   ((TH2D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_Energy),(*GS_BunchNum));

   if ((*GS_Time) - (*GS_BetaTime) >0 && (*GS_Time) - (*GS_BetaTime) <=150 ){ 
     hist2Fill.str("");
     hist2Fill<<(*GS_Type)<<"SuperBG";
     ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_Energy));
   }
   
   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<"ScalNum";
   ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_Num)); 

   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<"walkcheck";
   ((TH2D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill(((*GS_Time)-(*GS_BetaTime)+100),(*GS_BetaEnergy));  
   
   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<"walkcheckself";
   ((TH2D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill(((*GS_Time)-(*GS_BetaTime)+100),(*GS_Energy));
   
   //This one probably will be garbage because its its type unspecific 
   ((TH2D*)singHistArray->FindObject("fullwalkcheck"))->Fill(((*GS_Time)-(*GS_BetaTime)+100),(*GS_BetaEnergy));
                                                                     
                                                                     
   //These might need the multiplicities divided by the number of entries (because of how i filled the tree in paass); in which chase they will need reploted; but the relative heights should work
   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<"Multi";
   if ((*GS_Type) =="smallhag"){
     ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_smallhagmulti));
   }else if ((*GS_Type) =="nai"){
     ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_naimulti));
   }else if ((*GS_Type) == "clover"){
     ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_clovermulti));
   }
   
   ((TH1D*)singHistArray->FindObject("betaMulti"))->Fill((*GS_BetaMulti));
   
   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<"EvtStartTDiff";
   ((TH1D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill(((*GS_Time)-(*GS_EvtStartTime)+100));
     
   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<"InBun";
    Double_t InBunTime= (((*GS_Time)-(*GS_LastBunchTime))*1.e9) / 1.e3;
    cout<<"inbunTime= "<<endl;
   ((TH2D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_Energy),InBunTime);
   
   hist2Fill.str("");
   hist2Fill<<(*GS_Type)<<"InBunBG";
   ((TH2D*)singHistArray->FindObject(hist2Fill.str().c_str()))->Fill((*GS_Energy),InBunTime);
   
   
       
       
       
       
       
       
   return kTRUE;
}

void SinglesPlotter::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void SinglesPlotter::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  // write the histograms
  const std::string fNew = "SinglesPlotter_output.root";
  fOutputFile = new TFile(fNew.c_str(),"recreate");
  TIter next(GetOutputList());
  while( TObject* obj = next() ){
    obj->Write();
  }


}
