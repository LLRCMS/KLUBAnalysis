#include "HHReweight5D.h"

#define DEBUG true

HHReweight5D::HHReweight5D(std::string coeffFile, const TH2* hInput, std::string EFTBMname, std::string year, bool cms_fake, bool useAbsEta)
{
    // clone the input histogram
    TH2* cloneH = (TH2*) hInput->Clone("h_input");

    h_input_.reset(cloneH);

    A_13TeV_ = {62.5088, 345.604, 9.63451, 4.34841, 39.0143, -268.644, -44.2924, 96.5595, 53.515, -155.793, -23.678, 54.5601, 12.2273, -26.8654, -19.3723, -0.0904439, 0.321092, 0.452381, -0.0190758, -0.607163, 1.27408, 0.364487, -0.499263};
      
    useAbsEta_ = useAbsEta;

    // Return EFT benchmarks definition as https://arxiv.org/pdf/1710.08261.pdf
    if( EFTBMname == "SM" or EFTBMname == "sm" ) EFTBMcouplings_ = {1, 1, 0, 0, 0};
    if( EFTBMname ==  "1" ) EFTBMcouplings_ = {7.5, 1, -1, 0, 0};
    if( EFTBMname ==  "2" ) EFTBMcouplings_ = {1.0, 1.0, 0.5, -0.8, 0.6};
    if( EFTBMname ==  "3" ) EFTBMcouplings_ = {1.0, 1.0, -1.5, 0.0, -0.8};
    if( EFTBMname ==  "4" ) EFTBMcouplings_ = {-3.5, 1.5, -3.0, 0.0, 0.0};
    if( EFTBMname ==  "5" ) EFTBMcouplings_ = {1.0, 1.0, 0.0, 0.8, -1.0};
    if( EFTBMname ==  "6" ) EFTBMcouplings_ = {2.4, 1.0, 0.0, 0.2, -0.2};
    if( EFTBMname ==  "7" ) EFTBMcouplings_ = {5.0, 1.0, 0.0, 0.2, -0.2};
    if( EFTBMname ==  "8" ) EFTBMcouplings_ = {15.0, 1.0, 0.0, -1.0, 1.0};
    if( EFTBMname ==  "9" ) EFTBMcouplings_ = {1.0, 1.0, 1.0, -0.6, 0.6};
    if( EFTBMname ==  "10" ) EFTBMcouplings_ = {10.0, 1.5, -1.0, 0.0, 0.0};
    if( EFTBMname ==  "11" ) EFTBMcouplings_ = {2.4, 1.0, 0.0, 1.0, -1.0};
    if( EFTBMname ==  "12" ) EFTBMcouplings_ = {15.0, 1.0, 1.0, 0.0, 0.0};

    //Jona: what are these things with cms_fake tag??
    if( (year == "2017" or year == "2018") and EFTBMcouplings_.size() and cms_fake ){
      EFTBMcouplings_[4] = 1.0;
      double tr = EFTBMcouplings_[0];
      EFTBMcouplings_[0] = EFTBMcouplings_[1];
      EFTBMcouplings_[1] = tr;
    }
    if( (year == "2016") and EFTBMcouplings_.size() and cms_fake ){
      double tr = EFTBMcouplings_[0];
      EFTBMcouplings_[0] = EFTBMcouplings_[1];
      EFTBMcouplings_[1] = tr;
    }

    // 8a from https://link.springer.com/article/10.1007/JHEP09(2018)057
    if( EFTBMname ==  "8a" ) EFTBMcouplings_ = {1.0, 1.0, 0.5, 0.8/3, 0.0};

    // New benchmarks from https://arxiv.org/pdf/1908.08923.pdf
    if( EFTBMname == "JHEP03(2020)091_1" or EFTBMname == "1b") EFTBMcouplings_ = {  3.94, 0.94, -1./3.,  0.5 * 1.5,  1./3. * (-3.) };
    if( EFTBMname == "JHEP03(2020)091_2" or EFTBMname == "2b") EFTBMcouplings_ = {  6.84, 0.61,  1./3.,  0.0 * 1.5, -1./3. * (-3.) };
    if( EFTBMname == "JHEP03(2020)091_3" or EFTBMname == "3b") EFTBMcouplings_ = {  2.21, 1.05, -1./3.,  0.5 * 1.5,   0.5  * (-3.) };
    if( EFTBMname == "JHEP03(2020)091_4" or EFTBMname == "4b") EFTBMcouplings_ = {  2.79, 0.61,  1./3., -0.5 * 1.5,  1./6. * (-3.) };
    if( EFTBMname == "JHEP03(2020)091_5" or EFTBMname == "5b") EFTBMcouplings_ = {  3.95, 1.17, -1./3., 1./6.* 1.5,  -0.5  * (-3.) };
    if( EFTBMname == "JHEP03(2020)091_6" or EFTBMname == "6b") EFTBMcouplings_ = {  5.68, 0.83,  1./3., -0.5 * 1.5,  1./3. * (-3.) };
    if( EFTBMname == "JHEP03(2020)091_7" or EFTBMname == "7b") EFTBMcouplings_ = { -0.10, 0.94,     1., 1./6.* 1.5, -1./6. * (-3.) };
}

