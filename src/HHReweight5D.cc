#include "HHReweight5D.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <utility>

#include "TH2D.h"
#include "TMath.h"

using namespace std;

#define DEBUG false

HHReweight5D::HHReweight5D(std::string coeffFile, const TH2* hInput, bool useAbsEta)
{
    readInputFile(coeffFile); // initialize the reweight parameters from the txt input

    // clone the input histogram
    TH2* cloneH = (TH2*) hInput->Clone("h_input");
    if (!CheckConsistency(cloneH, h_A_vec_.at(0).get()))
    {
        cerr << " ** Error : the input histogram to HHReweight is not compatible with the reweight file, did you use the correct binning?" << endl;
        throw std::runtime_error("Histograms inconsistency");
    }
    h_input_.reset(cloneH);

    // set default values
    // A1_13TeV_ = 2.09078;
    // A3_13TeV_ = 0.282307;
    // A7_13TeV_ = -1.37309;
    A_13TeV_ = {2.09078, 10.1517, 0.282307, 0.101205, 1.33191, -8.51168, -1.37309, 2.82636, 1.45767, -4.91761, -0.675197, 1.86189, 0.321422, -0.836276, -0.568156};

    useAbsEta_ = useAbsEta;
}

HHReweight5D::~HHReweight5D()
{}

// return the weight to be applied for the reweight
// NOTE: normalization is arbitrary you'll have to scale by the sum of weights
double HHReweight5D::getWeight(double kl, double kt, double c2, double cg, double c2g, double mhh, double cth)
{
    if (useAbsEta_) cth = TMath::Abs(cth);

    pair<int,int> bins = find2DBin(h_input_.get(), mhh, cth);
    double denom = h_input_->GetBinContent(bins.first, bins.second);
    if (denom == 0)
        return 0;
    // double A1 = h_A1_->GetBinContent(bins.first, bins.second);
    // double A3 = h_A3_->GetBinContent(bins.first, bins.second);
    // double A7 = h_A7_->GetBinContent(bins.first, bins.second);

    double nEvSM = h_SM_->GetBinContent(bins.first, bins.second);
    
    std::array<double, NCOEFFSA> Acoeffs;
    for (uint ic = 0; ic < NCOEFFSA; ++ic)
    {
      Acoeffs[ic] = (h_A_vec_.at(ic))->GetBinContent(bins.first, bins.second);
    }
    double effBSM = nEvSM * functionGF(kl,kt,c2,cg,c2g,Acoeffs)/functionGF(kl,kt,c2,cg,c2g,A_13TeV_);

    if (DEBUG && effBSM/denom < 0){
      std::cout << "** HHReweight5D : warning : I am getting a negative weight "
        << "kl, kt, c2, cg, c2g, mhh, cth " << kl << " " << kt << " " << c2 << " " << cg << " " << c2g << " " << mhh << " " << cth << " | vals: "
        << nEvSM << " " << functionGF(kl,kt,c2,cg,c2g,Acoeffs) << " " << functionGF(kl,kt,c2,cg,c2g,A_13TeV_) << " " << denom
        << endl;
    }

    if (effBSM/denom < 0) return 0; // sometimes I get negative coeffs.. this should be a temporary fix!
    return (effBSM/denom) ;
}

double HHReweight5D::getWeight(double kl, double kt, double mhh, double cth)
{
  return getWeight(kl, kt, 0, 0, 0, mhh, cth);
}

