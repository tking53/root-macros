#include "TAxis.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TChain.h"
#include "TStyle.h"
#include "TTree.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TPaveStats.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TSelector.h"
#include "TProofServ.h"
#include "TProof.h"
#include "TROOT.h"
#include <climits>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <vector>
int cutStep_qdc=1000;
int minQDC=10;
int maxQDC=20000;
int qdc_bins=20000;

int cutStep_tof=10;
int minTOF=0;
int maxTOF=1500;
int tof_bins=3000;
TF1 *background_TOF_fit;
TCanvas* canvas;
// double minQDC_expFunc0=200;
// double maxQDC_expFunc0=2000;
// double minQDC_expFunc1=3000;
// double maxQDC_expFunc1=15000;
// double minQDC_background_TOF_fit=100;
// double maxQDC_background_TOF_fit=20000;


double EXP1(double* xx, double* par){
  double amp1 = par[0];
  double lambda1 = par[1];
  double mean1= par[2];
  double x = xx[0];
  double result1 = amp1*TMath::Exp(lambda1*(x-mean1)); // Xu's
  return (result1);
  
}
double EXP2(double* xx, double* par){
  double amp2 = par[0];
  double lambda2 = par[1];
  double mean2= par[2];
  double x = xx[0];
  double result2 = amp2*TMath::Exp(lambda2*(x-mean2)); // Xu's
  return (result2);
 
}
double GAUS1(double* xx, double* par){
  double gconst = par[0];
  double gmean = par[1];
  double gsigma= par[2];
  // double g3 = par[3];
  double x = xx[0];
  if (x<gmean){
    return 0;
  }else {
    return gconst * (1 - TMath::Gaus(x,gmean,gsigma,kFALSE));
  }
}

double GausExpExpLin(double* xx, double* par){
  double gconst = par[0];
  double gmean = par[3];
  double gsigma= par[2];

  double amp1 = par[4];
  double lambda1 = par[5];

  double amp2 = par[6];
  double lambda2 = par[7];

  double amp3 = gconst - amp2 - amp1;
  double lambda3 = par[8];

  // double linSlope = par[7];
  // double linInt = par[8];
  
  double x = xx[0];
  if (x<=par[3]){
    return GAUS1(xx,par);
  }else {
    double result1 = amp1*TMath::Exp(lambda1*(x-gmean)); // Xu's
    double result2 = amp2*TMath::Exp(lambda2*(x-gmean)); // Xu's
    double result3 = amp3*TMath::Exp(lambda3*(x-gmean)); // Xu's
    //double result1 = TMath::Exp(amp1*x+lambda1) ; //Jeremy's
    //double result2 = TMath::Exp(amp2*x+lambda2) ; //Jeremy's

    // double result3 = (linSlope*x)+linInt;
    return (result1+result2+result3);
  }

}

