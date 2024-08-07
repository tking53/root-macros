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

void CsEffFitter(){

  std::vector<std::string> HagHists={"1203","1204","1205","1206","1207","1208","1209","1210","1329","1330","1331","1332","1333","1334","1336","1344","2300"};

  std::string csvFName = "137CsFull.csv" ;
  //CSV output for import to excel, etc.
  ofstream csvOutput (csvFName);

  //Write Column Header
  csvOutput<<"Histogram#"<<","<<"DetType"<<","<<"Number"<<","<<"Energy"<<","<<"Efficiency (%)"<<","<<"Counts"<<","<<"Count Error (%)"<<","<<"HistSum"<<","<<"FitSum"<<","<<"FWHM"<<"\n";

  TCanvas* c1 = new TCanvas("c1","c1",960,510);

  std::pair<Double_t,Double_t> FitRange =make_pair(600,750); // fitting ranges for HAGRiD 137CsFull

  Double_t RunTime = 16384*4+ 7479; //in seconds gathered from 137CsFull's 1808 this is for the 137cs mtas tape source

  std::pair<Double_t,Double_t> SourceCal = {38.92,1513}; //{Activity, days since calibrated}
  //calibrated activity (nCi), time (days) since cal Source was calibrated on 9 Feb, 2012, data was acquired on April 4 2016 //page 66 in logbook

  Double_t Cs137HalfLDays=  30.08*365.25;//days

  //ln2/half = const
  Double_t CsDConst = log(2)/Cs137HalfLDays;
  Double_t currentSourceActivity = SourceCal.first * exp(-1*CsDConst*SourceCal.second);

  // 661.657 @ 85.10%  ENSDF report for 137Cs line  (37 is to convert nCi to decays/sec) 
  Double_t PeakArea = currentSourceActivity * 37 * 0.851;
  std::pair<Double_t,Double_t> Peak = make_pair(661.657,PeakArea);//center,rate g/s

  Double_t CsFWHMseed = 30.9 ; //inital value for the fwhm from 1204 this should be good enough init for the hagrids


  //LOOP HERE
  for (auto Looper=Histos.begin();Looper != Histos.end(); Looper++){
  TH1* h1 = 0;
  std::string dHisto = "d"+(*Looper);
  _file0->GetObject(dHisto.c_str(),h1);
  h1->GetXaxis()->SetRangeUser(500,800);

  TF1* Csfit = new TF1("csFit","gaus(0)+pol1(3)",FitRange.first,FitRange.second);

  Double_t Amp = ( Peak.second * RunTime) /((CsFWHMseed/2.355) * sqrt(2*TMath::Pi()));
  Csfit->SetParLimits(0,0,Amp*1.25);
  Csfit->SetParLimits(1, 640, 690);
  Csfit->SetParLimits(2, 5,40);
  h1->Fit(Csfit,"R");

  std::vector<Double_t> gausPars;
  std::vector<Double_t> bkgPars;

  gausPars.emplace_back(Csfit->GetParameter(0));
  gausPars.emplace_back(Csfit->GetParameter(1));
  gausPars.emplace_back(Csfit->GetParameter(2));

  bkgPars.emplace_back(Csfit->GetParameter(3));
  bkgPars.emplace_back(Csfit->GetParameter(4));
  

  TF1* gausFit = new TF1("gausFit","gaus(0)",FitRange.first,FitRange.second);
  TF1* bkgFit = new TF1("bkgFit","pol1(0)",FitRange.first,FitRange.second);

  for (auto it=0;it<gausPars.size();it++){
    gausFit->SetParameter(it,gausPars.at(it));
    if (it<2)
    bkgFit->SetParameter(it,bkgPars.at(it));
  }
  Double_t HistSum = 0;
  for (auto it = FitRange.first; it<FitRange.second;it++){
    HistSum += h1->GetBinContent(it);
  }

  gausFit->SetLineColor(kGreen+2);
  bkgFit->SetLineColor(kMagenta+2);
  gausFit->Draw("same");
  bkgFit->Draw("same");

  Double_t gausSum =  gausFit->Integral(FitRange.first,FitRange.second);
  Double_t bkgSum =  bkgFit->Integral(FitRange.first,FitRange.second);

  Double_t FitSum = (gausSum+bkgSum);
  Double_t ErrorPer = abs((FitSum-HistSum)/HistSum)*100;

  //Print relevant Stats
  printf("\nFit Integral   = %g\n", gausSum);
  printf("BKG Integral   = %g\n", bkgSum);

  printf("HistSum   = %g\n", HistSum);
  printf("FitsSum   = %g\n", gausSum+bkgSum);
  printf("\nDifference between Hist and Fit   = %g\n",HistSum-(gausSum+bkgSum));
  printf("Percent Error   = %g% \n",ErrorPer);

  //Parsing Histogram Title to get subtype and Number (should all be std::string's but auto saves me typing time)
  std::string HisTitle = h1->GetTitle();
  auto typeEnd = HisTitle.find_last_of(" ");
  auto typeStart = HisTitle.find_last_of(" ",typeEnd-1);
  auto detFullType = HisTitle.substr(typeStart+1,typeEnd-typeStart-1);
  auto detSubType = detFullType.substr(detFullType.find(":")+1);
  auto detNum = HisTitle.substr(typeEnd+1);

  auto HistoNum = h1->GetName(); //This lets us cross check with the right histogram number. this is needed to make sure that 12-14 and 11 are in the right spots due to them being moved around during the experiment 


  if ( (*Looper) == "2300"){
    detSubType = "smallHAGRiD";
    detNum = "Totals";
  }
  
  //Eff calculation

  Double_t EffPercent = (gausSum / ( Peak.second * RunTime)) * 100;
 

  printf("Efficiency  = %g% \n",EffPercent);

  //add FWHM to output for resolution Plotting
  //sigma -> FWHM = 2.355Sig -> so sigma =FWHM/2.355

  Double_t FWHM = gausFit->GetParameter(2) * 2.355;

  //Write the values in a CSV for import to EXCEL or equivalent (too lazy to figure out TGraph for now)
  //  csvOutput<<"Histogram#"<<","<<"DetType"<<","<<"Number"<<","<<"Energy"<<","<<"Efficiency (%)"<<","<<"Counts"<<","<<"Count Error (%)"<<","<<"HistSum"<<","<<"FitSum\n";
  csvOutput<<fixed<<HistoNum<<","<<detSubType <<","<<detNum <<","<<gausFit->GetParameter(1)<<","<<EffPercent<<","<<gausSum<<","<<ErrorPer<<","<<HistSum<<","<<FitSum<<","<<FWHM<<"\n";

  }
  csvOutput.close();
}



