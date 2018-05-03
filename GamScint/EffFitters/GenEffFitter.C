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

#include "EffSourceInfo.hpp"
#include "His2RootIDs.hpp"

using namespace std;

//Update here to change sources 
using namespace EffSources::CsTape::Cs137;
// using namespace EffSources::CoTape::Co60;
// using namespace EffSources::BaEff::Ba133;
// using namespace EffSources::MixedEu::Eu152;
// using namespace EffSources::MixedEu::Eu154;

void GenEffFitter(const std::string outFile = "GeneralEffFitterOut"){


  //SETUP -------------------------------------------------------------------------------------

  //This should be set by the 'using namespace' line above
  auto RunTime = RunTime_;

  //Choose a detector type to use
  auto Histos = hagrid::Hists; // get the list of HAGRiD DAMM ids
  // auto Histos = nai::Hists; // get the list of nai DAMM ids 

  //Update for which line to fit 
  auto FitRange = Line1::HagFitRange_;
  auto Peak = Line1::PeakInfo_;

  //additionally here swap detecotor types
  auto FWHMseed = Line1::HagFHWMseed_;
  //auto FWHMseed = Line1::NaiFHWMseed_;

 

  //BELOW here is the actual fitting and outut ---------------------------------------------------


  //CSV output for import to excel, etc.
  std::string csvFName = outFile + ".csv";
  ofstream csvOutput (csvFName, std::ofstream::out | std::ofstream::app); //open csv file to write (appending if already exists)

  ifstream csvTest (csvFName);
  if (csvTest.peek() == std::ifstream::traits_type::eof()){
  //Write Column Header
  csvOutput<<"Histogram#"<<","<<"DetType"<<","<<"Number"<<","<<"Energy"<<","<<"Efficiency (%)"<<","<<"Counts"<<","<<"Count Error (%)"<<","<<"HistSum"<<","<<"FitSum"<<","<<"FWHM"<<"\n";
  }

  TCanvas* c1 = new TCanvas("c1","c1",960,510);

  //LOOP over Histogram list
  for (auto Looper=Histos.begin();Looper != Histos.end(); Looper++){
  TH1* h1 = 0;
  std::string dHisto = "d"+(*Looper);
  _file0->GetObject(dHisto.c_str(),h1);
  h1->GetXaxis()->SetRangeUser(500,800);

  //gfit in damm uses a linear background estimation
  TF1* EPfit = new TF1("EffPeakFit","gaus(0)+pol1(3)",FitRange.first,FitRange.second);

  //estimate the 4Pi Amplitude of the Gaussian for limiting the fit 
  Double_t Amp = ( Peak.second * RunTime) /((FWHMseed/2.355) * sqrt(2*TMath::Pi()));

  EPfit->SetParameter(1,Peak.first);//give initial value for the centroid 
  EPfit->SetParameter(2,FWHMseed/2.355); //give initial Gaus sigma 

  // Setting fitting limits for the Combo fit
  EPfit->SetParLimits(0,1,Amp); //amp must be greater than 0 and cant be greater than the 4pi amp
  EPfit->SetParLimits(1, Peak.first-FWHMseed, Peak.first+FWHMseed); //centroid shouldnt be too far off from the literature value for the energy (should be centerish)
  EPfit->SetParLimits(2, (FWHMseed/2.355)-5, (FWHMseed/2.355)+5); //Sigma (from the Gaussian) limiting since this is ~1/2 the FWHM  


  //FINALLY its time to fit
  h1->Fit(EPfit,"R");


  // get the components of the fit from the combined one.
  std::vector<Double_t> gausPars;
  std::vector<Double_t> bkgPars;

  gausPars.emplace_back(EPfit->GetParameter(0));
  gausPars.emplace_back(EPfit->GetParameter(1));
  gausPars.emplace_back(EPfit->GetParameter(2));

  bkgPars.emplace_back(EPfit->GetParameter(3));
  bkgPars.emplace_back(EPfit->GetParameter(4));
  
  //create 2 new fits 1 for bkg and one for the Gaussian
  TF1* gausFit = new TF1("gausFit","gaus(0)",FitRange.first,FitRange.second);
  TF1* bkgFit = new TF1("bkgFit","pol1(0)",FitRange.first,FitRange.second);

  //Load up the parameters. NOTE: we are not constraining these because we will not be actually fitting them 
  for (auto it=0;it<gausPars.size();it++){
    gausFit->SetParameter(it,gausPars.at(it));
    if (it<2)
    bkgFit->SetParameter(it,bkgPars.at(it));
  }

  //Grab the Actual amount of counts in the fit range
  Double_t HistSum = 0;
  for (auto it = FitRange.first; it<FitRange.second;it++){
    HistSum += h1->GetBinContent(it);
  }


  //Doodling time
  gausFit->SetLineColor(kGreen+2);
  bkgFit->SetLineColor(kMagenta+2);
  gausFit->Draw("same");
  bkgFit->Draw("same");

  //Calculate the integrals of the fits and sum them, and compare to whats really in the histogram
  Double_t gausSum =  gausFit->Integral(FitRange.first,FitRange.second);
  Double_t bkgSum =  bkgFit->Integral(FitRange.first,FitRange.second);

  Double_t FitSum = (gausSum+bkgSum);
  Double_t ErrorPer = abs((FitSum-HistSum)/HistSum)*100; //error between the area of the integrals and the sum of the histogram

  //Eff calculation ((what we see)/(4Pi total))* 100%
  Double_t EffPercent = (gausSum / ( Peak.second * RunTime)) * 100;
 
  //add FWHM to output for resolution Plotting
  //sigma -> FWHM = 2.355Sig -> so sigma =FWHM/2.355
  Double_t FWHM = gausFit->GetParameter(2) * 2.355;



  //NOTE these are separate from the fit results which always print
  //Print relevant Stats
  printf("\nFit Integral   = %g\n", gausSum);
  printf("BKG Integral   = %g\n", bkgSum);
  printf("HistSum   = %g\n", HistSum);
  printf("FitsSum   = %g\n", gausSum+bkgSum);
  printf("\nDifference between Hist and Fit   = %g\n",HistSum-(gausSum+bkgSum));
  printf("Percent Error   = %g% \n",ErrorPer);
  printf("Efficiency  = %g% \n",EffPercent);


  //Parsing Histogram Title to get subtype and Number (should all be std::string's but auto saves me typing time)
  std::string HisTitle = h1->GetTitle(); //this one cant be auto because GetTitle give char*
  auto typeEnd = HisTitle.find_last_of(" ");
  auto typeStart = HisTitle.find_last_of(" ",typeEnd-1);
  auto detFullType = HisTitle.substr(typeStart+1,typeEnd-typeStart-1);
  auto detSubType = detFullType.substr(detFullType.find(":")+1);
  auto detNum = HisTitle.substr(typeEnd+1);

  auto HistoNum = h1->GetName(); //GetName returns "d"+ DAMM ID: This lets us cross check with the right histogram number. this is needed to make sure that 12-14 and 11 are in the right spots due to them being moved around during the experiment 

  //set the type/num for the Totals spectra
  if ( (*Looper) == "2300"){
    detSubType = "smallHAGRiD";
    detNum = "Totals";
  }


  //Write the values in a CSV for import to EXCEL or equivalent (too lazy to figure out TGraph for now)
  //  csvOutput<<"Histogram#"<<","<<"DetType"<<","<<"Number"<<","<<"Energy"<<","<<"Efficiency (%)"<<","<<"Counts"<<","<<"Count Error (%)"<<","<<"HistSum"<<","<<"FitSum\n";
  csvOutput<<fixed<<HistoNum<<","<<detSubType <<","<<detNum <<","<<gausFit->GetParameter(1)<<","<<EffPercent<<","<<gausSum<<","<<ErrorPer<<","<<HistSum<<","<<FitSum<<","<<FWHM<<"\n";

  }
  csvOutput.close(); //close the file when we are done writing 

  gApplication->Terminate(); //quit root so that this can be shell scripted 

}



