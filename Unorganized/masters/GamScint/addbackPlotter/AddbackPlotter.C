#define AddbackPlotter_cxx
// The class definition in AddbackPlotter.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called on the slave servers.
//    Terminate():    called at 1the end of the loop on the tree,
//    sometimes                a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("AddbackPlotter.C")
// root> T->Process("AddbackPlotter.C","some options")
// root> T->Process("AddbackPlotter.C+")
//

#include "AddbackPlotter.h"
#include <TH2.h>
#include <TStyle.h>
#include <TSystem.h>

void AddbackPlotter::Begin(TTree* PixTree )
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  outputFilePrefix = "nonGroupBool_test_";

  VanQdcCut ={100,25000}; //UNCOMPRESSED
  VanTofCut ={75,300}; //{1150,1400}->Damm Ranges -> 0.5ns/bin + 1000 bin offset

  TDiffCuts.emplace("clover",500);
  TDiffCuts.emplace("smallhag",500);
  TDiffCuts.emplace("nai",500);



  singReader.SetTree(PixTree);
  
 
  TString option = GetOption();
  GetOutputList()->Clear();
  
  cout<<"Begin Slaves"<<endl;
 }

void AddbackPlotter::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).
  gSystem->Load("/home/hanayo/programs/paass/install/lib/libSysRootStrucLib.so");
  gSystem->Load("/home/hanayo/programs/paass/install/lib/SysRootStruc_rdict.pcm");

    
  typeList = {"nai","smallhag","clover"};
  groupList = {"shg0","shg1","shg2","shg3","naiL0","naiL1","naiL2","clover"};

  NgammaThresh = 10.; // these defaults come from the clover processor in pixie16/paass
  SHgammaThresh = 10.; // these defaults come from the clover processor in pixie16/paass
  NSeW = 1e-6; // these defaults come from the clover processor in pixie16/paass
  SHSeW = 1e-6; // these defaults come from the clover processor in pixie16/paass
  GegammaThresh = 10.; // these defaults come from the clover processor in pixie16/paass
  GeSeW = 1e-6; // these defaults come from the clover processor in pixie16/paass

  std::map <std::string,double > paraData ;
  //setting map of standard parameters for addback. (see AddbackPlotter.hpp for values)
  paraData.insert(make_pair("thresh",NgammaThresh)); 
  paraData.insert(make_pair("subEvtWin",NSeW));
  ParameterMap.emplace("nai",paraData);
  paraData.clear();
    
  paraData.insert(make_pair("thresh",SHgammaThresh));
  paraData.insert(make_pair("subEvtWin",SHSeW));
  ParameterMap.emplace("smallhag",paraData);
  paraData.clear();

    
  paraData.insert(make_pair("thresh",GegammaThresh));
  paraData.insert(make_pair("subEvtWin",GeSeW));
  ParameterMap.emplace("clover", paraData);
  paraData.clear();

  TString option = GetOption();

  abHists = new TObjArray;

  abHists->Add(new TH1D("ABT","Pixie Event Addback(All Types and Groups)",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshT","Addback(All Groups) For 2\" HAGRiD",10000.,0.,10000.));
  abHists->Add(new TH1D("ABnaiT","Addback(All Groups) For NaI",10000.,0.,10000.));
  abHists->Add(new TH1D("ABgeT","Addback(All Groups) For Clover",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshG0","Addback(Group 0) For 2\" HAGRiD",10000.,0.,10000.));
  abHists->Add(new TH1D("ABnaiG0","Addback(Layer 0) For NaI",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshG1","Addback(Group 1) For 2\" HAGRiD",10000.,0.,10000.));
  abHists->Add(new TH1D("ABnaiG1","Addback(Layer 1) For NaI",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshG2","Addback(Group 2) For 2\" HAGRiD",10000.,0.,10000.));
  abHists->Add(new TH1D("ABnaiG2","Addback(Layer 2) For NaI",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshG3","Addback(Group 3) For 2\" HAGRiD",10000.,0.,10000.));

  abHists->Add(new TH1D("ABgeG","Addback(Clover)",10000.,0.,10000.));

  abHists->Add(new TH2F("ABvSg","Clover (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));
  abHists->Add(new TH2F("ABvSs","2\" HAGRiD (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));
  abHists->Add(new TH2F("ABvSn","NaI (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));


  //BETA GATED HISTS
  abHists->Add(new TH1D("ABTBG","BetaGated Pixie Event Addback(All Types and Groups)",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshTBG","BetaGated Addback(All Groups) For 2\" HAGRiD",10000.,0.,10000.));
  abHists->Add(new TH1D("ABnaiTBG","BetaGated Addback(All Groups) For NaI",10000.,0.,10000.));
  abHists->Add(new TH1D("ABgeTBG","BetaGated Addback(All Groups) For Clover",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshG0BG","BetaGated Addback(Group 0) For 2\" HAGRiD",10000.,0.,10000.));
  abHists->Add(new TH1D("ABnaiG0BG","BetaGated Addback(Layer 0) For NaI",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshG1BG","BetaGated Addback(Group 1) For 2\" HAGRiD",10000.,0.,10000.));
  abHists->Add(new TH1D("ABnaiG1BG","BetaGated Addback(Layer 1) For NaI",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshG2BG","BetaGated Addback(Group 2) For 2\" HAGRiD",10000.,0.,10000.));
  abHists->Add(new TH1D("ABnaiG2BG","BetaGated Addback(Layer 2) For NaI",10000.,0.,10000.));

  abHists->Add(new TH1D("ABshG3BG","BetaGated Addback(Group 3) For 2\" HAGRiD",10000.,0.,10000.));

  abHists->Add(new TH1D("ABgeGBG","BetaGated Addback(Clover)",10000.,0.,10000.));

  abHists->Add(new TH2F("ABvSgBG","BetaGated Clover (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));
  abHists->Add(new TH2F("ABvSsBG","BetaGated 2\" HAGRiD (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));
  abHists->Add(new TH2F("ABvSnBG","BetaGated NaI (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));

  //Trigger Beta Gated (2D's)
  abHists->Add(new TH2F("ABvSgTBG","Trigger Beta Gated Clover (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));
  abHists->Add(new TH2F("ABvSsTBG","Trigger Beta Gated 2\" HAGRiD (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));
  abHists->Add(new TH2F("ABvSnTBG","Trigger Beta Gated NaI (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));

  //Vandle Hists

  abHists->Add(new TH2D("VanQDCvsTof","Vandle QDC vs ToF (0.5ns/bin)", 1000.,0.,1000.,8192.,0,8192));
  abHists->Add(new TH2D("VanBarvsTDif","BarNum vs TDiff (0.5ns/bin)", 2000.,-1000.,1000.,50.,0.,50.));

  abHists->Add(new TH2F("ABvSgNG","Neutron Tof Gated Clover (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));
  abHists->Add(new TH2F("ABvSsNG","Neutron Tof Gated 2\" HAGRiD (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));
  abHists->Add(new TH2F("ABvSnNG","Neutron Tof Gated NaI (Singles) vs PixieEvent Addback",10000.,0.,10000.,10000.,0.,10000.));

  abHists->Add(new TH1D("SshNG","Neutron Gated Singles 2\" HAGRiD",7000.,0.,7000.));
  abHists->Add(new TH1D("SnaiNG","Neutron Gated Singles NaI",7000.,0.,7000.));
  abHists->Add(new TH1D("SgeNG","Neutron Gated Singles Clover",7000.,0.,7000.));

  abHists->Add(new TH1D("SshBG","Normal Beta Gated Singles 2\" HAGRiD",7000.,0.,7000.));
  abHists->Add(new TH1D("SnaiBG","Normal Beta Gated Singles NaI",7000.,0.,7000.));
  abHists->Add(new TH1D("SgeBG","Normal Beta Gated Singles Clover",7000.,0.,7000.));

  abHists->Add(new TH1D("SshTBG","Trigger Beta Gated Singles 2\" HAGRiD",7000.,0.,7000.));
  abHists->Add(new TH1D("SnaiTBG","Trigger Beta Gated Singles NaI",7000.,0.,7000.));
  abHists->Add(new TH1D("SgeTBG","Trigger Beta Gated Singles Clover",7000.,0.,7000.));

  //abHists->Add(new TH2D("VanQDCvsTof2","reGated Vandle QDC vs ToF (0.5ns/bin)", 1000.,0.,1000.,8192.,0,8192));

  //add to output list
  TIter next(abHists);
  //add the 1Ds 
  while (TH1D* hist = (TH1D*)next() ){
    GetOutputList()->Add(hist);
  }
  //add the 2Ds
  while (TH2* hist = (TH2*)next() ){
    GetOutputList()->Add(hist);
  }
 }

Bool_t AddbackPlotter::Process(Long64_t entry)
{
  std::map<std::string,GSAddback> naiAddMap, shAddMap, geAddMap;

  std::stringstream hist2Fill;
  std::set<std::string> iGroup;
  int counter=0; //GSVec loop counter
  Double_t PEvtTotal=0;
  Double_t PEvtGated =0;
 
  //  std::map<std::string,Bool_t> abNGbool; //group bool storage for N gating

  Bool_t  NGbool = false;

  naiAddMap.clear();
  shAddMap.clear();
  geAddMap.clear();

  std::vector<Double_t> *singlesVec;
  std::vector<Double_t> *NGsinglesVec;
  std::vector<Double_t> Gsingles, Nsingles,Hsingles; //vector of singles for the types (Used in the 2d's)
  std::vector<Double_t> GsinglesNG, NsinglesNG,HsinglesNG; //vector of NGated singles for the types (Used in the 2d's)


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

  singReader.SetLocalEntry(entry);

  //-------------------------VANDLE -----------------------------
  for (auto itVan=VanVec.begin();itVan!=VanVec.end();itVan++){

    ((TH2D*)abHists->FindObject("VanQDCvsTof"))->Fill(itVan->tof*2,itVan->qdc);
    ((TH2D*)abHists->FindObject("VanBarvsTDif"))->Fill(itVan->tdiff*2,itVan->barNum);
    }


  //-----------------------------------------------------------------------------------------------------
  //-----------------------ADDBACK STUFF-----------------------------------------------------

 
  // (converted to ns because the input times are in ns)
  //initalize addbacks (e=0,t= -2* the sub event window, firstTime,multiplicity,EvtNum)
  //initalize NG boolean map
  for (auto iGr=groupList.begin(); iGr!=groupList.end();iGr++){
       if (strncmp((*iGr).c_str(),"sh",2) == 0){
         shAddMap.emplace((*iGr),GSAddback(0.0,-2.0*SHSeW*1e9,0.0,0,0));
         //abNGbool.emplace((*iGr),false);
       }else if (strncmp((*iGr).c_str(),"nai",3) == 0){
         naiAddMap.emplace((*iGr),GSAddback(0.0,-2.0*NSeW*1e9,0.0,0,0));
         //abNGbool.emplace((*iGr),false);
       } else if ((*iGr) == "clover"){
         geAddMap.emplace("clover",GSAddback(0.0,-2.0*GeSeW*1e9,0.0,0,0));
         //abNGbool.emplace((*iGr),false);
       }
  }


  for (auto iGSv=GSVec.begin(); iGSv != GSVec.end(); iGSv++){
    //setting event wide things so it doesnt matter if it gets reset every for loop run since they should be the same (if they arent that is a bigger problem :) )
    ABbunchNum = iGSv->BunchNum;
    //PbetaEnergy = iGSv->BetaEnergy;
    ABhasLRBeta = iGSv->HasLowResBeta;
    ABhasTrigBeta = iGSv->HasTrigBeta;

    std::string shortName,grpName;
    std::map<std::string,GSAddback>::iterator ABST;
      
    if (iGSv->Type == "smallhag"){
      ABST = shAddMap.find(iGSv->Group);
      singlesVec = &Hsingles;
      NGsinglesVec = &HsinglesNG;
      shortName = "sh";
      grpName = shortName + "G" + iGSv->Group.back();
    } else if  (iGSv->Type == "nai"){
      ABST = naiAddMap.find(iGSv->Group);
      singlesVec = &Nsingles;
      NGsinglesVec = &NsinglesNG;
      shortName = "nai";
      grpName = shortName + "G" + iGSv->Group.back();
    }  else if  (iGSv->Type == "clover"){
      ABST = geAddMap.find(iGSv->Group);
      singlesVec = &Gsingles;
      NGsinglesVec = &GsinglesNG;
      shortName = "ge";
      grpName = shortName + "G"; 
    }else{
      Abort("UNKNOWN TYPE:: BAILING");
    }

    //Trigger Singles
    if (iGSv->HasTrigBeta){
      hist2Fill.str("");
      hist2Fill<<"S"<<shortName<<"TBG";
      ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(iGSv->Energy);
    }

    //normal betas
    if (iGSv->HasLowResBeta){
      hist2Fill.str("");
      hist2Fill<<"S"<<shortName<<"BG";
      ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(iGSv->Energy);
    }

    if (iGSv->Energy < ParameterMap.find(iGSv->Type)->second.find("thresh")->second){
      continue;
      }

    Double_t dTime = abs((iGSv->Time)-ABST->second.time);
    Double_t Sew = ParameterMap.find(iGSv->Type)->second.find("subEvtWin")->second;



    ////cout<<"Loop 1/2"<<endl;

    if (dTime > ParameterMap.find(iGSv->Type)->second.find("subEvtWin")->second){
      //filling group specific
      //cout<<"1"<<endl;
      if (ABST->second.energy>0){
      hist2Fill.str("");
      hist2Fill<<"AB"<<grpName;
      ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(ABST->second.energy);
      //cout<<"2"<<endl;
      hist2Fill.str("");
      hist2Fill<<"AB"<<shortName<<"T";
      ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(ABST->second.energy);

      if (ABhasLRBeta){
        //cout<<"3"<<endl;
        hist2Fill.str("");
        hist2Fill<<"AB"<<grpName<<"BG";
        ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(ABST->second.energy);
        //cout<<"4"<<endl;
        hist2Fill.str("");
        hist2Fill<<"AB"<<shortName<<"TBG";
        ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(ABST->second.energy);
      }//end beta gate

      PEvtTotal += ABST->second.energy;

      // if (abNGbool.find(iGSv->Group)->second == true){
      if (NGbool){
        PEvtGated += ABST->second.energy;
      }
      

      }//end 0 check 
      ABST->second = GSAddback(0.0,-2.0*Sew*1e9,0.0,0,0);
      
    } //if outside of sub Evt window
    
    
    if(ABST->second.multiplicity == 0){
      ABST->second.ftime = iGSv->Time;
      ABST->second.abevtnum = iGSv->EvtNum;

      //Neutron Gate 2

      for (auto itVan2= VanVec.begin();itVan2 != VanVec.end();itVan2++){
        Double_t BTimeComp = abs(itVan2->sTime - iGSv->Time);
        Bool_t InTime =false;
        if (iGSv->Type == "clover")
          InTime =true;
        else if (BTimeComp <= TDiffCuts.find(iGSv->Type)->second)
          InTime =true;

        if (itVan2->qdc >VanQdcCut.first && itVan2->tof >= VanTofCut.first && itVan2->tof <=VanTofCut.second ){
          //abNGbool.find(iGSv->Group)->second = true;
          NGbool =true;
          break;
        }
      }//VanVec loop 

    }//multi==0

    if (NGbool){
      NGsinglesVec->emplace_back(iGSv->Energy);
    }
    singlesVec->emplace_back(iGSv->Energy);
    ABST->second.energy += (iGSv->Energy);
    ABST->second.time += (iGSv->Time);
    ABST->second.multiplicity += 1;
    //cout<<"end for Loop"<<endl;

  }// end GSVec for loop


  // CATCH FOR SINGLE MULTI EVENTS 
  for (auto itGr = groupList.begin();itGr != groupList.end();itGr++){
    std::string shortName,grpName;
    std::map<std::string,GSAddback>::iterator ABST;
    std::string TypeName = (*itGr).substr(0,2); //get first 2 chars of the group name to compare against

    if (TypeName == "sh"){
      ABST = shAddMap.find(*itGr);
      shortName = "sh";
      grpName = shortName + "G" + (*itGr).back();
    } else if  (TypeName == "na"){
      ABST = naiAddMap.find(*itGr);
      shortName = "nai";
      grpName = shortName + "G" + (*itGr).back();
    }  else if  (TypeName == "cl"){
      ABST = geAddMap.find(*itGr); 
      shortName = "ge";
      grpName = shortName + "G"; 
    }else{
      Abort("UNKNOWN TYPE:: BAILING");
    }

    if (ABST->second.multiplicity == 1){
      PEvtTotal += ABST->second.energy;
      
      if (NGbool){
        PEvtGated += ABST->second.energy;
      }
      hist2Fill.str("");
      hist2Fill<<"AB"<<grpName;
      ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(ABST->second.energy);
      
      hist2Fill.str("");
      hist2Fill<<"AB"<<shortName<<"T";
      ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(ABST->second.energy);
      
      if (ABhasLRBeta){
        
        hist2Fill.str("");
        hist2Fill<<"AB"<<grpName<<"BG";
        ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(ABST->second.energy);
        
        hist2Fill.str("");
        hist2Fill<<"AB"<<shortName<<"TBG";
        ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(ABST->second.energy);
      }//end beta gate
      
    }else{
      continue;
    }
  }




  //Pixie event all addback
  hist2Fill.str("");
  hist2Fill<<"ABT";
  ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(PEvtTotal);

  if (ABhasLRBeta){
    hist2Fill.str("");
    hist2Fill<<"ABTBG";
    ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill(PEvtTotal);
  }//end beta gate

  //2D PLOTS------------------------------------------------------------------------------------
  for (auto itType=typeList.begin();itType!=typeList.end();itType++){

    std::string Tchar;
    std::string shortName;
    if ((*itType)== "smallhag"){
      Tchar = "s";
      shortName = "sh";
      singlesVec = &Hsingles;
      NGsinglesVec = &HsinglesNG;
    } else if  ((*itType) == "nai"){
      Tchar = "n";
      shortName = "nai";
      singlesVec = &Nsingles;
      NGsinglesVec= &NsinglesNG;
    }  else if  ((*itType) == "clover"){
      Tchar = "g";
      shortName = "ge";
      singlesVec = &Gsingles;
      NGsinglesVec= &GsinglesNG;
    }else{
      Abort("UNKNOWN TYPE:: BAILING");
    }
    // cout<<"NG Sizes"<<endl<<Tchar<<" size= "<< (Double_T*)NGsinglesVec->size()<<endl;
    for (auto itVec=singlesVec->begin();itVec!=singlesVec->end();itVec++){
      hist2Fill.str("");
      hist2Fill<<"ABvS"<<Tchar;
      ((TH2F*)abHists->FindObject(hist2Fill.str().c_str()))->Fill((*itVec),PEvtTotal);
      if (ABhasLRBeta){
        hist2Fill.str("");
        hist2Fill<<"ABvS"<<Tchar<<"BG";
        ((TH2F*)abHists->FindObject(hist2Fill.str().c_str()))->Fill((*itVec),PEvtTotal);
      }//end LR beta gate

      if (ABhasTrigBeta){
        hist2Fill.str("");
        hist2Fill<<"ABvS"<<Tchar<<"TBG";
        ((TH2F*)abHists->FindObject(hist2Fill.str().c_str()))->Fill((*itVec),PEvtTotal);
      }//End Trig beta gate
    }//end itVec

    for (auto itNGvec=NGsinglesVec->begin();itNGvec !=NGsinglesVec->end();itNGvec++){
      hist2Fill.str("");
      hist2Fill<<"ABvS"<<Tchar<<"NG";
      ((TH2F*)abHists->FindObject(hist2Fill.str().c_str()))->Fill((*itNGvec),PEvtGated);

      //neutronGated Singles
      hist2Fill.str("");
      hist2Fill<<"S"<<shortName<<"NG";
      ((TH1D*)abHists->FindObject(hist2Fill.str().c_str()))->Fill((*itNGvec));
    
    }//End neutron filling
    
}//end type list

  return kTRUE;
}

void AddbackPlotter::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void AddbackPlotter::Terminate()
{

  TNamed NeutronRangeL("NeutronRangeL",VanTofCut.first);
  TNamed NeutronRangeH("NeutronRangeH",VanTofCut.second);

  TNamed QDCRangeL("QDCRangeL",VanQdcCut.first);
  TNamed QDCRangeH("QDCRangeH",VanQdcCut.second);

  std::stringstream filename;
  filename << outputFilePrefix <<  "ABPlotter_Output.root";
  outFile = new TFile(filename.str().c_str(),"RECREATE");
  TIter next(GetOutputList());
  while( TObject* obj = next() ){
    obj->Write();
    //    delete (obj);
  }
  NeutronRangeL.Write();
  NeutronRangeH.Write();
  QDCRangeL.Write();
  QDCRangeH.Write();

  outFile->Close();
  
  // gApplication->Terminate();

  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

}
