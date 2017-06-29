#include "HHReweight.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <utility>

#include "TH2D.h"
#include "TMath.h"

using namespace std;

#define DEBUG false

HHReweight::HHReweight(std::string coeffFile, const TH2* hInput)
{
    readInputFile(coeffFile); // initialize the reweight parameters from the txt input

    // clone the input histogram
    TH2* cloneH = (TH2*) hInput->Clone("h_input");
    if (!CheckConsistency(cloneH, h_A1_.get()))
    {
        cerr << " ** Error : the input histogram to HHReweight is not compatible with the reweight file, did you use the correct binning?" << endl;
        throw std::runtime_error("Histograms inconsistency");
    }
    h_input_.reset(cloneH);

    // set default values
    A1_13TeV_ = 2.09078;
    A3_13TeV_ = 0.282307;
    A7_13TeV_ = -1.37309;
}

HHReweight::~HHReweight()
{}

// return the weight to be applied for the reweight
// NOTE: normalization is arbitrary you'll have to scale by the sum of weights
double HHReweight::getWeight(double kl, double kt, double mhh, double cth)
{
    pair<int,int> bins = find2DBin(h_input_.get(), mhh, cth);
    double denom = h_input_->GetBinContent(bins.first, bins.second);
    if (denom == 0)
        return 0;
    double A1 = h_A1_->GetBinContent(bins.first, bins.second);
    double A3 = h_A3_->GetBinContent(bins.first, bins.second);
    double A7 = h_A7_->GetBinContent(bins.first, bins.second);
    double nEvSM = h_SM_->GetBinContent(bins.first, bins.second);
    double effBSM = nEvSM * functionGF(kl,kt,0,0,0,A1,A3,A7)/functionGF(kl,kt,0,0,0,A1_13TeV_,A3_13TeV_,A7_13TeV_);
    return (effBSM/denom) ;
}

void HHReweight::readInputFile(std::string coeffFile)
{
    if (DEBUG) cout << " -- Reading input file" << coeffFile << endl;

    // create histograms to be filled from file
    // this is the binning of input file histogram
    // NOTE: code can be made more flexible to have binning inferred directly from the input file
    double binning_mHH   [14] = {250.,270.,300.,330.,360.,390., 420.,450.,500.,550.,600.,700.,800.,1000.} ;
    double binning_cth    [4] = {-1., -0.55,0.55,1. } ;
    int nbins_mHH = 13; // size of arrays - 1
    int nbins_cth = 3;  // size of arrays - 1
    h_A1_    = std::make_shared<TH2D> ("h_A1", "h_A1",       nbins_mHH, binning_mHH, nbins_cth, binning_cth );
    h_A3_    = std::make_shared<TH2D> ("h_A3", "h_A3",       nbins_mHH, binning_mHH, nbins_cth, binning_cth );
    h_A7_    = std::make_shared<TH2D> ("h_A7", "h_A7",       nbins_mHH, binning_mHH, nbins_cth, binning_cth );
    h_SM_    = std::make_shared<TH2D> ("h_SM", "h_SM",       nbins_mHH, binning_mHH, nbins_cth, binning_cth );
    h_sumv1_ = std::make_shared<TH2D> ("h_sumv1", "h_sumv1", nbins_mHH, binning_mHH, nbins_cth, binning_cth );

    if (DEBUG) cout << " -- Histograms done" << endl;


    // read and fill from the file
    std::ifstream infile;
    infile.open(coeffFile);
    if (!infile.is_open())
        throw std::runtime_error("Could not open input file");

    std::string line;
    while (std::getline(infile, line))
    {
        if (DEBUG) cout << " -- Reading line " << line << endl;
        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        if (!line.empty())
        {
            vector<string> tokens = tokenize(line);
            if (tokens.size() != 11)
            {
                cerr << " ** Error in reading input file: cannot interpret line: " << line << endl;
                throw std::runtime_error("Cannot parse input file");
            }
            //The columns are respectively: nbins GenMhh GenCostStar NenventsSM NenventsSumV1 A1 A3 A7 errorA1 errorA3 errorA7
            double mHH = std::stod(tokens.at(1));
            double cth = std::stod(tokens.at(2));
            int ibin = h_A1_->FindBin(mHH, cth);

            h_A1_->SetBinContent(ibin, std::stod(tokens.at(5)) );
            h_A3_->SetBinContent(ibin, std::stod(tokens.at(6)) );
            h_A7_->SetBinContent(ibin, std::stod(tokens.at(7)) );
            h_SM_->SetBinContent(ibin, std::stod(tokens.at(3)));
            h_sumv1_->SetBinContent(ibin, std::stod(tokens.at(4)));

            h_A1_->SetBinError(ibin, std::stod(tokens.at(8)) );
            h_A3_->SetBinError(ibin, std::stod(tokens.at(9)) );
            h_A7_->SetBinError(ibin, std::stod(tokens.at(10)) );

            if (DEBUG)
            {
                cout << " -- I'll store a file with the histograms" << endl;
                TFile* fOut = TFile::Open("HHReweight_histograms.root", "recreate");
                fOut->cd();
                h_A1_->Write();
                h_A3_->Write();
                h_A7_->Write();
                h_SM_->Write();
                h_sumv1_->Write();
                fOut->Close();
            }
        }
    }
}

