#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TKey.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TROOT.h"

using namespace std;

// vector<Double_t> GammaEnergies = {414.3, 692, 814, 1037.3, 1180, 1335.9, 1402.4, 1506.9, 2083};  //Combining 813 and 815 into 1 line since they are overlapping (will use a slightly larger sigma for that line)

vector<Double_t> GammaEnergies = {814};  //Combining 813 and 815 into 1 line since they are overlapping (will use a slightly larger sigma for that line)

map<string, pair<Double_t, Double_t>> GammaRes = {{"h", {4.8, 7}}, {"n", {7.6, 7}}, {"c", {0.75, 1.25}}};  // ~% resolutions as % first is single lines and second is 813/815

map<string, Color_t> Colors = {{"N", kBlack}, {"h", kRed}, {"n", kGreen}, {"c", kBlue}};

vector<string> AutoLoadTHnSparseList(TFile* file, vector<string> excludes = {"T", "G"}) {
    TIter keyList(file->GetListOfKeys());
    TKey* key;
    vector<string> sparseList;
    while ((key = (TKey*)keyList())) {
        string sparseClass = key->GetClassName();
        string sparseName = key->GetName();
        if (!strstr(sparseClass.c_str(), "THnSparseT") || !strstr(sparseName.c_str(), "dd_tof_")) {
            continue;
        }
        Bool_t excluded = false;
        for (auto it : excludes) {
            if (sparseName.substr(7, 1) == it) {
                excluded = true;
            }  //end if
        }      //end for

        if (!excluded) {
            sparseList.emplace_back(sparseName);
        }
    }  // end while
    return (sparseList);
}

double Exp1(double* xx, double* par) {
    double amp1 = par[0];
    double lambda1 = par[1];
    double mean1 = par[2];
    double x = xx[0];
    double result1 = amp1 * TMath::Exp(lambda1 * (x - mean1));  // Xu's
    return (result1);
}

double Gaus1(double* xx, double* par) {
    double gconst = par[0];
    double gmean = par[1];
    double gsigma = par[2];

    double x = xx[0];
    if (x < gmean) {
        return 0;
    } else {
        return gconst * (1 - TMath::Gaus(x, gmean, gsigma, kFALSE));
    }
}

double GausExpExpExp(double* xx, double* par) {
    double gconst = par[0];
    double gmean = par[3];
    double gsigma = par[2];

    double amp1 = par[4];
    double lambda1 = par[5];

    double amp2 = par[6];
    double lambda2 = par[7];

    double amp3 = gconst - amp2 - amp1;
    double lambda3 = par[8];

    double x = xx[0];

    if (x <= par[3]) {
        return Gaus1(xx, par);
    } else {
        double result1 = 0;
        double result2 = 0;
        double result3 = 0;
        if (amp1 >= 0) {
            result1 = amp1 * TMath::Exp(lambda1 * (x - gmean));
        }
        if (amp2 >= 0) {
            result2 = amp2 * TMath::Exp(lambda2 * (x - gmean));
        }
        if (amp3 >= 0) {
            result3 = amp3 * TMath::Exp(lambda3 * (x - gmean));
        }
        return (result1 + result2 + result3);
    }
}

//Range input format is <Axis,<RL,RH>>
TH2D* SparseZP(THnSparse* sparse, pair<Int_t, pair<Double_t, Double_t>> ZoomAxisRange1, pair<Int_t, pair<Double_t, Double_t>> ZoomAxisRange2, pair<Int_t, Int_t> AxisProj, Bool_t addDir = false) {
    if (addDir) {
        TH1::AddDirectory(kTRUE);
    } else {
        TH1::AddDirectory(kFALSE);
    }
    sparse->GetAxis(ZoomAxisRange1.first)->SetRangeUser((ZoomAxisRange1.second).first, (ZoomAxisRange1.second).second);
    sparse->GetAxis(ZoomAxisRange2.first)->SetRangeUser((ZoomAxisRange2.second).first, (ZoomAxisRange2.second).second);
    return (sparse->Projection(AxisProj.second, AxisProj.first));  //Root Likes Y:X for 2ds for some reason (X:Y:Z, X for the others)
}

