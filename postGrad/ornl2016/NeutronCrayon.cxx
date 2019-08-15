#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TROOT.h"
#include "TKey.h"
#include "TObjArray.h"

using namespace std;

vector<Double_t> GammaEnergies = {414.3,692,814,1037.3,1180,1335.9,1402.4,1506.9,2083}; //Combining 813 and 815 into 1 line since they are overlapping (will use a slightly larger sigma for that line)

map<string,pair<Double_t,Double_t>> GammaRes = {{"h",{4,7}},{"n",{7,7}},{"c",{0.75,1.25}}}; // ~% resolutions as % first is single lines and second is 813/815

map<string,Color_t> Colors = {{"N",kBlack},{"h",kRed},{"n",kGreen},{"c",kBlue}};

//Range input format is <Axis,<RL,RH>>
TH2D* SparseZP(THnSparse* sparse, pair<Int_t,pair<Double_t,Double_t>> ZoomAxisRange1, pair<Int_t,pair<Double_t,Double_t>> ZoomAxisRange2, pair<Int_t,Int_t> AxisProj, Bool_t addDir = false){
  if (addDir){
    TH1::AddDirectory(kTRUE);
  } else {
    TH1::AddDirectory(kFALSE);  
  }
  sparse->GetAxis(ZoomAxisRange1.first)->SetRangeUser((ZoomAxisRange1.second).first,(ZoomAxisRange1.second).second);
  sparse->GetAxis(ZoomAxisRange1.first)->SetRangeUser((ZoomAxisRange2.second).first,(ZoomAxisRange2.second).second);
  return (sparse->Projection(AxisProj.second,AxisProj.first));
}

vector<TH1D*> LineProjector(TH2D* histo,string HistBaseName, string GamType, Double_t GamEnergy ,Color_t Color, Bool_t subtract = false){
  TH1::AddDirectory(kFALSE);
  stringstream ss;
  ss<<GamEnergy;// This is needed to trim the trailing zeros after the conversion from the Double to a string
  vector<string> ProjOrder = {"On","Under","Over","Subtracted"};
  vector<string> HistoNames = {HistBaseName+"_"+GamType+"_"+ ss.str().c_str() + "_" + ProjOrder.at(0),
                               HistBaseName+"_"+GamType+"_"+ ss.str().c_str() + "_" + ProjOrder.at(1),
                               HistBaseName+"_"+GamType+"_"+ ss.str().c_str() + "_" + ProjOrder.at(2),
                               HistBaseName+"_"+GamType+"_"+ ss.str().c_str() + "_" + ProjOrder.at(3)};
  vector<TH1D*> returnVec;
  Double_t HalfWidth ;
  
  if (GamEnergy == 814) {
    HalfWidth = ((GammaRes.find(GamType.c_str())->second.first /  100 ) * GamEnergy) /2;
  }else {
    HalfWidth = ((GammaRes.find(GamType.c_str())->second.second / 100 ) * GamEnergy) /2;
  }

  vector<pair<Double_t,Double_t>> projectionBounds = {{GamEnergy-HalfWidth,GamEnergy+HalfWidth},{GamEnergy-3*HalfWidth,GamEnergy-HalfWidth},{GamEnergy+HalfWidth,GamEnergy+3*HalfWidth}};
  TH1::AddDirectory(kFALSE);
  for (auto it =0 ; it<3;it++){
    TH1D* tmp = histo->ProjectionX(ProjOrder.at(it).c_str(),projectionBounds.at(it).first,projectionBounds.at(it).second);
    returnVec.emplace_back(tmp);
  }

  if (subtract){
    TH1D* tmp0 = (TH1D*)returnVec.at(0)->Clone();// clone them to avoid messing up the originals
    TH1D* tmp1 = (TH1D*)returnVec.at(1)->Clone();// clone them to avoid messing up the originals
    TH1D* tmp2 = (TH1D*)returnVec.at(2)->Clone();// clone them to avoid messing up the originals
    tmp1->Scale(0.5);// the over and under gates are the same width as the on gate so scale by 1/2
    tmp2->Scale(0.5);// the over and under gates are the same width as the on gate so scale by 1/2
    tmp0->Add(tmp1,-1);
    tmp0->Add(tmp2,-1);
    returnVec.emplace_back(tmp0);
  }

  for (UInt_t it =0 ; it<returnVec.size();it++){
    returnVec.at(it)->SetName(HistoNames.at(it).c_str());
    Color_t LineColor = Colors.find(GamType)->second + it;
    returnVec.at(it)->SetLineColor(LineColor);
  }

  return (returnVec);
}


vector<string> AutoLoadTHnSparseList(TFile* file, vector<string> excludes ={"T","G"}){
  TIter keyList(file->GetListOfKeys());
  TKey *key;
  vector<string> sparseList;
  while ((key = (TKey*)keyList())) {
    string  sparseClass = key->GetClassName();
    string sparseName = key->GetName();
    if (!strstr(sparseClass.c_str(),"THnSparseT") ||  !strstr(sparseName.c_str(),"dd_tof_")){
      continue;
    }
    Bool_t excluded = false;
    for (auto it : excludes){
      if (sparseName.substr(7,1) == it){
        excluded = true;
      } //end if
    } //end for   
    if(!excluded){
      sparseList.emplace_back(sparseName);
    }
  }// end while 
  return (sparseList);
}

