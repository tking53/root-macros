#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TCutG.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>


void rotate2DN(const char* filename,int detNum){


  TFile *f = new TFile(filename);

  TCanvas* cnew= new TCanvas("cnew","",800.,500.);
  cnew->cd();
   

  stringstream sss;
  sss<<"Transposed-"<<filename;
  TFile f1(sss.str().c_str(),"RECREATE");

  for (int i=0;i<detNum;i++){
    stringstream orig ;
    orig<<"hNc"<<i ;
    TH2D* h1=(TH2D*)f->Get(orig.str().c_str());
    
    string gname = h1->GetTitle();
    stringstream hname ;
    hname<<"hN"<<i;
    TH2D* Hnew = new TH2D(hname.str().c_str(),gname.c_str(),1700.,0.,1700.,8000.,0.,8000.);
    for (int x=0;x<8000;x++){
      for (int y=0;y<1600;y++){
 	int bin = h1->GetBinContent(x,y);
	Hnew->Fill(y,x,bin);
      }
    }
    cout<<"Histogram Number "<<i<<" transpose completed"<<endl; 
    cnew->cd();
    
    Hnew->SetMinimum(7);
    Hnew->GetYaxis()->SetRangeUser(1300,1600);
    Hnew->GetYaxis()->SetTitle("Energy (keV)");
    Hnew->GetXaxis()->SetTitle("Cycle Number");
    Hnew->Draw("COLZ");

    TCutG *cutg = new TCutG("NaIcut1",5);
    cutg->SetVarX("y");
    cutg->SetVarY("x");
    cutg->SetPoint(0,0,1350);
    cutg->SetPoint(1,1500,1350);
    cutg->SetPoint(2,1500,1500);
    cutg->SetPoint(3,0,1500);
    cutg->SetPoint(4,0,1350);
     
    TProfile *profx = Hnew->ProfileX(Form("profx_%d",i),1,-1,"[NaIcut1]");

    profx->SetLineColor(kRed);   
    profx->Draw("same");
    cnew->Update();

    gname = gname + ".pdf";
    cnew->SaveAs(gname.c_str());

  }
    f1.Write();
 
}
