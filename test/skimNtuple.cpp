// ntuple skimmer for analysis and synchronization

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"

// bigTree is produced on an existing ntuple as follows (see at the end of the file) 
#include "bigTree.h" 
#include "smallTree.h"
#include "OfflineProducerHelper.h"
#include "PUReweight.h"
#include "triggerReader.h"
#include "bJetRegrVars.h"
#include "bTagSF.h"
#include "../../HHKinFit2/include/HHKinFitMasterHeavyHiggs.h"
//#include "../../HTT-utilities/LepEffInterface/interface/ScaleFactor.h"
#include "ScaleFactor.h"
#include "ConfigParser.h"
#include "exceptions/HHInvMConstraintException.h"
#include "exceptions/HHEnergyRangeException.h"
#include "exceptions/HHEnergyConstraintException.h"

#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

using namespace std ;

const double aTopRW = 0.156;
const double bTopRW = -0.00137;
const float DYscale_LL[3] = {1.17835, 1.80015, 0.809161} ; // computed from fit for LL and MM b tag
const float DYscale_MM[3] = {1.20859, 1.0445 , 1.54734 } ;

/* NOTE ON THE COMPUTATION OF STITCH WEIGHTS:
** - to be updated at each production, using the number of processed events N_inclusive and N_njets for each sample
** - say f_i is the fraction of inclusive events in the i bin on njets (can be 2D nb-njet as well)
** = then sigma_i = f_i * sigmal_LO
** - stitchWeight (0jet) = f_0 / (f_0 * N_inclusive)
** - stitchWeight (njet) = f_n / (f_n * N_inclusive + N_njets)
*/ 

// const float stitchWeights [5] = {1.11179e-7, 3.04659e-9, 3.28633e-9, 3.48951e-9, 2.5776e-9} ; // weights DY stitch in njets, to be updated at each production (depend on n evtsn processed)
// const float stitchWeights [5] = {11.55916, 0.316751, 0.341677, 0.362801, 0.267991} ; // weights DY stitch in njets, to be updated at each production (depend on n evts processed)

