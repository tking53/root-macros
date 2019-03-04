#include <TH1.h>
#include <TF1.h>
#include <TSystem.h>
#include <TH2.h>
#include <cmath>
#include <vector>
//#include "TBanCuts.h"



int FindNeutrons(const std::string iso= "",const Double_t QDCMIN = 0,const Bool_t CUT = true){
  if (iso == ""){
    cout<<"error i need an isotope"<<endl;
    return 1;
  }

  gStyle->SetPalette(1);
  gStyle->SetNumberContours(999);

  // std::stringstream CanName;
  // CanName<<"QDC cut is "<<QDCMIN<<" for all plots.";
  // auto c1 = new TCanvas("c1",CanName.str().c_str());

  if (CUT){
  gROOT->ProcessLine(".L Bans/TBanCut4");
  cout<<"Loading TBanCut4"<<endl;
  }
  TH2F* QDCvsTof = 0;

  std::vector<TH2F*> low2ds,high2ds,gate2ds;
  std::vector<TH1F*> prompt,delay,extdelay;
  std::vector<TH1*> lowProj,highProj,gateProj;
  std::vector<TH1*>bkgs,subd;

  int NumGammaLines;
  if (iso == "97")
    NumGammaLines = 5;//97rb
  else if (iso == "94")
    NumGammaLines = 4;//94rb
  else{
    cout<<"Unknown Isotope"<<endl;
    return 2;
  }

  std::vector <std::string> types ={"ge","sh"};
  std::string OnGate = "OnGateTQ";
  std::string OverGate ="OverGateTQ";
  std::string UnderGate ="UnderGateTQ";

  for (unsigned j=0;j<types.size();j++){
    std::stringstream d1name;
    std::stringstream key;

    cout<<"Working on "<<types.at(j)<<endl;
    key<<types.at(j)<<"Prompt";
    prompt.emplace_back((TH1F*)gROOT->FindObject(key.str().c_str()));
    key.str("");
    
    key<<types.at(j)<<"Delay";
    delay.emplace_back((TH1F*)gROOT->FindObject(key.str().c_str()));
    key.str("");
    
    key<<types.at(j)<<"EXTDelay";
    extdelay.emplace_back((TH1F*)gROOT->FindObject(key.str().c_str()));
    key.str("");
    
    for (unsigned it=0;it<NumGammaLines;it++){
      key.str("");
      key<<types.at(j)<<UnderGate<<it;
      low2ds.emplace_back((TH2F*)gROOT->FindObject(key.str().c_str()));
      key.str("");

      key<<types.at(j)<<OverGate<<it;
      high2ds.emplace_back((TH2F*)gROOT->FindObject(key.str().c_str()));
      key.str("");

      key<<types.at(j)<<OnGate<<it;
      gate2ds.emplace_back((TH2F*)gROOT->FindObject(key.str().c_str()));
      key.str("");
    }//end gamma list loop
    //end get histos from file


    //do the projections
    cout<<"Projecting"<<endl;
    for (auto it=0;it<NumGammaLines;it++){
      std::stringstream name;
      int currentIndex = NumGammaLines*j+it;

      if(CUT){
        name<<types.at(j)<<"lowProjNum"<<it;
        (low2ds.at(currentIndex))->ProjectionX(name.str().c_str(),QDCMIN,-1,"[BanCut4]");
        lowProj.emplace_back((TH1D*)gROOT->FindObject(name.str().c_str()));

        name.str("");
        name<<types.at(j)<<"highProjNum"<<it;
        (high2ds.at(currentIndex))->ProjectionX(name.str().c_str(),QDCMIN,-1,"[BanCut4]");
        highProj.emplace_back((TH1D*)gROOT->FindObject(name.str().c_str()));

        name.str("");
        name<<types.at(j)<<"gateProjNum"<<it;
        (gate2ds.at(currentIndex))->ProjectionX(name.str().c_str(),QDCMIN,-1,"[BanCut4]");
        gateProj.emplace_back((TH1D*)gROOT->FindObject(name.str().c_str()));

      }else { //no cut loaded
        name<<types.at(j)<<"lowProjNum"<<it;
        (low2ds.at(currentIndex))->ProjectionX(name.str().c_str(),QDCMIN,-1);
        lowProj.emplace_back((TH1D*)gROOT->FindObject(name.str().c_str()));

        name.str("");
        name<<types.at(j)<<"highProjNum"<<it;
        (high2ds.at(currentIndex))->ProjectionX(name.str().c_str(),QDCMIN,-1);
        highProj.emplace_back((TH1D*)gROOT->FindObject(name.str().c_str()));

        name.str("");
        name<<types.at(j)<<"gateProjNum"<<it;
        (gate2ds.at(currentIndex))->ProjectionX(name.str().c_str(),QDCMIN,-1);
        gateProj.emplace_back((TH1D*)gROOT->FindObject(name.str().c_str()));
      }
    }
    // Check for sizes so that we dont subtract a null

    if (gateProj.size()!=highProj.size()|| gateProj.size()!=lowProj.size()||lowProj.size()!=highProj.size()){
      cout<<"Error: Size Mismatch between the 3 vectors of projection pointers. This is fatal"<<endl;
      cout<<"Quitting Root to clear memory."<<endl;
      gApplication->Terminate();
    }

    //making empty histos to hold the "bkg" and the subtraction result
    for (auto it=0;it<NumGammaLines;it++){
      int currentIndex = NumGammaLines*j+it;

      //adds gamma energy to plot title vers just # in TSelector Vector
      std::string s = (gate2ds.at(currentIndex))->GetTitle();
      std::string delimiter1 = "(";
      std::string delimiter2 = "+";
      int sLength = s.find(delimiter2) - s.find(delimiter1);
      std::string token = s.substr(s.find(delimiter1),sLength); // parses on first ( and stops at +

      stringstream name,key;
      name<<"bkg for gamma ray #"<<it<<" "<<token<<")"; //Opening "(" not needed because the s.substr is inclusive for the first term
      key<<types.at(j)<<"bkg"<<it;
      bkgs.emplace_back(new TH1F(key.str().c_str(),name.str().c_str(),2000,0,2000));

      name.str("");
      key.str("");
      name<<"subtraction for gamma ray #"<<it<<" ("<<token<<")";
      key<<types.at(j)<<"subd"<<it;
      subd.emplace_back(new TH1F(key.str().c_str(),name.str().c_str(),2000,0,2000));

    
    }

    for (auto it=0;it<NumGammaLines;it++){
      int currentIndex = NumGammaLines*j+it;
      bkgs.at(currentIndex)->Reset();
      if (types.at(j) == "sh" && it == 0){
        bkgs.at(currentIndex)->Add(lowProj.at(currentIndex));
        bkgs.at(currentIndex)->Add(gateProj.at(currentIndex+1));
      }else {
        bkgs.at(currentIndex)->Add(lowProj.at(currentIndex));
        bkgs.at(currentIndex)->Add(highProj.at(currentIndex));
      }
      subd.at(currentIndex)->Add(gateProj.at(currentIndex));
      subd.at(currentIndex)->Add(bkgs.at(currentIndex),-1);
    }
  }//end type loop


  return 0 ;
}