void HHReweight5D::readInputFile(std::string coeffFile)
{
    if (DEBUG) cout << " -- Reading input file" << coeffFile << endl;

    // create histograms to be filled from file
    // this is the binning of input file histogram
    // NOTE: code can be made more flexible to have binning inferred directly from the input file
    // double binning_mHH   [14] = {250.,270.,300.,330.,360.,390., 420.,450.,500.,550.,600.,700.,800.,1000.} ;
    // double binning_cth    [4] = {-1., -0.55,0.55,1. } ;
    double binning_mHH [56] = { 250,260,270,280,290,300,310,320,330,340,
                                350,360,370,380,390,400,410,420,430,440, 
                                450,460,470,480,490,
                                500,510,520,530,540,550,600,610,620,630,
                                640,650,660,670,680,690,700,750,800,850,
                                900,950,1000,1100,1200,1300,1400,1500.,1750,2000,50000};
    double binning_cth [5]  = {0.0, 0.4, 0.6, 0.8, 1.0} ;

    // int nbins_mHH = 13; // size of arrays - 1
    // int nbins_cth = 3;  // size of arrays - 1
    int nbins_mHH = 55; // size of arrays - 1
    int nbins_cth = 4;  // size of arrays - 1

    for (uint ic = 0; ic < NCOEFFSA; ++ic)
    {
      string name = "h_A" + std::to_string(ic);
      h_A_vec_.at(ic) = std::make_shared<TH2D> (name.c_str(), name.c_str(), nbins_mHH, binning_mHH, nbins_cth, binning_cth );
    }

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
            if (tokens.size() != 35)
            {
                cerr << " ** Error in reading input file: cannot interpret line: " << line << endl;
                throw std::runtime_error("Cannot parse input file");
            }
            //The columns are respectively: nbins GenMhh GenCostStar NenventsSM NenventsSumV1 A1 A3 A7 errorA1 errorA3 errorA7
            double mHH = std::stod(tokens.at(1));
            double cth = std::stod(tokens.at(2));
            int ibin = h_A_vec_.at(0)->FindBin(mHH, cth);

            h_SM_->SetBinContent(ibin, std::stod(tokens.at(3)));
            h_sumv1_->SetBinContent(ibin, std::stod(tokens.at(4)));

            for (uint ic = 0; ic < NCOEFFSA; ++ic)
            {
              (h_A_vec_.at(ic))->SetBinContent(ibin, std::stod(tokens.at(ic+5)) );
              (h_A_vec_.at(ic))->SetBinError(ibin, std::stod(tokens.at(ic+5+15)) );
            }            

            if (DEBUG)
            {
                cout << " -- I'll store a file with the histograms" << endl;
                TFile* fOut = TFile::Open("HHReweight_histograms.root", "recreate");
                fOut->cd();
                for (uint ic = 0; ic < NCOEFFSA; ++ic)
                {
                  (h_A_vec_.at(ic))->Write();
                }
                // h_A1_->Write();
                // h_A3_->Write();
                // h_A7_->Write();
                h_SM_->Write();
                h_sumv1_->Write();
                fOut->Close();
            }
        }
    }
}

// split a string on whitespaces, return elements
vector<std::string> HHReweight5D::tokenize(std::string input)
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
pair<int,int> HHReweight5D::find2DBin(TH2* h, double x, double y)
{
    int ibinx = h->GetXaxis()->FindBin(x);
    int ibiny = h->GetYaxis()->FindBin(y);

    if (ibinx <= 0) ibinx = 1;
    if (ibinx > h->GetNbinsX()) ibinx = h->GetNbinsX();

    if (ibiny <= 0) ibiny = 1;
    if (ibiny > h->GetNbinsY()) ibiny = h->GetNbinsY();

    return make_pair(ibinx, ibiny);

}

// double HHReweight5D::functionGF(double kl, double kt, double c2, double cg, double c2g, double A1, double A3, double A7)
// {
//     // this can be extended to 5D coefficients; currently c2, cg, c2g are unused
//     return ( A1*pow(kt,4) + A3*pow(kt,2)*pow(kl,2) + A7*kl*pow(kt,3) );
// }

double HHReweight5D::functionGF(double kl, double kt, double c2, double cg, double c2g, std::array<double, NCOEFFSA> const &A)
{
    // this can be extended to 5D coefficients; currently c2, cg, c2g are unused
    // return ( A1*pow(kt,4) + A3*pow(kt,2)*pow(kl,2) + A7*kl*pow(kt,3) );
    return ( A[0]*pow(kt,4) + A[1]*pow(c2,2) + (A[2]*pow(kt,2) + A[3]*pow(cg,2))*pow(kl,2) + A[4]*pow(c2g,2) + ( A[5]*c2 + A[6]*kt*kl )*pow(kt,2) + (A[7]*kt*kl + A[8]*cg*kl )*c2 + A[9]*c2*c2g + (A[10]*cg*kl + A[11]*c2g)*pow(kt,2)+ (A[12]*kl*cg + A[13]*c2g )*kt*kl + A[14]*cg*c2g*kl );
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// the following is adapted from ROOT source - because yeah, they are protected TH1 members...


bool HHReweight5D::CheckConsistency(const TH1* h1, const TH1* h2)
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

bool HHReweight5D::CheckAxisLimits(const TAxis *a1, const TAxis *a2 )
{
   if ( ! TMath::AreEqualRel(a1->GetXmin(), a2->GetXmin(),1.E-12) ||
        ! TMath::AreEqualRel(a1->GetXmax(), a2->GetXmax(),1.E-12) ) {
      // throw DifferentAxisLimits();
      return false;
   }
   return true;
}

bool HHReweight5D::CheckBinLimits(const TAxis* a1, const TAxis * a2)
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