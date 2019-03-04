{
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  h->Draw("colz");
  c1->SetLogz();
  h->GetXaxis()->SetRangeUser(400,1500);
  h->SetContour(10000);
  h->SetMaximum(1000);
  h->SetMinimum(10);
  h->SetTitle("");
  h->GetXaxis()->SetTitle("TOF (0.5 ns / bin, 500 bin offset)");
  h->GetYaxis()->SetTitle("QDC(arb.)"); 
  h->GetYaxis()->SetTitleOffset(1.5);
}
