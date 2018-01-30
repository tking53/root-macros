#include "addback.h"
//#include "addbackSelector.h"


void addbackCalc(const char* inFile, const char* outFile = inFile+"-addback.root",const bool batchmode=false){
  if (batchmode){
    gROOT->SetBatch(kTRUE);
  }

  TFile *f = TFile::Open(inFile);
  TFile fnew(outFile,"RECREATE");

  TTree *Tsing = (TTree*)f->Get("GSsingles");


}




void addbackSelector::SetAddbackRefTime(const std::string &subtype, const double &newRefTime) {
  ParameterMap.find(subtype)->second.insert(make_pair("refTime",newRefTime));
}

double addbackSelector::GetAddbackPara(const std::string &subtype, const std::string &option) {
  return ParameterMap.find(subtype)->second.find(option)->second;
}

