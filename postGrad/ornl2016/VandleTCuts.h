
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

    std::vector<std::pair<Double_t, Double_t>> HagHRBetaDT =  {{1272.89,20.3925},{6242.8,16.97},{7976.49,16.97},{7967.6,49.1424},{4455.77,50.8537},{890.59,54.2763},{468.281,57.0143},{188.223,63.175},{103.761,70.0202},{77.0893,91.5826},{50.4172,96.7165},{32.6357,111.091},{23.745,137.103},{14.8543,144.633},{5.96358,124.44},{5.96358,60.0947},{10.4089,63.5173},{19.2997,44.0085},{32.6357,37.1633},{117.098,26.211},{1268.45,20.3925},{1272.89,20.3925}};

    std::vector<std::pair<Double_t, Double_t>> NaiBetaDT = {{389.882,247.477},{974.867,231.133},{3731.13,212.746},{6466.19,214.789},{7905.96,185.505},{5351.53,167.799},{4224.4,160.308},{3106.26,166.437},{1912.83,150.774},{1191.93,119.448},{986.463,51.3477},{279.985,47.9426},{44.2562,86.7599},{15.7255,150.093},{1.40077,175.29},{1.3843,333.283},{1.40907,438.839},{15.1774,422.495},{69.7745,353.032},{87.8702,293.104},{396.859,248.839},{389.882,247.477}};

    std::vector<std::pair<Double_t,Double_t>> newHagHRBdt ={{153.083,-4.36398},{868.021,-11.4641},{5753.49,-12.5686},{7014.28,-11.1485},{6565.97,-1.68172},{5385.77,-0.577258},{2380.19,1.78944},{899.862,3.05169},{318.462,7.46953},{184.401,11.7296},{95.8369,18.3564},{63.3296,26.7187},{30.4422,50.3857},{19.639,74.8417},{7.03423,55.2769},{22.8197,22.3009},{42.1006,8.57399},{150.35,-4.2062},{150.35,-4.2062},{153.083,-4.36398}};

    std::vector<std::pair<Double_t,Double_t>> newHagLRBdt = {{4171.41,153.516},{5709.8,137.135},{5709.8,114.851},{5142.5,108.259},{3981.84,106.147},{3344.57,106.147},{2925.98,106.147},{2278.8,105.957},{1694.11,106.147},{1095.43,106.528},{700.124,106.528},{118.882,113.217},{46.6259,129.728},{11.3529,176.219},{9.04981,231.362},{27.3116,231.362},{65.3228,196.564},{131.997,180.694},{317.545,172.781},{680.066,169.107},{1195.24,163.448},{1774.76,161.412},{4171.41,153.516}};

    std::vector<std::pair<Double_t,Double_t>> newNaiLRBdt ={{161.75,66.012},{876.564,26.8461},{2652.4,17.0547},{3836.41,11.1798},{4539.81,-24.0694},{2848.21,-34.84},{947.388,-37.7775},{193.902,-49.5272},{55.214,8.24237},{23.3373,33.7002},{15.1232,37.6167},{5.6889,41.5333},{1.88006,55.2414},{2.04517,213.863},{5.76305,191.343},{10.5921,172.739},{20.2388,154.135},{41.7957,120.844},{67.4873,86.574},{96.9829,77.7617},{117.776,69.9285},{163.859,66.9911},{163.859,66.9911},{161.75,66.012}};

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

    // TCutG *hagHRBdt = new TCutG("LaBr_HRB_TDIFF",22);
    // hagHRBdt->SetVarX("LaBr Energy vs LaBr- high res Beta dt");
    // hagHRBdt->SetVarY("");
    // hagHRBdt->SetFillStyle(1000);
    // hagHRBdt->SetPoint(0,1272.89,20.3925);
    // hagHRBdt->SetPoint(1,6242.8,16.97);
    // hagHRBdt->SetPoint(2,7976.49,16.97);
    // hagHRBdt->SetPoint(3,7967.6,49.1424);
    // hagHRBdt->SetPoint(4,4455.77,50.8537);
    // hagHRBdt->SetPoint(5,890.59,54.2763);
    // hagHRBdt->SetPoint(6,468.281,57.0143);
    // hagHRBdt->SetPoint(7,188.223,63.175);
    // hagHRBdt->SetPoint(8,103.761,70.0202);
    // hagHRBdt->SetPoint(9,77.0893,91.5826);
    // hagHRBdt->SetPoint(10,50.4172,96.7165);
    // hagHRBdt->SetPoint(11,32.6357,111.091);
    // hagHRBdt->SetPoint(12,23.745,137.103);
    // hagHRBdt->SetPoint(13,14.8543,144.633);
    // hagHRBdt->SetPoint(14,5.96358,124.44);
    // hagHRBdt->SetPoint(15,5.96358,60.0947);
    // hagHRBdt->SetPoint(16,10.4089,63.5173);
    // hagHRBdt->SetPoint(17,19.2997,44.0085);
    // hagHRBdt->SetPoint(18,32.6357,37.1633);
    // hagHRBdt->SetPoint(19,117.098,26.211);
    // hagHRBdt->SetPoint(20,1268.45,20.3925);
    // hagHRBdt->SetPoint(21,1272.89,20.3925);
  
    // TCutG *naiBdt = new TCutG("NaI_B_TDIFF",22);
    // naiBdt->SetVarX("nai Energy vs NaI-Beta dt");
    // naiBdt->SetVarY("");
    // naiBdt->SetTitle("Graph");
    // naiBdt->SetFillStyle(1000);
    // naiBdt->SetPoint(0,389.882,247.477);
    // naiBdt->SetPoint(1,974.867,231.133);
    // naiBdt->SetPoint(2,3731.13,212.746);
    // naiBdt->SetPoint(3,6466.19,214.789);
    // naiBdt->SetPoint(4,7905.96,185.505);
    // naiBdt->SetPoint(5,5351.53,167.799);
    // naiBdt->SetPoint(6,4224.4,160.308);
    // naiBdt->SetPoint(7,3106.26,166.437);
    // naiBdt->SetPoint(8,1912.83,150.774);
    // naiBdt->SetPoint(9,1191.93,119.448);
    // naiBdt->SetPoint(10,986.463,51.3477);
    // naiBdt->SetPoint(11,279.985,47.9426);
    // naiBdt->SetPoint(12,44.2562,86.7599);
    // naiBdt->SetPoint(13,15.7255,150.093);
    // naiBdt->SetPoint(14,1.40077,175.29);
    // naiBdt->SetPoint(15,1.3843,333.283);
    // naiBdt->SetPoint(16,1.40907,438.839);
    // naiBdt->SetPoint(17,15.1774,422.495);
    // naiBdt->SetPoint(18,69.7745,353.032);
    // naiBdt->SetPoint(19,87.8702,293.104);
    // naiBdt->SetPoint(20,396.859,248.839);
    // naiBdt->SetPoint(21,389.882,247.477);
    // naiBdt->Draw("");

    // newHagHRBdt = new TCutG("new_hagHRBdt",20);
    // newHagHRBdt->SetVarX("LaBr Energy vs LaBr- high res Beta dt");
    // newHagHRBdt->SetVarY("");
    // newHagHRBdt->SetTitle("Graph");
    // newHagHRBdt->SetFillStyle(1000);
    // newHagHRBdt->SetPoint(0,153.083,-4.36398);
    // newHagHRBdt->SetPoint(1,868.021,-11.4641);
    // newHagHRBdt->SetPoint(2,5753.49,-12.5686);
    // newHagHRBdt->SetPoint(3,7014.28,-11.1485);
    // newHagHRBdt->SetPoint(4,6565.97,-1.68172);
    // newHagHRBdt->SetPoint(5,5385.77,-0.577258);
    // newHagHRBdt->SetPoint(6,2380.19,1.78944);
    // newHagHRBdt->SetPoint(7,899.862,3.05169);
    // newHagHRBdt->SetPoint(8,318.462,7.46953);
    // newHagHRBdt->SetPoint(9,184.401,11.7296);
    // newHagHRBdt->SetPoint(10,95.8369,18.3564);
    // newHagHRBdt->SetPoint(11,63.3296,26.7187);
    // newHagHRBdt->SetPoint(12,30.4422,50.3857);
    // newHagHRBdt->SetPoint(13,19.639,74.8417);
    // newHagHRBdt->SetPoint(14,7.03423,55.2769);
    // newHagHRBdt->SetPoint(15,22.8197,22.3009);
    // newHagHRBdt->SetPoint(16,42.1006,8.57399);
    // newHagHRBdt->SetPoint(17,150.35,-4.2062);
    // newHagHRBdt->SetPoint(18,150.35,-4.2062);
    // newHagHRBdt->SetPoint(19,153.083,-4.36398);
    // newHagHRBdt->Draw("");

       
    //    TCutG *newHagLRBdt = new TCutG("newHagLRBdt_TCUT",23);
    //    newHagLRBdt->SetVarX("LaBr Energy vs LaBr- low res Beta dt");
    //    newHagLRBdt->SetVarY("");
    //    newHagLRBdt->SetTitle("Graph");
    //    newHagLRBdt->SetFillStyle(1000);
    //    newHagLRBdt->SetPoint(0,4171.41,153.516);
    //    newHagLRBdt->SetPoint(1,5709.8,137.135);
    //    newHagLRBdt->SetPoint(2,5709.8,114.851);
    //    newHagLRBdt->SetPoint(3,5142.5,108.259);
    //    newHagLRBdt->SetPoint(4,3981.84,106.147);
    //    newHagLRBdt->SetPoint(5,3344.57,106.147);
    //    newHagLRBdt->SetPoint(6,2925.98,106.147);
    //    newHagLRBdt->SetPoint(7,2278.8,105.957);
    //    newHagLRBdt->SetPoint(8,1694.11,106.147);
    //    newHagLRBdt->SetPoint(9,1095.43,106.528);
    //    newHagLRBdt->SetPoint(10,700.124,106.528);
    //    newHagLRBdt->SetPoint(11,118.882,113.217);
    //    newHagLRBdt->SetPoint(12,46.6259,129.728);
    //    newHagLRBdt->SetPoint(13,11.3529,176.219);
    //    newHagLRBdt->SetPoint(14,9.04981,231.362);
    //    newHagLRBdt->SetPoint(15,27.3116,231.362);
    //    newHagLRBdt->SetPoint(16,65.3228,196.564);
    //    newHagLRBdt->SetPoint(17,131.997,180.694);
    //    newHagLRBdt->SetPoint(18,317.545,172.781);
    //    newHagLRBdt->SetPoint(19,680.066,169.107);
    //    newHagLRBdt->SetPoint(20,1195.24,163.448);
    //    newHagLRBdt->SetPoint(21,1774.76,161.412);
    //    newHagLRBdt->SetPoint(22,4171.41,153.516);
    //    newHagLRBdt->Draw("");
    // }

    //    newNaiLRBdt = new TCutG("new_naiLRBdt",24);
   // newNaiLRBdt->SetVarX("nai Energy vs nai- low res Beta dt");
   // newNaiLRBdt->SetVarY("");
   // newNaiLRBdt->SetTitle("Graph");
   // newNaiLRBdt->SetFillStyle(1000);
   // newNaiLRBdt->SetPoint(0,161.75,66.012);
   // newNaiLRBdt->SetPoint(1,876.564,26.8461);
   // newNaiLRBdt->SetPoint(2,2652.4,17.0547);
   // newNaiLRBdt->SetPoint(3,3836.41,11.1798);
   // newNaiLRBdt->SetPoint(4,4539.81,-24.0694);
   // newNaiLRBdt->SetPoint(5,2848.21,-34.84);
   // newNaiLRBdt->SetPoint(6,947.388,-37.7775);
   // newNaiLRBdt->SetPoint(7,193.902,-49.5272);
   // newNaiLRBdt->SetPoint(8,55.214,8.24237);
   // newNaiLRBdt->SetPoint(9,23.3373,33.7002);
   // newNaiLRBdt->SetPoint(10,15.1232,37.6167);
   // newNaiLRBdt->SetPoint(11,5.6889,41.5333);
   // newNaiLRBdt->SetPoint(12,1.88006,55.2414);
   // newNaiLRBdt->SetPoint(13,2.04517,213.863);
   // newNaiLRBdt->SetPoint(14,5.76305,191.343);
   // newNaiLRBdt->SetPoint(15,10.5921,172.739);
   // newNaiLRBdt->SetPoint(16,20.2388,154.135);
   // newNaiLRBdt->SetPoint(17,41.7957,120.844);
   // newNaiLRBdt->SetPoint(18,67.4873,86.574);
   // newNaiLRBdt->SetPoint(19,96.9829,77.7617);
   // newNaiLRBdt->SetPoint(20,117.776,69.9285);
   // newNaiLRBdt->SetPoint(21,163.859,66.9911);
   // newNaiLRBdt->SetPoint(22,163.859,66.9911);
   // newNaiLRBdt->SetPoint(23,161.75,66.012);
   // newNaiLRBdt->Draw("");
};//namespace VandleTCuts

#endif 
