#ifndef __DETCHECKHISTOS_HPP__
#define __DETCHECKHISTOS_HPP__

#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TTreeReaderValue.h>

#include <vector>
#include <iostream>
#include <iomanip>

#include "TApplication.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TObjArray.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"

using namespace std;


class DetCheckHistos {
   public:
   struct detdata {
    int detNum;
    int locNum;
    TTreeReaderValue<Double_t>* eslow;
    TTreeReaderValue<Double_t>* efast;
    TTreeReaderValue<Double_t>* twrap;
    TTreeReaderValue<Double_t>* pheight;
   };
   struct detInfo {
        double psd_low;
        double psd_high;
        double psd_eslow_thresh;
        double pheight_clip;
        double eslow_cal_m;
    };
    
    void VecBuilder(TTreeReader* Reader, bool lanltree = false);

    DetCheckHistos(TFile* file, string treename = "stage0");
    DetCheckHistos(int startfile, int stopfile, string treeName = "stage0");
    ~DetCheckHistos() = default;

    int TreeWalker();

    int Make_Single_Type_Histos(vector<int> Chans, string dettype, TObjArray& objarray);

    int Make_Mike_Histos(vector<int> Chans, string dettype, TObjArray& objarray);

    void Plot2DirectoryDump(TObjArray*);

    int PlotNeutronSuperPlots(string type2Plot, string subplot2plot, TCanvas* SuperPlot);

    void GetArraySizes() {
        cout << "Stilbene Plot TObjArray size = " << stilbenePlots.GetEntriesFast() << endl;
        cout << "ODeSA Plot TObjArray size = " << odesaPlots.GetEntriesFast() << endl;
        cout << "HPGe Plot TObjArray size = " << hpgePlots.GetEntriesFast() << endl;
        cout << "Si Plot TObjArray size = " << siPlots.GetEntriesFast() << endl;
        cout << "Combo Plot TObjArray size = " << comboPlots.GetEntriesFast() << endl;
    }

    int BuildNeutronGatesVector(string dettype, vector<detInfo>& detInfoVec);

    TTree* GetLoadedTTree(){return tree;}

    void SetLanlTreeBool(bool a){lanlTree = a;}
    bool GetLanlTreeBool(){return lanlTree;}

   private:
    const vector<int> odesaChans = {32, 34, 36, 38, 40, 48, 50, 52, 54, 56};
    const vector<int> stilbeneChans = {4, 5, 6, 8, 9, 10, 12, 14};
    const vector<int> siChans = {16, 18, 20, 22, 24, 26};
    const vector<int> hpgeChans = {29, 31};

    vector<detdata> stilbene;
    vector<detdata> odesa;
    vector<detdata> hpge;
    vector<detdata> si;

    TObjArray stilbenePlots;
    TObjArray odesaPlots;
    TObjArray hpgePlots;
    TObjArray siPlots;
    TObjArray comboPlots;

    bool DataProcessed = false;

    TTreeReader LanlPlotter;
    TChain* chain;
    TFile* file;
    TTree* tree;

    bool lanlTree;

    vector<detInfo> odesa_gates;
    vector<bool> odesa_has_a_neutron;

  };

#endif