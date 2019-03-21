{
 
    TCanvas* c1 = new TCanvas;
    c1->SetLogz();
    c1->Divide(2,3);
    
    for (int it=1;it<6;it++){
        stringstream ss;
        ss<<"v_tq_g"<<it<<";1"<<endl;
        TH2* hh = ((TH2*) gFile->Get(ss.str().c_str()));
        hh->GetXaxis()->SetRangeUser(0,1500);
        hh->GetYaxis()->SetRangeUser(0,1500);
        if(it == 5 )
            hh->SetMinimum(1);
        else
            hh->SetMinimum(2);

        c1->cd((Int_t)it);
        gPad->SetLogz();
        // hh->Draw("COLZ"); 
    }
    TCanvas* c2 = new TCanvas;
    c2->SetLogy();
    //    c2->Divide(2,2);
    
    for (int it=0;it<4;it++){
        stringstream ss;
        ss<<"cloverChan"<<it<<";1"<<endl;
        TH1* h = ((TH1*) gFile->Get(ss.str().c_str()));
        h->GetXaxis()->SetRangeUser(600,900);

        enum EColor color;
        switch (it){
        case 0: color=kRed;break;
        case 1: color=kGreen;break;
        case 2: color=kBlue;break;
        case 3: color=kCyan;break;
        }
        h->SetLineColor(color);
        h->Draw("same");
    }

}