vector<TH1D*> LineProjector(TH2D* histo, string HistBaseName, string GamType, Double_t GamEnergy, Color_t Color, Bool_t subtract = false) {
    TH1::AddDirectory(kFALSE);
    stringstream ss;
    ss << GamEnergy;  // This is needed to trim the trailing zeros after the conversion from the Double to a string
    vector<string> ProjOrder = {"On", "Under", "Over", "Subtracted"};
    vector<string> HistoNames = {HistBaseName + "_" + GamType + "_" + ss.str().c_str() + "_" + ProjOrder.at(0),
                                 HistBaseName + "_" + GamType + "_" + ss.str().c_str() + "_" + ProjOrder.at(1),
                                 HistBaseName + "_" + GamType + "_" + ss.str().c_str() + "_" + ProjOrder.at(2),
                                 HistBaseName + "_" + GamType + "_" + ss.str().c_str() + "_" + ProjOrder.at(3)};
    vector<TH1D*> returnVec;
    Double_t HalfWidth;

    if (GamEnergy == 814) {
        HalfWidth = ((GammaRes.find(GamType.c_str())->second.first / 100) * GamEnergy) / 2;
    } else {
        HalfWidth = ((GammaRes.find(GamType.c_str())->second.second / 100) * GamEnergy) / 2;
    }

    vector<pair<Double_t, Double_t>> projectionBounds = {{GamEnergy - HalfWidth, GamEnergy + HalfWidth}, {GamEnergy - 3 * HalfWidth, GamEnergy - HalfWidth}, {GamEnergy + HalfWidth, GamEnergy + 3 * HalfWidth}};
    for (auto it = 0; it < 3; it++) {
        TH1D* tmp = histo->ProjectionX(ProjOrder.at(it).c_str(), projectionBounds.at(it).first, projectionBounds.at(it).second);
        returnVec.emplace_back(tmp);
    }

    if (subtract) {
        TH1D* tmp0 = (TH1D*)returnVec.at(0)->Clone();  // clone them to avoid messing up the originals
        TH1D* tmp1 = (TH1D*)returnVec.at(1)->Clone();  // clone them to avoid messing up the originals
        TH1D* tmp2 = (TH1D*)returnVec.at(2)->Clone();  // clone them to avoid messing up the originals
        tmp1->Scale(0.5);                              // the over and under gates are the same width as the on gate so scale by 1/2
        tmp2->Scale(0.5);                              // the over and under gates are the same width as the on gate so scale by 1/2
        tmp0->Add(tmp1, -1);
        tmp0->Add(tmp2, -1);
        returnVec.emplace_back(tmp0);
    }

    for (UInt_t it = 0; it < returnVec.size(); it++) {
        returnVec.at(it)->SetName(HistoNames.at(it).c_str());
        Color_t LineColor = Colors.find(GamType)->second + it;
        returnVec.at(it)->SetLineColor(LineColor);
    }

    return (returnVec);
}

