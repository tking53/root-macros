#include <TH1.h>
#include <TF1.h>
#include <TLinearFitter.h>
#include <TRandom3.h>
#include <TGraph.h>
#include <TGraphErrors.h>

#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

int gfit(const std::string id="",const Double_t peak=-1 ,const Double_t lowBound=-1,const Double_t highBound=-1){

  if (id == "" || peak == -1 || lowBound == -1 || highBound == -1){
    cout<<"you forgot an argument."<<endl<<"order is <hisName> <peak> <low bound> <high bound>"<<endl;
    return 1;
  }

  std::pair<Double_t,Double_t> FitRange = {lowBound,highBound};
  TH1* hist = (TH1*)gROOT->FindObject(id.c_str());
  hist->GetXaxis()->SetRangeUser(FitRange.first-10,FitRange.second+10);
  hist->Draw();

  Double_t HistSum = 0;
  for (auto it = FitRange.first; it<FitRange.second;it++){
    HistSum += hist->GetBinContent(it);
  }


  Double_t initAmp = hist->GetBinContent(peak);

  TF1* FirstFit = new TF1("FirstFit","gaus(0)",FitRange.first,FitRange.second);
  hist->Fit(FirstFit,"QRO");

  TF1* Tfit = new TF1("TFit","gaus(0)+pol1(3)",FitRange.first,FitRange.second);

  Double_t slope = (hist->GetBinContent(FitRange.second)-hist->GetBinContent(FitRange.first))/(FitRange.second-FitRange.first);

  Tfit->SetParameter(0,initAmp);
  Tfit->SetParameter(1,peak);
  Tfit->SetParameter(2,FirstFit->GetParameter(2));
  //  Tfit->SetParameter(3,(-1*slope*FitRange.first)+hist->GetBinContent(FitRange.first));
  Tfit->SetParameter(4,slope);

  //  m(x-FitRange.first)+FitRangconst Double_t peak=-1 ,const Double_t lowBound=-1,const Double_t highBound=-1e.second

  Int_t fitStatus =  hist->Fit(Tfit,"QRO");

  if (fitStatus != 0 )
    cout<<"Unknown Error occured. The Fit() returned a status of "<<fitStatus<<". Please check Root Documentation (TH1 Fit()) for details"<<endl;

  std::vector<Double_t> gausPars;
  std::vector<Double_t> bkgPars;

  gausPars.emplace_back(Tfit->GetParameter(0));
  gausPars.emplace_back(Tfit->GetParameter(1));
  gausPars.emplace_back(Tfit->GetParameter(2));

  bkgPars.emplace_back(Tfit->GetParameter(3));
  bkgPars.emplace_back(Tfit->GetParameter(4));
  
  TF1* gausFit = new TF1("gausFit","gaus(0)",FitRange.first,FitRange.second);
  TF1* bkgFit = new TF1("bkgFit","pol1(0)",FitRange.first,FitRange.second);

  for (unsigned it=0;it<gausPars.size();it++){
    gausFit->SetParameter(it,gausPars.at(it));
    if (it<2)
      bkgFit->SetParameter(it,bkgPars.at(it));
  }
  bkgFit->SetLineColor(kGreen+2);
  bkgFit->SetLineWidth(2);
  bkgFit->SetLineStyle(2);
  bkgFit->Draw("same");

   Double_t gausSum =  gausFit->Integral(FitRange.first,FitRange.second);
   Double_t bkgSum =  bkgFit->Integral(FitRange.first,FitRange.second);

  Double_t FitSum = (gausSum+bkgSum);
  Double_t ErrorPer = abs((FitSum-HistSum)/HistSum)*100;
 

  printf("\n\nPeak Center = %f",Tfit->GetParameter(1));
  printf("\nPeak ONLY Integral   = %f", gausSum);
  printf("\nFWHM = %f",2.634*Tfit->GetParameter(2));
  printf("\nDifference between Hist and Fit   = %f",HistSum-FitSum);
  printf("\nPercent Error   = %f \%\n\n\n",ErrorPer);

  // printf("HistSum   = %f\n", HistSum);
  // printf("FitsSum   = %f\n", FitSum);




  return 0;

}
