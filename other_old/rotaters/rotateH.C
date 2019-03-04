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


void rotate2DH(const char* filename,int detNum){
  TFile *f = new TFile(filename);
  TCanvas* cnew= new TCanvas("cnew","",800.,500.);
  cnew->cd();
   
  stringstream sss;
  sss<<"Transposed-"<<filename;
  TFile f1(sss.str().c_str(),"RECREATE");

  for (int i=0;i<detNum;i++){
    stringstream orig ;
    orig<<"hHc"<<i ;
    TH2D* h1=(TH2D*)f->Get(orig.str().c_str());

    string gname = h1->GetTitle();
    stringstream hname;

    hname<<"hH"<<i;
    
    cout<<"i= "<<i<<endl;
    cout<<"hname= "<<hname.str().c_str()<<endl;
 
    
    TH2D* Hnew = new TH2D(hname.str().c_str(),gname.c_str(),1700.,0.,1700.,8000.,0.,8000.);
    for (int x=0;x<8000;x++){
      for (int y=0;y<1700;y++){
    	int bin = h1->GetBinContent(x,y);
    	Hnew->Fill(y,x,bin);
      }
    }
    cout<<"Histogram Number "<<i<<" transpose completed"<<endl; 
    cnew->cd();
    
    Hnew->SetMinimum(7);
    Hnew->GetYaxis()->SetRangeUser(700,1000);
    Hnew->GetYaxis()->SetTitle("Energy (keV)");
    Hnew->GetXaxis()->SetTitle("Cycle Number");
    Hnew->Draw("COLZ");

    TCutG *cutg = new TCutG("Hagcut1",5);
    cutg->SetVarX("y");
    cutg->SetVarY("x");
    cutg->SetPoint(0,0,760);
    cutg->SetPoint(1,1500,760);
    cutg->SetPoint(2,1500,880);
    cutg->SetPoint(3,0,880);
    cutg->SetPoint(4,0,760);
    
    
    TProfile *prof = Hnew->ProfileX(Form("prof_%d",i),1,-1,"[Hagcut1]");
   
    prof->SetLineColor(kRed);   
    prof->Draw("same");
    cnew->Update();
    gname = gname + ".pdf";
    cnew->SaveAs(gname.c_str());
    
  }
f1.Write();
 
}
