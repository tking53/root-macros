#define AddbackCalculator_cxx
// The class definition in AddbackCalculator.h has been generated automatically
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
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("AddbackCalculator.C")
// root> T->Process("AddbackCalculator.C","some options")
// root> T->Process("AddbackCalculator.C+")
//
#include "GSaddback.hpp"
#include "AddbackCalculator.h"
#include <TH2.h>
#include <TStyle.h>

void AddbackCalculator::Begin(TTree* GSsinglesAB )
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  singReader.SetTree( GSsinglesAB );
  
  typeList = {"nai","smallhag","clover"};
  groupList = {"shg0","shg1","shg3","shg4","naiL0","naiL1","naiL2","clover"};

  stringstream typelist, grouplist;
  
  for (auto it=typeList.begin();it!=typeList.end();it++){
    if (it==typeList.begin())
      typelist<<" "<<(*it);
    else
      typelist<<", "<<(*it);
  }
  
  for (auto it=groupList.begin();it!=groupList.end();it++){
    if (it==groupList.begin())
      grouplist<<" "<<(*it);
    else
      grouplist<<", "<<(*it);
  }
  
  // cout<<"Number of Pixie Events in the Tree is "<<PEvt.size()<<endl
  //     <<"The Types that are present are"<<typelist.str().c_str()<<endl
  //     <<"The Groups present are"<<grouplist.str().c_str()<<endl;

  TString option = GetOption();
  NgammaThresh = 10.; // these defaults come from the clover processor in pixie16/paass
  SHgammaThresh = 10.; // these defaults come from the clover processor in pixie16/paass
  NSeW = 1e-6; // these defaults come from the clover processor in pixie16/paass
  SHSeW = 1e-6; // these defaults come from the clover processor in pixie16/paass
   GetOutputList()->Clear();
  
  if (!GetInputList()){
    NgammaThresh = 10.; // these defaults come from the clover processor in pixie16/paass
    SHgammaThresh = 10.; // these defaults come from the clover processor in pixie16/paass
    NSeW = 1e-6; // these defaults come from the clover processor in pixie16/paass
    SHSeW = 1e-6; // these defaults come from the clover processor in pixie16/paass
    GegammaThresh = 10.; // these defaults come from the clover processor in pixie16/paass
    GeSeW = 1e-6; // these defaults come from the clover processor in pixie16/paass
  } else {
    NgammaThresh = dynamic_cast<TParameter<Double_t>*>(GetInputList()->FindObject("NGT"))->GetVal();
    NSeW = dynamic_cast<TParameter<Double_t>*>(GetInputList()->FindObject("NSeW"))->GetVal();
    SHgammaThresh = dynamic_cast<TParameter<Double_t>*>(GetInputList()->FindObject("SHGT"))->GetVal();
    SHSeW = dynamic_cast<TParameter<Double_t>*>(GetInputList()->FindObject("SHSeW"))->GetVal();
    GegammaThresh = dynamic_cast<TParameter<Double_t>*>(GetInputList()->FindObject("GeGT"))->GetVal();
    GeSeW = dynamic_cast<TParameter<Double_t>*>(GetInputList()->FindObject("GeSeW"))->GetVal();
  }
  
  std::map <std::string,double > paraData ;
  //setting map of standard parameters for addback. (see AddbackCalculator.hpp for values)
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
  // need to put the new tree here.

}

