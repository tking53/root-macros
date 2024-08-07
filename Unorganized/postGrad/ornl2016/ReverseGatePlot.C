{
  auto c1 = new TCanvas("c1","c1");
  gStyle->SetOptStat(0);
  c1->Divide(1,2);
  c1->cd(1);
  gPad->SetLogy();
  geSinglesBG->SetLineColor(kGreen+2);
  geDelay->SetLineColor(kBlue+3);
  geInBan->SetLineColor(kRed);
  geDelay->SetLineColor(kYellow+2);
  geSingles->Draw();
  geSinglesBG->Draw("same");
  geInBan->Draw("same");
  geDelay->Draw("SAME");

  c1->cd(2);
  gPad->SetLogy();
  shSinglesBG->SetLineColor(kGreen+2);
  shDelay->SetLineColor(kBlue+3);
  shInBan->SetLineColor(kRed);
  shDelay->SetLineColor(kYellow+2);
  shSingles->Draw();
  shSinglesBG->Draw("same");
  shInBan->Draw("same");
  shDelay->Draw("SAME");

  auto c2 = new TCanvas("c2","c2");
  c2->Divide(1,2);
  c2->cd(1);
  gPad->SetLogz();
  shBanTofVsS->GetYaxis()->SetRangeUser(0,1000);
  shBanTofVsS->SetMinimum(1);
  shBanTofVsS->GetXaxis()->SetRangeUser(0,500);
  shBanTofVsS->Draw("COLZ");

  c2->cd(2);
  gPad->SetLogz();
  geBanTofVsS->GetYaxis()->SetRangeUser(0,1000);
  geBanTofVsS->SetMinimum(1);
  geBanTofVsS->GetXaxis()->SetRangeUser(0,500);
  geBanTofVsS->Draw("COLZ");

}



