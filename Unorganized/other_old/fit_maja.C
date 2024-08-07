#include "TMath.h"
#include "TFile.h"
#include "TPaveText.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TChain.h"
#include "TString.h"
#include <iostream>
#include <vector>
#include <string>

vector<TH1F*> hcoinc;


TChain *db = new TChain("AutoTree");
/*db->Add("cal_01300[25-39]*")
db->Add("cal_01600[42-49]*")
db->Add("cal_02200[57-59]*")
db->Add("cal_02300[61-80]*")
db->Add("cal_02900[87-92]*")
db->Add("cal_0310[094-115]*")*/

/*db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130025.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130026.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130027.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130028.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130029.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130030.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130031.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130032.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130033.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130034.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130035.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130036.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130037.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130038.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0130039.root");

db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0160042.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0160043.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0160044.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0160045.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0160046.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0160047.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0160048.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0160049.root");

db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0220057.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0220058.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0220059.root");

db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230060.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230061.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230062.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230063.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230064.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230065.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230066.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230067.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230068.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230069.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230070.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230071.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230072.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230073.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230074.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230075.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230076.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230077.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230078.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230079.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230080.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0230081.root");

db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0290087.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0290088.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0290089.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0290090.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0290091.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0290092.root");

db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310094.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310095.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310096.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310097.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310098.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310099.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310100.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310101.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310102.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310103.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310104.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310105.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310106.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310107.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310108.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310109.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310110.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310111.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310112.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310113.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310114.root");
db->Add("/mnt/hgfs/Maja2/gsi/r292_Root_files/calibrated/cal_0310115.root");*/


//Count the number of events in a gaussian fit
Double_t sqrt2pi = TMath::Sqrt(2*TMath::Pi());

//Log half life determination

//log fit with background taken into account
TF1 *log_halfLife = new TF1("log_halfLife","[0]/[1]*TMath::Exp(x*TMath::Log(2))*TMath::Exp(-1/[1]*TMath::Exp(x*TMath::Log(2)))*TMath::Exp(-1/[3]*TMath::Exp(x*TMath::Log(2)))+[2]/[3]*TMath::Exp(x*TMath::Log(2))*TMath::Exp(-1/[3]*TMath::Exp(x*TMath::Log(2)))",3,20);

//[0] amplitude of the signal
//[1] searched half life
//[2] amplitude of the background
//[3] background half life

//log fit without a background
TF1 *log_hl_1comp = new TF1("log_hl_1comp","[0]/[1]*TMath::Exp(x*TMath::Log(2))*TMath::Exp(-1/[1]*TMath::Exp(x*TMath::Log(2)))",3,20);

//[0] amplitude of the signal
//[1] searched half life

//gaussian fits

// 1 gaussian
TF1 *gaus_pol1 = new TF1("gaus_pol1","[0]*TMath::Exp(-pow((x-[1]),2)/(2*[2]*[2]))+[3]+[4]*x",0,6000);


//2 gaussians
TF1 *gaus2_pol1 = new TF1("gaus2_pol1","[0]*TMath::Exp(-pow((x-[1]),2)/(2*[2]*[2]))+[3]*TMath::Exp(-pow((x-[4]),2)/(2*[2]*[2]))+[5]+[6]*x",0,6000);

//3 gaussians
TF1 *gaus3_pol1 = new TF1("gaus3_pol1","[0]*TMath::Exp(-pow((x-[1]),2)/(2*[2]*[2]))+[3]*TMath::Exp(-pow((x-[4]),2)/(2*[2]*[2]))+[5]*TMath::Exp(-pow((x-[6]),2)/(2*[2]*[2]))+[7]+[8]*x",0,6000);

//4 gaussians
TF1 *gaus4_pol1 = new TF1("gaus4_pol1","[0]*TMath::Exp(-pow((x-[1]),2)/(2*[2]*[2]))+[3]*TMath::Exp(-pow((x-[4]),2)/(2*[2]*[2]))+[5]*TMath::Exp(-pow((x-[6]),2)/(2*[2]*[2]))+[7]*TMath::Exp(-pow((x-[8]),2)/(2*[2]*[2]))+[9]+[10]*x",0,6000);