// split a string on whitespaces, return elements
vector<std::string> HHReweight::tokenize(std::string input)
{    
    if (DEBUG) cout << " -- Tokenizing input " << input << endl;

    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), std::istream_iterator<std::string>());

    if (DEBUG){
        cout << "I got " << ret.size() << " tokens" << endl;
        cout << "     --> " ;
        for (string x : ret) cout << ":" << x << ": ";
        cout << endl;
    }

    return ret;
}

// return bin in 2D isto wihtout under/over flow (e.g. if ibin > ibinmax , ibin = ibinmax)
pair<int,int> HHReweight::find2DBin(TH2* h, double x, double y)
{
    int ibinx = h->GetXaxis()->FindBin(x);
    int ibiny = h->GetYaxis()->FindBin(y);

    if (ibinx <= 0) ibinx = 1;
    if (ibinx > h->GetNbinsX()) ibinx = h->GetNbinsX();

    if (ibiny <= 0) ibiny = 1;
    if (ibiny > h->GetNbinsY()) ibiny = h->GetNbinsY();

    return make_pair(ibinx, ibiny);

}

double HHReweight::functionGF(double kl, double kt, double c2, double cg, double c2g, double A1, double A3, double A7)
{
    // this can be extended to 5D coefficients; currently c2, cg, c2g are unused
    return ( A1*pow(kt,4) + A3*pow(kt,2)*pow(kl,2) + A7*kl*pow(kt,3) );
}




//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// the following is adapted from ROOT source - because yeah, they are protected TH1 members...


bool HHReweight::CheckConsistency(const TH1* h1, const TH1* h2)
{
   if (h1 == h2) return true;

   if (h1->GetDimension() != h2->GetDimension() ) {
      // throw DifferentDimension();
      return false;
   }
   Int_t dim = h1->GetDimension();

   // returns kTRUE if number of bins and bin limits are identical
   Int_t nbinsx = h1->GetNbinsX();
   Int_t nbinsy = h1->GetNbinsY();
   Int_t nbinsz = h1->GetNbinsZ();

   // Check whether the histograms have the same number of bins.
   if (nbinsx != h2->GetNbinsX() ||
       (dim > 1 && nbinsy != h2->GetNbinsY())  ||
       (dim > 2 && nbinsz != h2->GetNbinsZ()) ) {
      // throw DifferentNumberOfBins();
      return false;
   }

   bool ret = true;

   // check axis limits
   ret &= CheckAxisLimits(h1->GetXaxis(), h2->GetXaxis());
   if (dim > 1) ret &= CheckAxisLimits(h1->GetYaxis(), h2->GetYaxis());
   if (dim > 2) ret &= CheckAxisLimits(h1->GetZaxis(), h2->GetZaxis());

   // check bin limits
   ret &= CheckBinLimits(h1->GetXaxis(), h2->GetXaxis());
   if (dim > 1) ret &= CheckBinLimits(h1->GetYaxis(), h2->GetYaxis());
   if (dim > 2) ret &= CheckBinLimits(h1->GetZaxis(), h2->GetZaxis());

   // check labels if histograms are both not empty
   // if ( (h1->fTsumw != 0 || h1->GetEntries() != 0) &&
   //      (h2->fTsumw != 0 || h2->GetEntries() != 0) ) {
   //    ret &= CheckBinLabels(h1->GetXaxis(), h2->GetXaxis());
   //    if (dim > 1) ret &= CheckBinLabels(h1->GetYaxis(), h2->GetYaxis());
   //    if (dim > 2) ret &= CheckBinLabels(h1->GetZaxis(), h2->GetZaxis());
   // }

   return ret;
}

bool HHReweight::CheckAxisLimits(const TAxis *a1, const TAxis *a2 )
{
   if ( ! TMath::AreEqualRel(a1->GetXmin(), a2->GetXmin(),1.E-12) ||
        ! TMath::AreEqualRel(a1->GetXmax(), a2->GetXmax(),1.E-12) ) {
      // throw DifferentAxisLimits();
      return false;
   }
   return true;
}

bool HHReweight::CheckBinLimits(const TAxis* a1, const TAxis * a2)
{
   const TArrayD * h1Array = a1->GetXbins();
   const TArrayD * h2Array = a2->GetXbins();
   Int_t fN = h1Array->fN;
   if ( fN != 0 ) {
      if ( h2Array->fN != fN ) {
         // throw DifferentBinLimits();
         return false;
      }
      else {
         for ( int i = 0; i < fN; ++i ) {
            if ( ! TMath::AreEqualRel( h1Array->GetAt(i), h2Array->GetAt(i), 1E-10 ) ) {
               // throw DifferentBinLimits();
               return false;
            }
         }
      }
   }

   return true;
}