void AddbackCalculator::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  std::map<std::string,std::vector<GSAddback>> naiAddMap, shAddMap, geAddMap;

  TString option = GetOption();


  TNamed *out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE_LOCATION");
  Info("SlaveBegin", "PROOF_OUTPUTFILE_LOCATION: %s", (out ? out->GetTitle() : "undef"));
  fProofFile = new TProofOutputFile("/home/hanayo/research/thesis/ornl2016/interHolding/","M");
  out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE");
  if (out)
    fProofFile->SetOutputFileName(out->GetTitle());

  TDirectory *savedir = gDirectory;
  if (!(outFile = fProofFile->OpenFile("RECREATE"))) {
    Warning("SlaveBegin", "problems opening file: %s/%s", fProofFile->GetDir(), fProofFile->GetFileName());
  }

  outFile=fProofFile->OpenFile("RECREATE");
  if (outFile && outFile->IsZombie()) SafeDelete(outFile);

  // Cannot continue
  if (!outFile) {
    Info("SlaveBegin", "could not create '%s': instance is invalid!", fProofFile->GetName());
    return;
  }


  GabR = new TTree("GabRoot","Gamma Scint Addback(Root Calc)");
  GabR->Branch("ABEnergy",&ABEnergy);
  GabR->Branch("ABEvtNum",&ABEvtNum);
  GabR->Branch("ABMulti",&ABMulti);
  GabR->Branch("ABType",&ABType);
  GabR->Branch("ABGroup",&ABGroup);
  GabR->Branch("ABbunchNum",&ABbunchNum);
  GabR->Branch("ABhasLRBeta",&ABhasLRBeta);
  GabR->Branch("ABevtTotal",&ABevtTotal);
  GabR->Branch("ABhasLRBeta",&ABhasLRBeta);
  GabR->Branch("PbetaEnergy",&PbetaEnergy);

  //  GabR->Branch("PevtNum",&PevtNum);
  // GabR->Branch("PbunchNum",&PbunchNum);
  GabR->SetDirectory(outFile);
  GabR->AutoSave();
  //  fOutput->Add(GabR);
 }

