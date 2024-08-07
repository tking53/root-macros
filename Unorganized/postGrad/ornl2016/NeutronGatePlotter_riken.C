#define NeutronGatePlotter_riken_cxx
// The class definition in NeutronGatePlotter_riken.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what ed at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("NeutronGatePlotter_riken.C")
// root> T->Process("NeutronGatePlotter_riken.C","some options")
// root> T->Process("NeutronGatePlotter_riken.C+")
//


#include "NeutronGatePlotter_riken.h"
#include <TH2.h>
#include <TStyle.h>
#include <TSystem.h>


void NeutronGatePlotter_riken::Begin(TTree * PixTree)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  outputFilePrefix = "_NeutronGateplotter_Riken_Out";
  
  nReader.SetTree(PixTree);
  
  TString option = GetOption();
  
  GetOutputList()->Clear();

  // if (option == "94"){
  //   gammaLines = {213.4,219.2,432.6,710,986.1}; //094rb bn
  //   isotop = "094rb_14";
  // }else if (option == "97"){
  //   gammaLines = {414.3,692,813.2,815,1037.3,1180,1335.9,1402.4,1506.9,2083.9};//097rb bn
  //   isotope = "097rb_0253";
  // }

  cout<<"Begin Slaves"<<endl;

  //  isotope= "094rb_14";
  // isotope = "097rb_0253";
  //  isotope = "097rb_02";
  isotope = "097rb_53";
}

