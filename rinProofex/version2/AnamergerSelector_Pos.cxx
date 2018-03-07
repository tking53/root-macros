// #################################################################################################################
// 
// # Alvaro Tolosa Delgado @ IFIC(Valencia,Spain)  alvaro.tolosa@ific.uv.es
// # Copyright (c) 2016 Jorge Agramunt & Alvaro Tolosa. All rights reserved.
// 
// #################################################################################################################

#include "AnamergerSelector.h"
#include <iostream>
#include <fstream>
#include <string>

ClassImp(AnamergerSelector);
  
Double_t AnamergerSelector::test_val2 = 2;

AnamergerSelector::AnamergerSelector(TTree* mergedData):
   aReader (mergedData),
   bigrips  (aReader, "bigrips."),
   implant  (aReader, "implantation."),
   beta     (aReader, "beta."),
   neutron  (aReader, "neutron."),
   gamma    (aReader, "gamma."),
   ancillary(aReader, "ancillary.") 
{
   test_val = 1;
   fTree = nullptr;
}

AnamergerSelector::~AnamergerSelector()
{

}

void AnamergerSelector::Begin(TTree* mergedData)
{
  GetOutputList()->Clear();
  test_val = 10;
  test_val2 = 20;
}

void AnamergerSelector::SlaveBegin(TTree* mergedData)
{
  fHistArray = new TObjArray();
  fHistArray->Add(new TH1F("test","test",100,0,100));
  
  fTree = new TTree("test_tree","test_tree");
  fTree->Branch("event_id",&event_id,"event_id/L");
  fTree->Branch("proc_id",&proc_id,"proc_id/D");
  GetOutputList()->Add(fTree);

  //adding histograms to output list
  TIter next_hist(fHistArray);
  while( TH1F* hist = (TH1F*)next_hist() ){
     GetOutputList()->Add(hist);
  }

  if(!GetInputList()){
    test_param = nullptr;
  }
  else{
    test_param = dynamic_cast<TParameter<Double_t>*>(GetInputList()->FindObject("test"));
  }

  {
    const TString msg = TString::Format("SlaveBegin() of Ord = %s called. %d histograms are initialized.",
                                         gProofServ->GetOrdinal(),GetOutputList()->GetEntries());
    if (gProofServ) gProofServ->SendAsynMessage(msg);
  }

  return;
}

void AnamergerSelector::Init(TTree* mergedData)
{    
  aReader.SetTree( mergedData );
  return;
}


Bool_t AnamergerSelector::Process(Long64_t entry){


  aReader.SetLocalEntry(entry);
  {
      TH1F* hist = (TH1F*)fHistArray->FindObject("test");
      hist->Fill(test_val);
      hist->Fill(test_val2);
      if(test_param)
        hist->Fill(test_param->GetVal());
      event_id = aReader.GetCurrentEntry();
      proc_id = stod(gProofServ->GetOrdinal());
      fTree->Fill();

  }//end loop through the mergedData TTree

  return kTRUE;
}

void AnamergerSelector::Terminate(){
 
  {
    const string fname = ANAMERGER_PATH + "anamerger_output.root";
    fOutputFile = new TFile(fname.c_str(),"recreate");
  }

  // write the parameters

  // write the histograms
  TIter next(GetOutputList());
  while( TObject* obj = next() ){
    obj->Write();
  }
  fTree->Write();

  return ;
}

 
