#include "flattenTree.h"
using namespace std;
using namespace processor_struct;


flattenTree::flattenTree(TFile* file, vector<int> ids, Double_t evtPrintInterval, Bool_t outputTraces = true){ 
	evtInterval = evtPrintInterval; 
	InitOutTree(ids, file);
	SetOutputTraces(outputTraces);
};
flattenTree::flattenTree(TFile* file, pair<int,int> ids, Double_t evtPrintInterval, Bool_t outputTraces = true){
	evtInterval = evtPrintInterval; 
	vector<int> temp(ids.second-ids.first+1,ids.first);
	for (unsigned int i = 0; i <temp.size(); ++i){
		temp.at(i)=ids.first+i;
	}
	SetOutputTraces(outputTraces);
	InitOutTree(temp,file);
};
flattenTree::flattenTree(TFile* file, vector<pair<int,int>> ids, Double_t evtPrintInterval, Bool_t outputTraces = true){
	evtInterval = evtPrintInterval; 
	vector<int> temp;
	for (unsigned int i=0;i <ids.size();++i){
		for (int j = ids.at(i).first; j ==ids.at(i).second; ++j){
			temp.push_back(j);
		}
	}
	SetOutputTraces(outputTraces);
	InitOutTree(temp, file);
};

void flattenTree::InitOutTree(vector<int> &ChanIDs, TFile* file){
	
	// how often to print the process to the cli in decimal percent. Default is 2% 
	inputfile = file;
	processChanIDs = ChanIDs;
	PixTreeEvt = new TTreeReader("PixTree", file);
	
	TTree* PTree;
	inputfile->GetObject("PixTree",PTree);


	rd = new TTreeReaderArray<ROOTDEV>(*PixTreeEvt, "rootdev_vec_");
	gs = new TTreeReaderArray<GAMMASCINT>(*PixTreeEvt, "gammascint_vec_");
	ScanEvtNum = new TTreeReaderValue<Double_t>(*PixTreeEvt, "eventNum");

	TNamed* OGfilename = (TNamed*)file->Get("outputFile");
	string newFile = (string)OGfilename->GetTitle() + "_DD_flat.root";


	outFile = new TFile(newFile.c_str(),"RECREATE");
	outTree = new TTree("rd_flat", "Flattened RD");

	if (PTree != nullptr){
		if (PTree->GetBranch("pspmtsum_vec_") != nullptr){
			ps_sum = new TTreeReaderArray<PSPMTSUMMARY>(*PixTreeEvt, "pspmtsum_vec_");
			outTree->Branch("lowgain", &PSsum_low,"posX/D:posY:dyE:dyQ:posSumE:posSumQ:validPos/O");
			outTree->Branch("highgain", &PSsum_high, "posX/D:posY:dyE:dyQ:posSumE:posSumQ:validPos/O");
		}
		if (PTree->GetBranch("pid_vec_.ic_avg_energy")){
			pid_ = new TTreeReaderArray<PID>(*PixTreeEvt, "pid_vec_");
			outTree->Branch("icavgE", &icavgE,"icavgE/D");
		}
	}
	//! Add stats block to output
	outTree->Branch("stats",&evtStatsBlock,"PaassEvtNumber/D:evtmulti/I");
	
	chanHit = new TH1I("chanHit","Channel Hits",208,0,207);
	chanHitPileup = new TH1I("chanHitPileup","Channel with Pileup ",208,0,207);
	chanHitSat = new TH1I("chanHitSat","Channel with Trace Out Of Range",208,0,207);
	
	// preinit to a full module cuz eh 
	cout<<"Det Stucts Trace? : " << std::boolalpha<<GetOutputTraces()<<std::noboolalpha<<endl;
	branchVec = new std::vector<ROOTDEV>(16,DefaultStruct_RD); 
	for (unsigned i = 0; i < processChanIDs.size(); ++i){
		string name = "d" + to_string(processChanIDs.at(i));
		branchVec->emplace_back(DefaultStruct_RD);
		outTree->Branch(name.c_str(),&branchVec->at(i));
	}

	cout<<"making gslite branch"<<endl;
	outTree->Branch("gslite",&gscintVec);
	/*outTree->Branch("gslite",&gammas);*/
}

