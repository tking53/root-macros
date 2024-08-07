#include <TH1.h>
#include <TF1.h>
#include <cmath>
#include <vector>
#include <TLinearFitter.h>
#include <TRandom3.h>

void EuHagSim(const std::string HistId = "1205"){

  TCanvas*  c1 = new TCanvas("c1","c1",960,510);

  Double_t npeaks = 3;
  Double_t runTime = 6030; //Seconds ONLY FOR ORNL2016VANDLE run

  Double_t histoStats =3.5e6; //matches data

  //  Double_t euDoubletRate =(10851683.4350201+16796858.31960860)/runTime; //Doublet reduced rate 
  Double_t euDoubletRate = (1799.61582670317+2785.54864338451);

  std::vector<std::pair<Double_t,Double_t>> EuInits = {{30,(1.5e6/runTime)},{86.5,1898},{ 122.589767359497,euDoubletRate}}; //set as {Lit Energy, Area in reduced rate Photons/s } This reduced rate is valid ONLY for the ORNL2016 Vandle Run.
  //A=H * sigma /0.3989 = H * FWHM / (0.3989 *2.35)

  std::vector<Double_t> Sigmas, ConvoPar;

  std::vector<Double_t> ElinCenters = {30.,245.,345.,589.}; //peaks to fit for sigma estimation
  std::vector<std::pair<Double_t,Double_t>> ElinRanges = {{0,60},{200,320},{280,380},{520,650}};
  std::vector<std::pair<Double_t,Double_t>> ElinPars; //{Centroid,FWHM}
  std::vector<TF1*> SigEstFits;

  std::vector<Double_t> twinPar;

  if (ElinCenters.size() != ElinRanges.size()){
    cout<<"Sigma Estimation vectors size mismatch: This is fatal."<<endl;
    gApplication->Terminate();
  }

   


  TH1* h1 = 0;
  std::string dHisto = "d"+HistId;
  _file0->GetObject(dHisto.c_str(),h1);
  h1->GetXaxis()->SetRangeUser(0,700);
  
  auto *hTest = (TH1*)h1->Clone("hTest");
  hTest->Scale(150);
  hTest->SetLineColor(kGreen+2);
  hTest->GetXaxis()->SetRangeUser(0,700);

  for (auto linIt=0;linIt < ElinCenters.size(); linIt++ ){
    std::string Tkey;
    Tkey = "El"+ std::to_string(linIt);
    SigEstFits.emplace_back(new TF1(Tkey.c_str(),"gaus(0)+pol2(3)",ElinRanges.at(linIt).first,ElinRanges.at(linIt).second));

    SigEstFits.at(linIt)->SetParLimits(1,ElinCenters.at(linIt)-5,ElinCenters.at(linIt)+5); //limit centers to +/- 5 from vector value
    SigEstFits.at(linIt)->SetParLimits(2,5,60); //gaus sigma(not FHWM)
    SigEstFits.at(linIt)->SetParameter(1, ElinCenters.at(linIt)); //set center value
    SigEstFits.at(linIt)->SetParameter(2,23/2.355); //set sigma value

    std::string FitOpt = "R+";
    if (linIt == 0)
      FitOpt = "R";
    h1->Fit(SigEstFits.at(linIt),FitOpt.c_str());

  } // end peaks for sigma estimation for loop

  //Start Fitting the estimation Line
  TLinearFitter *sigEst = new TLinearFitter(1);
  sigEst->SetFormula("pol1"); 

  for (auto it=0; it<SigEstFits.size();it++){
    ElinPars.emplace_back(SigEstFits.at(it)->GetParameter(1),2.355*SigEstFits.at(it)->GetParameter(2));
    sigEst->AddPoint(&ElinPars.at(it).first,ElinPars.at(it).second);
  }
  sigEst->Eval();
  sigEst->Chisquare();
  sigEst->PrintResults(3);
  cout<<"Sigma Estimation (linear fit) ChiSquare = "<<sigEst->GetChisquare()<<endl;;
  //END SIGMA ESTIMATION LINEAR FIT


  //Loading Parameters for the Convolution "fit". This is Sim like, and it should produce something similar to the data in shape, BUT NOTE that it will be a 4Pi worth of stats
  for (int i =0; i < npeaks; i ++){
    //sigma -> FWHM = 2.355Sig -> so sigma =FWHM/2.355
    Double_t sigma = (sigEst->GetParameter(1)*EuInits.at(i).first + sigEst->GetParameter(0))/2.355;
    Sigmas.emplace_back(sigma);

    //    A=H * sigma /0.3989 = H * FWHM / (0.3989 *2.35) 

    Double_t Amp = (EuInits.at(i).second * runTime) /(sigma * sqrt(2*TMath::Pi()));

    if(EuInits.at(i).first != 30)
      Amp = (EuInits.at(i).second * runTime) /(sigma * sqrt(2*TMath::Pi()))*0.03;

    ConvoPar.emplace_back(Amp);
    ConvoPar.emplace_back(EuInits.at(i).first);
    ConvoPar.emplace_back(sigma); 
  }

  // BackGround Fit from readhis and Excel. This is a Cubic Poly, in increasing powers of X
  ConvoPar.emplace_back(-3141);//const
  ConvoPar.emplace_back(174);//X term
  ConvoPar.emplace_back(-1.2); // X^2 term
  ConvoPar.emplace_back(0.0024); //X^3 term

  //load the Combo Function
  //addition of 5 Gauss and 1 cubic poly ( gaus(3) means start with the 3rd para for this gaus etc.)
  TF1* convo = new TF1("convo","gaus(0)+gaus(3)+gaus(6)+pol3(9)",1,160);

   for (int ParNum=0 ; ParNum < ConvoPar.size(); ParNum++){
      convo->SetParameter(ParNum,ConvoPar.at(ParNum));
      cout<<"convoPar.at("<<ParNum<<")= "<<ConvoPar.at(ParNum)<<endl;
    }
   
    for (int parNum = 0 ;parNum<(npeaks); parNum++){
      convo->SetParLimits(parNum*3,4000, 25000);
      convo->SetParLimits(parNum*3+1,EuInits.at(parNum).first-10,EuInits.at(parNum).first+10);//Centroid Limit
      convo->SetParLimits(parNum*3+2,Sigmas.at(parNum)-10,Sigmas.at(parNum)+10); //Gaus Sigma Limit
      convo->SetParLimits(parNum*3+2,0,60); //Gaus Sigma Limit
      
    }

    convo->SetParLimits(9, -3250, -3000);
    convo->SetParLimits(10,100,200);
    convo->SetParLimits(11,-3,0);
    convo->SetParLimits(12,0.001,0.003);
    
     TH1D* convoH = new TH1D("convoH","convoH",160,1.,160.);
    
    convoH->FillRandom("convo",histoStats);
  
    
    
    TF1* twinFit = new TF1("twinFit","gaus(0)+gaus(3)+gaus(6)+pol3(9)",1,160);
    
    twinPar = {1000,31,20,10000,85,13,10000,115,10,-3141,174,-1.2,0.0024};
    
    for (int Dpar=0;Dpar<twinPar.size();Dpar++){
     twinFit->SetParameter(Dpar,twinPar.at(Dpar));
   }
   
 


   //  convoH->Fit(twinFit,"R+");

    //hTest->Draw();
    h1->Draw();
    convo->SetLineColor(kGreen+2);
    convo->Draw("same");
    //convoH->Draw("same");

    // twinFit->SetLineColor(kGreen+2);
    //twinFit->Draw("same");

   
}
//  Parameter order for gauss const/cent/sig
//Area =  const* sigma * sqrt(2*pi)
//0.007