Bool_t AddbackCalculator::Process(Long64_t entry)
{

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
  singReader.SetEntry(entry);

  naiAddMap.clear();
  shAddMap.clear();
  geAddMap.clear();
  std::set<std::string> iGroup;
  for (auto iGr=groupList.begin(); iGr!=groupList.end();iGr++){
       if (strncmp((*iGr).c_str(),"sh",2) == 0){
         auto *tmp1 = new std::vector<GSAddback>;
         //cout<<"vector made for "<<(*iGr)<<endl;
         //initalize addbacks (e=0,m=0, t= -2* the sub event window (converted to ns because the input times are in ns))
         tmp1->emplace_back(GSAddback(0.0,-2.0*NSeW*1e9,0.0,0,0));

         shAddMap.emplace((*iGr),(*tmp1));
       } else if (strncmp((*iGr).c_str(),"nai",3) == 0){
         auto *tmp1 = new std::vector<GSAddback>;
         //cout<<"vector made for "<<(*iGr)<<endl;
         //initalize addbacks (e=0,m=0, t= -2* the sub event window (converted to ns because the input times are in ns))
         tmp1->emplace_back(GSAddback(0.0,-2.0*SHSeW*1e9,0.0,0,0));
         naiAddMap.emplace((*iGr),(*tmp1));
       } else if ((*iGr) == "clover"){
         //init  clover addback
         auto *tmp1 = new std::vector<GSAddback>;
         tmp1->emplace_back(GSAddback(0.0,-2.0*GeSeW*1e9,0.0,0,0));
         geAddMap.emplace("clover",(*tmp1));
       }
  }



  //cout<<endl<<"NEW PIXIE EVENT"<<endl<<"maps Made"<<endl;
  int counter=0;
  for (auto iSing=singVec.begin(); iSing != singVec.end(); iSing++){
    //setting event wide things so it doesnt matter if it gets reset every for loop run since they should be the same
    ABbunchNum = iSing->BunchNum;
    PbetaEnergy = iSing->BetaEnergy;
    ABhasLRBeta = iSing->HasLowResBeta;




    if (iSing->Type == "smallhag"){
      if (iSing->Energy < ParameterMap.find("smallhag")->second.find("thresh")->second)
        continue;
      auto tmp = shAddMap.find((*iSing).Group);
      Double_t dtime = abs((iSing->Time) - tmp->second.back().time) ;
      if (dtime > ParameterMap.find("smallhag")->second.find("subEvtWin")->second){
        //fill plot etc
        tmp->second.emplace_back(GSAddback(0,-2*SHSeW*1e9,0.0,0,0));
      }
      if(tmp->second.back().multiplicity == 0){
        tmp->second.back().ftime = iSing->Time;
        tmp->second.back().abevtnum = iSing->EvtNum;
      }
      tmp->second.back().energy +=(iSing->Energy);
      tmp->second.back().time +=(iSing->Time);
      tmp->second.back().multiplicity += 1;
      

    }
    //NAI
    else if (iSing->Type == "nai") {
      if (iSing->Energy < ParameterMap.find("nai")->second.find("thresh")->second)
        continue;

      auto tmp = naiAddMap.find((*iSing).Group);
      Double_t dtime = abs((iSing->Time) - tmp->second.back().time) ;
      if (dtime > ParameterMap.find("nai")->second.find("subEvtWin")->second){
        //fill plot etc
        tmp->second.emplace_back(GSAddback(0,-2*NSeW*1e9,0.0,0,0));
      }
      if(tmp->second.back().multiplicity == 0){
        tmp->second.back().ftime = iSing->Time;
        tmp->second.back().abevtnum = iSing->EvtNum;
      }
      tmp->second.back().energy +=(iSing->Energy);
      tmp->second.back().time +=(iSing->Time);
      tmp->second.back().multiplicity += 1;

    }
    //CLOVER 
    else if (iSing->Type == "clover") {
      if (iSing->Energy < ParameterMap.find("clover")->second.find("thresh")->second)
        continue;
      
      auto tmp = geAddMap.find((*iSing).Group);
      Double_t dtime = abs((iSing->Time) - tmp->second.back().time) ;
      if (dtime > ParameterMap.find("clover")->second.find("subEvtWin")->second){
        //fill plot etc
        tmp->second.emplace_back(GSAddback(0,-2*GeSeW*1e9,0.0,0,0));
      }
      if(tmp->second.back().multiplicity == 0){
        tmp->second.back().ftime = iSing->Time;
        tmp->second.back().abevtnum = iSing->EvtNum;
      }
      tmp->second.back().energy +=(iSing->Energy);
      tmp->second.back().time +=(iSing->Time);
      tmp->second.back().multiplicity += 1;

    }
    else {
      counter++;
      continue;
    }
    //cout<<""<<endl;
    counter++;
  }

  Double_t ABVTotal=0;
  for (auto it = groupList.begin();it!=groupList.end();it++){
    std::vector<GSAddback> ABV;
    if (strncmp((*it).c_str(), "sh", 2) == 0){
      ABV = shAddMap.find((*it))->second;
      ABType = "smallhag";
      ABGroup = (*it);
    }else if (strncmp((*it).c_str(), "nai", 3) == 0){
      ABV = naiAddMap.find((*it))->second;
      ABType = "nai";
      ABGroup = (*it);
    }else if (strncmp((*it).c_str(), "clover", 4) == 0){
      ABV = geAddMap.find((*it))->second;
      ABType = "clover";
      ABGroup = (*it);
    } else{
      continue;
    }
    for (auto itABV = ABV.begin();itABV!=ABV.end();itABV++){
      ABEnergy = itABV->energy;
      ABEvtNum = itABV->abevtnum;
      ABMulti = itABV->multiplicity; 
      ABVTotal += itABV->energy;
      GabR->Fill();
    }
  }
      
  ABEnergy = 0;
  ABEvtNum = 0;
  ABMulti = 0;
  ABType = "";
  ABGroup = "";
  ABbunchNum = 0;
  ABhasLRBeta = false;
  ABevtTotal = 0;
  ABhasLRBeta = 0;
  PbetaEnergy = 0;

  ABevtTotal = ABVTotal;
  GabR->Fill();

  return kTRUE;
}

void AddbackCalculator::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
if 
}

void AddbackCalculator::Terminate()
{
  /* //Cant use fChain because the TNameds are outside of the tree/chain. need to use the file pointer 
  std::string origFile = fChain->Get("outputFile")->GetTitle();
  std::string origCreate = fChain->Get("createTime")->GetTitle();
  std::string FacilType = fChain->Get("facilityType")->GetTitle();
  std::string bunchType = fChain->Get("bunchingTime")->GetTitle();
  std::string origCFG = fChain->Get("config")->GetTitle();
  std::string origCFG = fChain->Get("outputRootFile")->GetTitle();

  TNamed origFile("origFilename",origFile.c_str()) ;
  TNamed origCreate("origCreateDate",origCreate.c_str()) ;
  TNamed FacilType("faciltityType",FacilType.c_str()) ;
  TNamed bunchType("",bunchType.c_str()) ;
  TNamed origCFG("",origCFG.c_str()) ;
  TNamed origCFG("",origCFG.c_str()) ;
  */

  outFile = new TFile("GabRootTestOutput.root","RECREATE");
  GetOutputList()->Write();
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

}
