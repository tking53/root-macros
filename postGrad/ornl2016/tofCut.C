{
  gStyle->SetPalette(1);
  gStyle->SetNumberContours(999);
  new TCanvas;
 
  c1->Divide(2,2);

  c1->cd(1);
  gPad->SetLogz();
  geGatedTof->SetContour(999);
  geGatedTof->Draw("COLZ");
  geGatedTof->SetMinimum(1);
  
  c1->cd(2);
  gPad->SetLogz();
  geTofHigh->SetContour(999);
  geTofHigh->Draw("COLZ");
  geTofHigh->SetMinimum(1);
  
  c1->cd(3);
  gPad->SetLogz();
  geTofLow->SetContour(999);
  geTofLow->Draw("COLZ");
  geTofLow->SetMinimum(1);
  
  c1->cd(4);
  gPad->SetLogy();
  geTofHigh->ProjectionX("highPj",0,-1);
  geTofLow->ProjectionX("lowPj",0,-1);
  geGatedTof->ProjectionX("gatePj",0,-1);
  
  highPj->Draw();
  lowPj->Draw("SAME");
  gatePj->Draw("SAME");
  //lowPj->SetLineColor(kRed);
  //highPj->SetLineColor(kGreen+2);
  c1->Modified();
}