TObjArray* QDCbackground_(TH2* hist, pair<Int_t, Int_t> tofRange, Bool_t addDir = false, pair<Int_t, Int_t> fullFit = {20, 20000}, pair<Int_t, Int_t> expFit0 = {20, 140}, pair<Int_t, Int_t> expFit1 = {140, 500}, pair<Int_t, Int_t> expFit2 = {500, 2000}, pair<Int_t, Int_t> expFit3 = {2000, 8000}) {
    if (addDir) {
        TH1::AddDirectory(kTRUE);
    } else {
        TH1::AddDirectory(kFALSE);
    }

    if (tofRange.first >= tofRange.second) {
        cout << "ToF Range Low must be less than ToF Range High" << endl;
        TObjArray* retVal = new TObjArray;
        retVal->SetName("error_array");
        return retVal;
    }

    pair<Int_t, Int_t> tofRangeBins = {hist->GetXaxis()->FindBin(tofRange.first),
                                       hist->GetXaxis()->FindBin(tofRange.second)};

    string qdcBKGname_ = (string)hist->GetName() + "qdcbackground_";
    TH1D* qdcbackground_ = hist->ProjectionY(qdcBKGname_.c_str(), tofRangeBins.first, tofRangeBins.second);
    qdcbackground_->GetXaxis()->SetRangeUser(20, 20000);
    if (addDir) {
        qdcbackground_->SetLineColor(kBlack);
        qdcbackground_->SetLineWidth(2);  //Set Color things for drawing if single run
    }

    //Fit the single funcs for initial paras
    TF1* expFunc0 = new TF1("expFunc0", "Gaus1", expFit0.first, expFit0.second, 3);
    TF1* expFunc1 = new TF1("expFunc1", "Exp1", expFit1.first, expFit1.second, 3);
    TF1* expFunc2 = new TF1("expFunc2", "Exp1", expFit2.first, expFit2.second, 3);
    TF1* expFunc3 = new TF1("expFunc3", "Exp1", expFit3.first, expFit3.second, 3);

    expFunc0->SetParameter(0, qdcbackground_->GetMaximum());
    expFunc0->SetParameter(1, 0);   // inverted Gaus has centroid at 0
    expFunc0->SetParameter(2, 75);  // sigma for the gaus

    expFunc1->SetParameter(0, qdcbackground_->GetMaximum());
    expFunc1->SetParameter(1, (Double_t)-2e-3);  // nominal from ornl2016 but works for IS632 as well
    expFunc1->FixParameter(2, qdcbackground_->GetBinCenter(qdcbackground_->GetMaximumBin()));

    expFunc2->SetParameter(0, 0.1 * qdcbackground_->GetMaximum());
    expFunc2->SetParameter(1, (Double_t)-2e-4);
    expFunc2->FixParameter(2, qdcbackground_->GetBinCenter(qdcbackground_->GetMaximumBin()));

    expFunc3->SetParameter(0, 0.01 * qdcbackground_->GetMaximum());
    expFunc3->SetParameter(1, (Double_t)-2e-5);
    expFunc3->FixParameter(2, qdcbackground_->GetBinCenter(qdcbackground_->GetMaximumBin()));

    qdcbackground_->Fit("expFunc0", "RL+");
    qdcbackground_->Fit("expFunc1", "RL+");
    qdcbackground_->Fit("expFunc2", "RL+");
    qdcbackground_->Fit("expFunc3", "RL+");

    // FULL FITTING
    string functionName = (string)hist->GetName() + "_qdcFunc";
    TF1* qdcFunc = new TF1(functionName.c_str(), GausExpExpExp, fullFit.first, fullFit.second, 9);
    qdcFunc->SetNpx(1000);  // add points to the fit

    qdcFunc->SetParameter(0, expFunc0->GetParameter(0));
    qdcFunc->FixParameter(1, expFunc0->GetParameter(1));  //fix the gaus centroid
    qdcFunc->SetParameter(2, expFunc0->GetParameter(2));

    qdcFunc->FixParameter(3, qdcbackground_->GetBinCenter(qdcbackground_->GetMaximumBin()));  //Set the inflection point for where we stop being the Gaus1 and start the Exp

    qdcFunc->SetParameter(4, expFunc1->GetParameter(0) * 0.55);  //0.55 is the scaling factor to make the exp1+exp2+exp3 match the gaus at Par[3]
    qdcFunc->SetParameter(5, expFunc1->GetParameter(1));

    qdcFunc->SetParameter(6, expFunc2->GetParameter(0) * 0.35);  //0.55 is the scaling factor to make the exp1+exp2+exp3 match the gaus at Par[3]
    qdcFunc->SetParameter(7, expFunc2->GetParameter(1));

    qdcFunc->SetParameter(8, expFunc3->GetParameter(1));  //the amp of the 3rd exp is derived from the other 2 so we dont set it

    qdcbackground_->Fit(functionName.c_str(), "RLM");

    if (addDir) {  // again add colors if we are running stand alone;
        qdcFunc->SetLineColor(kViolet - 2);
        qdcFunc->SetLineWidth(3);
        qdcFunc->Draw("SAME");
    }

    //Make the 2d background
    string exp2D_key = (string)hist->GetName() + "_expBackground";
    string exp2D_name = (string)hist->GetTitle() + " expBackground";
    TH2D* expBackground = new TH2D(exp2D_key.c_str(), exp2D_name.c_str(), hist->GetNbinsX(), 0, hist->GetXaxis()->GetXmax(), hist->GetNbinsY(), 0, hist->GetYaxis()->GetXmax());
    for (int tof_bin = 0; tof_bin < hist->GetNbinsX(); tof_bin++) {
        TH1D* qdc_Proj = hist->ProjectionY("qdc_Proj", tof_bin, tof_bin);
        for (int qdc_bin = 30; qdc_bin < qdc_Proj->GetNbinsX(); qdc_bin++) {
            expBackground->SetBinContent(tof_bin, qdc_bin, qdcFunc->Eval(qdc_Proj->GetBinCenter(qdc_bin)));
        }
    }
    TH2D* t1 = (TH2D*)hist->Clone();
    string t1Name = (string)hist->GetName() + "_sub";
    t1->SetName(t1Name.c_str());
    Double_t scaler = (Double_t)1.0 / (tofRangeBins.second - tofRangeBins.first);
    cout << "scaler = " << scaler << "     tofRangeBins.first= " << tofRangeBins.first << "     tofRangeBins.second=" << tofRangeBins.second << endl;
    expBackground->Scale(scaler);
    t1->Add(expBackground, -1);
    t1->SetMinimum(1);
    t1->SetMaximum(1200);

    TObjArray* retVal = new TObjArray;
    if (!addDir) {
        cout << "Adding to return array" << endl;
        string arrayName = (string)hist->GetName() + "_tofR_" + to_string(tofRange.first) + "-" + to_string(tofRange.second);
        retVal->SetName(arrayName.c_str());

        retVal->Add((TH2*)t1->Clone());
        retVal->Add((TH2*)hist->Clone());
        retVal->Add((TH2*)expBackground->Clone());
        retVal->Add((TH1*)qdcFunc->Clone());
        t1->Delete();
        expBackground->Delete();
        qdcFunc->Delete();
        // hist->Delete();
        cout << "returning array" << endl;
        return retVal;
    } else {
        cout << "Histograms returned to gDirectory. Array is empty" << endl;
        retVal->SetName("empty_array");
        return retVal;
    }
}

