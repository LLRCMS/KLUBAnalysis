#include "HHReweight5D.h"

#define DEBUG false

HHReweight5D::HHReweight5D(std::string coeffFile, const TH2* hInput, std::string EFTBMname, std::string year, std::string order, std::string uncertantie, bool cms_fake, bool useAbsEta)
{
    // clone the input histogram
    TH2* cloneH = (TH2*) hInput->Clone("h_input");

    h_input_.reset(cloneH);

    useAbsEta_ = useAbsEta;
    order_ = order; // default is "nlo"
    unc_ = uncertantie; // default is ""

    // --------------------------------------------------------
    // return coeficients for the total cross section at 13TeV
    A_13TeV_nlo_ = {62.5088, 345.604, 9.63451, 4.34841, 39.0143, -268.644, -44.2924, 96.5595, 53.515, -155.793, -23.678, 54.5601, 12.2273, -26.8654, -19.3723, -0.0904439, 0.321092, 0.452381, -0.0190758, -0.607163, 1.27408, 0.364487, -0.499263};
    A_13TeV_lo_ = {35.0111,169.908,4.72866,2.38523,22.3288,-142.521,-22.996,47.2901,28.0101,-82.3576,-13.1345,31.2217,6.37158,-13.9821,-10.8268};
    // the LO case has many different cases of uncertainty that can be considered
    if(uncertantie == "muR_UP")                            A_13TeV_lo_ = {29.339989,142.828652,3.944472,1.987152,18.841645,-119.547333,-19.223360,39.572711,23.399797,-69.040228,-10.957083,26.085625,5.308951,-11.681204,-9.055525,};
    else if(uncertantie == "muR_DN")                       A_13TeV_lo_ = {42.541636,205.687054,5.778218,2.919222,26.909724,-172.982077,-28.026622,57.564385,34.165875,-100.032567,-16.048800,38.075269,7.796551,-17.054354,-13.187661,};
    else if(uncertantie == "muF_UP")                       A_13TeV_lo_ = {33.214886,159.640761,4.551589,2.305399,20.727429,-134.819116,-21.986524,45.071311,26.839155,-78.057553,-12.641570,29.899841,6.155038,-13.395502,-10.335953,};
    else if(uncertantie == "muF_DN")                       A_13TeV_lo_ = {36.805367,180.705794,4.885372,2.452094,24.101164,-150.349033,-23.950697,49.443752,29.097654,-86.678003,-13.570472,32.448121,6.554115,-14.528044,-11.299634,};
    else if(uncertantie == "muRF_UP")                      A_13TeV_lo_ = {27.832322,134.173116,3.796522,1.920497,17.483549,-113.072653,-18.378290,37.711670,22.418837,-65.428160,-10.544753,24.981035,5.128215,-11.189653,-8.643234,};
    else if(uncertantie == "muRF_DN")                      A_13TeV_lo_ = {44.714854,218.701521,5.968957,3.000666,29.029593,-182.448410,-29.186181,60.175085,35.486974,-105.262740,-16.579853,39.569843,8.019070,-17.717366,-13.759752,};
    else if(uncertantie == "PDF4LHC15_nlo_30_pdfas_UP")    A_13TeV_lo_ = {36.208893,176.168489,4.879759,2.459933,23.284642,-147.500053,-23.752158,48.877622,28.922822,-85.183450,-13.550707,32.220345,6.571496,-14.439991,-11.195454,};
    else if(uncertantie == "PDF4LHC15_nlo_30_pdfas_DN")    A_13TeV_lo_ = {33.812066,163.645533,4.577366,2.310462,21.372602,-137.539060,-22.238752,45.701219,27.097318,-79.530065,-12.718469,30.222196,6.171466,-13.524284,-10.458108,};
    else if(uncertantie == "PDF4LHC15_nlo_30_pdfas_as_UP") A_13TeV_lo_ = {34.557787,167.736680,4.665718,2.353319,22.038584,-140.685346,-22.693751,46.672758,27.642437,-81.296870,-12.961223,30.811765,6.286366,-13.797880,-10.684346,};
    else if(uncertantie == "PDF4LHC15_nlo_30_pdfas_as_DN") A_13TeV_lo_ = {35.481751,172.242144,4.791610,2.416866,22.658916,-144.446484,-23.303070,47.923962,28.382176,-83.461994,-13.307699,31.634385,6.456062,-14.168891,-10.973232,};

    // -------------------------------------------------------------------------
    // Return EFT benchmarks definition as https://arxiv.org/pdf/1710.08261.pdf
    // kl - kt - c2 - cg - c2g
    if(EFTBMname == "sm")       EFTBMcouplings_ = {1, 1, 0, 0, 0};
    else if(EFTBMname ==  "1")  EFTBMcouplings_ = {7.5, 1, -1, 0, 0};
    else if(EFTBMname ==  "2")  EFTBMcouplings_ = {1.0, 1.0, 0.5, -0.8, 0.6};
    else if(EFTBMname ==  "3")  EFTBMcouplings_ = {1.0, 1.0, -1.5, 0.0, -0.8};
    else if(EFTBMname ==  "4")  EFTBMcouplings_ = {-3.5, 1.5, -3.0, 0.0, 0.0};
    else if(EFTBMname ==  "5")  EFTBMcouplings_ = {1.0, 1.0, 0.0, 0.8, -1.0};
    else if(EFTBMname ==  "6")  EFTBMcouplings_ = {2.4, 1.0, 0.0, 0.2, -0.2};
    else if(EFTBMname ==  "7")  EFTBMcouplings_ = {5.0, 1.0, 0.0, 0.2, -0.2};
    else if(EFTBMname ==  "8")  EFTBMcouplings_ = {15.0, 1.0, 0.0, -1.0, 1.0};
    else if(EFTBMname ==  "9")  EFTBMcouplings_ = {1.0, 1.0, 1.0, -0.6, 0.6};
    else if(EFTBMname ==  "10") EFTBMcouplings_ = {10.0, 1.5, -1.0, 0.0, 0.0};
    else if(EFTBMname ==  "11") EFTBMcouplings_ = {2.4, 1.0, 0.0, 1.0, -1.0};
    else if(EFTBMname ==  "12") EFTBMcouplings_ = {15.0, 1.0, 1.0, 0.0, 0.0};

    if( (year == "2017" or year == "2018") and EFTBMcouplings_.size() and cms_fake ){
      if (DEBUG) std::cout << "** DEBUG: before cms_fake: " << EFTBMcouplings_[0] << " " << EFTBMcouplings_[1] << " " << EFTBMcouplings_[2] << " "  << EFTBMcouplings_[3] << " "  << EFTBMcouplings_[4] << std::endl;
      EFTBMcouplings_[4] = 1.0;
      double tr = EFTBMcouplings_[0];
      EFTBMcouplings_[0] = EFTBMcouplings_[1];
      EFTBMcouplings_[1] = tr;
      if (DEBUG) std::cout << "** DEBUG: after cms_fake: " << EFTBMcouplings_[0] << " " << EFTBMcouplings_[1] << " " << EFTBMcouplings_[2] << " "  << EFTBMcouplings_[3] << " "  << EFTBMcouplings_[4] << std::endl;
    }

    if( (year == "2016") and EFTBMcouplings_.size() and cms_fake ){
      double tr = EFTBMcouplings_[0];
      EFTBMcouplings_[0] = EFTBMcouplings_[1];
      EFTBMcouplings_[1] = tr;
    }

    if(EFTBMname == "box")        EFTBMcouplings_ = {0, 1, 0, 0, 0};
    else if(EFTBMname == "cHHH0") EFTBMcouplings_ = {0.0,  1.0, 0.0, 0.0, 0.0};
    else if(EFTBMname == "cHHH1") EFTBMcouplings_ = {1.0,  1.0, 0.0, 0.0, 0.0};
    else if(EFTBMname == "cHHH2") EFTBMcouplings_ = {2.45, 1.0, 0.0, 0.0, 0.0};
    else if(EFTBMname == "cHHH5") EFTBMcouplings_ = {5.0,  1.0, 0.0, 0.0, 0.0};

    // 8a from https://link.springer.com/article/10.1007/JHEP09(2018)057
    if(EFTBMname ==  "8a") EFTBMcouplings_ = {1.0, 1.0, 0.5, 0.8/3, 0.0};

    // New benchmarks from https://arxiv.org/pdf/1908.08923.pdf
    if(EFTBMname == "JHEP03(2020)091_1" or EFTBMname == "1b") EFTBMcouplings_ = {  3.94, 0.94, -1./3.,  0.5 * 1.5,  1./3. * (-3.) };
    else if(EFTBMname == "JHEP03(2020)091_2" or EFTBMname == "2b") EFTBMcouplings_ = {  6.84, 0.61,  1./3.,  0.0 * 1.5, -1./3. * (-3.) };
    else if(EFTBMname == "JHEP03(2020)091_3" or EFTBMname == "3b") EFTBMcouplings_ = {  2.21, 1.05, -1./3.,  0.5 * 1.5,   0.5  * (-3.) };
    else if(EFTBMname == "JHEP03(2020)091_4" or EFTBMname == "4b") EFTBMcouplings_ = {  2.79, 0.61,  1./3., -0.5 * 1.5,  1./6. * (-3.) };
    else if(EFTBMname == "JHEP03(2020)091_5" or EFTBMname == "5b") EFTBMcouplings_ = {  3.95, 1.17, -1./3., 1./6.* 1.5,  -0.5  * (-3.) };
    else if(EFTBMname == "JHEP03(2020)091_6" or EFTBMname == "6b") EFTBMcouplings_ = {  5.68, 0.83,  1./3., -0.5 * 1.5,  1./3. * (-3.) };
    else if(EFTBMname == "JHEP03(2020)091_7" or EFTBMname == "7b") EFTBMcouplings_ = { -0.10, 0.94,     1., 1./6.* 1.5, -1./6. * (-3.) };

    if(EFTBMname == "c2scan" or EFTBMname == "manual") EFTBMcouplings_ = {1,1,0,0,0}; // Set to SM values

    // --------------------------------------------------------------------
    // read input file containing all the coefficients for the reweighting
    if (DEBUG) std::cout << " -- Reading input file" << coeffFile << std::endl;

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
            if ((tokens.size()) != 28 && (tokens.size() != 20))
            {
                std::cerr << " ** Error in reading input file: cannot interpret line: " << line << std::endl;
                std::cerr << " ** Error in reading input file: expecting 28(NLO) or 20(LO) tokens, found " << tokens.size() << std::endl;
                throw std::runtime_error("Cannot parse input file");
            } 

            //The columns of the file are respectively: uncertainty - Mhh_ll - Mhh_ul - GenCostStar_ll - GenCostStar_ul - 23(nlo)/15(lo) A values
            if (DEBUG) std::cout << "uncertainty= :" << tokens.at(0) << ": ; requested uncertainty= :" << unc_ << ":" << std::endl;
            if (tokens.at(0) != unc_) continue; // fill the map with the correct lines of the file corresponding to the correct uncertainty

            std::vector<double> A_vec;
            for (int i = 1; i<int(tokens.size()); i++) { // start loop from 1 to skip the uncertainty cause it is not nedeed in the following
                A_vec.push_back(std::stod(tokens.at(i)));
            }
            A_map_.push_back(A_vec);
            if (DEBUG) std::cout << "** DEBUG: A_vec size " << A_vec.size() << std::endl;
        }
    }
    if (DEBUG) std::cout << "** DEBUG: A_map size " << A_map_.size() << std::endl;
}

