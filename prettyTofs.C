{
  gStyle->SetPalette(kCherry);
  gStyle->SetOptStat(0);

    TString fname0(_file0->GetName());
  fname0.Remove(fname0.Index(".root"));

  cout<<"fname0= "<<fname0<<endl;
  /*if(fname0 == "094rb_14"){
    std::pair<double,double> window(800,1100);
  }else if (fname0 == "097rb_02_ignored"){
    std::pair<double,double> window(750,900);
  }else{
    std::pair<double,double> window(0,4000);
  }
   */

  //std::pair<double,double> tofproj(750,900); //097rb window
   std::pair<double,double> tofproj(300,500); //094rb window
  

  TCanvas* Tof = new TCanvas("Tof","Tof",900,400);

  /*TCanvas* Tofge = new TCanvas("Tofge","Tofge",900,400);
  TCanvas* Tofnai = new TCanvas("Tofnai","Tofnai",900,400);
  TCanvas* Tofhag = new TCanvas("Tofhag","Tofhag",900,400);
  */ 
  TCanvas* TofGe = new TCanvas("TofGe","TofGe",900,400);
  TCanvas* TofHag = new TCanvas("TofHag","TofHag",900,400);
  TCanvas* TofNai = new TCanvas("TofNai","TofNai",900,400);
  
  TCanvas*  GeTofVHagTof = new TCanvas("GeTofVHagTof","GeTofVHagTof",900,400);
  TCanvas* GeTofVNaiTof = new TCanvas("GeTofVNaiTof","GeTofVNaiTof",900,400);

  /*
  Tofge->SetLogz();
  Tofnai->SetLogz();
  Tofhag->SetLogz();
  
  dd6064->SetMinimum(1);
  dd6065->SetMinimum(1);
  dd6066->SetMinimum(1);
 
  dd6064->GetXaxis()->SetRangeUser(0,2000);
  dd6065->GetXaxis()->SetRangeUser(0,2000); 
  dd6066->GetXaxis()->SetRangeUser(0,2000);
  
   
  Tofge->cd();
  //dd6066->Draw("colz");

  Tofnai->cd();
  //dd6064->Draw("colz");
  
  Tofhag->cd();
  //dd6065->Draw("colz");
*/
  
  Tof->cd();
  Tof->SetLogz();
  TH2D* dd3246R1 = (TH2D*)dd3246->Rebin2D(1,4,"dd3246R1");
  //dd3246R1->SetMinimum(1); //97
  dd3246R1->SetMinimum(3);  //94
  dd3246R1->GetXaxis()->SetRangeUser(500,3000);
  dd3246R1->GetYaxis()->SetRangeUser(0,5000);
  dd3246R1->GetXaxis()->SetTitle("ToF (0.5ns/bin) (1000 bin offset)");
  dd3246R1->GetYaxis()->SetTitle("QDC (arb.)");
  dd3246R1->SetTitle("ToF vs QDC");
  dd3246R1->Draw("colz");

  TofGe->cd();
  TH1D* GeFlash = (TH1D*)dd6066->ProjectionX("_Gflash",230,260);
  TH1D* GeNotFlash = (TH1D*)dd6066->ProjectionX("_NGflash",300,2048);
  GeNotFlash->SetLineColor(kRed);
  GeFlash->Draw();
  GeNotFlash->Draw("same");

  GeNotFlash->GetXaxis()->SetTitle("Energy (keV)");
  GeFlash->GetXaxis()->SetRangeUser(tofproj.first,tofproj.second);
  GeFlash->GetXaxis()->SetRangeUser(tofproj.first,tofproj.second); 
  
  GeFlash->GetYaxis()->SetRangeUser(800,2600);

 
  TofGe->Modified();

  TofHag->cd();
  TH1D* HagFlash = (TH1D*)dd6065->ProjectionX("_Hflash",230,260);
  TH1D* HagNotFlash = (TH1D*)dd6065->ProjectionX("_NHflash",300,2048);
  HagNotFlash->SetLineColor(kRed);
  HagNotFlash->SetTitle("Tof vs. HAGRiD (LaBr3)");
  HagFlash->Draw();
  HagNotFlash->Draw("same");
  HagFlash->SetTitle("Tof vs. HAGRiD (LaBr3)");

  HagNotFlash->GetXaxis()->SetTitle("Energy (keV)");
  HagFlash->GetXaxis()->SetRangeUser(tofproj.first,tofproj.second); //94rb
  
  HagFlash->GetYaxis()->SetRangeUser(2000,8000); //only if displaying Full E range

  
  TofHag->Modified();

  TofNai->cd();
  TH1D* NaiFlash = (TH1D*)dd6064->ProjectionX("_Nflash",230,260);
  TH1D* NaiNotFlash = (TH1D*)dd6064->ProjectionX("_NNflash",300,2048);
  NaiNotFlash->SetLineColor(kRed);
  NaiFlash->Draw();
  NaiNotFlash->Draw("Same");

  NaiNotFlash->GetXaxis()->SetTitle("Energy (keV)");
  NaiFlash->GetXaxis()->SetRangeUser(tofproj.first,tofproj.second); //94rb
  
  NaiFlash->GetYaxis()->SetRangeUser(2500,10000); //only if displaying Full E range
  
  
  TofNai->Modified();


  TH1D *NaiNotFlash2 = (TH1D*) NaiNotFlash->Clone();
  TH1D *GeNotFlash2  = (TH1D*) GeNotFlash->Clone();
  TH1D *HagNotFlash2 = (TH1D*) HagNotFlash->Clone();

  NaiNotFlash2->SetTitle("Gamma Detector Comparisons");
  GeNotFlash2->SetTitle("Gamma Detector Comparisons"); 
  HagNotFlash2->SetTitle("Gamma Detector Comparisons");
  
  GeNotFlash2->SetLineColor(kBlue);
  NaiNotFlash2->SetLineColor(kRed); 
  HagNotFlash2->SetLineColor(kRed); 
  
  GeTofVNaiTof->cd();


  NaiNotFlash2->Draw();
  GeNotFlash2->Draw("same");

  
  GeTofVHagTof->cd();
  
  HagNotFlash2->Draw();
  GeNotFlash2->Draw("same");
 

  
  TImage *Tofimg = TImage::Create();
  TImage *TofGeimg = TImage::Create();
  TImage *TofHagimg = TImage::Create();
  TImage *TofNaiimg = TImage::Create();
  /*
  TImage *Full2DTofGe = TImage::Create();
  TImage *Full2DTofNai = TImage::Create();
  TImage *Full2DTofHag = TImage::Create();
  */
  TImage *NotflashCompGeHag = TImage::Create();
  TImage *NotflashCompGeNai = TImage::Create();
  
    
  Tofimg->FromPad(Tof);
  TofGeimg->FromPad(TofGe);
  TofHagimg->FromPad(TofHag);
  TofNaiimg->FromPad(TofNai);
  /*
  Full2DTofGe->FromPad(TofGe);
  Full2DTofNai->FromPad(TofNai);
  Full2DTofHag->FromPad(TofHag);
  */
  NotflashCompGeHag->FromPad(GeTofVHagTof);
  NotflashCompGeNai->FromPad(GeTofVNaiTof);
  
  
  Tofimg->WriteImage("Tof_"+fname0+".png");
  TofGeimg->WriteImage("TofGe_"+fname0+".png");
  TofHagimg->WriteImage("TofHag_"+fname0+".png");
  TofNaiimg->WriteImage("TofNai_"+fname0+".png");
  /*
  Full2DTofGe->WriteImage("FullTofGe_"+fname0+".png");
  Full2DTofNai->WriteImage("FullTofHag_"+fname0+".png"); 
  Full2DTofHag->WriteImage("FullTofNai_"+fname0+".png");
  */
  NotflashCompGeHag->WriteImage("NonFlashComparisonGeHag_"+fname0+".png");
  NotflashCompGeNai->WriteImage("NonFlashComparisonGeNai_"+fname0+".png");
}