//! Gamma Line Tof Projector using the qdc depended background subtraction

map<string, TObjArray*> GammaProjector(Double_t GamEnergy, vector<string> nSparseList = {}, pair<Int_t, Int_t> tapeCut = {1, 300}, pair<Int_t, Int_t> TOFRANGE = {400, 600}) {
    map<string, TObjArray*> returnMap_;

    for (auto it = nSparseList.begin(); it != nSparseList.end(); it++) {
        auto SparseIt = (*it);
        THnSparse* curSparse = (THnSparse*)gDirectory->Get(SparseIt.c_str());
        string GammaType, GamDet, projecBaseName = "dd_tof_qdc";
        GammaType = SparseIt.substr(7, 1);  // New97Rb THnSparse is 8th letter
        if (GammaType == "c") {
            GamDet = "Clover";
            projecBaseName += "_c_";
        } else if (GammaType == "h") {
            GamDet = "LaBr";
            projecBaseName += "_h_";
        } else if (GammaType == "n") {
            GamDet = "NaI";
            projecBaseName += "_n_";
        } else if (GammaType == "T") {
            GamDet = "PixieTAS";
            projecBaseName += "_T_";
        } else if (GammaType == "N") {
            GamDet = "None";
            continue;
        } else {
            cout << "ERROR::Unknown Gamma Det Type (" << GammaType << ")" << endl;
            continue;
        }
        stringstream projectionName;
        projectionName << projecBaseName << GamEnergy;
        //projecBaseName += to_string(GamEnergy);

        Double_t GamL, GamH;
        if (GamEnergy == 814) {
            // if (GammaType == "c") {
            //     cout<<"Special CLover"<<endl;
            //     GamEnergy = 809;
            // }
            GamL = GamEnergy - ((GammaRes.find(GammaType.c_str())->second.first / 100) * GamEnergy) / 2;
            GamH = GamEnergy + ((GammaRes.find(GammaType.c_str())->second.first / 100) * GamEnergy) / 2;
            // if (GammaType == "c") {
            //     GamEnergy = 814;
            // }
        } else {
            GamL = GamEnergy - ((GammaRes.find(GammaType.c_str())->second.second / 100) * GamEnergy) / 2;
            GamH = GamEnergy + ((GammaRes.find(GammaType.c_str())->second.second / 100) * GamEnergy) / 2;
        }

        TH2D* workingHist = SparseZP(curSparse, {3, {tapeCut.first, tapeCut.second}}, {1, {GamL, GamH}}, {0, 2}, false);
        cout<<"GamL = "<<GamL << "       GamH= "<< GamH<<endl;
        workingHist->SetName(projectionName.str().c_str());
        workingHist->Sumw2();
        projectionName << "_subd";
        TObjArray* tmp2 = QDCbackground_(workingHist, {TOFRANGE.first, TOFRANGE.second}, false);
        ((TH2D*)tmp2->First())->SetName(projectionName.str().c_str());
        string newTOAName = (string)tmp2->GetName() + "_" + to_string(GamEnergy);
        tmp2->SetName(newTOAName.c_str());
        cout << "Previous fit was for detector type:" << GammaType << " and  GammaEnergy= " << GamEnergy << endl;
        returnMap_.emplace(GammaType, tmp2);
        workingHist->Delete();
        //tmp2->Delete();
        cout << "deleted the things" << endl;
    }

    cout << "returning the things" << endl;
    return returnMap_;
}

