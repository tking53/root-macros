int PlotSubd(const int gamNum=0){
  auto c1 = new TCanvas("c1","c1");
  gStyle->SetOptStat(0);

  std::string GeHist = "gesubd" + std::to_string(gamNum);
  std::string ShHist = "shsubd" + std::to_string(gamNum);
  
  TH1* ge =0;
  gDirectory->GetObject(GeHist.c_str(),ge);
  TH1* sh =0;
  gDirectory->GetObject(ShHist.c_str(),sh);

  if (ge == NULL || sh == NULL){
    cout<<"Unable to find histogram. Probably Gamma index does not exist "<<endl;
    return 1;
  }
  ShHist += "_Rebin";
  GeHist += "_Rebin";

  //rebin the hists to 2ns bins and store in a new histogram
  sh->Rebin(2,ShHist.c_str());
  ge->Rebin(2,GeHist.c_str());

  //replace existing pointers to new rebined ones
  gDirectory->GetObject(GeHist.c_str(),ge);
  gDirectory->GetObject(ShHist.c_str(),sh);

  ge->SetLineColor(kRed);
  ge->GetXaxis()->SetTitle("Time Of Flight (ns)");
  ge->GetYaxis()->SetTitle("Counts per 2 ns");
  ge->GetXaxis()->SetRangeUser(0,400);
  ge->SetMinimum(0);

  sh->GetXaxis()->SetTitle("Time Of Flight (ns)");
  sh->GetYaxis()->SetTitle("Counts per 2 ns");
  sh->GetXaxis()->SetRangeUser(0,400);
  sh->SetMinimum(0);

  //adds gamma energy to plot title vers just # in TSelector Vector
  std::string s = sh->GetTitle();
  std::string delimiter1 = "(";
  std::string delimiter2 = ")";
  int sLength = s.find(delimiter2) - s.find(delimiter1);
  std::string energy = s.substr(s.find(delimiter1)+2,sLength-2); 
  std::string Title = "Time of Flight Gated on the " + energy  +  " keV Gamma-ray";

  sh->SetTitle(Title.c_str());
  ge->SetTitle(Title.c_str());

  sh->Draw();
  ge->Draw("SAME");

  //ge->Draw();

  auto leg = c1->BuildLegend();
  TList *p = leg->GetListOfPrimitives();
  TIter next(p);
  TObject *obj;
  TLegendEntry *legEn;
  int i = 0;

  while ((obj = next())) {
    legEn = (TLegendEntry*)obj;
    i++;
    if (i==1) legEn->SetLabel("2\" HAGRiD");
    if (i==2) legEn->SetLabel("Clover");

    // if (i==1) legEn->SetLabel("Clover");
  }

  return 0;
}
