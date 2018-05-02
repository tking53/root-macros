#include "TH1.h"
#include "TF1.h"
#include "vector"
#include "TMath.h"
#include <cmath>


void EuFitHag(){

  Double_t npeaks = 5;
 
  std::vector<Double_t> E0 = {60.0,86.5,105.3,121.8,123.1};
  std::vector<Double_t> A = {67.,1898.,1324.,1799.,2785.};
  std::vector<Double_t> Sigs;
  //Double_t par[npeaks*3];
 
  std::vector<Double_t> par;

  // TH1* h1 = (TH1*)gDirectory->GetList()->FindObject("d1203");
  TH1* h1 = 0;
  _file0->GetObject("d1205",h1);

  TH1D* hF = new TH1D("hF","Fitting Sum",200,0,200);

  for (int i =0; i < npeaks; i ++){
    Double_t sigma = 1/2.35 * (0.009136*E0.at(i) + 22.382597);
    Sigs.emplace_back(sigma);

    Double_t firstP = A.at(i)/(sigma * sqrt(2*TMath::Pi()) );

    par.emplace_back(firstP);
    par.emplace_back(E0.at(i));
    par.emplace_back(sigma);

    }

  std::vector<Double_t> bkg; //increasing power of x (like the cfg)
  // bkg.emplace_back(-16828);
  // bkg.emplace_back(937);
  // bkg.emplace_back(-6.48);
  // bkg.emplace_back(0.0132);

  par.emplace_back(-3141);
  par.emplace_back(174);
  par.emplace_back(-1.2);
  par.emplace_back(0.0024);

  TF1* hi0 = new TF1("hi0","gaus",55,E0.at(0)+Sigs.at(0));
  TF1* hi1 = new TF1("hi1","gaus",E0.at(1)-Sigs.at(1),E0.at(1)+Sigs.at(1));
  Tf1* hi2 = new TF1("hi2","gaus",E0.at(2)-Sigs.at(2),E0.at(2)+Sigs.at(2));
  Tf1* hi3 = new TF1("hi3","gaus",E0.at(3)-Sigs.at(3),E0.at(3)+Sigs.at(3));
  Tf1* hi4 = new TF1("hi4","gaus",E0.at(4)-Sigs.at(4),E0.at(4)+Sigs.at(4));
  Tf1* hi5 = new TF1("hi5","gaus",E0.at(5)-Sigs.at(5),E0.at(5)+Sigs.at(5));
  Tf1* hiB = new TF1("hiB","pol3",55,160);

  TF1* total =  new TF1("total","gaus(0)+gaus(3)+gaus(6)+gaus(9)+gaus(12)+pol3(15)",55,160);

  for (int ParNum=0 ; ParNum < par.size(); ParNum++){
    total->SetParameter(ParNum,par.at(ParNum));
  }

  for (int parNum = 0 ;parNum<=(npeaks-1); parNum++){
    total->SetParLimits(parNum*3+2,Sigs.at(parNum)-10,Sigs.at(parNum)+10);
    total->SetParLimits(parNum*3+1,E0.at(parNum)-10,E0.at(parNum)+10);
  }
  
  // total->SetParLimits(par.size(),par.at(par.size())-2,par.at(par.size()+2));
  // total->SetParLimits(par.size()-1,par.at(par.size()-1)-2,par.at(par.size()-1)+2 );
  
  h1->Fit(total,"R");

}
//  Parameter order for gauss const/cent/sig
//Area =  const* sigma * sqrt(2*pi)