HHReweight5D::~HHReweight5D()
{}

double HHReweight5D::functionGF(double kl, double kt, double c2, double cg, double c2g, std::vector<double> A)
{
    if (order_ == "lo") {
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
                 A[14]*cg*c2g*kl );
    }
    else {
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
}

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
    if (DEBUG) std::cout << "** DEBUG: Noutputev=" << Noutputev << " ; Nev=" << Nev << " ; Nevtot=" << Nevtot << " ; XStot=" << XStot << " ; XS=" << XS << " ; mhhBinW=" << h_input_->GetXaxis()->GetBinWidth(ibinmhh) << " ; cthBinW=" << h_input_->GetYaxis()->GetBinWidth(ibincosthetaHH) << " --> HHweight=" << Noutputev/Nev * Nevtot/XStot << std::endl;
    return Noutputev/Nev * Nevtot/XStot;
}

// return the weight to be applied for the for c2 scan
double HHReweight5D::getWeight(double mhh, double cth, double c2)
{
    if (useAbsEta_) cth = TMath::Abs(cth);

    double kl  = 1;
    double kt  = 1;
    double cg  = 0;
    double c2g = 0;
    if (DEBUG) std::cout << "** DEBUG: c2 scan point requested: " << "kl=" << kl << " ; kt=" << kt << " ; c2=" << c2 << " ; cg=" << cg << " ; c2g="  << c2g << std::endl;

    double Nevtot = h_input_->Integral();
    double XStot = getTotXS(kl,kt,c2,cg,c2g);
    double Nev = h_input_->GetBinContent( h_input_->FindBin(mhh, cth) );
    double XS = getDiffXS(kl,kt,c2,cg,c2g,mhh,cth,A_map_);
    int ibinmhh = h_input_->GetXaxis()->FindBin(mhh);
    int ibincosthetaHH = h_input_->GetYaxis()->FindBin(cth);
    double Noutputev = XS * h_input_->GetXaxis()->GetBinWidth(ibinmhh) * h_input_->GetYaxis()->GetBinWidth(ibincosthetaHH);
    if (DEBUG) std::cout << "** DEBUG: Noutputev=" << Noutputev << " ; Nev=" << Nev << " ; Nevtot=" << Nevtot << " ; XStot=" << XStot << " ; XS=" << XS << " ; mhhBinW=" << h_input_->GetXaxis()->GetBinWidth(ibinmhh) << " ; cthBinW=" << h_input_->GetYaxis()->GetBinWidth(ibincosthetaHH) << " --> HHweight=" << Noutputev/Nev * Nevtot/XStot << std::endl;
    return Noutputev/Nev * Nevtot/XStot;
}