Int_t NewCrayon(string inFileStr = "_file0", vector<string> nSparseList = {}, pair<Double_t, Double_t> qdcCut = {0, -1}, pair<Double_t, Double_t> TOFRANGE = {400, 600}) {
    if (!gROOT->GetFile() && inFileStr == "_file0") {
        cout << "No File is opened" << endl;
        return 1;
    }

    if (inFileStr == "_file0") {
        inFileStr = gROOT->GetFile()->GetName();
    }
    TFile* inFile = new TFile(inFileStr.c_str());

    if (nSparseList.empty()) {
        nSparseList = AutoLoadTHnSparseList(inFile);
    }

    // vector<map<string, TObjArray*>> fullContainer;  //<GammaIndex,< return maps >>
    TFile* outFile = new TFile("newCrayon_2.root", "RECREATE");
    inFile->cd();
    TObjArray* shortList = new TObjArray;
    shortList->SetName("shortList");

    //! Do the N singles here
    TH2* singlesHist = SparseZP((THnSparse*)gDirectory->Get("dd_tof_NA_qdc_tape"), {3, {1, 300}}, {1, {0, 1}}, {0, 2}, false);
    singlesHist->SetName("dd_tof_qdc_neutronSingles");
    singlesHist->Sumw2();  //needed to care errors through
    TObjArray* nSinglesArray_ = QDCbackground_(singlesHist, {TOFRANGE.first, TOFRANGE.second}, false);
    ((TH2D*)nSinglesArray_->First())->SetName("dd_tof_qdc_singles_subed");
    string newTOAName = (string)nSinglesArray_->GetName() + "_neutronSingles";
    nSinglesArray_->SetName(newTOAName.c_str());

    outFile->cd();
    nSinglesArray_->Write();
    inFile->cd();

    //! store the full return for debuggin and checks
    //fullContainer.emplace_back(singlesMap);

    //! pull the subtracted one out
    shortList->Add((TH2D*)nSinglesArray_->First()->Clone());

    for (UInt_t it = 0; it < GammaEnergies.size(); it++) {
        cout << "gamm loop (" << it << ")" << endl;
        cout << "curEnergy = " << GammaEnergies.at(it) << endl;
        Double_t curEnergy = GammaEnergies.at(it);
        map<string, TObjArray*> tmp = GammaProjector(curEnergy, nSparseList);

        cout << "Writing" << endl;
        //! Pull the subtracted histos out of the map
        for (auto iti = tmp.begin(); iti != tmp.end(); iti++) {
            outFile->cd();
            (*iti).second->Write();
            inFile->cd();

            TH2D* subtractedHist = (TH2D*)((*iti).second->First()->Clone());
            shortList->Add(subtractedHist);
            (*iti).second->Delete();
        }
        tmp.clear();
    }

    inFile->Close();
    outFile->cd();
    shortList->Write();
    gApplication->Terminate();

    return 0;
}

