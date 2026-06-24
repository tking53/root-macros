
// RD tree flattener header file
//


#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <PaassRootStruct.hpp>
#include <iomanip>
#include <TH1.h>

/**/
/*class GS_LITE :public TObject {*/
/*	public:*/
/*	double energy;*/
/*	double qdc	 ;*/
/*	double time	 ;*/
/*	double detNum;*/
/**/
/*	GS_LITE():energy(-999),qdc(-999),time(-999),detNum(-999) {};*/
/*	GS_LITE(double a, double b, double c, double d):energy(a),qdc(b),time(c),detNum(d) {};*/
/*	ClassDef(GS_LITE, 1);*/
/**/
/**/
/*};*/

using namespace processor_struct;
using namespace std;
class flattenTree{

public:
	struct PSPMT_Sum {
		double posX = -999;
		double posY = -999;
		double dyE = -999;
		double dyQ = -999;
		double posSumE = -999;
		double posSumQ = -999;
		bool validPos = false;
	} ;

	
	struct EvtStats{
		double PaassEvtNumber = -100;
		int evtmulti = 0;
	} ;

	flattenTree() = default;

	flattenTree(TFile* file, vector<int> ids, Double_t evtPrintInterval, Bool_t outputTraces);
	flattenTree(TFile* file, pair<int,int> ids, Double_t evtPrintInterval, Bool_t outputTraces);
	flattenTree(TFile* file, vector<pair<int,int> > ids, Double_t evtPrintInterval, Bool_t outputTraces);

	flattenTree(const flattenTree &) = default;
	flattenTree(flattenTree &&) = default;
	flattenTree &operator=(const flattenTree &) = default;
	flattenTree &operator=(flattenTree &&) = default;

	void InitOutTree(vector<int> &ids, TFile* file);

	void ResetDataTreeBranches(vector<ROOTDEV>* brVec);

	void FillRDStruct(TTreeReaderArray<ROOTDEV>::iterator &iter, ROOTDEV &det);

	/*void FillGSStruct(TTreeReaderArray<GAMMASCINT>::iterator &iter, GS_LITE &det);*/

	void ProcessTree();

	Bool_t GetOutputTraces(){return outputTraces_;};
	void SetOutputTraces(Bool_t &a){outputTraces_ = a;};

	EvtStats evtStatsBlock, defaultStatsBlock;

private:
	double icavgE ;

	TFile* inputfile;
	vector<int> processChanIDs;
	Double_t evtInterval ;

	TTreeReader* PixTreeEvt;

	TTreeReaderArray<ROOTDEV>* rd ;
	TTreeReaderArray<GAMMASCINT>* gs ;
	TTreeReaderArray<PSPMTSUMMARY>* ps_sum ;
	TTreeReaderArray<PID>* pid_ ;

	TTreeReaderValue<Double_t>* ScanEvtNum ;

	ROOTDEV DefaultStruct_RD;
	std::vector<ROOTDEV> *branchVec; 

	GAMMASCINT DefaultStruct_GS ;
	std::vector<GAMMASCINT> gscintVec ;

	/*typedef std::vector<std::tuple<Double_t,Double_t,Double_t,Double_t>> gTuple;*/
	/*gTuple gammas;*/

	PSPMT_Sum DefaultStruct_PSsum;
	PSPMT_Sum PSsum_low,PSsum_high;

	TH1I* chanHit ;
	TH1I* chanHitPileup;
	TH1I* chanHitSat;
	TFile* outFile;
	TTree* outTree;

	int maxChannel = 13*16;// hard coding to full crate of RevF or X1s

	Bool_t outputTraces_;

};


/* det.energy									 = -999; */
/* det.rawEnergy								= -999; */
/* det.timeSansCfd							= -999; */
/* det.time										 = -999; */
/* det.timeSansCfdInNs					= -999; */
/* det.timeInNs								 = -999; */
/* det.cfdForcedBit						 = -999; */
/* det.cfdFraction							= -999; */
/* det.cfdSourceBit						 = -999; */
/* det.detNum									 = -999; */
/* det.modNum									 = -999; */
/* det.chanNum									= -999; */
/* det.tickToNS								 = -999; */
/* det.adcTickToNS							= -999; */
/* det.modfreq									= -999; */
/* det.revision								 = ""; */
/* det.type										 = ""; */
/* det.subtype									= ""; */
/* det.group										= ""; */
/* det.pileup									 = -999; */
/* det.saturation							 = -999; */
/* det.trace										= {}; */
/* det.baseline								 = -999; */
/* det.stdBaseline							= -999; */
/* det.phase										= -999; */
/* det.tqdc										 = -999; */
/* det.maxPos									 = -999; */
/* det.maxVal									 = -999; */
/* det.extMaxVal								= -999; */
/* det.highResTime							= -999; */
/* det.qdcSums									= {}; */
/* det.hasValidTimingAnalysis	 = false; */
/* det.hasValidWaveformAnalysis = false; */