Int_t NeutronCrayon(string inFileStr = "_file0", vector<string> nSparseList = {}, pair<Double_t,Double_t> qdcCut = {0,-1}, pair<Int_t,Int_t> tapeCut = {1,300}, Bool_t subtract = false){
  if (!gROOT->GetFile() && inFileStr == "_file0"){
    cout<<"No File is opened"<<endl;
    return 1;
  }

  if (inFileStr == "_file0") {
    inFileStr = gROOT->GetFile()->GetName();
  }
  TFile* inFile = new TFile(inFileStr.c_str());

  if(nSparseList.empty()){
    nSparseList = AutoLoadTHnSparseList(inFile); 
  }

  for (auto it = nSparseList.begin(); it != nSparseList.end();it++) {
    Int_t CurrentSparseIndex = it - nSparseList.begin();
    auto SparseIt = (*it);
    string GammaType , GamDet, projecBaseName = "dd_tof";
    GammaType = SparseIt.substr(7, 1);  // New97Rb THnSparse is 8th letter
    if (GammaType == "c") {
      GamDet = "Clover";
      // projecBaseName = "dd_c_tof";
    } else if (GammaType == "h") {
      GamDet = "LaBr";
      // projecBaseName = "dd_h_tof";
    } else if (GammaType == "n") {
      GamDet = "NaI";
      // projecBaseName = "dd_n_tof";
    } else if (GammaType == "T") {
      GamDet = "PixieTAS";
      // projecBaseName = "dd_T_tof";
    }else if (GammaType == "N"){
      GamDet = "None";
      projecBaseName = "dd_tof_qdc";
    }else {
      cout << "ERROR::Unknown Gamma Det Type (" << GammaType << ")" << endl;
      return 2;
    }
    cout<<"Processing Sparse: "<<SparseIt<<endl;

    TObjArray* output;
    string TOA_name = "Crayon_" + GammaType;
    if ((TObjArray*)gDirectory->Get(TOA_name.c_str())) {
      output = (TObjArray*)gDirectory->Get(TOA_name.c_str());
      output->Clear();
    } else {
      output = new TObjArray;
      output->SetName(TOA_name.c_str());
    }
    THnSparse* curSparse = (THnSparse*)gDirectory->Get(SparseIt.c_str());
    //(sparse , first zoom axis, second zoom axis, projection axis {x,y} ) the zoom axis format is <axis # ,< RL, RH>>
    // returns the projected 2d histogram
    TH2D* projectedSparse;
    if (GammaType == "N"){
      projectedSparse = SparseZP(curSparse,{2,{qdcCut}},{3,{tapeCut}},{0,2});
    } else {
      projectedSparse = SparseZP(curSparse,{2,{qdcCut}},{3,{tapeCut}},{0,1});
    }
    output->Add(projectedSparse);

    cout<<"Projecting: "<<nSparseList.at(CurrentSparseIndex)<<endl;
    vector<TH1D*> Projec_vec;
    if (GammaType == "N"){ // Neutron Singles dont need the LineProjector() so ill do its full projection here
      TH1::AddDirectory(kFALSE);
      //      TH1D* SinglesProj = projectedSparse->ProjectionX("dd_tof_PX",qdcCut.first,qdcCut.second);
      output->Add(projectedSparse->ProjectionX("dd_tof_PX",qdcCut.first,qdcCut.second));
      //delete SinglesProj;
    } else {
      for (auto itGamma : GammaEnergies){
        Projec_vec = LineProjector(projectedSparse,projecBaseName,GammaType,itGamma,Colors.find(GammaType)->second);
      
        //add to the TObjArray
        for(auto itProjection : Projec_vec){
          output->Add(itProjection);
        }
      }
    }
    // add the finalized TObjArray to the TDirectory
    gDirectory->Add(output);

  } // end for over sparse list
  TH1::AddDirectory(kTRUE); // Reset this to normal 
  return 0;
}

void PlotFullCrayon(string type){

  string Array = "Crayon_" + type;
  if (strlen(type.c_str()) >1){
    cout<<"ERROR:: Type Specifier too long Needs to be (h,n,c,N)"<<endl;
    return ;    
  }

  TObjArray* array = (TObjArray*)gDirectory->Get(Array.c_str());
  
  if (!array){
    cout<<"ERROR:: TObjArray Not found"<<endl;
    return ;
  } 

  cout<<"2"<<endl;
  string canv = "c_"+type;
  TCanvas* c1 = new TCanvas(canv.c_str(),canv.c_str());
  
  Bool_t FirstPlot = true;
  for (auto it=0; it<array->GetSize(); it++){
    cout<<"start"<<endl;
    string classn = array->At(it)->ClassName();
    string HistName = array->At(it)->GetName();
    if(strncmp(classn.c_str(),"TH1D",10) !=0 || strstr(HistName.c_str(),"_On") == NULL){
      continue;
    }

    cout<<"it="<<it<<endl;
    cout<<"Current address = "<<array->At(it)<<endl;
    cout<<"Current class = "<<classn<<endl;
    cout<<"Current name = "<<HistName<<endl;

    TH1D* tmp = (TH1D*)array->At(it);

    if (FirstPlot){
      tmp->Draw();
      FirstPlot=false;
    }else{
      tmp->Draw("SAME");
    }
    cout<<"loopend"<<endl;
  }
  cout<<"3"<<endl;
  return;
}

void PlotSingleCrayonHist(string type = "h", string gamE = "814", string subtype = "On",string overlay = ""){
  TH1D* his2Plot;
  string Array = "Crayon_" + type;

  TObjArray* array = (TObjArray*)gDirectory->Get(Array.c_str());

  if (!array){
    cout<<"ERROR:: TObjArray Not found"<<endl;
    return ;
  }
  if (strlen(gamE.c_str()) <3){
    his2Plot = (TH1D*) array->At(stoi(gamE));
  } else { 
    string histString = "dd_tof_"+type+"_"+gamE+"_"+subtype;
    his2Plot = (TH1D*)array->FindObject(histString.c_str());
  }

  his2Plot->Draw(overlay.c_str());
  return;
}