Int_t OldNeutronCrayon(string inFileStr = "_file0", vector<string> nSparseList = {}, pair<Double_t, Double_t> qdcCut = {0, -1}, pair<Int_t, Int_t> tapeCut = {1, 300}, Bool_t subtract = false) {
    if (!gROOT->GetFile() && inFileStr == "_file0") {
        cout << "No File is opened" << endl;
        return 1;
    }

    if (inFileStr == "_file0") {
        inFileStr = gROOT->GetFile()->GetName();
    }
    TFile* inFile = new TFile(inFileStr.c_str());

    if (nSparseList.empty()) {
        nSparseList = AutoLoadTHnSparseList(inFile);
    }

    for (auto it = nSparseList.begin(); it != nSparseList.end(); it++) {
        Int_t CurrentSparseIndex = it - nSparseList.begin();
        auto SparseIt = (*it);
        string GammaType, GamDet, projecBaseName = "dd_tof";
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
        } else if (GammaType == "N") {
            GamDet = "None";
            projecBaseName = "dd_tof_qdc";
        } else {
            cout << "ERROR::Unknown Gamma Det Type (" << GammaType << ")" << endl;
            return 2;
        }
        cout << "Processing Sparse: " << SparseIt << endl;

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
        if (GammaType == "N") {
            projectedSparse = SparseZP(curSparse, {2, {qdcCut}}, {3, {tapeCut}}, {0, 2});
        } else {
            projectedSparse = SparseZP(curSparse, {2, {qdcCut}}, {3, {tapeCut}}, {0, 1});
        }
        output->Add(projectedSparse);

        cout << "Projecting: " << nSparseList.at(CurrentSparseIndex) << endl;
        vector<TH1D*> Projec_vec;
        if (GammaType == "N") {  // Neutron Singles dont need the LineProjector() so ill do its full projection here
            TH1::AddDirectory(kFALSE);
            //      TH1D* SinglesProj = projectedSparse->ProjectionX("dd_tof_PX",qdcCut.first,qdcCut.second);
            output->Add(projectedSparse->ProjectionX("dd_tof_PX", qdcCut.first, qdcCut.second));
            //delete SinglesProj;
        } else {
            for (auto itGamma : GammaEnergies) {
                Projec_vec = LineProjector(projectedSparse, projecBaseName, GammaType, itGamma, Colors.find(GammaType)->second);

                //add to the TObjArray
                for (auto itProjection : Projec_vec) {
                    output->Add(itProjection);
                }
            }
        }
        // add the finalized TObjArray to the TDirectory
        gDirectory->Add(output);

    }                          // end for over sparse list
    TH1::AddDirectory(kTRUE);  // Reset this to normal
    return 0;
}

void PlotFullCrayon(string type) {
    string Array = "Crayon_" + type;
    if (strlen(type.c_str()) > 1) {
        cout << "ERROR:: Type Specifier too long Needs to be (h,n,c,N)" << endl;
        return;
    }

    TObjArray* array = (TObjArray*)gDirectory->Get(Array.c_str());

    if (!array) {
        cout << "ERROR:: TObjArray Not found" << endl;
        return;
    }

    cout << "2" << endl;
    string canv = "c_" + type;
    TCanvas* c1 = new TCanvas(canv.c_str(), canv.c_str());

    Bool_t FirstPlot = true;
    for (auto it = 0; it < array->GetSize(); it++) {
        cout << "start" << endl;
        string classn = array->At(it)->ClassName();
        string HistName = array->At(it)->GetName();
        if (strncmp(classn.c_str(), "TH1D", 10) != 0 || strstr(HistName.c_str(), "_On") == NULL) {
            continue;
        }

        cout << "it=" << it << endl;
        cout << "Current address = " << array->At(it) << endl;
        cout << "Current class = " << classn << endl;
        cout << "Current name = " << HistName << endl;

        TH1D* tmp = (TH1D*)array->At(it);

        if (FirstPlot) {
            tmp->Draw();
            FirstPlot = false;
        } else {
            tmp->Draw("SAME");
        }
        cout << "loopend" << endl;
    }
    cout << "3" << endl;
    return;
}

void PlotSingleCrayonHist(string type = "h", string gamE = "814", string subtype = "On", string overlay = "") {
    TH1D* his2Plot;
    string Array = "Crayon_" + type;

    TObjArray* array = (TObjArray*)gDirectory->Get(Array.c_str());

    if (!array) {
        cout << "ERROR:: TObjArray Not found" << endl;
        return;
    }
    if (strlen(gamE.c_str()) < 3) {
        his2Plot = (TH1D*)array->At(stoi(gamE));
    } else {
        string histString = "dd_tof_" + type + "_" + gamE + "_" + subtype;
        his2Plot = (TH1D*)array->FindObject(histString.c_str());
    }

    his2Plot->Draw(overlay.c_str());
    return;
}
