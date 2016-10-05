


#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
//#include "TNtuple.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// Declaration
void PlotDoubleColumnFile( void );


//THIS IS WHERE YOU CHANGE FILENAME, BINNING AND ETC!


static const int numberFiles = 3;// number of files to plot


//name of text file(s) to plot
static const char* s_FileNames[] =
{
    "84Br_data_6320.txt","84Br_corr20_6320.txt","84Br_corr2_6320.txt",
};

//example:
//static const char* s_FileNames[] =
//{
//filename1,filename2
//};

//range over which to plot (range muse = bin number, must be factor of 2)
float minXRange=0.0;
float maxXRange=8192;//5000.5;




//number of bins to use, sums up adjacent bins, is a good idea to bin to be able to see data well. will also go a lot faster

//default: 1024
int binNumber=maxXRange;

//name for axes and title
const char *titleName=" ";
const char *XaxisName="Energy (keV)";
const char *YaxisName="Counts";


//Normalization for multiple histograms set here

static const int normalize = 1;// if = 1, will normalize histograms (if plotting multiples)


//For now normalizes all histograms to 1
//static const int normHist = 1;// choose histogram want to normalize on, 0 = first imported hist in s_FileNames and etc...


//Normalizes over full range for now
//Set range over which want to normalize
//Float_t normstart=11700;
//Float_t normend=12100;


// scale factor for histogram 0 (should fix so can scale each one), could do automatically like dnorm with region defined, to be thought about later
//Float_t scale0 = 1.0;




// Equivalent to main
void PlotDoubleColumnFile( void )
{

ifstream inputFile;
string inputFileName;
string inputXString;
string inputYString;

float x = 0.0;
float y;

std::vector<float> X;
std::vector<float> Y;


int numberX[numberFiles];
//float* graphX[numberFiles];
//float* graphY[numberFiles];
double scaleHist[numberFiles];

//TGraph *graphs[numberFiles];
TH1F *histograms[numberFiles];

for( int i = 0; i < numberFiles; i++ )
{
    	histograms[i] = new TH1F( "XXX", s_FileNames[i], binNumber, minXRange, maxXRange );
}


for( int i = 0; i < numberFiles; i++ )
{
	inputFile.open( s_FileNames[i] );

	if( ! inputFile.is_open() ) continue;
	
	X.clear();
	Y.clear();

	while ( inputFile.good() )
	{
		getline (inputFile,inputXString, ' ' );
		std::istringstream issX(inputXString);
		issX >> x;

		getline (inputFile,inputYString );
		std::istringstream issY(inputYString);
		issY >> y;

// Simple Error Checking. Shold probably add more...
		if( fabs(y) > 100000000.0 ) continue;

		X.push_back(x);
		Y.push_back(y);

//		x += 1.0;
	}
	
	inputFile.close();

numberX[i] = X.size()-1;//
//	graphX[i] = new float[numberX[i]];
//	graphY[i] = new float[numberX[i]];

	for( int j = 0; j < numberX[i]; j++ )
	{
//			histograms[i]->Fill( X[j], -Y[j] );
		histograms[i]->Fill( X[j], Y[j] );
		
//		graphX[i][j] = X[j];
//		if( Y[j] > 0.0 ) graphY[i][j] = Y[j];
//		else graphY[i][j] = 0.0001;
	}
}

//for( int i = 0; i < numberFiles; i++ )
//{
//	graphs[i] = new TGraph( numberX[i], graphX[i], graphY[i]);
//}
    
    
    //Normalize histograms if normalization set =1
    if( numberFiles > 1 && normalize == 1)
        for( int j = 0; j < numberFiles; j++ )
        {
            scaleHist[j]=1/histograms[j]->Integral();
        }
    else
        for( int j = 0; j < numberFiles; j++ )
        {
            scaleHist[j]=1;
        };
    

TCanvas *canvas0 = 0;
canvas0 = new TCanvas("canvas0","Histogram",200,10,600,400); 
canvas0->SetGridx(true);
canvas0->SetGridy(true);
canvas0->SetLogy(true);

histograms[0]->GetXaxis()->SetTitle(XaxisName);
histograms[0]->GetXaxis()->CenterTitle();
histograms[0]->GetYaxis()->SetTitle(YaxisName);
histograms[0]->GetYaxis()->CenterTitle();
histograms[0]->SetTitle(titleName);
histograms[0]->SetMarkerStyle(0);
    
    
histograms[0]->SetMarkerColor(1);
histograms[0]->SetLineColor(1);
histograms[0]->SetLineWidth(1);
histograms[0]->Scale(scaleHist[0]); // for scaling histogram 0;
histograms[0]->Draw("HIST");
    
if( numberFiles > 1 )
    for( int j = 1; j < numberFiles; j++ )
    {
        histograms[j]->SetMarkerColor(j+1);
        histograms[j]->SetLineColor(j+1);
        histograms[j]->SetLineWidth(1);
        histograms[j]->Scale(scaleHist[j]); // set scaling
        histograms[j]->Draw("HIST same");
    };
    
    
    

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

