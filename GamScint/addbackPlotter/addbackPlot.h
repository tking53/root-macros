#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TH1.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TChain.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TStopwatch.h"
#include "TSelector.h"
#include "TProofServ.h"
#include "TMath.h"


#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>

class addbackSelector : public TSelector {
public:
  addbackSelector(TTree *=0);
  virtual ~addbackSelector();

  virtual Int_t   Version() const { return 0.5; }
  virtual void    Init(TTree* mergedData);
  virtual void    Begin(TTree *mergedData);
  virtual void    SubBegin(TTree *mergedData);
  virtual Bool_t  Notify() { return kTRUE;}
  virtual Bool_t  Process(Long64_t entry);
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SubTerminate(){}
  virtual void    Terminate();

  
  /** Returns the addback parameters for known types
   * \param [in] subtype : The known subtype to lookup
   * \param [in] option : Parameter to lookup
   * @return Returns (by reference ) parameter
   */
  virtual double GetAddbackPara(const std::string &subtype,const std::string &option);

  /** Updates the Addback Reference time for known types
   * \param [in] subtype : The subtype's whose RefTime needs updating
   * \param [in] newRefTime : New Reference time
   */
  virtual void SetAddbackRefTime(const std::string &subtype, const double &newRefTime);


protected:
  const Double_t naisubwin = 0;
  const Double_t shsubwin = 0;
  const Double_t naithresh = 0;
  const Double_t shthresh = 0;

  TTreeReader gsReader;

  //his array
  TObjArray* fHistos;

  TFile* fout;
  TFile* fin;

  /** The double map structure is <subtype , < parameter , value > >.
   *    Parameters are "thresh" , "subEvtWin", "refTime".
   *    The refTimes need to be in ns because the Gtime is in ns.
   */ std::map<std::string,std::map<std::string,double> > ParameterMap; //!<Map of addback parameter

  std::map <std::string,double > paraData ;
  
}

class GSAddback {
public:
  /** Default constructor setting things to zero */
  GSAddback() {
    energy = time = multiplicity = 0;
  }
  
  /** Default constructor setting default values
   * \param [in] ienergy : the initial energy
   * \param [in] itime : the initial time
   * \param [in] imultiplicity : multiplicity of the event */
  GSAddback(Double_t ienergy, Double_t itime, Double_t imultiplicity) {
    energy = ienergy;
    time = itime;
    multiplicity = imultiplicity;
  }
  
  Double_t energy;//!< Energy of the addback event
  Double_t time;//!< time of the addback event
  Double_t multiplicity;//!< multiplicity of the event
};