// return the weight to be applied for the reweight -> take mhh, cth, and couplings from input
double HHReweight5D::getWeight(double mhh, double cth, double kl, double kt, double c2, double cg, double c2g)
{
    if (useAbsEta_) cth = TMath::Abs(cth);
    if (DEBUG) std::cout << "** DEBUG: reqested coplings set: kl=" << kl << " ; kt=" << kt << " ; c2=" << c2 << " ; cg=" << cg << " ; c2g=" << c2g << std::endl;

    double Nevtot = h_input_->Integral();
    double XStot = getTotXS(kl,kt,c2,cg,c2g);
    double Nev = h_input_->GetBinContent( h_input_->FindBin(mhh, cth) );
    double XS = getDiffXS(kl,kt,c2,cg,c2g,mhh,cth,A_map_);
    int ibinmhh = h_input_->GetXaxis()->FindBin(mhh);
    int ibincosthetaHH = h_input_->GetYaxis()->FindBin(cth);
    double Noutputev = XS * h_input_->GetXaxis()->GetBinWidth(ibinmhh) * h_input_->GetYaxis()->GetBinWidth(ibincosthetaHH);
    if (DEBUG) std::cout << "** DEBUG: Noutputev=" << Noutputev << " ; Nev=" << Nev << " ; Nevtot=" << Nevtot << " ; XStot=" << XStot << " ; XS=" << XS << " ; mhhBinW=" << h_input_->GetXaxis()->GetBinWidth(ibinmhh) << " ; cthBinW=" << h_input_->GetYaxis()->GetBinWidth(ibincosthetaHH) << " --> HHweight=" << Noutputev/Nev * Nevtot/XStot << std::endl;
    return Noutputev/Nev * Nevtot/XStot;
}

