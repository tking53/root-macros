{
	bool genscanLoaded = gSystem->Getenv("GENSCANSYS");

	const char* MacMacroPath_TK="${HOME}/TTK/root-macros";

	if (genscanLoaded){
		cout << "GenScan ENV module loaded, Pulling in GenScan Libs" << endl;
		gSystem->Load("libBSMStruct");
		gSystem->Load("libCloverStruct");
		gSystem->Load("libDSSDStruct");
		gSystem->Load("libEBSSStruct");
		gSystem->Load("libGroverStruct");
		gSystem->Load("libMtasImplantStruct");
		gSystem->Load("libMtasStruct");
		gSystem->Load("libMusesStruct");
		gSystem->Load("libPSPMTStruct");
		gSystem->Load("libPidStruct");
		gSystem->Load("libRootDevStruct");
		gSystem->Load("libVetoStruct");
	}

	gStyle->SetNumberContours(999);
	gStyle->SetPalette(kRainbow); //This is probably a bad color map
																//gStyle->SetPalette(kCherry);
																//TColor::InvertPalette();
																//gStyle->SetOptStat(0);
	gStyle->SetLabelSize(0.04,"XYZ");
	gStyle->SetTitleSize(0.04,"XYZ");
	//gStyle->SetTitleOffset(1.375,"XYZ");

	std::cout << "Loading Basic TTK Specifics" << std::endl;
	const char* FormStyle = ".L %s%s+";

	gInterpreter->AddIncludePath(Form("%s%s",MacMacroPath_TK, "/utilities/include"));
	gROOT->ProcessLine(Form(FormStyle, MacMacroPath_TK, "/utilities/src/CanvasCreate.cxx"));
	gROOT->ProcessLine(Form(FormStyle, MacMacroPath_TK, "/utilities/src/TPaveText_setup.cxx"));
	gROOT->ProcessLine(Form(FormStyle, MacMacroPath_TK, "/utilities/src/EBSSBKG_Sub.cxx"));
	gROOT->ProcessLine(Form(FormStyle, MacMacroPath_TK, "/utilities/src/Root_Hist_Manipulation.cxx"));
	std::cout << "Finished Loading TTK Macros" << std::endl;

	std::cout << "Finished Loading Custom Settings" << std::endl;
}