// const float stitchWeights [5] = {2.01536E-08, 2.71202E-09, 2.92616E-09, 3.0373E-09, 2.38728E-09} ; // jet binned only, 27 giu 2016
const float stitchWeights [][5] = {
    {2.0049265858e-08 , 0.0 , 0.0 , 0.0 , 0.0},
    {2.69618755131e-09 , 2.10733126107e-09 , 0.0 , 0.0 , 0.0},
    {2.92455388799e-09 , 2.24661641826e-09 , 6.92704012573e-10 , 0.0 , 0.0},
    {3.02046065594e-09 , 2.3004219177e-09 , 6.84321462783e-10 , 6.78248143287e-10 , 0.0},
    {2.38520634128e-09 , 1.91846200951e-09 , 6.57271388073e-10 , 6.62884752046e-10 , 6.29760043228e-10}
}; // jet binned and b binned, 8 jul 2016

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// open input txt file and append all the files it contains to TChain
void appendFromFileList (TChain* chain, TString filename)
{
    //cout << "=== inizio parser ===" << endl;
    std::ifstream infile(filename.Data());
    std::string line;
    while (std::getline(infile, line))
    {
        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
        while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
        while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
        if (!line.empty()) // skip empty lines
            chain->Add(line.c_str());
    }
    return;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// open the first file in the input list, retrieve the histogram "Counters" for the trigger names and return a copy of it
TH1F* getFirstFileHisto (TString filename, bool isForTriggers=true)
{
    std::ifstream infile(filename.Data());
    std::string line;
    while (std::getline(infile, line))
    {
        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
        while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
        while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
        if (!line.empty()) // skip empty lines
          break;
    }
    
    TFile* fIn = TFile::Open (line.c_str());
    TH1F* dummy = (TH1F*) fIn->Get ("HTauTauTree/Counters");
    TString name = "Counters_perTrigger";
    if(!isForTriggers) {
      dummy = (TH1F*) fIn->Get ("HTauTauTree/TauIDs");
      name = "Counters_pertauID";
    }
    TH1F* histo = new TH1F (*dummy);
    histo-> SetDirectory(0);
    histo->SetName (name.Data());
    fIn->Close();
    return histo;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/**
muons = 0
electrons = 1
taus = 2
*/
float getIso (unsigned int iDau, float pt, bigTree & theBigTree)
{
  int type = theBigTree.particleType->at (iDau) ;
  // is tauH
  if (type == 2)
    return theBigTree.daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(iDau) ;
  // muon
  if (type == 1 || type == 0)
    return theBigTree.combreliso->at(iDau);

  return -1 ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

bool CheckBit (int number, int bitpos)
{
    bool res = number & (1 << bitpos);
    return res;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

// implement operator < for b tag . first : CSV score ;  second : index
bool bJetSort (const pair<float, int>& i, const pair<float, int>& j) {
  if (i.first != j.first) return (i.first < j.first) ; // lowest CVS
  return i.second > j.second ; // highest index == lowest pt
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


float deltaPhi (float phi1, float phi2)
{
  float deltaphi = fabs (phi1 - phi2) ;
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308 ;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi ;
  return deltaphi ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


vector<int> findSubjetIdxs (unsigned int iFatJet, bigTree & theBigTree)
{
    vector<int> indexes;
    int idxFatJet = (int) iFatJet;
    for (unsigned int isj = 0; isj < theBigTree.subjets_ak8MotherIdx->size(); isj++)
    {
        if (theBigTree.subjets_ak8MotherIdx->at(isj) == idxFatJet)
            indexes.push_back(isj);
    }
    return indexes;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// turn on function for trigger reweight

float turnOnCB(float x, float m0, float sigma, float alpha, float n, float norm)
{
  float sqrtPiOver2 = TMath::Sqrt(TMath::PiOver2());
  float sqrt2 = TMath::Sqrt(2.);
  float sig = abs(sigma);
  float t = (x - m0)/sig * alpha / abs(alpha);
  float absAlpha = abs(alpha/sig);
  float a = TMath::Power(n/absAlpha, n) * TMath::Exp(-0.5 * absAlpha * absAlpha);
  float b = absAlpha - n/absAlpha;
  float arg = absAlpha / sqrt2;
  float ApproxErf = TMath::Erf(arg);
  if (arg > 5.) ApproxErf = 1.;
  else if(arg < -5.) ApproxErf = -1.;
  float leftArea = (1. + ApproxErf) * sqrtPiOver2;
  float rightArea = ( a * 1./TMath::Power(absAlpha-b, n-1) ) / (n-1);
  float area = leftArea + rightArea;
  if (t <= absAlpha){
    arg = t / sqrt2;
    if (arg > 5.) ApproxErf = 1.;
    else {
      if(arg < -5.) ApproxErf = -1.;
      else ApproxErf = TMath::Erf(arg);
    }
    return norm * (1 + ApproxErf) * sqrtPiOver2 / area;
  }
  return norm * (leftArea + a * (1/TMath::Power(t-b,n-1) - 1/TMath::Power(absAlpha - b,n-1)) / (1-n)) / area;
}

// WP is 0 : Noiso , 1 : VLoose, 2: Loose, 3: Medium , 4: Tight 5: Vtight 6: VVtight
float turnOnSF(float pt, int WP)
{
  //return 1.0/turnOnCB(pt,3.60274e+01,5.89434e+00,5.82870e+00,1.83737e+00,9.58000e-01)*turnOnCB(pt,3.45412e+01,5.63353e+00,2.49242e+00,3.35896e+00,1);
  //return turnOnCB(pt,3.45412e+01,5.63353e+00,2.49242e+00,3.35896e+00,1);
  // return turnOnCB(pt,3.60274e+01,5.89434e+00,5.82870e+00,1.83737e+00,9.58000e-01);

  float m0    [7] = {3.86506E+01 , 3.86057E+01 , 3.85953E+01 , 3.81821E+01 , 3.81919E+01 , 3.77850E+01 , 3.76157E+01} ;
  float sigma [7] = {5.81155E+00 , 5.77127E+00 , 5.74632E+00 , 5.33452E+00 , 5.38746E+00 , 4.93611E+00 , 4.76127E+00} ;
  float alpha [7] = {5.82783E+00 , 5.61388E+00 , 5.08553E+00 , 4.42570E+00 , 4.44730E+00 , 4.22634E+00 , 3.62497E+00} ;
  float n     [7] = {3.38903E+00 , 3.77719E+00 , 5.45593E+00 , 4.70512E+00 , 7.39646E+00 , 2.85533E+00 , 3.51839E+00} ;
  float norm  [7] = {9.33449E+00 , 9.30159E-01 , 9.42168E-01 , 9.45637E-01 , 9.33402E-01 , 9.92196E-01 , 9.83701E-01} ;

  return turnOnCB (pt, m0[WP], sigma[WP], alpha[WP], n[WP], norm[WP] );  
}


// float getTriggerWeight(int partType, float pt, TH1F* weightHisto)
// {
//     if (partType == 0) return 0.95;
//     else if (partType == 1) return 0.95;
//     else if (partType == 2)
//     {
//         int ibin = weightHisto->FindBin(pt);
//         if (ibin < 1) ibin = 1;
//         if (ibin > weightHisto->GetNbinsX()) ibin = weightHisto->GetNbinsX() ;
//         return weightHisto->GetBinContent(ibin);
//     }
//     cout << "** WARNING: trigger weight now known for particle type " << partType << endl;
//     return 1.;
// }


float getTriggerWeight(int partType, float pt, float eta, TH1F* rewHisto = 0, ScaleFactor* sfreader = 0, int tauWP = 0)
{
    float weight = 1.0;
    
    switch(partType)
    {
      case 0: // mu
      {
        weight = sfreader->get_EfficiencyData(pt, eta);        
        break;
      }
      case 1: // ele
      {
        float xmin = rewHisto->GetBinLowEdge(1);
        float xmax = rewHisto->GetBinLowEdge(rewHisto->GetNbinsX()+1);
        if (pt < xmin) pt = xmin + 1.E-6;
        if (pt > xmax) pt = xmax - 1.E-6;
        int bin = rewHisto->FindBin (pt);
        weight =  rewHisto->GetBinContent (bin);
        break;
      }
      case 2: // tau
      {
        weight = turnOnSF (pt, tauWP) ;
        break;
      }
      default:
      {
        cout << "** WARNING: trigger weight now known for particle type " << partType << endl;
        weight =  1.;
        break;
      }
    }

    return weight;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


int main (int argc, char** argv)
{
    // read input file and cfg
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  if (argc < 13)
  { 
      cerr << "missing input parameters : argc is: " << argc << endl ;
      cerr << "usage: " << argv[0]
           << " inputFileNameList outputFileName crossSection isData configFile runHHKinFit"
           << " xsecScale(stitch) HTMax(stitch) isTTBar DY_Nbs HHreweightFile TT_stitchType" << endl ; 
      return 1;
  }

  TString inputFile = argv[1] ;
  TString outputFile = argv[2] ;
  cout << "** INFO: inputFile  : " << inputFile << endl;
  cout << "** INFO: outputFile : " << outputFile << endl;
  
  float XS = atof (argv[3]) ;  
  bool isMC = true;
  int isDatabuf = atoi (argv[4]);
  if (isDatabuf == 1)
  {
    isMC = false; 
    XS = 1.;
  }
  cout << "** INFO: x-section: " << XS << endl;
  cout << "** INFO: is it MC?  " << isMC << endl;

  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser () ;  
  TString config ; 
  config.Form ("%s",argv[5]) ;
  cout << "** INFO: reading config : " << config << endl;

  bool runHHKinFit = false;
  string opt7 (argv[6]);
  if (opt7 == "1") runHHKinFit = true;
  cout << "** INFO: running HHKinFit: " << runHHKinFit << endl;

  float xsecScale = 1.0;
  xsecScale = atof (argv[7]);
  cout << "** INFO: xsec scaled by: " << xsecScale << endl;
  XS = XS*xsecScale;

  float HTMax = -999.0;
  HTMax = atof(argv[8]);
  cout << "** INFO: removing HT < " << HTMax << " [-999 means no removal]" << endl;

  int isTTBarI = atoi(argv[9]);
  bool isTTBar = (isTTBarI == 1) ? true : false;
  if (!isMC) isTTBar = false; // force it, you never know...
  cout << "** INFO: is this a TTbar sample? : " << isTTBar << endl;

  bool DY_Nbs = false; // run on genjets to count in DY samples the number of b jets
  bool DY_tostitch = false;
  int I_DY_Nbs = atoi(argv[10]);
  if (I_DY_Nbs == 1)
  {
    DY_Nbs = true; 
    DY_tostitch = true; // FIXME!! this is ok only if we use jet binned samples
  }
  cout << "** INFO: loop on gen jet to do a b-based DY split? " << DY_Nbs << " " << DY_tostitch << endl;

  TFile* HHreweightFile = 0;
  TString doreweight = argv[11];
  cout << "** INFO: reweightin file for non-resonant hh is: " << doreweight << " [ 0 for no reweghting done ]" << endl;
  if (doreweight != TString("0"))
    HHreweightFile = new TFile (doreweight);

  int TT_stitchType = atoi(argv[12]);
  if (!isTTBar) TT_stitchType = 0; // just force if not TT...
  cout << "** INFO: TT stitch type: " << TT_stitchType << " [0: no stitch , 1: fully had, 2: semilept t, 3: semilept tbar, 4: fully lept, 5: semilept all]" << endl;

  // prepare variables needed throughout the code
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    
  if (! (gConfigParser->init (config)))
  {
    cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
    return -1 ;
  }

  bool   beInclusive         = gConfigParser->readBoolOption   ("selections::beInclusive") ;
  float  PUjetID_minCut      = gConfigParser->readFloatOption  ("parameters::PUjetIDminCut") ;
  // int    saveOS              = gConfigParser->readIntOption    ("parameters::saveOS") ;
  float  lepCleaningCone     = gConfigParser->readFloatOption  ("parameters::lepCleaningCone") ;
  int    bChoiceFlag         = gConfigParser->readFloatOption  ("parameters::bChoiceFlag") ;
  int    PUReweight_MC       = gConfigParser->readFloatOption  ("parameters::PUReweightMC") ; 
  int    PUReweight_target   = gConfigParser->readFloatOption  ("parameters::PUReweighttarget") ; 
  string leptonSelectionFlag = gConfigParser->readStringOption ("parameters::lepSelections") ;
  int maxNjetsSaved          = gConfigParser->readIntOption    ("parameters::maxNjetsSaved") ;

  vector<string> trigMuTau   =  (isMC ? gConfigParser->readStringListOption ("triggersMC::MuTau")  : gConfigParser->readStringListOption ("triggersData::MuTau")) ;
  vector<string> trigTauTau  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::TauTau") : gConfigParser->readStringListOption ("triggersData::TauTau")) ;
  vector<string> trigEleTau  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::EleTau") : gConfigParser->readStringListOption ("triggersData::EleTau")) ;
  // vector<string> trigEleMu   =  (isMC ? gConfigParser->readStringListOption ("triggersMC::EleMu")  : gConfigParser->readStringListOption ("triggersData::EleMu")) ;
  //I didn't store MuMu and I don't care for eleele
  // vector<string> trigEleEle  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::EleEle")  : gConfigParser->readStringListOption ("triggersData::EleEle")) ;
  // vector<string> trigMuMu    =  (isMC ? gConfigParser->readStringListOption ("triggersMC::MuMu")  : gConfigParser->readStringListOption ("triggersData::MuMu")) ;

  bool applyTriggers = isMC ? false : true; // true if ask triggerbit + matching, false if doing reweight
  cout << "** INFO: apply triggers? " << applyTriggers << " [ 0: reweight , 1: triggerbit+matching ]" << endl;
  if (applyTriggers)
  {
    cout << "** INFO: triggers used in the skim : " << endl;
    
    cout << "  @ MuTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < trigMuTau.size(); i++) cout << "  " << trigMuTau.at(i);
    cout << endl;

    cout << "  @ EleTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < trigEleTau.size(); i++) cout << "  " << trigEleTau.at(i);
    cout << endl;

    cout << "  @ TauTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < trigTauTau.size(); i++) cout << "  " << trigTauTau.at(i);
    cout << endl;
  }

  // // histo reweight triggers
  // TFile* trigRewFiles  [3];
  // TH1F*  trigRewHistos [3];
  
  // trigRewFile[0]   = new TFile ();  // mu
  // trigRewHistos[0] = (TH1F*) trigRewFile[0] ->Get ();

  // trigRewFile[1]   = new TFile (); // ele
  // trigRewHistos[1] = (TH1F*) trigRewFile[1] ->Get ();

  // trigRewFile[2]   = 0; // tau : WARNING: UNUSED!!
  // trigRewHistos[2] = 0;
  TFile* trigRewEle = new TFile ("weights/ele25TightEff.root"); //FIXME: move to cfg ?
  TH1F*  trigRewEleHisto = (TH1F*) trigRewEle->Get("ele25TightEff");

  string bRegrWeights("");
  bool computeBregr = gConfigParser->readBoolOption ("bRegression::computeBregr");
  if (computeBregr) bRegrWeights = gConfigParser->readStringOption("bRegression::weights");

  cout << "** INFO: computing b jet regression? " << computeBregr << " with weights " << bRegrWeights << endl;

  // input and output setup
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
  appendFromFileList (bigChain, inputFile);
  bigTree theBigTree (bigChain) ;

  //Create a new file + a clone of old tree header. Do not copy events
  TFile * smallFile = new TFile (outputFile, "recreate") ;
  smallFile->cd () ;
  smallTree theSmallTree ("HTauTauTree") ;

  // for HHKinFit
  int hypo_mh1 = 125;
  int hypo_mh2 = 125;

  // for efficiencies
  float totalEvents = 0. ;
  float selectedEvents = 0. ;
  int totalNoWeightsEventsNum = 0 ;
  int selectedNoWeightsEventsNum = 0 ;

  // ------------------------------

  TH1F* hTriggers = getFirstFileHisto (inputFile);
  TH1F* hTauIDS = getFirstFileHisto (inputFile,false);
  triggerReader trigReader (hTriggers);
  trigReader.addTauTauTrigs (trigTauTau);
  trigReader.addMuTauTrigs  (trigMuTau);
  trigReader.addEleTauTrigs (trigEleTau);
  // trigReader.addMuEleTrigs  (trigEleMu);
  // trigReader.addMuMuTrigs   (trigMuMu);
  // trigReader.addEleEleTrigs (trigEleEle);

  // ------------------------------

  OfflineProducerHelper oph (hTriggers, hTauIDS) ;

  // ------------------------------

  bJetRegrVars bjrv;
  TMVA::Reader *bRreader = new TMVA::Reader();
  bjrv.setReader (bRreader);
  string bRegrMethodName = "BDTG method";
  if (computeBregr)
    bRreader->BookMVA( bRegrMethodName.c_str(), bRegrWeights.c_str() ); 
  
  // ------------------------------

  PUReweight reweight (PUReweight::RUN2ANALYSIS); // none : no PU reweight (always returns 1) - RUN2ANALYSIS: get weights according to MC and data targets

  // ------------------------------

  string bTag_SFFile = gConfigParser->readStringOption("bTagScaleFactors::SFFile") ;
  string bTag_effFile = gConfigParser->readStringOption("bTagScaleFactors::effFile") ;
  cout << "B Tag SF file: " << bTag_SFFile << endl;
  bTagSF bTagSFHelper (bTag_SFFile, bTag_effFile, ""); // third field unused, but could be needed to select efficiencies for different selection levels

  // ------------------------------
  
  ScaleFactor * myScaleFactor[2][2]; // [0: mu, 1: ele] [0: trigger, 1: ID]
  for (int i = 0 ; i < 2; i++)
    for (int j = 0; j < 2; j++)
      myScaleFactor[i][j]= new ScaleFactor();
 
  // FIXME: move to cfg?
  myScaleFactor[0][0] -> init_ScaleFactor("weights/data/data/Muon/Muon_IsoMu22_eff_Spring16.root");
  myScaleFactor[0][1] -> init_ScaleFactor("weights/data/data/Muon/Muon_IdIso_eff_Spring16.root");
  myScaleFactor[1][0] -> init_ScaleFactor("weights/data/data/Electron/Electron_Ele23_eff_Spring16.root"); // note! not our trigger
  myScaleFactor[1][1] -> init_ScaleFactor("weights/data/data/Electron/Electron_IdIso_eff_Spring16.root");

  // muon POG SFs
  TFile* fMuPOGSF_ID = new TFile ("weights/MuPogSF/MuonID_Z_2016runB_2p6fb.root");
  TFile* fMuPOGSF_ISO = new TFile ("weights/MuPogSF/MuonISO_Z_2016runB_2p6fb.root");
  TH2F* hMuPOGSF_ID  = (TH2F*) fMuPOGSF_ID -> Get("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/pt_abseta_ratio");  // pt: x, eta: y
  TH2F* hMuPOGSF_ISO = (TH2F*) fMuPOGSF_ISO -> Get("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/pt_abseta_ratio"); // pt: x, eta: y
  // for loose ID:
  // MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1
  // MC_NUM_LooseRelIso_DEN_TightID_PAR_pt_spliteta_bin1

  // ------------------------------
  // reweighting file for HH non resonant
  
  TH1F* hreweightHH   = 0;
  TH2F* hreweightHH2D = 0;
  if (HHreweightFile)
  {
    cout << "** INFO: doing reweight for HH samples" << endl;
    if (HHreweightFile->GetListOfKeys()->Contains("hratio") )
    {  
      hreweightHH = (TH1F*) HHreweightFile->Get("hratio");
      cout << "** INFO: 1D reweight using hratio" << endl;
    }
    else if (HHreweightFile->GetListOfKeys()->Contains("hratio2D") )
    {
      hreweightHH2D = (TH2F*) HHreweightFile->Get("hratio2D");            
      cout << "** INFO: 2D reweight using hratio2D" << endl;
    }
    else
    {
      cout << "** ERROR: reweight histo not found in file provided, stopping execuction" << endl;
      return 1;
    }
  }

  // ------------------------------
  // indexes of tau ID bits

  map <string, int> tauIDsMap;
  for (int ibin = 0; ibin < hTauIDS->GetNbinsX(); ++ibin)
  {
    if (string(hTauIDS->GetXaxis()->GetBinLabel(ibin+1)) == string("byLooseCombinedIsolationDeltaBetaCorr3Hits") )
      tauIDsMap ["byLooseCombinedIsolationDeltaBetaCorr3Hits"] = ibin ;

    if (string(hTauIDS->GetXaxis()->GetBinLabel(ibin+1)) == string("byMediumCombinedIsolationDeltaBetaCorr3Hits") )
      tauIDsMap ["byLooseCombinedIsolationDeltaBetaCorr3Hits"] = ibin ;

    if (string(hTauIDS->GetXaxis()->GetBinLabel(ibin+1)) == string("byTightCombinedIsolationDeltaBetaCorr3Hits") )
      tauIDsMap ["byLooseCombinedIsolationDeltaBetaCorr3Hits"] = ibin ;
  }

  // loop over events
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  for (Long64_t iEvent = 0 ; true ; ++iEvent) 
  {
    if (iEvent % 10000 == 0)  cout << "- reading event " << iEvent << endl ;
    // cout << "- reading event " << iEvent << endl ;
    
    theSmallTree.clearVars () ;
    int got = theBigTree.fChain->GetEntry(iEvent);
    if (got == 0) break;


    // directly reject events outside HT range in case of stitching of inclusive sample-- they should not count in weights
    if (HTMax > 0)
    {
       if (theBigTree.lheHt > HTMax) continue;
    }
    float stitchWeight = 1.0;
    if (DY_tostitch)
    {
      int njets = theBigTree.lheNOutPartons;
      int nb    = theBigTree.lheNOutB;
      // these protections should be useless
      if (njets < 0) njets = 0;
      if (njets > 4) njets = 4;
      if (nb < 0)    nb = 0;
      if (nb > 4)    nb = 4;

      stitchWeight = stitchWeights[njets][nb];
    }

    // gen info -- fetch tt pair and compute top PT reweight
    float topPtReweight = 1.0; // 1 for all the other samples      
    theSmallTree.m_TTtopPtreweight =  1.0 ;
    if (isTTBar)
    {
      float ptTop1 = -1.0;
      float ptTop2 = -1.0;
      int decayTop1 = -999;
      int decayTop2 = -999;
      int pdgIdTop1 = -999;
      int pdgIdTop2 = -999;

      for (unsigned int igen = 0; igen < theBigTree.genpart_pdg->size(); igen++)
      {
        int pdg = theBigTree.genpart_pdg->at(igen);
        //int genflags = theBigTree.genpart_flags->at(igen);
        //int topDM = theBigTree.genpart_TopDecayMode->at(igen);

        if (abs(pdg) == 6) // top -- from LLRNtuples, I am guaranteed to have only 2 tops
        {
          TLorentzVector TopV;
          TopV.SetPxPyPzE (theBigTree.genpart_px->at(igen), theBigTree.genpart_py->at(igen), theBigTree.genpart_pz->at(igen), theBigTree.genpart_e->at(igen) ) ;
          if (ptTop1 < 0)
          {
            ptTop1 = TopV.Pt();
            decayTop1 = theBigTree.genpart_TopDecayMode->at(igen);
            pdgIdTop1 = theBigTree.genpart_pdg->at(igen);
          }
          else if (ptTop2 < 0)
          { 
            ptTop2 = TopV.Pt(); 
            decayTop2 = theBigTree.genpart_TopDecayMode->at(igen);
            pdgIdTop2 = theBigTree.genpart_pdg->at(igen);
            break;
          } // check done in paralles shows that I never have > 2 top .  break is safe .
          //else cout << " !! skim warning: sample is declared as as ttbar, but I have > 2 gen top in the event! " << endl;
        }
      }
      if (ptTop1 < 0 || ptTop2 < 0)
      {
        cout << "** WARNING: sample is declared as TTbar but in the event I didn't find 2 tops (1,2) :" << ptTop1 << " " << ptTop2 << endl;
      }
      else
      {
          // filter by decay mode if needed for stitching
          // [0: no stitch , 1: fully had, 2: semilept t, 3: semilept tbar, 4: fully lept, 5: semilept all]
          // TopDecayMode: 0: Had, 1-5: leptonic, 6: other -- consider "other" as a possible hadronic decay (includes rare W->bc)
          
          bool isT1Lept = (decayTop1 >= 1 && decayTop1 <= 5) ;
          bool isT2Lept = (decayTop2 >= 1 && decayTop2 <= 5) ;
          if (pdgIdTop1 / pdgIdTop2 > 0) cout << "** WARNING: I found two tops with the same sign " << pdgIdTop1 << " " << pdgIdTop2 << endl;
          
          // cout << "DEBUG: event with " << pdgIdTop1 << "-->  " << decayTop1 << " | " << pdgIdTop2 << "--> " << decayTop2 << " SKIMTYPE=" << TT_stitchType << endl;
          switch (TT_stitchType)
          {
            case 0:
              break; // no stitching
            
            case 1:
              if (isT1Lept || isT2Lept) continue; // fully had
              break;
            
            case 2: // top leptonic, antitop hadronic
              if (pdgIdTop1 > 0) // 1 is top
              {
                if (! (isT1Lept && !isT2Lept) ) continue;
              }
              else // 1 is antitop
              {
                if (! (isT2Lept && !isT1Lept) ) continue;
              }
              break;
            
            case 3:
              if (pdgIdTop1 > 0) // 1 is top
              {
                if (! (!isT1Lept && isT2Lept) ) continue;
              }
              else // 1 is antitop
              {
                if (! (!isT2Lept && isT1Lept) ) continue;
              }
              break;
            
            case 4:
              if (!isT1Lept || !isT2Lept) continue;
              break;

            case 5:
              if (isT1Lept == isT2Lept) continue; // must be one had and the other lep, not equal
              break;
            
            default:
              cout << "** WARNING: unknown TT stytch type " << TT_stitchType << endl;
          }

          // cout << "WAS ACCEPTED" << endl;

          float SFTop1 = TMath::Exp(aTopRW+bTopRW*ptTop1);
          float SFTop2 = TMath::Exp(aTopRW+bTopRW*ptTop2);
          topPtReweight = TMath::Sqrt (SFTop1*SFTop2); // save later together with other weights
          theSmallTree.m_TTtopPtreweight = topPtReweight ;
      }
    }

    // For Drell-Yan only: loop on genjets and count how many are there with 0,1,2 b
    // 0: 0bjet, 1: 1 b jet, 2: >= 2 b jet
    theSmallTree.m_DYscale_LL = 1.0; // all the other MC samples + data have no weight
    theSmallTree.m_DYscale_MM = 1.0;        
    if (isMC && DY_Nbs)
    {
      TLorentzVector vgj;
      int nbs = 0;
      for (unsigned int igj = 0; igj < theBigTree.genjet_px->size(); igj++)
      {
          vgj.SetPxPyPzE(theBigTree.genjet_px->at(igj), theBigTree.genjet_py->at(igj), theBigTree.genjet_pz->at(igj), theBigTree.genjet_e->at(igj));
          if (vgj.Pt() > 20 && TMath::Abs(vgj.Eta()) < 2.5)
          {
              int theFlav = theBigTree.genjet_hadronFlavour->at(igj);
              if (abs(theFlav) == 5) nbs++;
              
              // about 2% of DY events print the following message :-(
              // if (theFlav == -999) cout << "** warning: gen jet with flav = -999 of pt: " << vgj.Pt() << " eta: " << vgj.Eta() << endl;
          }
      }
      if (nbs > 2) nbs = 2;
      theSmallTree.m_nBhadrons = nbs;

      theSmallTree.m_DYscale_LL = DYscale_LL[nbs];
      theSmallTree.m_DYscale_MM = DYscale_MM[nbs];
    }

    // HH reweight for non resonant
    float HHweight = 1.0;
    if (hreweightHH || hreweightHH2D)
    {
      // cout << "DEBUG: reweight!!!" << endl;
      TLorentzVector vH1, vH2, vBoost, vSum;
      float mHH = -1;
      float ct1 = -999;
      // loop on gen to find Higgs
      int idx1 = -1;
      int idx2 = -1;
      for (unsigned int igen = 0; igen < theBigTree.genpart_px->size(); igen++)
      {
          if (theBigTree.genpart_pdg->at(igen) == 25)
          {
              bool isFirst = CheckBit (theBigTree.genpart_flags->at(igen), 12) ; // 12 = isFirstCopy
              if (isFirst)
              {
                  if (idx1 >= 0 && idx2 >= 0)
                  {
                      cout << "** ERROR: more than 2 H identified " << endl;
                      continue;
                  }
                  (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
              }
          }
      }

      if (idx1 == -1 || idx2 == -1)
      {
          cout << "** ERROR: couldn't find 2 H" << endl;
          continue;
      }

      vH1.SetPxPyPzE (theBigTree.genpart_px->at(idx1), theBigTree.genpart_py->at(idx1), theBigTree.genpart_pz->at(idx1), theBigTree.genpart_e->at(idx1) );
      vH2.SetPxPyPzE (theBigTree.genpart_px->at(idx2), theBigTree.genpart_py->at(idx2), theBigTree.genpart_pz->at(idx2), theBigTree.genpart_e->at(idx2) );
      vSum = vH1+vH2;
      mHH = vSum.M();
      vH1.Boost(-vSum.BoostVector());                     
      ct1 = vH1.CosTheta();

      if (hreweightHH) // 1D
      {
        int ibin = hreweightHH->FindBin(mHH);
        HHweight = hreweightHH->GetBinContent(ibin);
      }
      else if (hreweightHH2D) // 2D
      {
        int ibin = hreweightHH2D->FindBin(mHH, ct1);
        HHweight = hreweightHH2D->GetBinContent(ibin);        
      }
    }

    ///////////////////////////////////////////////////////////
    // END of gen related stuff -- compute tot number of events

    if (isMC)
    {
      totalEvents += theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu) * topPtReweight * HHweight * stitchWeight;
    }
    else
    {
      totalEvents += 1 ;
    }
    ++totalNoWeightsEventsNum ;
    
    // ----------------------------------------------------------
    //  apply MET filters -- FIXME: not applied now
    
    int metbit = theBigTree.metfilterbit;
    int metpass = metbit & (1 << 0);
    metpass += metbit & (1 << 2);
    metpass += metbit & (1 << 5);
    metpass += metbit & (1 << 6);
    // if(metpass > 0) continue ; // FIXME!!! disabled only temporarily

    // ----------------------------------------------------------
    // require at least 1 pair
    if (theBigTree.indexDau1->size () == 0) continue ;


    // ----------------------------------------------------------
    // assess the pair type 
    // loop over the daughters to select pair type: mu > e > tau
    // apply tight baseline (with iso to check)
    
    int nmu = 0;
    int nele = 0;
    // int ntau = 0;
    for (unsigned int idau = 0; idau < theBigTree.daughters_px->size(); ++idau)
    {
        int dauType = theBigTree.particleType->at(idau);
        if (oph.isMuon(dauType))
        {
            if (oph.muBaseline (&theBigTree, idau, 23., 2.1, 0.1, string("All")) ) ++nmu;
        }
        else if (oph.isElectron(dauType))
        {
            if (oph.eleBaseline (&theBigTree, idau, 27., 2.1, 0.1, 0, string("All")) ) ++nele;
        }
    }
    int pairType = 2; // tau tau
    if (nmu > 0) pairType = 0 ; // mu tau
    else if (nele > 0) pairType = 1 ; // ele tau

    // ----------------------------------------------------------
    // choose the first pair passing baseline and being of the right pair type

    int chosenTauPair = -1;

    // vector <pair<float, int>> chosenTauPairsIso;   // sum pt , index
    // vector <pair<float, int>> chosenTauPairsRlxIso;

    for (unsigned int iPair = 0 ; iPair < theBigTree.indexDau1->size () ; ++iPair)
    {
        int t_firstDaughterIndex  = theBigTree.indexDau1->at (iPair) ;  
        int t_secondDaughterIndex = theBigTree.indexDau2->at (iPair) ;
        int t_type1 = theBigTree.particleType->at (t_firstDaughterIndex) ;
        int t_type2 = theBigTree.particleType->at (t_secondDaughterIndex) ;        
        if ( oph.getPairType (t_type1, t_type2) != pairType ) continue ;
        // string whatApplyForIsoLep = "Vertex-LepID-pTMin-etaMax-againstEle-againstMu-Iso" ;
        // if ( oph.pairPassBaseline (&theBigTree, iPair, string("Vertex-LepID-pTMin-etaMax-againstEle-againstMu") ) )
        
        // TLorentzVector t_tlv_firstLepton (
        //   theBigTree.daughters_px->at (t_firstDaughterIndex),
        //   theBigTree.daughters_py->at (t_firstDaughterIndex),
        //   theBigTree.daughters_pz->at (t_firstDaughterIndex),
        //   theBigTree.daughters_e->at (t_firstDaughterIndex)
        // );
        // TLorentzVector t_tlv_secondLepton (
        //   theBigTree.daughters_px->at (t_secondDaughterIndex),
        //   theBigTree.daughters_py->at (t_secondDaughterIndex),
        //   theBigTree.daughters_pz->at (t_secondDaughterIndex),
        //   theBigTree.daughters_e->at (t_secondDaughterIndex)
        // );

        if ( oph.pairPassBaseline (&theBigTree, iPair, leptonSelectionFlag ) )
        {
            chosenTauPair = iPair;
            break;          
        }
        // if ( oph.pairPassBaseline (&theBigTree, iPair, (leptonSelectionFlag+string("-Iso")) ) )
        // {
        //     chosenTauPairsIso.push_back(make_pair(t_tlv_firstLepton.Pt()+t_tlv_secondLepton.Pt(), iPair));
        //     // chosenTauPair = iPair;
        //     // break;
        // }
        // if ( oph.pairPassBaseline (&theBigTree, iPair, (leptonSelectionFlag+string("-TauRlxIzo")) ) )
        // {
        //     chosenTauPairsIso.push_back(make_pair(t_tlv_firstLepton.Pt()+t_tlv_secondLepton.Pt(), iPair));
        // }
    }

    // if (chosenTauPairsIso.size() > 0)
    // {
    //   sort(chosenTauPairsIso.begin(), chosenTauPairsIso.end()); // will get highest pt sum
    //   chosenTauPair = chosenTauPairsIso.back().second;
    // }
    // else if (chosenTauPairsRlxIso.size() > 0)
    // {
    //   sort(chosenTauPairsRlxIso.begin(), chosenTauPairsRlxIso.end()); // will get highest pt sum
    //   chosenTauPair = chosenTauPairsRlxIso.back().second;
    // }
    // else continue; // no pair found

    if (chosenTauPair < 0) continue; // no pair found over baseline

    // ----------------------------------------------------------
    // pair has been assessed , check trigger information

    const int firstDaughterIndex  = theBigTree.indexDau1->at (chosenTauPair) ;  
    const int secondDaughterIndex = theBigTree.indexDau2->at (chosenTauPair) ;
    const int type1 = theBigTree.particleType->at (firstDaughterIndex) ;
    const int type2 = theBigTree.particleType->at (secondDaughterIndex) ;        
    const int pType = pairType ;
    const int isOS  = theBigTree.isOSCand->at (chosenTauPair) ;

    const TLorentzVector tlv_firstLepton (
          theBigTree.daughters_px->at (firstDaughterIndex),
          theBigTree.daughters_py->at (firstDaughterIndex),
          theBigTree.daughters_pz->at (firstDaughterIndex),
          theBigTree.daughters_e->at (firstDaughterIndex)
    );

    const TLorentzVector tlv_secondLepton (
          theBigTree.daughters_px->at (secondDaughterIndex),
          theBigTree.daughters_py->at (secondDaughterIndex),
          theBigTree.daughters_pz->at (secondDaughterIndex),
          theBigTree.daughters_e->at (secondDaughterIndex)
    );

    // the following code is the match of the tau to a L1 tau seed due to an error in seed removal from path
    // needed only when analyzing 2015 data
    // if (pairType == 2 && isMC)
    // {
    //   bool hasL1Match_1 = theBigTree.daughters_isL1IsoTau28Matched->at (firstDaughterIndex);
    //   bool hasL1Match_2 = theBigTree.daughters_isL1IsoTau28Matched->at (secondDaughterIndex);
    //   bool goodL1 = (hasL1Match_1 && hasL1Match_2);
    //   if (!goodL1) continue;
    // }

    // DATA strategy
    float trgEvtWeight = 1.0;
    if (applyTriggers)
    {
      Long64_t triggerbit = theBigTree.triggerbit;
      bool passTrg = trigReader.checkOR (pairType, triggerbit) ;
      Long64_t matchFlag1 = (Long64_t) theBigTree.daughters_trgMatched->at(firstDaughterIndex);
      Long64_t matchFlag2 = (Long64_t) theBigTree.daughters_trgMatched->at(secondDaughterIndex);
      bool passMatch1 = false;
      bool passMatch2 = false;
      // FIXME!! true only if single lep trigger for eTau and muTau
      if (pairType == 0 || pairType == 1)
      {
          passMatch1 = trigReader.checkOR (pairType, matchFlag1) ;
          passMatch2 = true;
      }
      else if (pairType == 2)
      {
          passMatch1 = trigReader.checkOR (pairType, matchFlag1) ;
          passMatch2 = trigReader.checkOR (pairType, matchFlag2) ;            
      }
      // require trigger + legs matched
      bool triggerAccept = (passTrg && passMatch1 && passMatch2) ;
      
      if (!triggerAccept) continue;
    }

    // MC strategy
    else
    {
      float evtLeg1weight = 1.0;
      float evtLeg2weight = 1.0;

      if (pairType == 0)  //mutau
      {
          evtLeg1weight = getTriggerWeight(type1, tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), 0, myScaleFactor[type1][0], 0) ;
          evtLeg2weight = 1.0;        
      }

      else if (pairType == 1) //eletau
      {
          evtLeg1weight = getTriggerWeight(type1, tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), trigRewEleHisto, 0, 0) ;
          evtLeg2weight = 1.0;        
      }

      else if (pairType == 2) //tautau
      {
          evtLeg1weight = getTriggerWeight(type1, tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), 0, 0, 1) ;
          evtLeg2weight = getTriggerWeight(type2, tlv_secondLepton.Pt(), tlv_secondLepton.Eta(), 0, 0, 1) ;
      }
      trgEvtWeight = evtLeg1weight*evtLeg2weight;
    }

    // ----------------------------------------------------------
    // pair selection is now complete, compute oher quantitites
    
    TLorentzVector tlv_tauH = tlv_firstLepton + tlv_secondLepton ;
    TLorentzVector tlv_tauH_SVFIT ;

    theSmallTree.m_tauH_SVFIT_mass = theBigTree.SVfitMass->at (chosenTauPair) ;
    // in case the SVFIT mass is calculated
    if (theBigTree.SVfitMass->at (chosenTauPair) > -900.)
    {
      theSmallTree.m_tauH_SVFIT_pt     = theBigTree.SVfit_pt->at  (chosenTauPair) ;
      theSmallTree.m_tauH_SVFIT_eta    = theBigTree.SVfit_eta->at (chosenTauPair) ;
      theSmallTree.m_tauH_SVFIT_phi    = theBigTree.SVfit_phi->at (chosenTauPair) ;
      theSmallTree.m_tauH_SVFIT_METphi = theBigTree.SVfit_fitMETPhi->at (chosenTauPair) ;
      theSmallTree.m_tauH_SVFIT_METrho = theBigTree.SVfit_fitMETRho->at (chosenTauPair) ;      

      tlv_tauH_SVFIT.SetPtEtaPhiM (
          theBigTree.SVfit_pt->at (chosenTauPair),
          theBigTree.SVfit_eta->at (chosenTauPair),
          theBigTree.SVfit_phi->at (chosenTauPair),
          theBigTree.SVfitMass->at (chosenTauPair)
        ) ;
    }


    theSmallTree.m_pairType    = pType ;
    theSmallTree.m_PUReweight  = (isMC ? reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu) : 1) ;      
    theSmallTree.m_MC_weight   = (isMC ? theBigTree.aMCatNLOweight * XS * stitchWeight * HHweight * trgEvtWeight : 1) ;
    theSmallTree.m_lheht       = (isMC ? theBigTree.lheHt : 0) ;
    theSmallTree.m_EventNumber = theBigTree.EventNumber ;
    theSmallTree.m_RunNumber   = theBigTree.RunNumber ;
    theSmallTree.m_npv  = theBigTree.npv ;
    theSmallTree.m_npu  = theBigTree.npu ;
    theSmallTree.m_lumi = theBigTree.lumi ;
    theSmallTree.m_triggerbit = theBigTree.triggerbit ;
    theSmallTree.m_rho  = theBigTree.rho ;
    theSmallTree.m_isMC = isMC ;
    theSmallTree.m_isOS = theBigTree.isOSCand->at (chosenTauPair) ;
    // theSmallTree.m_met_phi = theBigTree.metphi ;
    // theSmallTree.m_met_et = theBigTree.met ;
    TVector2 vMET (theBigTree.METx->at(chosenTauPair) , theBigTree.METy->at(chosenTauPair));
    theSmallTree.m_met_phi = vMET.Phi();
    theSmallTree.m_met_et  =  vMET.Mod();
    theSmallTree.m_mT      = theBigTree.mT_Dau1->at (chosenTauPair) ;

    theSmallTree.m_tauH_pt   = tlv_tauH.Pt () ;
    theSmallTree.m_tauH_eta  = tlv_tauH.Eta () ;
    theSmallTree.m_tauH_phi  = tlv_tauH.Phi () ;
    theSmallTree.m_tauH_e    = tlv_tauH.E () ;
    theSmallTree.m_tauH_mass = tlv_tauH.M () ;

    // useless, can be removed from smallTree
    // theSmallTree.m_L3filter1 = theBigTree.daughters_L3FilterFiredLast->at (firstDaughterIndex) ;
    // theSmallTree.m_L3filterlast1 = theBigTree.daughters_L3FilterFiredLast->at (firstDaughterIndex) ;
    // theSmallTree.m_L3filter2 = theBigTree.daughters_L3FilterFiredLast->at (secondDaughterIndex) ;
    // theSmallTree.m_L3filterlast2 = theBigTree.daughters_L3FilterFiredLast->at (secondDaughterIndex) ;

    theSmallTree.m_dau1_iso = getIso (firstDaughterIndex, tlv_firstLepton.Pt (), theBigTree) ;
    theSmallTree.m_dau1_photonPtSumOutsideSignalCone = theBigTree.photonPtSumOutsideSignalCone->at (firstDaughterIndex) ;

    int ibit = tauIDsMap["byLooseCombinedIsolationDeltaBetaCorr3Hits"] ;
    theSmallTree.m_dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (firstDaughterIndex)  & (1 << ibit) ) ? true : false ;
    theSmallTree.m_dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (secondDaughterIndex) & (1 << ibit) ) ? true : false ;

    ibit = tauIDsMap["byMediumCombinedIsolationDeltaBetaCorr3Hits"] ;
    theSmallTree.m_dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (firstDaughterIndex)  & (1 << ibit) ) ? true : false ;
    theSmallTree.m_dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (secondDaughterIndex) & (1 << ibit) ) ? true : false ;

    ibit = tauIDsMap["byTightCombinedIsolationDeltaBetaCorr3Hits"] ;
    theSmallTree.m_dau1_byTightCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (firstDaughterIndex)  & (1 << ibit) ) ? true : false ;
    theSmallTree.m_dau2_byTightCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (secondDaughterIndex) & (1 << ibit) ) ? true : false ;


    theSmallTree.m_dau1_pt = tlv_firstLepton.Pt () ;
    theSmallTree.m_dau1_eta = tlv_firstLepton.Eta () ;
    theSmallTree.m_dau1_phi = tlv_firstLepton.Phi () ;
    theSmallTree.m_dau1_e = theBigTree.daughters_e->at (firstDaughterIndex) ;
    theSmallTree.m_dau1_flav = theBigTree.daughters_charge->at (firstDaughterIndex) * 
                               (theBigTree.particleType->at (firstDaughterIndex) + 1) ;
                               // 1 = from muons collection
                               // 2 = from electrons collection
                               // 3 = from tauH collection
                               
    theSmallTree.m_dau2_iso = getIso (secondDaughterIndex, tlv_secondLepton.Pt (), theBigTree) ;
    theSmallTree.m_dau2_photonPtSumOutsideSignalCone = theBigTree.photonPtSumOutsideSignalCone->at (secondDaughterIndex) ;      
    theSmallTree.m_dau2_pt = tlv_secondLepton.Pt () ;
    theSmallTree.m_dau2_eta = tlv_secondLepton.Eta () ;
    theSmallTree.m_dau2_phi = tlv_secondLepton.Phi () ;
    theSmallTree.m_dau2_e = theBigTree.daughters_e->at (secondDaughterIndex) ;
    theSmallTree.m_dau2_flav = theBigTree.daughters_charge->at (secondDaughterIndex) * 
                               (theBigTree.particleType->at (secondDaughterIndex) + 1) ;

    float trigSF = 1.0;
    float idAndIsoSF = 1.0;
    // particle 2 is always a TAU --  FIXME: not good for emu
    if (type1 == 0 && isMC) // mu
    {
      // trigSF = myScaleFactor[type1][0]->get_ScaleFactor(tlv_firstLepton.Pt(),tlv_firstLepton.Eta());
      trigSF = 1.0; // no trigger info available in MC
      
      int ptbin = hMuPOGSF_ID->GetXaxis()->FindBin(tlv_firstLepton.Pt());
      if (ptbin > hMuPOGSF_ID->GetNbinsX()) ptbin = hMuPOGSF_ID->GetNbinsX();
      else if (ptbin < 1) ptbin = 1;

      int etabin = hMuPOGSF_ID->GetYaxis()->FindBin(TMath::Abs(tlv_firstLepton.Eta()));
      if (etabin > hMuPOGSF_ID->GetNbinsY()) etabin = hMuPOGSF_ID->GetNbinsY();
      else if (etabin < 1) etabin = 1;

      idAndIsoSF = hMuPOGSF_ID->GetBinContent(ptbin, etabin);

      ptbin = hMuPOGSF_ISO->GetXaxis()->FindBin(tlv_firstLepton.Pt());
      if (ptbin > hMuPOGSF_ISO->GetNbinsX()) ptbin = hMuPOGSF_ISO->GetNbinsX();
      else if (ptbin < 1) ptbin = 1;

      etabin = hMuPOGSF_ISO->GetYaxis()->FindBin(TMath::Abs(tlv_firstLepton.Eta()));
      if (etabin > hMuPOGSF_ISO->GetNbinsY()) etabin = hMuPOGSF_ISO->GetNbinsY();
      else if (etabin < 1) etabin = 1;

      idAndIsoSF *= hMuPOGSF_ISO->GetBinContent(ptbin, etabin);
      // idAndIsoSF = myScaleFactor[type1][1]->get_ScaleFactor(tlv_firstLepton.Pt(),tlv_firstLepton.Eta());
    }

    else if (type1 == 1 && isMC) // mu
    {
      // trigSF = myScaleFactor[type1][0]->get_ScaleFactor(tlv_firstLepton.Pt(),tlv_firstLepton.Eta());
      trigSF = 1.0; // no trigger info available in MC
      idAndIsoSF = myScaleFactor[type1][1]->get_ScaleFactor(tlv_firstLepton.Pt(),tlv_firstLepton.Eta());
    }


    theSmallTree.m_trigSF     = (isMC ? trigSF : 1.0);
    theSmallTree.m_IdAndIsoSF = (isMC ? idAndIsoSF : 1.0);

    // loop over leptons
    vector<pair<float, int> > thirdLeptons ; // pt, idx
    for (unsigned int iLep = 0 ; (iLep < theBigTree.daughters_px->size ()) ; ++iLep)
    {
      // skip the H decay candiates
      if (int (iLep) == firstDaughterIndex || 
          int (iLep) == secondDaughterIndex) continue ;

      // remove taus
      if (theBigTree.particleType->at (iLep) == 2)
      {
        continue ;
      }  
      else if (theBigTree.particleType->at (iLep) == 0) // muons
      {
        if (!oph.muBaseline (&theBigTree, iLep, 10., 2.4, 0.3)) continue ;
      }
      else if (theBigTree.particleType->at (iLep) == 1) // electrons
      {
        if (!oph.eleBaseline (&theBigTree, iLep, 10., 2.4, 0.3, 1)) continue ;
        // if (!oph.eleBaseline (&theBigTree, iLep, 10., 2.5, 0.3, 1)) continue ;
      }
      TLorentzVector tlv_dummyLepton
      (
        theBigTree.daughters_px->at (iLep),
        theBigTree.daughters_py->at (iLep),
        theBigTree.daughters_pz->at (iLep),
        theBigTree.daughters_e->at (iLep)
      ) ;
      thirdLeptons.push_back (make_pair(tlv_dummyLepton.Pt(), iLep)) ;
      
    } // loop over leptons
    sort (thirdLeptons.begin(), thirdLeptons.end()) ;
    // reverse loop to start from last one == highest pT
    for (int iLep = thirdLeptons.size() -1; (iLep >=0) && (theSmallTree.m_nleps < 2) ; iLep--)
    {
      TLorentzVector tlv_dummyLepton
      (
        theBigTree.daughters_px->at (iLep),
        theBigTree.daughters_py->at (iLep),
        theBigTree.daughters_pz->at (iLep),
        theBigTree.daughters_e->at (iLep)
      ) ;

      theSmallTree.m_leps_pt.push_back   (tlv_dummyLepton.Pt ()) ;
      theSmallTree.m_leps_eta.push_back  (tlv_dummyLepton.Eta ()) ;
      theSmallTree.m_leps_phi.push_back  (tlv_dummyLepton.Phi ()) ;
      theSmallTree.m_leps_e.push_back    (tlv_dummyLepton.E ()) ;
      theSmallTree.m_leps_flav.push_back (theBigTree.particleType->at (iLep)) ;
      ++theSmallTree.m_nleps ;
    } 

    // ----------------------------------------------------------
    // select jets 

    vector <pair <float, int> > jets_and_sortPar ;
    // loop over jets
    for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
    {
      // JET PU ID cut 
      if (theBigTree.jets_PUJetID->at (iJet) < PUjetID_minCut) continue ;
  
      TLorentzVector tlv_jet 
      (
        theBigTree.jets_px->at (iJet),
        theBigTree.jets_py->at (iJet),
        theBigTree.jets_pz->at (iJet),
        theBigTree.jets_e->at (iJet)
      ) ;
      if (tlv_jet.Pt () < 30. /*GeV*/) continue ;  // 30 for b-tag
      if (tlv_jet.DeltaR (tlv_firstLepton) < lepCleaningCone) continue ;
      if (tlv_jet.DeltaR (tlv_secondLepton) < lepCleaningCone) continue ;
      if (TMath::Abs(tlv_jet.Eta()) > 2.4) continue; // 2.4 for b-tag

      float sortPar = (bChoiceFlag == 1 ) ? theBigTree.bCSVscore->at (iJet) : tlv_jet.Pt() ;
      if (bChoiceFlag != 1 && bChoiceFlag != 2) cout << "** WARNING : bChoiceFlag now known :" << bChoiceFlag << endl;
      
      jets_and_sortPar.push_back (make_pair (sortPar, iJet) );          

    } // loop over jets

    if (!beInclusive && jets_and_sortPar.size () < 2) continue ;

    // sort jet collection by CSV
    sort (jets_and_sortPar.begin(), jets_and_sortPar.end(), bJetSort); //sort by first parameter, then pt (dummy if pt order chosen)

    if (jets_and_sortPar.size () >= 2) 
    { 
      vector<pair <int, float> > jets_and_BTag;
      for (auto pair : jets_and_sortPar) jets_and_BTag.push_back (make_pair(pair.second, pair.first)); // just for compatibility... 

      // NB !!! the following function only works if jets_and_sortPar contains <CVSscore, idx> 
      vector<float> bTagWeight = bTagSFHelper.getEvtWeight (jets_and_BTag, theBigTree.jets_px, theBigTree.jets_py, theBigTree.jets_pz, theBigTree.jets_e, theBigTree.jets_HadronFlavour, pType) ;
      theSmallTree.m_bTagweightL = (isMC ? bTagWeight.at(0) : 1.0) ;
      theSmallTree.m_bTagweightM = (isMC ? bTagWeight.at(1) : 1.0) ;
      theSmallTree.m_bTagweightT = (isMC ? bTagWeight.at(2) : 1.0) ;

      int njets = jets_and_sortPar.size();
      const int bjet1idx = jets_and_sortPar.at(njets-1).second ;
      const int bjet2idx = jets_and_sortPar.at(njets-2).second ;

      TLorentzVector tlv_firstBjet 
      (
        theBigTree.jets_px->at (bjet1idx),
        theBigTree.jets_py->at (bjet1idx),
        theBigTree.jets_pz->at (bjet1idx),
        theBigTree.jets_e->at  (bjet1idx)
      ) ;
      TLorentzVector tlv_secondBjet 
      (
        theBigTree.jets_px->at (bjet2idx),
        theBigTree.jets_py->at (bjet2idx),
        theBigTree.jets_pz->at (bjet2idx),
        theBigTree.jets_e->at  (bjet2idx)
      ) ;

      double ptRegr[2] = {tlv_firstBjet.Pt(), tlv_secondBjet.Pt()};
      if (computeBregr)
      {
        for (int iBJet = 0; iBJet <=1; iBJet++)
        { 
          int bidx = (iBJet == 0 ? bjet1idx : bjet2idx);
          bjrv.Jet_pt     = (iBJet == 0 ? tlv_firstBjet.Pt()  : tlv_secondBjet.Pt());
          bjrv.Jet_eta    = (iBJet == 0 ? tlv_firstBjet.Eta() : tlv_secondBjet.Eta());
          //bjrv.Jet_corr         = theBigTree.jets_rawPt->at(bidx);
          bjrv.Jet_corr   = theBigTree.jetRawf->at(bidx); // should be 1./jetrawf ??
          bjrv.rho              = theBigTree.rho;
          bjrv.Jet_mt           = theBigTree.jets_mT->at(bidx);
          bjrv.Jet_leadTrackPt  = theBigTree.jets_leadTrackPt->at(bidx);
          bjrv.Jet_leptonPtRel  = theBigTree.jets_leptonPtRel->at(bidx);
          bjrv.Jet_leptonPt     = theBigTree.jets_leptonPt->at(bidx);
          bjrv.Jet_leptonDeltaR = theBigTree.jets_leptonDeltaR->at(bidx);
          bjrv.Jet_neHEF   = theBigTree.jets_nHEF->at(bidx);
          bjrv.Jet_neEmEF  = theBigTree.jets_nEmEF->at(bidx);
          bjrv.Jet_chMult  = theBigTree.jets_chMult->at(bidx);
          bjrv.Jet_vtxPt   = theBigTree.jets_vtxPt->at(bidx);
          bjrv.Jet_vtxMass = theBigTree.jets_vtxMass->at(bidx);
          bjrv.Jet_vtx3dL  = theBigTree.jets_vtx3dL->at(bidx);
          bjrv.Jet_vtxNtrk = theBigTree.jets_vtxNtrk->at(bidx);
          bjrv.Jet_vtx3deL = theBigTree.jets_vtx3deL->at(bidx);

          ptRegr[iBJet] = (bRreader->EvaluateRegression (bRegrMethodName.c_str()))[0];
        }
      }

      TLorentzVector tlv_firstBjet_raw = tlv_firstBjet;
      TLorentzVector tlv_secondBjet_raw = tlv_secondBjet;

      theSmallTree.m_bjet1_pt_raw = tlv_firstBjet_raw.Pt();
      theSmallTree.m_bjet2_pt_raw = tlv_secondBjet_raw.Pt();

      TLorentzVector tlv_bH_raw = tlv_firstBjet + tlv_secondBjet ;
      theSmallTree.m_bH_mass_raw = tlv_bH_raw.M();
      // FIXME : here mass is manually set to 0, should we change it?
      float ptScale1 = ptRegr[0] / tlv_firstBjet.Pt() ;
      float ptScale2 = ptRegr[1] / tlv_secondBjet.Pt() ;
      tlv_firstBjet.SetPtEtaPhiE (ptRegr[0], tlv_firstBjet.Eta(), tlv_firstBjet.Phi(), ptScale1*tlv_firstBjet.Energy());
      tlv_secondBjet.SetPtEtaPhiE (ptRegr[1], tlv_secondBjet.Eta(), tlv_secondBjet.Phi(), ptScale2*tlv_secondBjet.Energy());

      theSmallTree.m_bjet1_pt   = tlv_firstBjet.Pt () ;
      theSmallTree.m_bjet1_eta  = tlv_firstBjet.Eta () ;
      theSmallTree.m_bjet1_phi  = tlv_firstBjet.Phi () ;
      theSmallTree.m_bjet1_e    = theBigTree.jets_e->at (bjet1idx) ;
      theSmallTree.m_bjet1_bID  = theBigTree.bCSVscore->at (bjet1idx) ;
      theSmallTree.m_bjet1_flav = theBigTree.jets_HadronFlavour->at (bjet1idx) ;

      theSmallTree.m_bjet2_pt   = tlv_secondBjet.Pt () ;
      theSmallTree.m_bjet2_eta  = tlv_secondBjet.Eta () ;
      theSmallTree.m_bjet2_phi  = tlv_secondBjet.Phi () ;
      theSmallTree.m_bjet2_e    = theBigTree.jets_e->at (bjet2idx) ;
      theSmallTree.m_bjet2_bID  = theBigTree.bCSVscore->at (bjet2idx) ;
      theSmallTree.m_bjet2_flav = theBigTree.jets_HadronFlavour->at (bjet2idx) ;

      bool hasgj1 = false;
      bool hasgj2 = false;          
      if (isMC){            
        
        int mcind = theBigTree.jets_genjetIndex->at(bjet2idx);
        if (mcind>=0){
          TLorentzVector gen(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
          theSmallTree.m_genjet1_pt = gen.Pt();
          theSmallTree.m_genjet1_eta = gen.Eta();
          theSmallTree.m_genjet1_phi = gen.Phi();
          theSmallTree.m_genjet1_e = gen.E();
          if (gen.Pt() > 8) hasgj1 = true;
        }

        mcind = theBigTree.jets_genjetIndex->at(bjet2idx);
        if (mcind>=0){
          TLorentzVector gen(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
          theSmallTree.m_genjet2_pt = gen.Pt();
          theSmallTree.m_genjet2_eta = gen.Eta();
          theSmallTree.m_genjet2_phi = gen.Phi();
          theSmallTree.m_genjet2_e = gen.E();
          if (gen.Pt() > 8) hasgj2 = true;
        }
      }
      theSmallTree.m_bjet1_hasgenjet = hasgj1 ;
      theSmallTree.m_bjet2_hasgenjet = hasgj2 ;

      float METx = theBigTree.METx->at (chosenTauPair) ;
      float METy = theBigTree.METy->at (chosenTauPair) ;
      //float METpt = 0;//TMath::Sqrt (METx*METx + METy*METy) ;

      TLorentzVector tlv_bH = tlv_firstBjet + tlv_secondBjet ;
      TLorentzVector tlv_neutrinos =  tlv_bH - tlv_bH_raw;
      theSmallTree.m_met_et_corr = theBigTree.met - tlv_neutrinos.Et() ;

      const TVector2 ptmiss = TVector2(METx, METy) ;
      //TVector2 ptmiss = TVector2(METx,METy);
      TMatrixD metcov (2, 2) ;
      metcov (0,0) = theBigTree.MET_cov00->at (chosenTauPair) ;
      metcov (1,0) = theBigTree.MET_cov10->at (chosenTauPair) ;
      metcov (0,1) = theBigTree.MET_cov01->at (chosenTauPair) ;    
      metcov (1,1) = theBigTree.MET_cov11->at (chosenTauPair) ;

      const TMatrixD stableMetCov = metcov; 
      theSmallTree.m_bH_pt = tlv_bH.Pt () ;
      theSmallTree.m_bH_eta = tlv_bH.Eta () ;
      theSmallTree.m_bH_phi = tlv_bH.Phi () ;
      theSmallTree.m_bH_e = tlv_bH.E () ;
      theSmallTree.m_bH_mass = tlv_bH.M () ;

      TLorentzVector tlv_HH = tlv_bH + tlv_tauH ;
      TLorentzVector tlv_HH_raw = tlv_bH_raw + tlv_tauH ;
      theSmallTree.m_HH_pt = tlv_HH.Pt () ;
      theSmallTree.m_HH_eta = tlv_HH.Eta () ;
      theSmallTree.m_HH_phi = tlv_HH.Phi () ;
      theSmallTree.m_HH_e = tlv_HH.E () ;
      theSmallTree.m_HH_mass = tlv_HH.M () ;
      theSmallTree.m_HH_mass_raw = tlv_HH_raw.M () ;
      // in case the SVFIT mass is calculated
      if (theBigTree.SVfitMass->at (chosenTauPair) > -900.)
      {
        TLorentzVector tlv_HHsvfit  = tlv_bH + tlv_tauH_SVFIT ;
        theSmallTree.m_HHsvfit_pt   = tlv_HHsvfit.Pt () ;
        theSmallTree.m_HHsvfit_eta  = tlv_HHsvfit.Eta () ;
        theSmallTree.m_HHsvfit_phi  = tlv_HHsvfit.Phi () ;
        theSmallTree.m_HHsvfit_e    = tlv_HHsvfit.E () ;
        theSmallTree.m_HHsvfit_mass = tlv_HHsvfit.M () ;
      } // in case the SVFIT mass is calculated

      // compute HHKinFit -- ask a reasonable mass window to suppress most error messages
      bool wrongHHK=false;
      float HHKmass = -999;
      float HHKChi2 = -999;
      // if (runHHKinFit && tlv_HH_raw.M() > 20 && tlv_HH_raw.M() < 200)
      if (runHHKinFit)
      {
        HHKinFit2::HHKinFitMasterHeavyHiggs kinFits = HHKinFit2::HHKinFitMasterHeavyHiggs ( tlv_firstBjet, tlv_secondBjet, 
                                                   tlv_firstLepton, tlv_secondLepton,  ptmiss, stableMetCov) ;
        HHKinFit2::HHKinFitMasterHeavyHiggs kinFitsraw = HHKinFit2::HHKinFitMasterHeavyHiggs ( tlv_firstBjet_raw, tlv_secondBjet_raw, 
                                                   tlv_firstLepton, tlv_secondLepton,  ptmiss, stableMetCov) ;
//           kinFits.setAdvancedBalance (&ptmiss, metcov) ;
//           kinFits.setSimpleBalance (ptmiss.Pt (),10) ; //alternative which uses only the absolute value of ptmiss in the fit
// 
//           kinFits.addMh1Hypothesis (hypo_mh1) ;
//           kinFits.addMh2Hypothesis (hypo_mh2) ;
        kinFits.   addHypo(hypo_mh1,hypo_mh2);
        kinFitsraw.addHypo(hypo_mh1,hypo_mh2);
        try{           
            kinFits.fit();//doFit () ; 
        }
        catch(HHKinFit2::HHInvMConstraintException e){
          cout<<"INVME THIS EVENT WAS WRONG, INV MASS CONSTRAIN EXCEPTION"<<endl;
          cout<<"INVME masshypo1 = 125,    masshypo2 = 125"<<endl;
          cout<<"INVME Tau1"<<endl;
          cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_firstLepton.E()<<","<<tlv_firstLepton.Px()<<","<<tlv_firstLepton.Py()<<","<<tlv_firstLepton.Pz()<<","<<tlv_firstLepton.M()<<endl;//tlv_firstLepton.Print();
          cout<<"INVME Tau2"<<endl;
          cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_secondLepton.E()<<","<<tlv_secondLepton.Px()<<","<<tlv_secondLepton.Py()<<","<<tlv_secondLepton.Pz()<<","<<tlv_secondLepton.M()<<endl;
          cout<<"INVME B1"<<endl;
          cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_firstBjet.E()<<","<<tlv_firstBjet.Px()<<","<<tlv_firstBjet.Py()<<","<<tlv_firstBjet.Pz()<<","<<tlv_firstBjet.M()<<endl;
          cout<<"INVME B2"<<endl;
          cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_secondBjet.E()<<","<<tlv_secondBjet.Px()<<","<<tlv_secondBjet.Py()<<","<<tlv_secondBjet.Pz()<<","<<tlv_secondBjet.M()<<endl;
          cout<<"INVME MET"<<endl;
          cout<<"INVME (E,Px,Py,Pz,M) "<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<endl;
          cout<<"INVME METCOV "<<endl;
          cout<<"INVME "<<metcov (0,0)<<"  "<<metcov (0,1)<<endl;// = theBigTree.MET_cov00->at (chosenTauPair) ;
          cout<<"INVME "<<metcov (1,0)<<"  "<<metcov (1,1)<<endl;// = theBigTree.MET_cov10->at (chosenTauPair) ;
          cout<<"INVME tau1, tau2, b1, b2"<<endl;
          cout<<"INVME ";
          tlv_firstLepton.Print();
          cout<<"INVME ";
          tlv_secondLepton.Print();
          cout<<"INVME ";
          tlv_firstBjet.Print();
          cout<<"INVME ";
          tlv_secondBjet.Print();
          wrongHHK=true;
          }
        catch (HHKinFit2::HHEnergyRangeException e){
          cout<<"ERANGE THIS EVENT WAS WRONG, ENERGY RANGE EXCEPTION"<<endl;
          cout<<"ERANGE masshypo1 = 125,    masshypo2 = 125"<<endl;
          cout<<"ERANGE Tau1"<<endl;
          cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_firstLepton.E()<<","<<tlv_firstLepton.Px()<<","<<tlv_firstLepton.Py()<<","<<tlv_firstLepton.Pz()<<","<<tlv_firstLepton.M()<<endl;//tlv_firstLepton.Print();
          cout<<"ERANGE Tau2"<<endl;
          cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_secondLepton.E()<<","<<tlv_secondLepton.Px()<<","<<tlv_secondLepton.Py()<<","<<tlv_secondLepton.Pz()<<","<<tlv_secondLepton.M()<<endl;
          cout<<"ERANGE B1"<<endl;
          cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_firstBjet.E()<<","<<tlv_firstBjet.Px()<<","<<tlv_firstBjet.Py()<<","<<tlv_firstBjet.Pz()<<","<<tlv_firstBjet.M()<<endl;
          cout<<"ERANGE B2"<<endl;
          cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_secondBjet.E()<<","<<tlv_secondBjet.Px()<<","<<tlv_secondBjet.Py()<<","<<tlv_secondBjet.Pz()<<","<<tlv_secondBjet.M()<<endl;
          cout<<"ERANGE MET"<<endl;
          cout<<"ERANGE (E,Px,Py,Pz,M) "<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<endl;
          cout<<"ERANGE METCOV "<<endl;
          cout<<"ERANGE "<<metcov (0,0)<<"  "<<metcov (0,1)<<endl;// = theBigTree.MET_cov00->at (chosenTauPair) ;
          cout<<"ERANGE "<<metcov (1,0)<<"  "<<metcov (1,1)<<endl;// = theBigTree.MET_cov10->at (chosenTauPair) ;
          cout<<"ERANGE tau1, tau2, b1, b2"<<endl;
          cout<<"ERANGE ";
          tlv_firstLepton.Print();
          cout<<"ERANGE ";
          tlv_secondLepton.Print();
          cout<<"ERANGE ";
          tlv_firstBjet.Print();
          cout<<"ERANGE ";
          tlv_secondBjet.Print();
          wrongHHK=true;
        }
        catch(HHKinFit2::HHEnergyConstraintException e){
          cout<<"ECON THIS EVENT WAS WRONG, ENERGY CONSTRAIN EXCEPTION"<<endl;
          cout<<"ECON masshypo1 = 125,    masshypo2 = 125"<<endl;
          cout<<"ECON Tau1"<<endl;
          cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_firstLepton.E()<<","<<tlv_firstLepton.Px()<<","<<tlv_firstLepton.Py()<<","<<tlv_firstLepton.Pz()<<","<<tlv_firstLepton.M()<<endl;//tlv_firstLepton.Print();
          cout<<"ECON Tau2"<<endl;
          cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_secondLepton.E()<<","<<tlv_secondLepton.Px()<<","<<tlv_secondLepton.Py()<<","<<tlv_secondLepton.Pz()<<","<<tlv_secondLepton.M()<<endl;
          cout<<"ECON B1"<<endl;
          cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_firstBjet.E()<<","<<tlv_firstBjet.Px()<<","<<tlv_firstBjet.Py()<<","<<tlv_firstBjet.Pz()<<","<<tlv_firstBjet.M()<<endl;
          cout<<"ECON B2"<<endl;
          cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_secondBjet.E()<<","<<tlv_secondBjet.Px()<<","<<tlv_secondBjet.Py()<<","<<tlv_secondBjet.Pz()<<","<<tlv_secondBjet.M()<<endl;
          cout<<"ECON MET"<<endl;
          cout<<"ECON (E,Px,Py,Pz,M) "<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<endl;
          cout<<"ECON METCOV "<<endl;
          cout<<"ECON "<<metcov (0,0)<<"  "<<metcov (0,1)<<endl;// = theBigTree.MET_cov00->at (chosenTauPair) ;
          cout<<"ECON "<<metcov (1,0)<<"  "<<metcov (1,1)<<endl;// = theBigTree.MET_cov10->at (chosenTauPair) ;
          cout<<"ECON tau1, tau2, b1, b2"<<endl;
          cout<<"ECON ";
          tlv_firstLepton.Print();
          cout<<"ECON ";
          tlv_secondLepton.Print();
          cout<<"ECON ";
          tlv_firstBjet.Print();
          cout<<"ECON ";
          tlv_secondBjet.Print();
          wrongHHK=true;
          }
        if(!wrongHHK){
          HHKmass = kinFits.getMH () ;
          HHKChi2 = kinFits.getChi2 () ;
        }else{
          if(isOS)HHKmass = -333;
        }
        bool wrongHHKraw =false;
        try {
          kinFitsraw.fit();
        }
        catch(HHKinFit2::HHInvMConstraintException e){wrongHHKraw=true;}
        catch(HHKinFit2::HHEnergyConstraintException e){wrongHHKraw=true;}
        catch (HHKinFit2::HHEnergyRangeException e){wrongHHKraw=true;}
        if(!wrongHHKraw){
          theSmallTree.m_HHKin_mass_raw = kinFitsraw.getMH();
        }
        else theSmallTree.m_HHKin_mass_raw = -100 ;
        if (theBigTree.SVfitMass->at (chosenTauPair) > -900. && !wrongHHK)
        {
          TLorentzVector b1 = kinFits.getFittedBJet1();
          TLorentzVector b2 = kinFits.getFittedBJet2();
          TLorentzVector bH_HKin = b1 + b2;
          TLorentzVector tlv_HHsvfit = bH_HKin + tlv_tauH_SVFIT ;

          theSmallTree.m_HHkinsvfit_bHmass = bH_HKin.M();
          theSmallTree.m_HHkinsvfit_pt  = tlv_HHsvfit.Pt () ;
          theSmallTree.m_HHkinsvfit_eta = tlv_HHsvfit.Eta () ;
          theSmallTree.m_HHkinsvfit_phi = tlv_HHsvfit.Phi () ;
          theSmallTree.m_HHkinsvfit_e   = tlv_HHsvfit.E () ;
          theSmallTree.m_HHkinsvfit_m   = tlv_HHsvfit.M () ;
        } // in case the SVFIT mass is calculated
      } // end if doing HHKinFit

      theSmallTree.m_HHKin_mass_raw_copy = theSmallTree.m_HHKin_mass_raw ; // store twice if different binning needed

      theSmallTree.m_HHKin_mass = HHKmass;//kinFits.getMH () ;
      theSmallTree.m_HHKin_chi2 = HHKChi2;//kinFits.getChi2 () ;

      theSmallTree.m_HH_deltaPhi = deltaPhi (tlv_bH.Phi (), tlv_tauH.Phi ()) ;
      theSmallTree.m_tauHMet_deltaPhi = deltaPhi (theBigTree.metphi, tlv_tauH.Phi ()) ;
      theSmallTree.m_bHMet_deltaPhi = deltaPhi (theBigTree.metphi, tlv_bH.Phi ()) ;

      theSmallTree.m_ditau_deltaPhi = deltaPhi (tlv_firstLepton.Phi (), tlv_secondLepton.Phi ()) ;
      theSmallTree.m_dib_deltaPhi = deltaPhi (tlv_firstBjet.Phi (), tlv_secondBjet.Phi ()) ;

      theSmallTree.m_ditau_deltaR = tlv_firstLepton.DeltaR(tlv_secondLepton) ;
      theSmallTree.m_dib_deltaR = tlv_firstBjet.DeltaR(tlv_secondBjet) ;

      vector <float> dRBTau;
      dRBTau.push_back (tlv_firstLepton.DeltaR(tlv_firstBjet));
      dRBTau.push_back (tlv_firstLepton.DeltaR(tlv_secondBjet));
      dRBTau.push_back (tlv_secondLepton.DeltaR(tlv_firstBjet));
      dRBTau.push_back (tlv_secondLepton.DeltaR(tlv_secondBjet));
      theSmallTree.m_btau_deltaRmin = *std::min_element(dRBTau.begin(), dRBTau.end());
      theSmallTree.m_btau_deltaRmax = *std::max_element(dRBTau.begin(), dRBTau.end());

      // loop over jets
      for (unsigned int iJet = 0 ; 
           (iJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < maxNjetsSaved) ;
           ++iJet)
        {
          // PG filter jets at will
          if (theBigTree.jets_PUJetID->at (iJet) < PUjetID_minCut) continue ;
      
          // skip the H decay candiates
          if (int (iJet) == bjet1idx ){
            theSmallTree.m_bjet1_jecUnc = theBigTree.jets_jecUnc->at(iJet);
            continue;
          }else if(int (iJet) == bjet2idx){
            theSmallTree.m_bjet2_jecUnc = theBigTree.jets_jecUnc->at(iJet);
            continue ;
          }
          TLorentzVector tlv_dummyJet (
              theBigTree.jets_px->at (iJet),
              theBigTree.jets_py->at (iJet),
              theBigTree.jets_pz->at (iJet),
              theBigTree.jets_e->at (iJet)
            ) ;

          // remove jets that overlap with the tau selected in the leg 1 and 2
          if (type1 == 2) {
            if (tlv_firstLepton.DeltaR(tlv_dummyJet) < 0.5){
              theSmallTree.m_dau1_jecUnc = theBigTree.jets_jecUnc->at(iJet);
              continue;
            }
          }
          if (type2 == 2) {
            if (tlv_secondLepton.DeltaR(tlv_dummyJet) < 0.5){
              theSmallTree.m_dau2_jecUnc = theBigTree.jets_jecUnc->at(iJet);
              continue;
            }
          }

          // find matching gen jet
          bool hasgj = false;
          if (isMC)
          {
            int mcind = theBigTree.jets_genjetIndex->at(iJet);
            if (mcind>=0)
            {
              TLorentzVector thisGenJet(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
              if (thisGenJet.Pt() > 8) hasgj = true;
            }     
          }

          theSmallTree.m_jets_pt.push_back (tlv_dummyJet.Pt ()) ;
          theSmallTree.m_jets_eta.push_back (tlv_dummyJet.Eta ()) ;
          theSmallTree.m_jets_phi.push_back (tlv_dummyJet.Phi ()) ;
          theSmallTree.m_jets_e.push_back (theBigTree.jets_e->at (iJet)) ;
          theSmallTree.m_jets_btag.push_back (theBigTree.bCSVscore->at (iJet)) ;
          theSmallTree.m_jets_flav.push_back (theBigTree.jets_HadronFlavour->at (iJet)) ;
          theSmallTree.m_jets_jecUnc.push_back (theBigTree.jets_jecUnc->at (iJet)) ;
          theSmallTree.m_jets_hasgenjet.push_back (hasgj) ;
          ++theSmallTree.m_njets ;
        } // loop over jets

        theSmallTree.m_nfatjets = theBigTree.ak8jets_px->size();
        theSmallTree.m_isBoosted = 0;
        if (theBigTree.ak8jets_px->size() > 0)
        {
            // int idxSub1 = -1;
            // int idxSub2 = -1;
            // int idxFatj = -1;

            vector<pair<float, int>> fatjets_bTag;
            for (unsigned int ifj = 0; ifj < theBigTree.ak8jets_px->size(); ++ifj)
            {
              TLorentzVector tlv_fj (theBigTree.ak8jets_px->at(ifj) , theBigTree.ak8jets_py->at(ifj) , theBigTree.ak8jets_pz->at(ifj) , theBigTree.ak8jets_e->at(ifj));
              if (theBigTree.ak8jets_SoftDropMass -> at(ifj) < 30) continue;
              if ( theBigTree.ak8jets_nsubjets->at(ifj) < 2 ) continue;
              
              TLorentzVector tlv_subj1;
              TLorentzVector tlv_subj2;
              vector<int> sjIdxs = findSubjetIdxs(ifj, theBigTree);

              int nSJ = 0;
              for (int isj : sjIdxs)
              {
                  ++nSJ;
                  if (nSJ > 2) break; // FIXME: storing first two <--> highest pt, order subjets for b tag?
                  if (nSJ == 1)
                  {
                      tlv_subj1.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
                  }
                  if (nSJ == 2)
                  {
                      tlv_subj2.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
                  }
              }

              bool A1B2 = (tlv_subj1.DeltaR(tlv_firstBjet) < 0.4)   && (tlv_subj2.DeltaR(tlv_secondBjet) < 0.4 );
              bool A2B1 = (tlv_subj1.DeltaR(tlv_secondBjet) < 0.4)  && (tlv_subj2.DeltaR(tlv_firstBjet) < 0.4 );
              if (!A1B2 && !A2B1) continue; // is not matched to resolved jets

              fatjets_bTag.push_back(make_pair(theBigTree.ak8jets_CSV->size(), ifj));
            }
            if (fatjets_bTag.size() != 0) 
            {
              theSmallTree.m_isBoosted = 1;
              sort (fatjets_bTag.begin(), fatjets_bTag.end());
              int fjIdx = fatjets_bTag.back().second;
              TLorentzVector tlv_fj (theBigTree.ak8jets_px->at(fjIdx) , theBigTree.ak8jets_py->at(fjIdx) , theBigTree.ak8jets_pz->at(fjIdx) , theBigTree.ak8jets_e->at(fjIdx));
              theSmallTree.m_fatjet_pt   = tlv_fj.Pt();
              theSmallTree.m_fatjet_eta  = tlv_fj.Eta();
              theSmallTree.m_fatjet_phi  = tlv_fj.Phi();
              theSmallTree.m_fatjet_e    = tlv_fj.E();
              theSmallTree.m_fatjet_bID  = theBigTree.ak8jets_CSV->at(fjIdx);
              theSmallTree.m_fatjet_filteredMass = theBigTree.ak8jets_FilteredMass -> at(fjIdx) ;
              theSmallTree.m_fatjet_prunedMass   = theBigTree.ak8jets_PrunedMass   -> at(fjIdx) ;
              theSmallTree.m_fatjet_trimmedMass  = theBigTree.ak8jets_TrimmedMass  -> at(fjIdx) ;
              theSmallTree.m_fatjet_softdropMass = theBigTree.ak8jets_SoftDropMass -> at(fjIdx) ;
              theSmallTree.m_fatjet_tau1 = theBigTree.ak8jets_tau1->at(fjIdx);
              theSmallTree.m_fatjet_tau2 = theBigTree.ak8jets_tau2->at(fjIdx);
              theSmallTree.m_fatjet_tau3 = theBigTree.ak8jets_tau3->at(fjIdx);
              theSmallTree.m_fatjet_nsubjets = theBigTree.ak8jets_nsubjets->at(fjIdx);
              // FIXME: redoing this a second time, can be optimized
              if ( theBigTree.ak8jets_nsubjets->at(fjIdx) < 2) cout << "ERROR: there are not 2 subjets. Should not happen!!" << endl;
              TLorentzVector tlv_subj1;
              TLorentzVector tlv_subj2;
              vector<int> sjIdxs = findSubjetIdxs(fjIdx, theBigTree);
              int nSJ = 0;
              for (int isj : sjIdxs)
              {
                  ++nSJ;
                  if (nSJ > 2) break; // FIXME: storing first two <--> highest pt, order subjets for b tag?
                  if (nSJ == 1)
                  {
                      tlv_subj1.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
                      theSmallTree.m_subjetjet1_pt   = tlv_subj1.Pt();
                      theSmallTree.m_subjetjet1_eta  = tlv_subj1.Eta();
                      theSmallTree.m_subjetjet1_phi  = tlv_subj1.Phi();
                      theSmallTree.m_subjetjet1_e    = tlv_subj1.E();
                      theSmallTree.m_subjetjet1_bID  = theBigTree.subjets_CSV->at(isj) ;
                  }
                  if (nSJ == 2)
                  {
                      tlv_subj2.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
                      theSmallTree.m_subjetjet2_pt   = tlv_subj2.Pt();
                      theSmallTree.m_subjetjet2_eta  = tlv_subj2.Eta();
                      theSmallTree.m_subjetjet2_phi  = tlv_subj2.Phi();
                      theSmallTree.m_subjetjet2_e    = tlv_subj2.E();
                      theSmallTree.m_subjetjet2_bID  = theBigTree.subjets_CSV->at(isj) ;
                  }
                  theSmallTree.m_dR_subj1_subj2 = tlv_subj1.DeltaR(tlv_subj2);
              } 
            }
        }
    } // if there's two jets in the event, at least
  
    if (isMC) selectedEvents += theBigTree.aMCatNLOweight ;  //FIXME: probably wrong, but unused up to now
    else selectedEvents += 1 ;
    ++selectedNoWeightsEventsNum ;

    theSmallTree.Fill () ;
  }

  cout << "1: " << totalEvents << endl ;
  cout << "2: " << selectedEvents << endl ;
  cout << "3: " << totalNoWeightsEventsNum << endl ;
  cout << "4: " << selectedNoWeightsEventsNum << endl ;

  if (totalEvents != 0) cout << "efficiency = " << selectedEvents / totalEvents << endl ;
  else                  cout << "NO events found\n" ;
  TH1F h_eff ("h_eff", "h_eff", 4 , 0, 4) ;
  h_eff.SetBinContent (1, totalEvents) ;
  h_eff.SetBinContent (2, selectedEvents) ;
  h_eff.SetBinContent (3, totalNoWeightsEventsNum) ;
  h_eff.SetBinContent (4, selectedNoWeightsEventsNum) ;
  
  // for (unsigned int i = 0 ; i < counter.size () ; ++i)
  //   h_eff.SetBinContent (5 + i, counter.at (i)) ;

  smallFile->cd() ;
  h_eff.Write () ;
  smallFile->Write () ;
  smallFile->Close () ;

  bool computeMVA = gConfigParser->readBoolOption ("TMVA::computeMVA");
  
  if (computeMVA)
  {  
    bool doMuTau  = gConfigParser->isDefined("TMVA::weightsMuTau");
    bool doETau   = gConfigParser->isDefined("TMVA::weightsETau");
    bool doTauTau = gConfigParser->isDefined("TMVA::weightsTauTau");
    bool doLepTau = gConfigParser->isDefined("TMVA::weightsLepTau");

    string TMVAweightsTauTau  = "";
    string TMVAweightsMuTau   = "";
    string TMVAweightsETau    = "";
    string TMVAweightsLepTau    = "";
    
    if (doMuTau)  TMVAweightsMuTau  = gConfigParser->readStringOption ("TMVA::weightsMuTau");
    if (doETau)   TMVAweightsETau   = gConfigParser->readStringOption ("TMVA::weightsETau");
    if (doTauTau) TMVAweightsTauTau = gConfigParser->readStringOption ("TMVA::weightsTauTau");
    if (doLepTau) TMVAweightsLepTau = gConfigParser->readStringOption ("TMVA::weightsLepTau");

    bool TMVAspectatorsIn      = gConfigParser->readBoolOption   ("TMVA::spectatorsPresent");
    vector<string> TMVAspectators = gConfigParser->readStringListOption   ("TMVA::spectators");
    vector<string> TMVAvariables  = gConfigParser->readStringListOption   ("TMVA::variables");

    TFile *outFile = TFile::Open(outputFile,"UPDATE");
    TTree *treenew = (TTree*)outFile->Get("HTauTauTree");

    TMVA::Reader * reader = new TMVA::Reader () ;
    Float_t mvatautau,mvamutau, mvaetau, mvaleptau;
    TBranch *mvaBranchmutau;
    TBranch *mvaBranchtautau;
    TBranch *mvaBranchetau;
    TBranch *mvaBranchleptau;

    vector<float> address (TMVAvariables.size () + TMVAspectators.size () * TMVAspectatorsIn, 0.) ; 
    for (unsigned int iv = 0 ; iv < TMVAvariables.size () ; ++iv)
    {
      treenew->SetBranchAddress (TMVAvariables.at (iv).c_str (), &(address.at (iv))) ;
      reader->AddVariable (TMVAvariables.at (iv), &(address.at (iv))) ;
    }  

    for (unsigned int iv = 0 ; iv < TMVAspectators.size () && TMVAspectatorsIn ; ++iv)
    {
      int addressIndex = iv + TMVAvariables.size () ;
      treenew->SetBranchAddress (TMVAspectators.at (iv).c_str (), &(address.at (addressIndex))) ;
      reader->AddSpectator (TMVAspectators.at (iv), &(address.at (addressIndex))) ;
    }  

    //if (treenew->GetListOfBranches ()->FindObject (mvaName.c_str ())) {
    //  treenew->SetBranchAddress ("MuTauKine", &mvamutau, &mvaBranchmutau) ;
    //  treenew->SetBranchAddress ("TauTauKine", &mvatautau, &mvaBranchtautau) ;
    //}
    //else{   

    if (doMuTau)  mvaBranchmutau = treenew->Branch ("MuTauKine", &mvamutau, "MuTauKine/F") ;
    if (doETau)   mvaBranchetau = treenew->Branch ("ETauKine", &mvaetau, "ETauKine/F") ;
    if (doTauTau) mvaBranchtautau = treenew->Branch ("TauTauKine", &mvatautau, "TauTauKine/F") ;
    if (doLepTau) mvaBranchleptau = treenew->Branch ("LepTauKine", &mvaleptau, "LepTauKine/F") ;
    //}
    if (doMuTau)   reader->BookMVA ("MuTauKine",  TMVAweightsMuTau.c_str ()) ;
    if (doETau)    reader->BookMVA ("ETauKine",  TMVAweightsETau.c_str ()) ;
    if (doTauTau)  reader->BookMVA ("TauTauKine",  TMVAweightsTauTau.c_str ()) ;
    if (doLepTau)  reader->BookMVA ("LepTauKine",  TMVAweightsLepTau.c_str ()) ;

    int nentries = treenew->GetEntries();
    for(int i=0;i<nentries;i++){
      treenew->GetEntry(i);

      if (doMuTau)   mvamutau= reader->EvaluateMVA ("MuTauKine") ;  
      if (doETau)    mvaetau= reader->EvaluateMVA ("ETauKine") ;  
      if (doTauTau)  mvatautau= reader->EvaluateMVA ("TauTauKine") ;  
      if (doLepTau)  mvaleptau= reader->EvaluateMVA ("LepTauKine") ;  

      if (doMuTau)    mvaBranchmutau->Fill();
      if (doETau)     mvaBranchetau->Fill();
      if (doTauTau)   mvaBranchtautau->Fill();
      if (doLepTau)   mvaBranchleptau->Fill();
    }

    outFile->cd () ;
    h_eff.Write () ;
    treenew->Write ("", TObject::kOverwrite) ;

    delete reader;
  }

  cout << "... SKIM finished, exiting." << endl;
  return 0 ;
}