void expQDCbackground(TH2* hist, pair<Int_t,Int_t> tofRange,  pair<Int_t,Int_t> fullFit = {20,20000},  pair<Int_t,Int_t> expFit0 = {20,250},Int_t expFit0C = 120, pair<Int_t,Int_t> expFit1 = {200,2000}, pair<Int_t,Int_t> expFit2 = {2000,12000},pair<Int_t,Int_t> expFit3 = {12000,20000}){

  double minQDC_expFunc0=expFit0.first;
  double maxQDC_expFunc0=expFit0.second;

  double minQDC_expFunc1=expFit1.first;
  double maxQDC_expFunc1=expFit1.second;

  double minQDC_expFunc2=expFit2.first;
  double maxQDC_expFunc2=expFit2.second;

  double minQDC_expFunc3=expFit3.first;
  double maxQDC_expFunc3=expFit3.second;

  double minQDC_background_TOF_fit=fullFit.first;
  double maxQDC_background_TOF_fit=fullFit.second;

  
  if ((TCanvas*)gDirectory->Get("c1")) {
    canvas = (TCanvas*)gDirectory->Get("c1");
  } else {
    canvas = new TCanvas("c1","c1");
   
  }

  TH1D* qdcbackground_ = hist->ProjectionY("qdcbackground_",hist->GetXaxis()->FindBin(tofRange.first),hist->GetXaxis()->FindBin(tofRange.second));
  // qdcbackground_->Rebin(10);
  qdcbackground_->SetLineColor(kBlack);
  qdcbackground_->SetLineWidth(2);
  qdcbackground_->GetXaxis()->SetRangeUser(50,20000);
  //  canvas->SetLogy();
  qdcbackground_->Draw();

  // TF1 *expFunc0 = new TF1("expFunc0","gaus",minQDC_expFunc0,maxQDC_expFunc0);
  // expFunc0->SetParameter(1,expFit0C);
  // expFunc0->SetParLimits(1,expFit0C-10,expFit0C+10);

  // expFunc0->SetLineColor(kRed);expFunc0->SetLineWidth(4);
  // TF1 *expFunc1 = new TF1("expFunc1","exp([1]*x+[0])",minQDC_expFunc1,maxQDC_expFunc1);
  // expFunc1->SetLineColor(kGreen);expFunc1->SetLineWidth(4);
  // TF1 *expFunc2 = new TF1("expFunc2","exp([1]*x+[0])",minQDC_expFunc2,maxQDC_expFunc2);
  // expFunc2->SetLineColor(kBlue);expFunc2->SetLineWidth(4);
  // TF1 *expFunc3 = new TF1("expFunc3","pol1",minQDC_expFunc3,maxQDC_expFunc3);
  // expFunc3->SetLineColor(kMagenta);expFunc3->SetLineWidth(4);

  // qdcbackground_->Fit("expFunc0","R+");
  // qdcbackground_->Fit("expFunc1","R+");
  // qdcbackground_->Fit("expFunc2","R+");
  // qdcbackground_->Fit("expFunc3","R+");

  // expFunc0->Draw("same");
  // expFunc1->Draw("same");
  // expFunc2->Draw("same");
  // expFunc3->Draw("same");

  TF1 *expFunc0 = new TF1("expFunc0","GAUS1",minQDC_expFunc0,maxQDC_expFunc0,3);
  TF1 *expFunc1 = new TF1("expFunc1","EXP1",minQDC_expFunc1,maxQDC_expFunc1,3);
  TF1 *expFunc2 = new TF1("expFunc2","EXP1",minQDC_expFunc2,maxQDC_expFunc2,3);
  TF1 *expFunc3 = new TF1("expFunc3","EXP1",minQDC_expFunc3,maxQDC_expFunc3,3);

  expFunc0->SetParameter(0,qdcbackground_->GetMaximum());
  expFunc0->SetParameter(1,0);
  expFunc0->SetParameter(2,75);

  expFunc1->SetParameter(0,qdcbackground_->GetMaximum());
  expFunc1->SetParameter(1,(Double_t)-2e-3);
  expFunc1->FixParameter(2,qdcbackground_->GetBinCenter(qdcbackground_->GetMaximumBin()));

  expFunc2->SetParameter(0,0.1*qdcbackground_->GetMaximum());
  expFunc2->SetParameter(1,(Double_t)-2e-4);
  expFunc2->FixParameter(2,qdcbackground_->GetBinCenter(qdcbackground_->GetMaximumBin()));


  expFunc3->SetParameter(0,0.01*qdcbackground_->GetMaximum());
  expFunc3->SetParameter(1,(Double_t)-2e-5);
  expFunc3->FixParameter(2,qdcbackground_->GetBinCenter(qdcbackground_->GetMaximumBin()));


  qdcbackground_->Fit("expFunc0","R+");
  qdcbackground_->Fit("expFunc1","R+");
  qdcbackground_->Fit("expFunc2","R+");
  qdcbackground_->Fit("expFunc3","R+");
  // expFunc0->Draw("same");
  // expFunc1->Draw("same");
  // expFunc2->Draw("same");
  // expFunc3->Draw("same");


  // //normal gaus
  TF1 *myF = new TF1("myF",GausExpExpLin,minQDC_background_TOF_fit,maxQDC_background_TOF_fit,9);
  myF->SetNpx(1000);

  myF->SetParameter(0,expFunc0->GetParameter(0));
  // double low = expFunc0->GetParameter(0)*0.05 - expFunc0->GetParameter(0);
  // double high = expFunc0->GetParameter(0)*0.05 + expFunc0->GetParameter(0);
  // myF->SetParLimits(0,low,high);
  myF->FixParameter(1,expFunc0->GetParameter(1));
  myF->SetParameter(2,expFunc0->GetParameter(2));
  // myF->SetParLimits(1,100,expFunc0->GetParameter(1)+20);

  myF->FixParameter(3,qdcbackground_->GetBinCenter(qdcbackground_->GetMaximumBin()));

  myF->SetParameter(4,expFunc1->GetParameter(0)*0.55);
  myF->SetParameter(5,expFunc1->GetParameter(1));

  myF->SetParameter(6,expFunc2->GetParameter(0)*0.35);
  myF->SetParameter(7,expFunc2->GetParameter(1));

  myF->SetParameter(8,expFunc3->GetParameter(1));

  

  cout<<endl<<"Starting total fit"<<endl<<endl;

  qdcbackground_->Fit("myF","RL");
  myF->SetLineColor(kViolet-2);
  myF->SetLineWidth(2);
  myF->Draw("SAME");
  
  // background_TOF_fit = new TF1("background_TOF_fit","expFunc0+expFunc1+expFunc2",minQDC_background_TOF_fit,maxQDC_background_TOF_fit);
  // background_TOF_fit->SetLineColor(kViolet);
  // background_TOF_fit->SetLineWidth(5);

  // cout<<"1"<<endl;
  // double firstexp = (expFunc1->Eval(expFunc0->GetParameter(1)));
  // double upperConst = ((firstexp-expFunc0->GetParameter(0)))+expFunc0->GetParameter(0);
  // cout<<"2"<<endl;
  // //gaus height
  // background_TOF_fit->SetParameter(4,expFunc0->GetParameter(0)-2*firstexp);
  // background_TOF_fit->SetParLimits(4,0,expFunc0->GetParameter(0)+100);
  // cout<<"3"<<endl;
  // //center
  // background_TOF_fit->FixParameter(5,expFunc0->GetParameter(1));
  // // background_TOF_fit->SetParLimits(5,0,expFunc0->GetParameter(1)+1);
  // cout<<"4"<<endl;
  // //sigma
  // background_TOF_fit->SetParameter(6,expFunc0->GetParameter(2)-1);
  // background_TOF_fit->SetParLimits(6,0,expFunc0->GetParameter(2));
  // cout<<"5"<<endl;

  // qdcbackground_->Fit("background_TOF_fit","RL");
  // cout<<"6"<<endl;
  // background_TOF_fit->Draw("same");
  // qdcbackground_->GetXaxis()->SetRangeUser(50,2000);
  // canvas->Modified();
  // if ((TH2*)gDirectory->Get("expBackGround")) {
  //   ((TH2*)gDirectory->Get("expBackGround"))->Delete();
  // } 
  TH2D* expBackGround = new TH2D("expBackGround","expBackGround",tof_bins,0,1500,qdc_bins,0,maxQDC);

  for (int tof_bin = 0; tof_bin < hist->GetNbinsX(); tof_bin++) {
    TH1D* qdc_Proj = hist->ProjectionY("qdc_Proj",tof_bin,tof_bin);
    // qdc_Proj->Rebin(10);
    for (int qdc_bin = 6; qdc_bin < qdc_Proj->GetNbinsX(); qdc_bin++) {
      expBackGround->SetBinContent(tof_bin,qdc_bin,myF->Eval(qdc_Proj->GetBinCenter(qdc_bin)));

  //     //   if(tof_bin==0)
  //     //   cout<<"Bin= "<<qdc_bin<<"       Center= "<<qdc_Proj->GetBinCenter(qdc_bin)<<"     Value="<<background_TOF_fit->Eval(qdc_Proj->GetBinCenter(qdc_bin))<<endl;
    }
  }
  //expBackGround->Scale((Double_t)1/(Double_t)100);

  // TH2D* t1 = (TH2D*)hist->Clone();
  // t1->SetName("t1");
  // // t1->RebinY(10);
  // // t1->RebinX(2);
  // t1->Add(expBackGround,-1);
  // t1->SetMinimum(1);
  // t1->SetMaximum(1200);
  // t1->Draw("colz");
  //for (int i =0 ;i<6;i++){cout<<myF->GetParameter(i)<<endl;}
}
