#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>
#include <PaassRootStruct.hpp>
#include <vector>
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TObjArray.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
#include "TChain.h"

TObjArray* tracePlots;

using namespace std;
void TracePlotter( TTree* PTree, vector<string> chanTrace,Int_t bitMax = 66000, Int_t traceLength = 500) {
cout<<"test"<<endl;
  TTreeReader PixTreeEvt;

  PixTreeEvt.SetTree(PTree);
  TTreeReaderArray<processor_struct::ROOTDEV> rd = {PixTreeEvt, "root_dev_vec_"};

  tracePlots = new TObjArray;
  tracePlots->SetName("tracePlots");
  Int_t TraceXBins = traceLength;
  Int_t TraceYBins = bitMax;
  for (unsigned itDetNum = 0; itDetNum <20; ++itDetNum){
      for (unsigned it = 0; it < chanTrace.size(); it++) {
          string combo = chanTrace.at(it) + "_" + to_string(itDetNum);
          string title = chanTrace.at(it) + "_" + to_string(itDetNum) + " Traces";
          tracePlots->Add(new TH2D(combo.c_str(), title.c_str(), TraceXBins,0,TraceXBins,TraceYBins,0,TraceYBins));

          combo += "_sat";
          title += "::Saturated";
          tracePlots->Add(new TH2D(combo.c_str(), title.c_str(), TraceXBins,0,TraceXBins,TraceYBins,0,TraceYBins));
      }
  }

    double TotalEvts=PixTreeEvt.GetEntries();
  /* map<Int_t, Double_t> eventsWithNoTrace; */ 
    cout<<"TotalEvts= "<<TotalEvts<<endl;
  while (PixTreeEvt.Next()) {
    double CurEntryNum = PixTreeEvt.GetCurrentEntry();
    for (auto itC = rd.begin(); itC != rd.end(); itC++) {
      bool hasValidWaveForm = itC->hasValidWaveformAnalysis;
      vector<unsigned> trace = itC->trace;
      double energy = itC->energy;
      string dtype = itC->subtype.Data();
      string dgroup = itC->group.Data();
      string key;// = dtype + ":" + dgroup;
      int chanNum = itC->chanNum;
      int modNum = itC->modNum;

      int chanloc = (modNum * 16)+chanNum;

      /* if (itC->saturation || itC->pileup){ */
      /*   continue; */
      /* } */

      if (trace.size()==0 || itC->detNum >=20){
          continue;
          /* eventsWithNoTrace.find(chanloc)->second++; */
          /* cout<<"Det Event With No Trace"<<endl; */
      } else {
          if (itC->saturation){
              key = dtype + "_" + to_string(itC->detNum) + "_sat";
          } else if (!itC->saturation && itC->pileup) {
              key = dtype + "_" + to_string(itC->detNum);
          } else {
              continue;
          }
        
        for (unsigned int it = 0, itend = trace.size(); it < itend; it++) {
          ((TH2*)tracePlots->FindObject(key.c_str()))->Fill(it, trace.at(it));
        }

      }
    cout<<"\rOnEntry "<< CurEntryNum <<" of "<< TotalEvts <<" ( "<< floor((CurEntryNum/TotalEvts)*100) << " %)"<<flush;
    }
  }

  /* cout<<endl<<endl<<"Events without traces at the Processor Level (either no trace or it was lost during analysis)"<<endl */
  /*     <<"-----------------------------------------------------------------------------------" <<endl; */
  /* for (auto it=eventsWithNoTrace.begin();it!=eventsWithNoTrace.end();it++){ */
  /*   cout<<"Linear Channel Number " << (*it).first <<" had " << (*it).second <<" events WITHOUT a trace"<<endl; */
  /* } */

  gDirectory->Add(tracePlots);
  cout<<endl;
}