//5 gaussians
TF1 *gaus5_pol1 = new TF1("gaus5_pol1","[0]*TMath::Exp(-pow((x-[1]),2)/(2*[2]*[2]))+[3]*TMath::Exp(-pow((x-[4]),2)/(2*[2]*[2]))+[5]*TMath::Exp(-pow((x-[6]),2)/(2*[2]*[2]))+[7]*TMath::Exp(-pow((x-[8]),2)/(2*[2]*[2]))+[9]*TMath::Exp(-pow((x-[10]),2)/(2*[2]*[2]))+[11]+[12]*x",0,6000);

//6 gaussians
TF1 *gaus6_pol1 = new TF1("gaus6_pol1","[0]*TMath::Exp(-pow((x-[1]),2)/(2*[2]*[2]))+[3]*TMath::Exp(-pow((x-[4]),2)/(2*[2]*[2]))+[5]*TMath::Exp(-pow((x-[6]),2)/(2*[2]*[2]))+[7]*TMath::Exp(-pow((x-[8]),2)/(2*[2]*[2]))+[9]*TMath::Exp(-pow((x-[10]),2)/(2*[2]*[2]))+[11]*TMath::Exp(-pow((x-[12]),2)/(2*[2]*[2]))+[13]+[14]*x",0,6000);

//7 gaussians
TF1 *gaus7_pol1 = new TF1("gaus7_pol1","[0]*TMath::Exp(-pow((x-[1]),2)/(2*[2]*[2]))+[3]*TMath::Exp(-pow((x-[4]),2)/(2*[2]*[2]))+[5]*TMath::Exp(-pow((x-[6]),2)/(2*[2]*[2]))+[7]*TMath::Exp(-pow((x-[8]),2)/(2*[2]*[2]))+[9]*TMath::Exp(-pow((x-[10]),2)/(2*[2]*[2]))+[11]*TMath::Exp(-pow((x-[12]),2)/(2*[2]*[2]))+[13]*TMath::Exp(-pow((x-[14]),2)/(2*[2]*[2]))+[15]+[16]*x",0,6000);

//exponential fit

//simple exponential
TF1 *expo_simple = new TF1("expo_simple","[0] * TMath::Log(2)/[1] * TMath::Exp(-TMath::Log(2)/[1] * x)",0,3000);
//[0] amplitude
//[1] half life (or equivalent)

//simple exponential with a linear fit
TF1 *expo_lin = new TF1("expo_lin","[0] * TMath::Log(2)/[1] * TMath::Exp(-TMath::Log(2)/[1] * x) + [2] * x + [3]",0,100000);
//[0] amplitude
//[1] half life (or equivalent)
//[2] 
//[3] background offset on the y-axis

//2 combined exponentials
TF1 *expo_wbdf = new TF1("expo_wbdf","[0] * TMath::Log(2)/[1] * TMath::Exp(-TMath::Log(2)/[1] * x) + [2] * TMath::Log(2)/[3] * TMath::Exp(-TMath::Log(2)/[3] * x)",0,3000);
//[0] amplitude first 
//[1] half life (or equivalent) first
//[2] amplitude background
//[3] half life background

//2 half lives - beta with background
TF1 *hl_feed = new TF1("hl_feed","([0]/([0]-[1]))*[2]*(TMath::Exp(-x*TMath::Log(2)/[0]) - TMath::Exp(-x*TMath::Log(2)/[1]))+[3]*TMath::Exp(-x*TMath::Log(2)/[1])+ [4]*x + [5]",0,30000);

//2 half lives no background
TF1 *hl_feed_no = new TF1("hl_feed_no","([0]/([0]-[1]))*[2]*(TMath::Exp(-x*TMath::Log(2)/[0]) - TMath::Exp(-x*TMath::Log(2)/[1]))+[3]*TMath::Exp(-x*TMath::Log(2)/[1])",0,30000);