double HHReweight5D::getTotXS(double kl, double kt, double c2, double cg, double c2g)
{
    if (order_ == "lo") return functionGF(kl, kt, c2, cg, c2g, A_13TeV_lo_);
    else return functionGF(kl, kt, c2, cg, c2g, A_13TeV_nlo_);
}

double HHReweight5D::getDiffXS(double kl, double kt, double c2, double cg, double c2g, double mhh, double cth, std::vector< std::vector<double> > A_map)
{  
   if (DEBUG) std::cout << "** DEBUG: AmapSize=" << A_map.size() << std::endl;
   for(int i = 0; i < int(A_map.size()); i++)
   {
      //The entries are respectively: Mhh_ll - Mhh_ul - GenCostStar_ll - GenCostStar_ul - 23(nlo)/15(lo) A values
      const std::vector<double> & values = A_map.at(i);
      double mass_bin_end = values.at(1);
      double cos_bin_end  = values.at(3);
      
      if(mhh > mass_bin_end || cth > cos_bin_end) continue;
      
      double dXsec = 0.;
      
      std::vector<double> As;
      for (int j=4; j<int(values.size()); j++) As.push_back(values.at(j));
      if (DEBUG) {
        std::cout << "** DEBUG: A coeffs: ";
        for (int j=0; j<int(As.size()); j++) std::cout << As[j] << " ; ";
        std::cout << std::endl;  
      }
      dXsec += functionGF(kl, kt, c2, cg, c2g, As);

      return dXsec / 1000; //diff XS in [fb] 
    }
 
   return 0;
}



//======================================================================================================================
// SUPPORTING FUNCTIONS AND THINGS

// this is the binning of input file histogram
// double binning_mHH [37] = {250.,270.,290.,310.,330.,
//                            350.,370.,390.,410.,430., 
//                            450.,470.,490.,510.,530.,
//                            550.,570.,590.,610.,630.,
//                            650.,670.,700.,750.,800.,
//                            850.,900.,950.,1000.,1100.,
//                            1200.,1300.,1400.,1500.,1750.,2000.,5000.};
//              
// double binning_cth [5]  = {0.0, 0.4, 0.6, 0.8, 1.0};
// 
// int nbins_mHH = 36; // size of arrays - 1
// int nbins_cth = 4;  // size of arrays - 1


// split a string on comma, return elements
std::vector<std::string> HHReweight5D::tokenize(std::string input)
{    
    if (DEBUG) std::cout << " -- Tokenizing input " << input << std::endl;
    
    std::stringstream buffer(input);
    std::string token;
    std::vector<std::string> ret;
    while (getline(buffer, token, ',')){
        ret.push_back(token);
    }

    if (DEBUG){
        std::cout << "I got " << ret.size() << " tokens" << std::endl;
        std::cout << "     --> " ;
        for (std::string x : ret) std::cout << ":" << x << ": ";
        std::cout << std::endl;
    }

    return ret;
}
