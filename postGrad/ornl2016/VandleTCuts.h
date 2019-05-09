
#ifndef Vandle_TCuts_h
#define Vandle_TCuts_h
// Root Headers needed
#include <TObject.h>
#include <TROOT.h>
#include "TCutG.h"

namespace VandleTCuts {
// TCutG* VTQ_fullQ = new TCutG("VTQ_fullQ", 12);
// TCutG* CBdT = new TCutG("clover_beta_tdiff", 17);

//! Example on how to load the TCUTs 
// nGinV = new TCutG("VTQ_fullQ", 12);
// for (Int_t it = 0; it < VandleTCuts::VTQ_fullQ_vec.size(); it++) {
//     nGinV->SetPoint(it, VandleTCuts::VTQ_fullQ_vec.at(it).first, VandleTCuts::VTQ_fullQ_vec.at(it).second);
// }

// valid_CBdT = new TCutG("clover_beta_tdiff", 17);
// for (Int_t it = 0; it < VandleTCuts::CloBetaTDiff_cut.size(); it++) {
//     valid_CBdT->SetPoint(it, VandleTCuts::CloBetaTDiff_cut.at(it).first, VandleTCuts::CloBetaTDiff_cut.at(it).second);
// }

std::vector<std::pair<Double_t, Double_t>> VTQ_fullQ_vec = {{21.7007, 11570.8}, {48.781, 534.735}, {48.2701, 40.886}, {391.117, 45.9992}, {391.117, 229.405}, {316.263, 263.684}, {189.547, 721.779}, {106.007, 2582.46}, {69.9854, 9959.34}, {63.0876, 11821.4}, {23.489, 11821.4}, {21.7007, 11570.8}};

std::vector<std::pair<Double_t, Double_t>> CloBetaTDiff_cut = {{67.2575, 935.835}, {125.912, 691.18}, {297.967, 512.45}, {599.061, 417.225}, {1920.75, 348.37}, {3582.64, 314.675}, {5549.53, 291.235}, {5690.3, 232.635}, {5338.37, 179.895}, {3402.76, 200.405}, {1005.74, 200.405}, {638.165, 204.8}, {137.643, 247.285}, {55.5265, 399.645}, {32.0646, 595.955}, {32.0646, 921.185}, {67.2575, 927.045}};

// TCutG* CBdt = new TCutG("clover_beta_tdiff", 17);
// CBdT->SetVarX("Clover Energy");
// CBdT->SetVarY("Clover-Beta dt");
// CBdT->SetTitle("Graph");
// CBdT->SetFillStyle(1000);
// CBdT->SetPoint(0, 67.2575, 935.835);
// CBdT->SetPoint(1, 125.912, 691.18);
// CBdT->SetPoint(2, 297.967, 512.45);
// CBdT->SetPoint(3, 599.061, 417.225);
// CBdT->SetPoint(4, 1920.75, 348.37);
// CBdT->SetPoint(5, 3582.64, 314.675);
// CBdT->SetPoint(6, 5549.53, 291.235);
// CBdT->SetPoint(7, 5690.3, 232.635);
// CBdT->SetPoint(8, 5338.37, 179.895);
// CBdT->SetPoint(9, 3402.76, 200.405);
// CBdT->SetPoint(10, 1005.74, 200.405);
// CBdT->SetPoint(11, 638.165, 204.8);
// CBdT->SetPoint(12, 137.643, 247.285);
// CBdT->SetPoint(13, 55.5265, 399.645);
// CBdT->SetPoint(14, 32.0646, 595.955);
// CBdT->SetPoint(15, 32.0646, 921.185);
// CBdT->SetPoint(16, 67.2575, 927.045);

// TCutG* VTQ_fullQ = new TCutG("VTQ_fullQ", 12);
// VTQ_fullQ->SetVarX("Tof");
// VTQ_fullQ->SetVarY("QDC");
// VTQ_fullQ->SetTitle("Graph");
// VTQ_fullQ->SetFillStyle(1000);
// VTQ_fullQ->SetPoint(0, 21.7007, 11570.8);
// VTQ_fullQ->SetPoint(1, 48.781, 534.735);
// VTQ_fullQ->SetPoint(2, 48.2701, 40.886);
// VTQ_fullQ->SetPoint(3, 391.117, 45.9992);
// VTQ_fullQ->SetPoint(4, 391.117, 229.405);
// VTQ_fullQ->SetPoint(5, 316.263, 263.684);
// VTQ_fullQ->SetPoint(6, 189.547, 721.779);
// VTQ_fullQ->SetPoint(7, 106.007, 2582.46);
// VTQ_fullQ->SetPoint(8, 69.9854, 9959.34);
// VTQ_fullQ->SetPoint(9, 63.0876, 11821.4);
// VTQ_fullQ->SetPoint(10, 23.489, 11821.4);
// VTQ_fullQ->SetPoint(11, 21.7007, 11570.8);

};  // namespace VandleTCuts

#endif