void NeutronGatePlotter_riken::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  //94rb things
  // gammaLines = {213.4,432.6,986.1,1142.5}; //094rb bn
  //  HagMinGammaNum=0; //first gamma index to plot for hagrid

  //97rb things
  // gammaLines = {414.3,692,813.2,815,1037.3,1180,1335.9,1402.4,1506.9,2083.9};//097rb bn not right levels just bn lines

  gammaLines = {815,1229.5,1506.9,1628.2,2150.9};//097rb bn levels
  HagMinGammaNum=0; //first gamma index to plot for hagrid


  gSystem->Load("/home/tking/RIKEN2018/paass-deploy/install/lib/libSysRootStrucLib.so");

  cutg = new TCutG("BanCut5",13);
  cutg->SetVarX("Vandle QDC vs ToF (1ns/bin)");
  cutg->SetVarY("");
  cutg->SetTitle("Graph");
  cutg->SetFillStyle(1000);
  cutg->SetPoint(0 , 40.0,1537.0);
  cutg->SetPoint(1 , 54.7684 , 277.4   );
  cutg->SetPoint(2 , 65.8706 , 113.475 );
  cutg->SetPoint(3 , 70.1558 , 40      );
  cutg->SetPoint(4 , 400.0,40.0  );
  cutg->SetPoint(5 , 400.0,150.0 );
  cutg->SetPoint(6 , 234.448 , 378.559 );
  cutg->SetPoint(7 , 165.043 , 712.474 );
  cutg->SetPoint(8 , 110.602 , 1832.98 );
  cutg->SetPoint(9 , 88.2626 , 3442.14 );
  cutg->SetPoint(10, 64.695  , 7725.02 );
  cutg->SetPoint(11, 32.3427 , 7725.02 );
  cutg->SetPoint(12, 40.0,1537.0);   
  
  qdcCut = {30,3000};
  PromptTof = {10,60};
  DelayTof = {60,600};

  CloverGateWidth = 6;
  HagGateWidth = 35;
  vTDiff = {-10,10};
  CloverGoodTDiff = 500;
  HagGoodTDiff =225;
  
  neutronHists = new TObjArray;
  neutronHists->Add(new TH2F("QDCvsTof","Vandle QDC vs ToF (1ns/bin)", 1000.,0.,1000.,8192.,0,8192));
  neutronHists->Add(new TH2F("gatedQDCvsTof","Gated Vandle QDC vs ToF (1ns/bin)", 1000.,0.,1000.,8192.,0,8192));
  neutronHists->Add(new TH2F("BarvsTDif","BarNum vs TDiff (1ns/bin)", 2000.,-1000.,1000.,50.,0.,50.));
  neutronHists->Add(new TH2F("QdcvsBar","QDC vs BarNum", 2000.,0,2000.,50.,0.,50.));
  
  neutronHists->Add(new TH2F("geTofVsS","Tof vs Clover Singles",4000.,0,4000.,8000.,0.,8000.));
  neutronHists->Add(new TH2F("shTofVsS","Tof vs 2\" HAGRiD Singles",4000.,0,4000.,8000.,0.,8000.));

  neutronHists->Add(new TH2F("geBanTofVsS","Banana Gated Tof vs Clover Singles",4000.,0,4000.,8000.,0.,8000.));
  neutronHists->Add(new TH2F("shBanTofVsS","Banana Gated Tof vs 2\" HAGRiD Singles",4000.,0,4000.,8000.,0.,8000.));

  neutronHists->Add(new TH2F("geEnVsDT","Gamma-Beta TDiff vs Gamma Energy (Clover)",2000.,0.,2000.,8000.,0.,8000));
  neutronHists->Add(new TH2F("shEnVsDT","Gamma-Beta TDiff vs Gamma Energy (2\" HAGRiD)",2000.,0.,2000.,8000.,0.,8000));
  neutronHists->Add(new TH2F("naiEnVsDT","Gamma-Beta TDiff vs Gamma Energy (NaI)",2000.,0.,2000.,8000.,0.,8000));
  
  neutronHists->Add(new TH1F("naiSingles","NaI Singles(DetNum>3)",8000.,0.,8000.));
  neutronHists->Add(new TH1F("naiSinglesBG","NaI BG Singles(DetNum>3)",8000.,0.,8000.));
  
  neutronHists->Add(new TH1F("geSingles","Clover Singles",8000.,0.,8000.));
  neutronHists->Add(new TH1F("geSinglesBG","Clover BG Singles",8000.,0.,8000.));
  
  neutronHists->Add(new TH1F("shSingles","2\" HAGRiD Singles",8000.,0.,8000.));
  neutronHists->Add(new TH1F("shSinglesBG","2\" HAGRiD BG Singles",8000.,0.,8000.));
  
  neutronHists->Add(new TH1F("geSingleMultiBG1","Clover Multiplicity 1  Singles",8000.,0.,8000.));

  neutronHists->Add(new TH1F("geInBan","Clover Banana Gated Singles",8000.,0.,8000.));
  neutronHists->Add(new TH1F("shInBan","2\" HAGRiD Banana Gated Singles",8000.,0.,8000.));
 
  std::string front = "ToF Vs QDC Gated ";
  std::string OnGate = "OnGateTQ";
  std::string OverGate ="OverGateTQ";
  std::string UnderGate ="UnderGateTQ";
  std::vector <std::string> types ={"ge","sh"};

  for (unsigned j=0;j<types.size();j++){
    std::stringstream d1name;
    std::stringstream key;

    key<<types.at(j)<<"Prompt";
    d1name<<types.at(j)<<" Prompt <"<<PromptTof.second<<"ns" ;
    neutronHists->Add(new TH1F(key.str().c_str(),d1name.str().c_str(),8000.,0.,8000.));
    d1name.str("");
    key.str("");

    key<<types.at(j)<<"Delay";
    d1name<<types.at(j)<<" (all) Delay >"<<DelayTof.first<<"ns";
    neutronHists->Add(new TH1F(key.str().c_str(),d1name.str().c_str(),8000.,0.,8000.));
    d1name.str("");
    key.str("");

    key<<types.at(j)<<"EXTDelay";
    d1name<<types.at(j)<<" extreme delay > "<<DelayTof.second<<"ns";
    neutronHists->Add(new TH1F(key.str().c_str(),d1name.str().c_str(),8000.,0.,8000.));  
    d1name.str("");
    key.str("");

    Double_t gateWidth;
    if (types.at(j) == "ge")
      gateWidth=CloverGateWidth;
    else if (types.at(j) == "sh")
      gateWidth=HagGateWidth;

    for (unsigned it=0;it<gammaLines.size();it++){
      std::stringstream name;    

      key.str("");
      key<<types.at(j)<<OnGate<<it;
      name<<front<<"on Gamma ("<<gammaLines.at(it)<<"+/- "<<gateWidth/2<<")";
      neutronHists->Add(new TH2F(key.str().c_str(),name.str().c_str(),2000.,0.,2000.,4000.,0.,4000));
      name.str("");
      key.str("");

      key<<types.at(j)<<OverGate<<it;
      name<<front<<"above "<<gammaLines.at(it)<<" (width of gate is "<<gateWidth/2<<")";
      neutronHists->Add(new TH2F(key.str().c_str(),name.str().c_str(),2000.,0.,2000.,4000.,0.,4000));
      name.str("");
      key.str("");

      key<<types.at(j)<<UnderGate<<it;
      name<<front<<"below "<<gammaLines.at(it)<<" (width of gate is "<<gateWidth/2<<")";
      neutronHists->Add(new TH2F(key.str().c_str(),name.str().c_str(),2000.,0.,2000.,4000.,0.,4000));
    }//end gamma list loop
  }//end type loop



  TIter next(neutronHists);
  
  while(TH1* hist=(TH1*)next()){
    GetOutputList()->Add(hist);
  }
  
  while(TH2* hist=(TH2*)next()){
    GetOutputList()->Add(hist);
  }
  
  TString option = GetOption();

}