void flattenTree::ProcessTree() {

	long long TotalEvts = PixTreeEvt->GetEntries();
	cout<< "TotalEvts= " << TotalEvts << endl;
	long long CurEntryNum=0;
	
	long long eventModulo = (TotalEvts * evtInterval) - (Int_t)(TotalEvts * evtInterval) % 10;


	while (PixTreeEvt->Next()){
		CurEntryNum = PixTreeEvt->GetCurrentEntry();
		evtStatsBlock.PaassEvtNumber = (**ScanEvtNum);
		if (CurEntryNum == 0 ) {
			cout<< "Processing Start"<<endl;
		}
		for (auto itr = rd->begin(); itr != rd->end(); ++itr) {
			//! Stats ONLY these work for ALL RD channels even those which arent being flattened
			++evtStatsBlock.evtmulti;
			chanHit->Fill(itr->channelID);
			if (itr->pileup){
			chanHitPileup->Fill(itr->channelID);
			}
			if (itr->saturation){
			chanHitSat->Fill(itr->channelID);
			}

			//! Actual export data
			for (unsigned vecFind = 0; vecFind < processChanIDs.size(); ++vecFind){
				if ( itr->channelID == processChanIDs.at(vecFind) ) {
					FillRDStruct(itr, branchVec->at(vecFind));
					break;
				}
			}
		}
		if (!ps_sum->IsEmpty()){
			PSsum_low.posX =		ps_sum->At(0).posXlow;
			PSsum_low.posY = 		ps_sum->At(0).posYlow;
			PSsum_low.dyE =			ps_sum->At(0).dynEnergylow; 
			PSsum_low.dyQ = 		ps_sum->At(0).dynQdclow; 	 
			PSsum_low.posSumE = ps_sum->At(0).ansumEnergylow;
			PSsum_low.posSumQ = ps_sum->At(0).ansumQdclow;
			PSsum_low.validPos= ps_sum->At(0).validPoslow;

			PSsum_high.posX =			ps_sum->At(0).posXhigh;
			PSsum_high.posY = 		ps_sum->At(0).posYhigh;
			PSsum_high.dyE =			ps_sum->At(0).dynEnergyhigh; 
			PSsum_high.dyQ = 			ps_sum->At(0).dynQdchigh; 	 
			PSsum_high.posSumE =	ps_sum->At(0).ansumEnergyhigh;
			PSsum_high.posSumQ =	ps_sum->At(0).ansumQdchigh;
			PSsum_high.validPos=	ps_sum->At(0).validPoshigh;
		}

	for (auto gsIT = gs->begin(); gsIT!=gs->end();++gsIT){
		/*gammas.push_back(make_tuple(gsIT->energy,gsIT->time,gsIT->qdc,gsIT->detNum));*/
			gscintVec.push_back((*gsIT));
	}

		if (! pid_->IsEmpty()){
			icavgE = pid_->At(0).ic_avg_energy;
		} else {
			icavgE = -999;
		}



		//! Fill and Reset
		outTree->Fill();
		ResetDataTreeBranches(branchVec);
		evtStatsBlock = defaultStatsBlock;
		PSsum_low = PSsum_high = DefaultStruct_PSsum;
		/*gammas.clear();*/
		gscintVec.clear();
		icavgE = -999;


		//! Print Processing Stats
		if (CurEntryNum % eventModulo == 0 || CurEntryNum == TotalEvts-1) {
			cout << std::setprecision(1000000000) << "\rEvent::" << CurEntryNum
				<< " of " << TotalEvts - 1 << " ( "
				<< ceil(((double)CurEntryNum / (double)TotalEvts) * 100) << " %)" << flush;
		}
	}
	outFile->Write(0,2,0);
	outFile->Close();
	cout<<endl;
}

void flattenTree::ResetDataTreeBranches(vector<ROOTDEV>* brVec){
		for (unsigned resetiter = 0; resetiter < brVec->size(); ++resetiter){
			branchVec->at(resetiter) = DefaultStruct_RD;
		}
}
void flattenTree::FillRDStruct(TTreeReaderArray<ROOTDEV>::iterator &iter, ROOTDEV &det) {
	det.energy = iter->energy;
	det.rawEnergy = iter->rawEnergy;
	det.timeSansCfd = iter->timeSansCfd;
	det.time = iter->time;
	det.timeSansCfdInNs = iter->timeSansCfdInNs;
	det.timeInNs = iter->timeInNs;
	det.cfdForcedBit = iter->cfdForcedBit;
	det.cfdFraction = iter->cfdFraction;
	det.cfdSourceBit = iter->cfdSourceBit;
	det.detNum = iter->detNum;
	det.modNum = iter->modNum;
	det.chanNum = iter->chanNum;
	det.tickToNS = iter->tickToNS;
	det.adcTickToNS = iter->adcTickToNS;
	det.modfreq = iter->modfreq;
	det.revision = iter->revision;
	det.type = iter->type;
	det.subtype = iter->subtype;
	det.group = iter->group;
	det.pileup = iter->pileup;
	det.saturation = iter->saturation;
	det.baseline = iter->baseline;
	det.stdBaseline = iter->stdBaseline;
	det.phase = iter->phase;
	det.tqdc = iter->tqdc;
	det.maxPos = iter->maxPos;
	det.maxVal = iter->maxVal;
	det.extMaxVal = iter->extMaxVal;
	det.highResTime = iter->highResTime;
	det.qdcSums = iter->qdcSums;
	det.hasValidTimingAnalysis = iter->hasValidTimingAnalysis;
	det.hasValidWaveformAnalysis = iter->hasValidWaveformAnalysis;
	if(GetOutputTraces()){
		det.trace = iter->trace;
	}
}
/**/
/*void flattenTree::FillGSStruct(TTreeReaderArray<GAMMASCINT>::iterator &iter, GS_LITE &det){*/
/*	det.energy = iter->energy;*/
/*	det.qdc		 = iter->qdc;*/
/*	det.detNum = iter->detNum;*/
/*	det.time	 = iter->time;*/
/*}*/
