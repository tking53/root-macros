#include "TApplication.h"
#include "TCanvas.h"
#include "TObjArray.h"
#include "TROOT.h"
#include "TSystem.h"

using namespace std;
TCanvas* TC(string name) {
    TCanvas* tmp = new TCanvas(name.c_str(),name.c_str());
    
    //gDirectory->Add(tmp); //Canvas' dont get added to Dir
    return tmp;
}
TCanvas* tc(string name) {
    TCanvas* tmp = new TCanvas(name.c_str(),name.c_str());
    
   // gDirectory->Add(tmp); //Canvas' dont get added to Dir
    return tmp;
}