HHReweight5D::HHReweight5D(std::string coeffFile, const TH2* hInput, bool useAbsEta)
{
    // clone the input histogram
    TH2* cloneH = (TH2*) hInput->Clone("h_input");

    h_input_.reset(cloneH);

    A_13TeV_ = {62.5088, 345.604, 9.63451, 4.34841, 39.0143, -268.644, -44.2924, 96.5595, 53.515, -155.793, -23.678, 54.5601, 12.2273, -26.8654, -19.3723, -0.0904439, 0.321092, 0.452381, -0.0190758, -0.607163, 1.27408, 0.364487, -0.499263};
      
    useAbsEta_ = useAbsEta;
    
    EFTBMcouplings_ = {1,1,0,0,0}; // Set to SM values, will be passed to the getWeight method that takes couolings as input
}

HHReweight5D::~HHReweight5D()
{}

// return the weight to be applied for the reweight -> take only mhh and cth from input, retrieve couplings from constructor
double HHReweight5D::getWeight(double mhh, double cth)
{
    if (useAbsEta_) cth = TMath::Abs(cth);

    double kl  = EFTBMcouplings_[0];
    double kt  = EFTBMcouplings_[1];
    double c2  = EFTBMcouplings_[2];
    double cg  = EFTBMcouplings_[3];
    double c2g = EFTBMcouplings_[4];

    double Nevtot = h_input_->Integral();
    double XStot = getTotXS(kl,kt,c2,cg,c2g);
    double Nev = h_input_->GetBinContent( h_input_->FindBin(mhh, cth) );
    double XS = getDiffXS(kl,kt,c2,cg,c2g,mhh,cth,A_map_);
    int ibinmhh = h_input_->GetXaxis()->FindBin(mhh);
    int ibincosthetaHH = h_input_->GetYaxis()->FindBin(cth);
    double Noutputev = XS * h_input_->GetXaxis()->GetBinWidth(ibinmhh) * h_input_->GetYaxis()->GetBinWidth(ibincosthetaHH);
    return Noutputev/Nev * Nevtot/XStot;
}

// return the weight to be applied for the reweight -> take mhh, cth, and couplings from input
double HHReweight5D::getWeight(double kl, double kt, double c2, double cg, double c2g, double mhh, double cth)
{
    if (useAbsEta_) cth = TMath::Abs(cth);

    double Nevtot = h_input_->Integral();
    double XStot = getTotXS(kl,kt,c2,cg,c2g);
    double Nev = h_input_->GetBinContent( h_input_->FindBin(mhh, cth) );
    double XS = getDiffXS(kl,kt,c2,cg,c2g,mhh,cth,A_map_);
    int ibinmhh = h_input_->GetXaxis()->FindBin(mhh);
    int ibincosthetaHH = h_input_->GetYaxis()->FindBin(cth);
    double Noutputev = XS * h_input_->GetXaxis()->GetBinWidth(ibinmhh) * h_input_->GetYaxis()->GetBinWidth(ibincosthetaHH);
    return Noutputev/Nev * Nevtot/XStot;
}


double HHReweight5D::getTotXS(double kl, double kt, double c2, double cg, double c2g)
{
    return functionGF(kl, kt, c2, cg, c2g, A_13TeV_);
}

double HHReweight5D::getDiffXS(double kl, double kt, double c2, double cg, double c2g, double mhh, double cth, std::map< std::pair<double, double>, std::vector<double> > A_map)
{
   double dXsec = 0.;
   
   for(std::map< std::pair<double, double>, std::vector<double> >::iterator it = A_map.begin(); it != A_map.end(); it++)
   {
      std::vector<double> const & values = it->second;
      double mass_bin_end = it->first.first;
      double cos_bin_end  = it->first.second;
      if(mhh > mass_bin_end || cth > cos_bin_end) continue;
      dXsec += functionGF(kl, kt, c2, cg, c2g, values);
   }   
      
   return dXsec;
}