Bool_t NeutronGatePlotter_riken::Process(Long64_t entry)
{
  std::stringstream HistName;
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // When processing keyed objects with PROOF, the object is already loaded
  // and is available via the fObject pointer.
  //
  // This function should contain the \"body\" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.

  nReader.SetLocalEntry(entry);
  // cout<<"190"<<endl;

  std::vector<Double_t> GoodCloverEnergys,GoodHagEnergys;
  Bool_t NaiPres=false;

  Bool_t GoodCloverGamma = false;
  Bool_t GoodHagGamma = false;
  Bool_t InPrompt = false;
  Bool_t InDelay = false;
  Bool_t InExtDelay = false;
  Bool_t InBan = false;
  //Bool_t gGateLow, gGateIn, gGateHigh;
  std::vector<std::vector<Bool_t>> CloverGateBools,HagGateBools;

  for (unsigned it=0;it<gammaLines.size();it++){
    std::vector<Bool_t> tmp = {false,false,false};
    CloverGateBools.emplace_back(tmp); //under,on,high for each gamma line
    HagGateBools.emplace_back(tmp); //under,on,high for each gamma line
  }

  //-------------------------VANDLE -----------------------------

  for (auto itV=vandle.begin();itV!=vandle.end();itV++){
    
    Bool_t GoodVandleTDiff = false;
    if (vTDiff.first <= itV->tdiff && itV->tdiff <=vTDiff.second){
      GoodVandleTDiff = true;
      ((TH2D*)neutronHists->FindObject("QDCvsTof"))->Fill(itV->tof,itV->qdc);
      ((TH2D*)neutronHists->FindObject("BarvsTDif"))->Fill(itV->tdiff,itV->barNum);
      ((TH2D*)neutronHists->FindObject("QdcvsBar"))->Fill(itV->qdc,itV->barNum);
     
    } 
   
    if (cutg->IsInside(itV->tof,itV->qdc) && GoodVandleTDiff){
     
      ((TH2D*)neutronHists->FindObject("gatedQDCvsTof"))->Fill(itV->tof,itV->qdc);
    }
   
    //--------------gammaScint in vandle--------------------
    for (auto itG=gamscint.begin();itG!=gamscint.end();itG++){
       
      HistName.str("");
      int Type = itG->NumType;
      std::string name;
      Bool_t GoodTDiff =false;
      // cout<<"225"<<endl;
      // cout<<"Type="<<Type<<endl;
      if (Type != 2 ){
        continue;
      }else if (Type == 2){ //only do things for small hagrid
        name = "sh";
        // cout<<"231"<<endl;
        if(itG->BetaGammaTDiff<=HagGoodTDiff){
          GoodTDiff = true;
        }//bg tdiff
      }//if small hag
      
      if (GoodTDiff && GoodVandleTDiff){ //
        HistName.str("");
        HistName<<name<<"TofVsS";
        ((TH2D*)neutronHists->FindObject(HistName.str().c_str()))->Fill(itV->tof,itG->Energy);
      }//end good tdiff

      if (cutg->IsInside(itV->tof,itV->qdc) && GoodTDiff && GoodVandleTDiff){
        HistName.str("");
        HistName<<name<<"BanTofVsS";
        ((TH2D*)neutronHists->FindObject(HistName.str().c_str()))->Fill(itV->tof,itG->Energy);
      }//end in Ban && good tdiffs 
    } //end GammaScint loop
    
    //-------------CLOVER-in-vandle-----------------------------------------
    for (auto itClover=clover.begin();itClover!= clover.end();itClover++){
      std::string name;
      Bool_t GoodTDiff =false;
      if(itClover->BetaCloverTDiff<=CloverGoodTDiff){
        GoodTDiff = true;
      }
      if (GoodTDiff && GoodVandleTDiff){
        ((TH2D*)neutronHists->FindObject("geTofVsS"))->Fill(itV->tof,itClover->Energy);
      }//end goodTDiff

      if (cutg->IsInside(itV->tof,itV->qdc) && GoodTDiff && GoodVandleTDiff){
        ((TH2D*)neutronHists->FindObject("geBanTofVsS"))->Fill(itV->tof,itClover->Energy);
      }//end in Ban && good tdiffs 

    }//end in vandle clover loop
  }//end vandle loop


  //-------------------CLOVER----ALONE-------------------------------
  //  cout<<"cloveM1"<<endl;
 
  for (auto iClover=clover.begin();iClover!=clover.end();iClover++){
    // cout<<"clover loop start"<<endl;
    HistName.str("");
    Double_t gEnergy = iClover->Energy;
    std::string name;
     
    name="ge";
    HistName.str("");
    HistName<<name<<"Singles";
    ((TH1F*)neutronHists->FindObject(HistName.str().c_str()))->Fill(iClover->Energy);
    if (iClover->HasLowResBeta){
      HistName<<"BG";
      ((TH1F*)neutronHists->FindObject(HistName.str().c_str()))->Fill(iClover->Energy);
      if ( clover.GetSize() == 1){
        ((TH1F*)neutronHists->FindObject("geSingleMultiBG1"))->Fill(clover.begin()->Energy);
      }
    }     
    HistName.str("");
    HistName<<name<<"EnVsDT";
    ((TH2D*)neutronHists->FindObject(HistName.str().c_str()))->Fill(iClover->BetaCloverTDiff,iClover->Energy);
	
 
    // ------------------Gateing ------------
    if (iClover->BetaCloverTDiff<=CloverGoodTDiff ){
      GoodCloverGamma = true;
      GoodCloverEnergys.emplace_back(gEnergy);
          for (unsigned it=0;it<gammaLines.size();it++){
        // cout<<"cloverGating"<<endl;
        if (gammaLines.at(it) - CloverGateWidth < gEnergy && gEnergy < gammaLines.at(it) - CloverGateWidth/2){
          // cout<<"Clover Internal 0"<<endl;
          CloverGateBools.at(it).at(0)=true;
        }else if (gammaLines.at(it) - CloverGateWidth/2 < gEnergy && gEnergy < gammaLines.at(it) + CloverGateWidth/2){
          // cout<<"Clover Internal 1"<<endl;
          CloverGateBools.at(it).at(1)=true;
        } else if (gammaLines.at(it) + CloverGateWidth/2 < gEnergy && gEnergy < gammaLines.at(it) + CloverGateWidth){
          // cout<<"Clover Internal 2"<<endl;
          CloverGateBools.at(it).at(2)=true;
        }else {
          continue;
        }
      }//end gamma lines loop
    }//end ge gate && BG Tdiff cut
  }//end primary loop over gammas

  cout<<"gamsin"<<endl;
  // -------------------------------gamma scint ---------------------------------------
  for (auto itG=gamscint.begin();itG!=gamscint.end();itG++){
    Double_t gEnergy = itG->Energy;
    HistName.str("");
    int Type = itG->NumType;
    int DetNum = itG->DetNum;
    std::string name;
    if (Type == 2){
      name = "sh";
      // ------------------Gateing ------------
      if (itG->BetaGammaTDiff<=HagGoodTDiff ){
        GoodHagGamma = true;
        GoodHagEnergys.emplace_back(gEnergy);
        for (unsigned it=HagMinGammaNum;it<gammaLines.size();it++){ //starting at 3rd (0 counting) because the low 2 lines are too close need human touch
          // cout<<"hag gateing"<<endl;
          if (gammaLines.at(it) - HagGateWidth < gEnergy && gEnergy < gammaLines.at(it) - HagGateWidth/2)
            HagGateBools.at(it).at(0)=true;
          else if (gammaLines.at(it) - HagGateWidth/2 < gEnergy && gEnergy < gammaLines.at(it) + HagGateWidth/2)
            HagGateBools.at(it).at(1)=true;
          else if (gammaLines.at(it) + HagGateWidth/2 < gEnergy && gEnergy < gammaLines.at(it) + HagGateWidth)
            HagGateBools.at(it).at(2)=true;
        }//end gamma lines loop
      }//end sh gate && BG Tdiff cut
    }else if (Type == 0 && DetNum >3){
      name = "nai";
      NaiPres=true;
    }else{
      continue;
    }
    
    HistName.str("");
    HistName<<name<<"EnVsDT";
    ((TH2D*)neutronHists->FindObject(HistName.str().c_str()))->Fill(itG->BetaGammaTDiff,itG->Energy);
	
	
    HistName.str("");
    HistName<<name<<"Singles";
    ((TH1F*)neutronHists->FindObject(HistName.str().c_str()))->Fill(itG->Energy);
    if (itG->HasLowResBeta){
      HistName<<"BG";
      ((TH1F*)neutronHists->FindObject(HistName.str().c_str()))->Fill(itG->Energy);
    }//end If HasLowResBeta
  }
  // cout<<"flagged vandle"<<endl;
  //VANDLE---------------------------   
  for (auto iV=vandle.begin();iV!=vandle.end();iV++){
    std::stringstream hisName;
    if (iV->tdiff>=vTDiff.first && iV->tdiff<=vTDiff.second){

      //tof v qdc gated on gammas
      for (unsigned it=0;it<gammaLines.size();it++){
        if (CloverGateBools.at(it).at(0)){
          hisName.str("");
          hisName<<"geUnderGateTQ"<<it;
          ((TH2F*)neutronHists->FindObject(hisName.str().c_str()))->Fill(iV->tof,iV->qdc);
        }else if (CloverGateBools.at(it).at(1)){
          hisName.str("");
          hisName<<"geOnGateTQ"<<it;
          ((TH2F*)neutronHists->FindObject(hisName.str().c_str()))->Fill(iV->tof,iV->qdc);
        }else if (CloverGateBools.at(it).at(2)){
          hisName.str("");
          hisName<<"geOverGateTQ"<<it;
          ((TH2F*)neutronHists->FindObject(hisName.str().c_str()))->Fill(iV->tof,iV->qdc);           
        }//end Clover gates

        if (HagGateBools.at(it).at(0)){
          hisName.str("");
          hisName<<"shUnderGateTQ"<<it;
          ((TH2F*)neutronHists->FindObject(hisName.str().c_str()))->Fill(iV->tof,iV->qdc);
        }else if (HagGateBools.at(it).at(1)){
          hisName.str("");
          hisName<<"shOnGateTQ"<<it;
          ((TH2F*)neutronHists->FindObject(hisName.str().c_str()))->Fill(iV->tof,iV->qdc);
        }else if (HagGateBools.at(it).at(2)){
          hisName.str("");
          hisName<<"shOverGateTQ"<<it;
          ((TH2F*)neutronHists->FindObject(hisName.str().c_str()))->Fill(iV->tof,iV->qdc);           
        }//end hagrid gates


        //reverse gating
        if(iV->tof <= PromptTof.second){
          InPrompt = true;
        } else if (DelayTof.first < iV->tof){
          InDelay = true;
        }else if (DelayTof.second < iV->tof){
          InExtDelay = true;
        }
        // in ban needs to be separate
        if (cutg->IsInside(iV->tof,iV->qdc)){
          InBan=true;
        }


      }//end gammaline loop
    }//end Vandle TDiff cut
  }//end vandle loop

  // if (!GoodCloverEnergys.empty()){
    for (auto it=GoodCloverEnergys.begin();it!=GoodCloverEnergys.end();it++){
      if (InBan){
        ((TH1F*)neutronHists->FindObject("geInBan"))->Fill((*it));
      }
      if(InPrompt){
        ((TH1F*)neutronHists->FindObject("gePrompt"))->Fill((*it));
      }
      if (InDelay){
        ((TH1F*)neutronHists->FindObject("geDelay"))->Fill((*it));
      }
      if (InExtDelay){
        ((TH1F*)neutronHists->FindObject("geEXTDelay"))->Fill((*it));
      }
    }//end Good Clover Energy loop
    //  }//end empty Clover gamma protection
  
    //if (!GoodHagEnergys.empty()){
    for (auto it=GoodHagEnergys.begin();it!=GoodHagEnergys.end();it++){
      if (InBan){
        ((TH1F*)neutronHists->FindObject("shInBan"))->Fill((*it));
      }
      if(InPrompt){
        ((TH1F*)neutronHists->FindObject("shPrompt"))->Fill((*it));
      }
      if (InDelay){
        ((TH1F*)neutronHists->FindObject("shDelay"))->Fill((*it));
      }
      if (InExtDelay){
        ((TH1F*)neutronHists->FindObject("shEXTDelay"))->Fill((*it));
      }
    }//end good hag energy loop
    //  }//end empty Hag gamma protection
  
 
  // cout<<"endProcess"<<endl;
   
  return kTRUE;
}

void NeutronGatePlotter_riken::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void NeutronGatePlotter_riken::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  TIter next(GetOutputList());
  
  std::stringstream filename;
  TDatime now;
  filename <<isotope <<outputFilePrefix <<"_" << now.GetDate() << "_" << now.GetTime() << ".root";
  outFile = new TFile(filename.str().c_str(),"RECREATE");
  

  while( TObject* obj = next() ){
    obj->Write();
    //    delete (obj);
  }
 
  
  // named.str("");
  // named<<Gmin<<"keV";
  // TNamed GamMin("GammaThreshold",named.str().c_str());
  // GamMin.Write();
  
  // named.str("");
  // named<<QDCmin;
  // TNamed QDCmin("QDCmin",named.str().c_str());
  // QDCmin.Write();

  outFile->Close();
}

