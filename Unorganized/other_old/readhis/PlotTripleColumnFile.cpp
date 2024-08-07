#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"



#include "TCanvas.h"
#include "TGraph.h"
//#include "TH1F.h"
#include "TH2F.h"
#include "TRandom3.h"
#include "TFile.h"
//#include "TNtuple.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Declaration
void PlotTripleColumnFile( void );



//THIS IS WHERE YOU CHANGE FILENAME, BINNING AND ETC!

//name of text file to plot
const char *filename="multiTFilltest-3245.txt";

//range
float minXRange=0.0;
float maxXRange=4096.0;

float minYRange=0.0;
float maxYRange=8192.0;



//number of bins to use, sums up adjacent bins, is a good idea to bin to be able to see data well. will also go a lot faster

//default: 512
int binNumber=4096;

//name for axes and title
const char *titleName="QDC vs ToF ";
const char *XaxisName="ToF";
const char *YaxisName="QDC";








static const int numberFiles = 1;// SHould always be 1 for now...
static const char* s_FileNames[] =
{
filename,
};

// Equivalent to main
void PlotTripleColumnFile( void )
{
//TRandom3 randGen;

ifstream inputFile;
string inputFileName;
string inputXString;
string inputYString;
string inputZString;
float x = 0.0;
float y;
float z;

std::vector<float> X;
std::vector<float> Y;
std::vector<float> Z;

int numberX[numberFiles];
//float* graphX[numberFiles];
//float* graphY[numberFiles];

//TGraph *graphs[numberFiles];
TH2F *histograms[numberFiles];

for( int i = 0; i < numberFiles; i++ )
{
	histograms[i] = new TH2F( s_FileNames[i], s_FileNames[i], binNumber, minXRange-0.5, maxXRange+0.5, binNumber, minYRange-0.5, maxYRange+0.5 );
}

int k = 0;

for( int i = 0; i < numberFiles; i++ )
{
	inputFile.open( s_FileNames[i] );

	if( ! inputFile.is_open() ) continue;
	
	X.clear();
	Y.clear();
	Z.clear();
	
	while ( inputFile.good() )
	{
		getline (inputFile,inputXString, ' ' );
		std::istringstream issX(inputXString);
		issX >> x;

		getline (inputFile,inputYString, ' ' );
		std::istringstream issY(inputYString);
		issY >> y;
	
		getline (inputFile,inputZString );
		std::istringstream issZ(inputZString);
		issZ >> z;
    
//    x = (x+randGen.Uniform())* 10.0;

//		std::cout << inputXString << ": " << inputYString << std::endl;
//		std::cout << x << ", " << y << ", " << z << std::endl;
// Simple Error Checking. Shold probably add more...
		if( fabs(y) > 100000000.0 ) continue;
		
		k++;

		X.push_back(x);
		Y.push_back(y);
		Z.push_back(z);

//		x += 1.0;
	}
	
	inputFile.close();

	numberX[i] = X.size();// The last one always seems to be messed up so just ignore it.
//	graphX[i] = new float[numberX[i]];
//	graphY[i] = new float[numberX[i]];

	for( int j = 0; j < numberX[i]; j++ )
	{
//			histograms[i]->Fill( X[j], -Y[j] );
		if( X[j] < maxXRange && Y[j] < maxYRange ) histograms[i]->Fill( X[j], Y[j], Z[j] );
//		graphX[i][j] = X[j];
//		if( Y[j] > 0.0 ) graphY[i][j] = Y[j];
//		else graphY[i][j] = 0.0001;
	}
}

//for( int i = 0; i < numberFiles; i++ )
//{
//	graphs[i] = new TGraph( numberX[i], graphX[i], graphY[i]);
//}

TFile *saveFile = new TFile("multi-3245.root","RECREATE");

 histograms[0]->Write();
 saveFile->Close();

TCanvas *canvas0 = 0;
canvas0 = new TCanvas("canvas0","Histogram",200,10,600,600); 
canvas0->SetGridx(true);
canvas0->SetGridy(true);
canvas0->SetLogz(true);
gStyle->SetPalette(1);

histograms[0]->GetXaxis()->SetTitle(XaxisName);
histograms[0]->GetXaxis()->CenterTitle();
histograms[0]->GetYaxis()->SetTitle(YaxisName);
histograms[0]->GetYaxis()->CenterTitle();
histograms[0]->SetTitle(titleName);
 histograms[0]->SetMinimum(1);
    
    //if want title to be histogram filename:
   // histograms[0]->SetTitle( s_FileNames[0] );
    
    
//histograms[0]->SetMarkerStyle(7);
//histograms[0]->SetMarkerColor(1);
//histograms[0]->SetLineColor(1);
//histograms[0]->SetLineWidth(1);
histograms[0]->Draw("COLZ");

/*
graphs[0]->GetXaxis()->SetTitle("X");
graphs[0]->GetXaxis()->CenterTitle();
graphs[0]->GetYaxis()->SetTitle("Y");
graphs[0]->GetYaxis()->CenterTitle();
graphs[0]->SetTitle( s_FileNames[0] );
graphs[0]->SetMarkerStyle(7);
graphs[0]->SetMarkerColor(1);
graphs[0]->SetLineColor(1);
graphs[0]->SetLineWidth(1);
graphs[0]->Draw("ALP");
*/

}