void HHReweight5D::readInputFile(std::string coeffFile)
{
    if (DEBUG) std::cout << " -- Reading input file" << coeffFile << std::endl;

    // create histograms to be filled from file
    // this is the binning of input file histogram

//     double binning_mHH [37] = {250.,270.,290.,310.,330.,
//                                350.,370.,390.,410.,430., 
//                                450.,470.,490.,510.,530.,
//                                550.,570.,590.,610.,630.,
//                                650.,670.,700.,750.,800.,
//                                850.,900.,950.,1000.,1100.,
//                                1200.,1300.,1400.,1500.,1750.,2000.,5000.};
//              
//     double binning_cth [5]  = {0.0, 0.4, 0.6, 0.8, 1.0};
// 
//     int nbins_mHH = 36; // size of arrays - 1
//     int nbins_cth = 4;  // size of arrays - 1

    // read and fill from the file
    std::ifstream infile;
    infile.open(coeffFile);
    if (!infile.is_open())
        throw std::runtime_error("Could not open input file");

    std::string line;
    while (std::getline(infile, line))
    {
        if (DEBUG) std::cout << " -- Reading line " << line << std::endl;
        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        if (!line.empty()) {
            std::vector<std::string> tokens = tokenize(line);
            if (tokens.size() != 28)
            {
                std::cerr << " ** Error in reading input file: cannot interpret line: " << line << std::endl;
                throw std::runtime_error("Cannot parse input file");
            }
            //The columns are respectively: uncertainty Mhh_ll Mhh_ul GenCostStar_ll GenCostStar_ul 23 A values
            double mHH_ll = std::stod(tokens.at(1));
            double cth_ll = std::stod(tokens.at(3));
                        
            auto bin = std::make_pair(mHH_ll, cth_ll);

            std::vector<double> A_vec;
            for (int i = 5; i<=28; i++) {
                A_vec.push_back(std::stod(tokens.at(i)));
            }

            // fill the correct maps with the correct stuff
            if (tokens.at(0) == "") A_map_[bin] = A_vec ;
            else if (tokens.at(0) == "fit_UP") A_map_UP_[bin] = A_vec ;
            else A_map_DN_[bin] = A_vec ;
        }
    }
}

// split a string on comma, return elements
std::vector<std::string> HHReweight5D::tokenize(std::string input)
{    
    if (DEBUG) std::cout << " -- Tokenizing input " << input << std::endl;

    std::istringstream buffer(input);
    std::vector<std::string> ret;
    for (std::string i; buffer >> i;) 
    {
        ret.push_back(i);    
        if (buffer.peek() == ',')
            buffer.ignore();
    }
    
    if (DEBUG){
        std::cout << "I got " << ret.size() << " tokens" << std::endl;
        std::cout << "     --> " ;
        for (std::string x : ret) std::cout << ":" << x << ": ";
        std::cout << std::endl;
    }

    return ret;
}

double HHReweight5D::functionGF(double kl, double kt, double c2, double cg, double c2g, std::vector<double> A)
{
    return ( A[0]*pow(kt,4) + \
             A[1]*pow(c2,2) + \
             A[2]*pow(kt,2)*pow(kl,2) + \
             A[3]*pow(cg,2)*pow(kl,2) + \
             A[4]*pow(c2g,2) + \
             A[5]*c2*pow(kt,2) + \
             A[6]*kl*pow(kt,3) + \
             A[7]*kt*kl*c2 + \
             A[8]*cg*kl*c2 + \
             A[9]*c2*c2g + \
             A[10]*cg*kl*pow(kt,2) + \
             A[11]*c2g*pow(kt,2) +\
             A[12]*pow(kl,2)*cg*kt + \
             A[13]*c2g*kt*kl + \
             A[14]*cg*c2g*kl + \
             A[15] * pow(kt,3) * cg + \
             A[16] * kt * c2 * cg + \
             A[17] * kt * pow(cg,2) * kl + \
             A[18] * cg * kt * c2g + \
             A[19] * pow(kt,2) * pow(cg,2) + \
             A[20] * c2 * pow(cg,2) + \
             A[21] * pow(cg,3) * kl + \
             A[22] * pow(cg,2) * c2g );
}
