{
TCanvas* c2 = new TCanvas("c2","c2");
  tof0->SetMinimum(1);
  tof1->SetMinimum(1);
  tof2->SetMinimum(1);
  tof3->SetMinimum(1);
  tof4->SetMinimum(1);
  tof5->SetMinimum(1);
  tof6->SetMinimum(1);
  tof7->SetMinimum(1);


  c2->Divide(3,3);
  
  c2->cd(1);
  gPad->SetLogz();
  tof0->Draw("COLZ");
  
  c2->cd(2);
  gPad->SetLogz();
  tof1->Draw("COLZ");
  
  c2->cd(3);
  gPad->SetLogz();
  tof2->Draw("COLZ");
  
  c2->cd(4);
  gPad->SetLogz();
  tof3->Draw("COLZ");
  
  c2->cd(5);
  gPad->SetLogz();
  tof4->Draw("COLZ");
  
  c2->cd(6);
  gPad->SetLogz();
  tof5->Draw("COLZ");
  
  c2->cd(7);
  gPad->SetLogz();
  tof6->Draw("COLZ");

  c2->cd(8);
  gPad->SetLogz();
  tof7->Draw("COLZ");



}
