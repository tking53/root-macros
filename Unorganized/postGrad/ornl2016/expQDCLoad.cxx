{
  TFile *_file0 = TFile::Open("New97Rb_test.root");
  gROOT->ProcessLine(".L ornlRoot-macros/NeutronCrayon.cxx+");
  gROOT->ProcessLine(".L ornlRoot-macros/expQDCbackground.cxx+");
  // TH2D* TvQ_h = SparseZP(dd_tof_h_qdc_tape,{3,{1,300}},{1,{780,830}},{0,2},true)
  ;
  // TvQ_h->SetName("TvQ_h");
  // expQDCbackground(TvQ_h,{400,600}, {20,20000},{20,140},132,{140,500},{500,2000},{2000,8000});

   // TH2D* TvQ_c = SparseZP(dd_tof_c_qdc_tape,{3,{1,300}},{1,{1,-1}},{0,2},true);
  // TvQ_c->SetName("TvQ_c");
  // // expQDCbackground(TvQ_c,{400,600}, {20,20000},{20,140},132,{140,500},{500,2000},{2000,8000});

  //  TH2D* TvQ_n = SparseZP(dd_tof_n_qdc_tape,{3,{1,300}},{1,{1,-1}},{0,2},true);
  // TvQ_n->SetName("TvQ_n");
  // expQDCbackground(TvQ_n,{400,600}, {20,20000},{20,140},132,{140,500},{500,2000},{2000,8000});


TH2D* TvQ_S = SparseZP(dd_tof_NA_qdc_tape, {3, {1, 300}}, {1, {0, 1}}, {0, 2}, true);
TvQ_S->SetName("TvQ_S");
  expQDCbackground(TvQ_S,{400,600}, {20,20000},{20,140},132,{140,500},{500,2000},{2000,8000});

}
