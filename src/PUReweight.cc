#include "../interface/PUReweight.h"

#include <iostream>
#include <cstdlib>
#include <fstream>

// ---------------------------------------------
// open input txt file and return a vector with PU weights
std::vector<float> PUReweight::LoadExternalWeights (TString filename)
{
  //cout << "=== begin parser ===" << endl;
  std::ifstream infile(filename.Data());
  if(!infile.good()){
    throw std::runtime_error("ERROR: PUReweight - Cannot open external file: " + filename);
  }
  std::vector<float> externalPUweights;
  std::string line;
  while (std::getline(infile, line))
  {
    line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
    while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
    while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
    while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
    if (!line.empty()) // skip empty lines
      externalPUweights.push_back(atof(line.c_str()));
  }

  return externalPUweights;
}
// ---------------------------------------------

float PUReweight::weight(int MC, int target, int input, TString filename) {

  if (theType == NONE) return 1.0;

  else if (theType == RUN2ANALYSIS) {

    if        (MC==25 && target==25) {
      if (input < 52)
        return hT2015_MC25ns_Data25ns.at(input) ;
      else
        // In case nPU is out-of data-profile binning,
        // use weight from last bin
        return hT2015_MC25ns_Data25ns.at(51) ;
    } else if (MC==25 && target==50) {
      return hT2015_MC25ns_Data50ns.at(input) ;
    } else if (MC==50 && target==25) {
      return hT2015_MC50ns_Data25ns.at(input) ;
    } else if (MC==50 && target==50) {
      return hT2015_MC50ns_Data50ns.at(input) ;
    } else if (MC==2016 && target==26) {
      return hT2016_MCSpring16_Data2c6fb.at(input) ;
    } else if (MC==2016 && target==40) {
      return hT2016_MCSpring16_Data4c0fb.at(input) ;
    } else if (MC==2016 && target==92) {
      return hT2016_MCSpring16_Data9c2fb.at(input) ;
    } else if (MC==2016 && target==129) {
      return hT2016_MCSpring16_Data12c9fb.at(input) ;
    } else if (MC==2016 && target==364) {
      return hT2016_MCSpring16_Data36c4fb.at(input) ;
    } else if (MC==20161 && target==364) {
      return hT2016_MCSummer16_Data36c4fb.at(input) ;
    } else if (MC==2017 && target==47) {                    // 2017 B
      return hT2017_MCFall17_Data4c7fb.at(input) ;
    } else if (MC==2017 && target==134) {                   // 2017 F
      return hT2017_MCFall17_Data13c4fb.at(input) ;
    } else if (MC==2017 && target==413) {
      return hT2017_MCFall17_Data41c3fb.at(input) ;         // Full 2017
    } else if (MC==2018 && target==597) {
      return hT2018_MCAutumn18_Data59c7fb.at(input) ;         // Full 2018
    } else if (MC==2017 && target==999) {                   // Ful 2017 with weights for each sample read from external txt file
      if (filename == TString(""))
      {
        std::cout << "ERROR: PUReweight - You requested special weights but didn't pass the external file: aborting !!! " << std::endl;
        abort();
      }
      else
      {
        return hT2017_MCFall17_Data41c3fb_special.at(input) ;
      }
    } else if (MC==2018 && target==999) {                   // Ful 2018 with weights for each sample read from external txt file
      if (filename == TString(""))
      {
        std::cout << "ERROR: PUReweight - You requested special weights but didn't pass the external file: aborting !!! " << std::endl;
        abort();
      }
      else
      {
        return hT2018_MCAutumn18_Data59c7fb_special.at(input) ;
      }
    } else if (MC==2016 && target==999) {                   // Ful 2016 with weights for each sample read from external txt file
      if (filename == TString(""))
      {
        std::cout << "ERROR: PUReweight - You requested special weights but didn't pass the external file: aborting !!! " << std::endl;
        abort();
      }
      else
      {
        return hT2016_MCSummer16_Data36c4fb_special.at(input) ;
      }

    } else{
      std::cout << "ERROR: PUReweight: " << MC << " " << target << std::endl;
      abort();
    }

  }

  else if (theType == RUN1ANALYSIS) {

    if        (MC==2011 && target==2011) {
      return hT2011.at(input) ;
    } else if (MC==2012 && target==2012) {
      return hT2012.at(input) ;
    } else{
      std::cout << "ERROR: PUReweight: " << MC << " " << target << std::endl;
      abort();
    }

  }

  else return 0 ;

}

PUReweight::PUReweight(Type type, TString filename) :
  theType(type),
  theFilename(filename) {
  // RUN2ANALYSIS MC Autumn2018 target Data 59.7/fb - Full Run 2018

  double npuAutumn18_59c7fb[100];
  npuAutumn18_59c7fb[0] =  0;
  npuAutumn18_59c7fb[1] =  0.087799;
  npuAutumn18_59c7fb[2] =  0.279066;
  npuAutumn18_59c7fb[3] =  0.130626;
  npuAutumn18_59c7fb[4] =  0.0858532;
  npuAutumn18_59c7fb[5] =  0.0477885;
  npuAutumn18_59c7fb[6] =  0.0352194;
  npuAutumn18_59c7fb[7] =  0.0264359;
  npuAutumn18_59c7fb[8] =  0.020543;
  npuAutumn18_59c7fb[9] =  0.0151677;
  npuAutumn18_59c7fb[10] = 0.0124101;
  npuAutumn18_59c7fb[11] = 0.0105321;
  npuAutumn18_59c7fb[12] = 0.00932549;
  npuAutumn18_59c7fb[13] = 0.00860153;
  npuAutumn18_59c7fb[14] = 0.00825717;
  npuAutumn18_59c7fb[15] = 0.00800769;
  npuAutumn18_59c7fb[16] = 0.00796;
  npuAutumn18_59c7fb[17] = 0.00800505;
  npuAutumn18_59c7fb[18] = 0.00805122;
  npuAutumn18_59c7fb[19] = 0.00807362;
  npuAutumn18_59c7fb[20] = 0.00800283;
  npuAutumn18_59c7fb[21] = 0.00790362;
  npuAutumn18_59c7fb[22] = 0.00769818;
  npuAutumn18_59c7fb[23] = 0.00741456;
  npuAutumn18_59c7fb[24] = 0.00715694;
  npuAutumn18_59c7fb[25] = 0.00685878;
  npuAutumn18_59c7fb[26] = 0.00659998;
  npuAutumn18_59c7fb[27] = 0.00640788;
  npuAutumn18_59c7fb[28] = 0.00624424;
  npuAutumn18_59c7fb[29] = 0.00610392;
  npuAutumn18_59c7fb[30] = 0.0060178;
  npuAutumn18_59c7fb[31] = 0.00597335;
  npuAutumn18_59c7fb[32] = 0.00592001;
  npuAutumn18_59c7fb[33] = 0.0058976;
  npuAutumn18_59c7fb[34] = 0.00588401;
  npuAutumn18_59c7fb[35] = 0.00587624;
  npuAutumn18_59c7fb[36] = 0.00583334;
  npuAutumn18_59c7fb[37] = 0.005793;
  npuAutumn18_59c7fb[38] = 0.00566443;
  npuAutumn18_59c7fb[39] = 0.00552869;
  npuAutumn18_59c7fb[40] = 0.00535985;
  npuAutumn18_59c7fb[41] = 0.00514221;
  npuAutumn18_59c7fb[42] = 0.00485622;
  npuAutumn18_59c7fb[43] = 0.00456103;
  npuAutumn18_59c7fb[44] = 0.00417179;
  npuAutumn18_59c7fb[45] = 0.00380562;
  npuAutumn18_59c7fb[46] = 0.00340141;
  npuAutumn18_59c7fb[47] = 0.00301339;
  npuAutumn18_59c7fb[48] = 0.00263398;
  npuAutumn18_59c7fb[49] = 0.00229733;
  npuAutumn18_59c7fb[50] = 0.0019863;
  npuAutumn18_59c7fb[51] = 0.00169135;
  npuAutumn18_59c7fb[52] = 0.00145007;
  npuAutumn18_59c7fb[53] = 0.00124755;
  npuAutumn18_59c7fb[54] = 0.00106857;
  npuAutumn18_59c7fb[55] = 0.000919685;
  npuAutumn18_59c7fb[56] = 0.000793965;
  npuAutumn18_59c7fb[57] = 0.000688101;
  npuAutumn18_59c7fb[58] = 0.000604069;
  npuAutumn18_59c7fb[59] = 0.000527584;
  npuAutumn18_59c7fb[60] = 0.000459603;
  npuAutumn18_59c7fb[61] = 0.000412641;
  npuAutumn18_59c7fb[62] = 0.00035373;
  npuAutumn18_59c7fb[63] = 0.000305004;
  npuAutumn18_59c7fb[64] = 0.000265356;
  npuAutumn18_59c7fb[65] = 0.000238;
  npuAutumn18_59c7fb[66] = 0.000195732;
  npuAutumn18_59c7fb[67] = 0.000166222;
  npuAutumn18_59c7fb[68] = 0.000152534;
  npuAutumn18_59c7fb[69] = 0.000120627;
  npuAutumn18_59c7fb[70] = 0.000103871;
  npuAutumn18_59c7fb[71] = 9.33404e-05;
  npuAutumn18_59c7fb[72] = 6.99581e-05;
  npuAutumn18_59c7fb[73] = 6.18898e-05;
  npuAutumn18_59c7fb[74] = 5.52046e-05;
  npuAutumn18_59c7fb[75] = 4.83474e-05;
  npuAutumn18_59c7fb[76] = 3.70325e-05;
  npuAutumn18_59c7fb[77] = 3.66288e-05;
  npuAutumn18_59c7fb[78] = 1.88255e-05;
  npuAutumn18_59c7fb[79] = 2.08066e-05;
  npuAutumn18_59c7fb[80] = 2.31704e-05;
  npuAutumn18_59c7fb[81] = 9.74299e-06;
  npuAutumn18_59c7fb[82] = 7.49028e-06;
  npuAutumn18_59c7fb[83] = 5.34533e-06;
  npuAutumn18_59c7fb[84] = 8.46013e-06;
  npuAutumn18_59c7fb[85] = 6.59564e-06;
  npuAutumn18_59c7fb[86] = 0;
  npuAutumn18_59c7fb[87] = 1.13356e-06;
  npuAutumn18_59c7fb[88] = 0;
  npuAutumn18_59c7fb[89] = 4.10787e-07;
  npuAutumn18_59c7fb[90] = 9.84473e-08;
  npuAutumn18_59c7fb[91] = 0;
  npuAutumn18_59c7fb[92] = 0;
  npuAutumn18_59c7fb[93] = 0;
  npuAutumn18_59c7fb[94] = 0;
  npuAutumn18_59c7fb[95] = 0;
  npuAutumn18_59c7fb[96] = 3.31534e-09;
  npuAutumn18_59c7fb[97] = 0;
  npuAutumn18_59c7fb[98] = 0;
  npuAutumn18_59c7fb[99] = 2.41889e-10;

  for(int k = 0 ; k < 100 ; ++k)
    hT2018_MCAutumn18_Data59c7fb.push_back(npuAutumn18_59c7fb[k]) ;

  // RUN2ANALYSIS MC Autumn2018 target Data 59.7/fb - Full Run 2018
  // Special weights for each sample read from external txt file
  if (filename == TString(""))
  {
    for(int k = 0 ; k < 100 ; ++k)
      hT2018_MCAutumn18_Data59c7fb_special.push_back(0.0) ; // if no filename passe, initialize a vector of 0s
  }
  else
    hT2018_MCAutumn18_Data59c7fb_special = LoadExternalWeights(theFilename);

  // RUN2ANALYSIS MC Fall2017 target Data 4.7/fb - Run 2017B
  double npuFall17_4c7fb[100];
  npuFall17_4c7fb[0] = 5.0573e-11;
  npuFall17_4c7fb[1] = 1.1063e-07;
  npuFall17_4c7fb[2] = 0.000219936;
  npuFall17_4c7fb[3] = 0.000583219;
  npuFall17_4c7fb[4] = 0.00110071;
  npuFall17_4c7fb[5] = 0.00245114;
  npuFall17_4c7fb[6] = 0.00326315;
  npuFall17_4c7fb[7] = 0.00541582;
  npuFall17_4c7fb[8] = 0.00370288;
  npuFall17_4c7fb[9] = 0.00808729;
  npuFall17_4c7fb[10] = 0.0140935;
  npuFall17_4c7fb[11] = 0.0203551;
  npuFall17_4c7fb[12] = 0.0225992;
  npuFall17_4c7fb[13] = 0.0235963;
  npuFall17_4c7fb[14] = 0.0226469;
  npuFall17_4c7fb[15] = 0.0229251;
  npuFall17_4c7fb[16] = 0.0239265;
  npuFall17_4c7fb[17] = 0.0265013;
  npuFall17_4c7fb[18] = 0.0304041;
  npuFall17_4c7fb[19] = 0.0337741;
  npuFall17_4c7fb[20] = 0.0367271;
  npuFall17_4c7fb[21] = 0.039128;
  npuFall17_4c7fb[22] = 0.041116;
  npuFall17_4c7fb[23] = 0.0422544;
  npuFall17_4c7fb[24] = 0.0430938;
  npuFall17_4c7fb[25] = 0.0433803;
  npuFall17_4c7fb[26] = 0.0445453;
  npuFall17_4c7fb[27] = 0.0452539;
  npuFall17_4c7fb[28] = 0.0462681;
  npuFall17_4c7fb[29] = 0.0463272;
  npuFall17_4c7fb[30] = 0.0446593;
  npuFall17_4c7fb[31] = 0.0415155;
  npuFall17_4c7fb[32] = 0.0380809;
  npuFall17_4c7fb[33] = 0.0341602;
  npuFall17_4c7fb[34] = 0.0297901;
  npuFall17_4c7fb[35] = 0.0253602;
  npuFall17_4c7fb[36] = 0.0214611;
  npuFall17_4c7fb[37] = 0.0177568;
  npuFall17_4c7fb[38] = 0.0140154;
  npuFall17_4c7fb[39] = 0.0107101;
  npuFall17_4c7fb[40] = 0.00822515;
  npuFall17_4c7fb[41] = 0.00627565;
  npuFall17_4c7fb[42] = 0.0046823;
  npuFall17_4c7fb[43] = 0.00334397;
  npuFall17_4c7fb[44] = 0.00232447;
  npuFall17_4c7fb[45] = 0.00156623;
  npuFall17_4c7fb[46] = 0.00100737;
  npuFall17_4c7fb[47] = 0.000591927;
  npuFall17_4c7fb[48] = 0.00034207;
  npuFall17_4c7fb[49] = 0.000188086;
  npuFall17_4c7fb[50] = 9.70828e-05;
  npuFall17_4c7fb[51] = 5.07534e-05;
  npuFall17_4c7fb[52] = 2.686e-05;
  npuFall17_4c7fb[53] = 1.36568e-05;
  npuFall17_4c7fb[54] = 7.00633e-06;
  npuFall17_4c7fb[55] = 3.58238e-06;
  npuFall17_4c7fb[56] = 1.84616e-06;
  npuFall17_4c7fb[57] = 9.59841e-07;
  npuFall17_4c7fb[58] = 5.07704e-07;
  npuFall17_4c7fb[59] = 2.72568e-07;
  npuFall17_4c7fb[60] = 1.50005e-07;
  npuFall17_4c7fb[61] = 8.46057e-08;
  npuFall17_4c7fb[62] = 5.05581e-08;
  npuFall17_4c7fb[63] = 2.76879e-08;
  npuFall17_4c7fb[64] = 1.62277e-08;
  npuFall17_4c7fb[65] = 8.7941e-09;
  npuFall17_4c7fb[66] = 4.4519e-09;
  npuFall17_4c7fb[67] = 2.41922e-09;
  npuFall17_4c7fb[68] = 1.46638e-09;
  npuFall17_4c7fb[69] = 8.30616e-10;
  npuFall17_4c7fb[70] = 4.49238e-10;
  npuFall17_4c7fb[71] = 2.04164e-10;
  npuFall17_4c7fb[72] = 1.55623e-10;
  npuFall17_4c7fb[73] = 5.20729e-11;
  npuFall17_4c7fb[74] = 2.38806e-11;
  npuFall17_4c7fb[75] = 9.07855e-12;
  npuFall17_4c7fb[76] = 4.86726e-12;
  npuFall17_4c7fb[77] = 1.89847e-12;
  npuFall17_4c7fb[78] = 3.57262e-13;
  npuFall17_4c7fb[79] = 4.6723e-14;
  npuFall17_4c7fb[80] = 1.0584e-14;
  npuFall17_4c7fb[81] = 5.99902e-15;
  npuFall17_4c7fb[82] = 2.89702e-15;
  npuFall17_4c7fb[83] = 5.0164e-16;
  npuFall17_4c7fb[84] = 1.31379e-16;
  npuFall17_4c7fb[85] = 1.16221e-16;
  npuFall17_4c7fb[86] = 1.58368e-17;
  npuFall17_4c7fb[87] = 3.08417e-18;
  npuFall17_4c7fb[88] = 0;
  npuFall17_4c7fb[89] = 0;
  npuFall17_4c7fb[90] = 0;
  npuFall17_4c7fb[91] = 0;
  npuFall17_4c7fb[92] = 0;
  npuFall17_4c7fb[93] = 0;
  npuFall17_4c7fb[94] = 0;
  npuFall17_4c7fb[95] = 0;
  npuFall17_4c7fb[96] = 0;
  npuFall17_4c7fb[97] = 0;
  npuFall17_4c7fb[98] = 0;
  npuFall17_4c7fb[99] = 0;

  for(int k = 0 ; k < 100 ; ++k)
    hT2017_MCFall17_Data4c7fb.push_back(npuFall17_4c7fb[k]) ;

  // RUN2ANALYSIS MC Fall2017 target Data 13.4/fb - Run 2017F
  double npuFall17_13c4fb[100];
  npuFall17_13c4fb[0] = 9.89522e-06;
  npuFall17_13c4fb[1] = 0.00101106;
  npuFall17_13c4fb[2] = 0.00143483;
  npuFall17_13c4fb[3] = 0.00210392;
  npuFall17_13c4fb[4] = 0.00123114;
  npuFall17_13c4fb[5] = 0.0021592;
  npuFall17_13c4fb[6] = 0.0020239;
  npuFall17_13c4fb[7] = 0.00200777;
  npuFall17_13c4fb[8] = 0.00181845;
  npuFall17_13c4fb[9] = 0.00895319;
  npuFall17_13c4fb[10] = 0.0141745;
  npuFall17_13c4fb[11] = 0.0183453;
  npuFall17_13c4fb[12] = 0.0165334;
  npuFall17_13c4fb[13] = 0.012658;
  npuFall17_13c4fb[14] = 0.0092043;
  npuFall17_13c4fb[15] = 0.00779417;
  npuFall17_13c4fb[16] = 0.00736106;
  npuFall17_13c4fb[17] = 0.00736252;
  npuFall17_13c4fb[18] = 0.00756199;
  npuFall17_13c4fb[19] = 0.00769292;
  npuFall17_13c4fb[20] = 0.00823143;
  npuFall17_13c4fb[21] = 0.00938701;
  npuFall17_13c4fb[22] = 0.0108095;
  npuFall17_13c4fb[23] = 0.0116972;
  npuFall17_13c4fb[24] = 0.0119152;
  npuFall17_13c4fb[25] = 0.0116079;
  npuFall17_13c4fb[26] = 0.0114019;
  npuFall17_13c4fb[27] = 0.0111123;
  npuFall17_13c4fb[28] = 0.0110952;
  npuFall17_13c4fb[29] = 0.0111809;
  npuFall17_13c4fb[30] = 0.0112607;
  npuFall17_13c4fb[31] = 0.0113653;
  npuFall17_13c4fb[32] = 0.0117032;
  npuFall17_13c4fb[33] = 0.0121198;
  npuFall17_13c4fb[34] = 0.0125431;
  npuFall17_13c4fb[35] = 0.0130545;
  npuFall17_13c4fb[36] = 0.013902;
  npuFall17_13c4fb[37] = 0.0148434;
  npuFall17_13c4fb[38] = 0.0154898;
  npuFall17_13c4fb[39] = 0.016134;
  npuFall17_13c4fb[40] = 0.0176283;
  npuFall17_13c4fb[41] = 0.0202253;
  npuFall17_13c4fb[42] = 0.024126;
  npuFall17_13c4fb[43] = 0.0291746;
  npuFall17_13c4fb[44] = 0.0359265;
  npuFall17_13c4fb[45] = 0.0441436;
  npuFall17_13c4fb[46] = 0.0524222;
  npuFall17_13c4fb[47] = 0.0567467;
  npuFall17_13c4fb[48] = 0.0595603;
  npuFall17_13c4fb[49] = 0.0580685;
  npuFall17_13c4fb[50] = 0.0514677;
  npuFall17_13c4fb[51] = 0.0444839;
  npuFall17_13c4fb[52] = 0.0373051;
  npuFall17_13c4fb[53] = 0.0287289;
  npuFall17_13c4fb[54] = 0.021327;
  npuFall17_13c4fb[55] = 0.0151018;
  npuFall17_13c4fb[56] = 0.0103571;
  npuFall17_13c4fb[57] = 0.00692589;
  npuFall17_13c4fb[58] = 0.00458553;
  npuFall17_13c4fb[59] = 0.00302045;
  npuFall17_13c4fb[60] = 0.00201276;
  npuFall17_13c4fb[61] = 0.00136465;
  npuFall17_13c4fb[62] = 0.000977936;
  npuFall17_13c4fb[63] = 0.000643173;
  npuFall17_13c4fb[64] = 0.000454705;
  npuFall17_13c4fb[65] = 0.000299269;
  npuFall17_13c4fb[66] = 0.000185651;
  npuFall17_13c4fb[67] = 0.000125002;
  npuFall17_13c4fb[68] = 9.5157e-05;
  npuFall17_13c4fb[69] = 6.88218e-05;
  npuFall17_13c4fb[70] = 4.85073e-05;
  npuFall17_13c4fb[71] = 2.94691e-05;
  npuFall17_13c4fb[72] = 3.09919e-05;
  npuFall17_13c4fb[73] = 1.4871e-05;
  npuFall17_13c4fb[74] = 1.02353e-05;
  npuFall17_13c4fb[75] = 6.14578e-06;
  npuFall17_13c4fb[76] = 5.49176e-06;
  npuFall17_13c4fb[77] = 3.76442e-06;
  npuFall17_13c4fb[78] = 1.30737e-06;
  npuFall17_13c4fb[79] = 3.29375e-07;
  npuFall17_13c4fb[80] = 1.49065e-07;
  npuFall17_13c4fb[81] = 1.73969e-07;
  npuFall17_13c4fb[82] = 1.77548e-07;
  npuFall17_13c4fb[83] = 6.64346e-08;
  npuFall17_13c4fb[84] = 3.82441e-08;
  npuFall17_13c4fb[85] = 7.45601e-08;
  npuFall17_13c4fb[86] = 2.26898e-08;
  npuFall17_13c4fb[87] = 1.05562e-08;
  npuFall17_13c4fb[88] = 6.57167e-09;
  npuFall17_13c4fb[89] = 1.15991e-08;
  npuFall17_13c4fb[90] = 1.14241e-08;
  npuFall17_13c4fb[91] = 3.89972e-09;
  npuFall17_13c4fb[92] = 2.65404e-07;
  npuFall17_13c4fb[93] = 6.4007e-09;
  npuFall17_13c4fb[94] = 3.38508e-09;
  npuFall17_13c4fb[95] = 1.34335e-10;
  npuFall17_13c4fb[96] = 2.26515e-09;
  npuFall17_13c4fb[97] = 6.33711e-10;
  npuFall17_13c4fb[98] = 3.4643e-11;
  npuFall17_13c4fb[99] = 1.92764e-11;

  for(int k = 0 ; k < 100 ; ++k)
    hT2017_MCFall17_Data13c4fb.push_back(npuFall17_13c4fb[k]) ;

  // RUN2ANALYSIS MC Fall2017 target Data 41.3/fb - Full Run 2017
  double npuFall17_41c3fb[100];

  npuFall17_41c3fb[0] = 4.9051e-06;
  npuFall17_41c3fb[1] = 0.000472154;
  npuFall17_41c3fb[2] = 0.000688731;
  npuFall17_41c3fb[3] = 0.00122529;
  npuFall17_41c3fb[4] = 0.00128052;
  npuFall17_41c3fb[5] = 0.00223758;
  npuFall17_41c3fb[6] = 0.00239566;
  npuFall17_41c3fb[7] = 0.00320187;
  npuFall17_41c3fb[8] = 0.00209271;
  npuFall17_41c3fb[9] = 0.00580224;
  npuFall17_41c3fb[10] = 0.00869655;
  npuFall17_41c3fb[11] = 0.0119093;
  npuFall17_41c3fb[12] = 0.0129334;
  npuFall17_41c3fb[13] = 0.013594;
  npuFall17_41c3fb[14] = 0.0139208;
  npuFall17_41c3fb[15] = 0.0154465;
  npuFall17_41c3fb[16] = 0.0173105;
  npuFall17_41c3fb[17] = 0.0193026;
  npuFall17_41c3fb[18] = 0.0210863;
  npuFall17_41c3fb[19] = 0.0220537;
  npuFall17_41c3fb[20] = 0.0229544;
  npuFall17_41c3fb[21] = 0.0238812;
  npuFall17_41c3fb[22] = 0.0248906;
  npuFall17_41c3fb[23] = 0.0255494;
  npuFall17_41c3fb[24] = 0.0259594;
  npuFall17_41c3fb[25] = 0.0258511;
  npuFall17_41c3fb[26] = 0.0260982;
  npuFall17_41c3fb[27] = 0.0260495;
  npuFall17_41c3fb[28] = 0.0263201;
  npuFall17_41c3fb[29] = 0.0262965;
  npuFall17_41c3fb[30] = 0.0255884;
  npuFall17_41c3fb[31] = 0.0243244;
  npuFall17_41c3fb[32] = 0.023135;
  npuFall17_41c3fb[33] = 0.021847;
  npuFall17_41c3fb[34] = 0.0204247;
  npuFall17_41c3fb[35] = 0.0190721;
  npuFall17_41c3fb[36] = 0.0181929;
  npuFall17_41c3fb[37] = 0.0174875;
  npuFall17_41c3fb[38] = 0.0165716;
  npuFall17_41c3fb[39] = 0.0157859;
  npuFall17_41c3fb[40] = 0.0158111;
  npuFall17_41c3fb[41] = 0.0166047;
  npuFall17_41c3fb[42] = 0.0181107;
  npuFall17_41c3fb[43] = 0.0200922;
  npuFall17_41c3fb[44] = 0.0229032;
  npuFall17_41c3fb[45] = 0.0263843;
  npuFall17_41c3fb[46] = 0.0297868;
  npuFall17_41c3fb[47] = 0.0310556;
  npuFall17_41c3fb[48] = 0.0317504;
  npuFall17_41c3fb[49] = 0.0304409;
  npuFall17_41c3fb[50] = 0.0267479;
  npuFall17_41c3fb[51] = 0.0230812;
  npuFall17_41c3fb[52] = 0.0194507;
  npuFall17_41c3fb[53] = 0.015146;
  npuFall17_41c3fb[54] = 0.0114399;
  npuFall17_41c3fb[55] = 0.00829491;
  npuFall17_41c3fb[56] = 0.00586446;
  npuFall17_41c3fb[57] = 0.00407174;
  npuFall17_41c3fb[58] = 0.00282059;
  npuFall17_41c3fb[59] = 0.00196016;
  npuFall17_41c3fb[60] = 0.00139088;
  npuFall17_41c3fb[61] = 0.00101465;
  npuFall17_41c3fb[62] = 0.000791774;
  npuFall17_41c3fb[63] = 0.000574993;
  npuFall17_41c3fb[64] = 0.000456188;
  npuFall17_41c3fb[65] = 0.000343267;
  npuFall17_41c3fb[66] = 0.000248572;
  npuFall17_41c3fb[67] = 0.000199778;
  npuFall17_41c3fb[68] = 0.000185688;
  npuFall17_41c3fb[69] = 0.000167503;
  npuFall17_41c3fb[70] = 0.000149903;
  npuFall17_41c3fb[71] = 0.000117057;
  npuFall17_41c3fb[72] = 0.000158975;
  npuFall17_41c3fb[73] = 9.80907e-05;
  npuFall17_41c3fb[74] = 8.56725e-05;
  npuFall17_41c3fb[75] = 6.3935e-05;
  npuFall17_41c3fb[76] = 6.92299e-05;
  npuFall17_41c3fb[77] = 5.60262e-05;
  npuFall17_41c3fb[78] = 2.24435e-05;
  npuFall17_41c3fb[79] = 6.40351e-06;
  npuFall17_41c3fb[80] = 3.24068e-06;
  npuFall17_41c3fb[81] = 4.19803e-06;
  npuFall17_41c3fb[82] = 4.74042e-06;
  npuFall17_41c3fb[83] = 1.96247e-06;
  npuFall17_41c3fb[84] = 1.25252e-06;
  npuFall17_41c3fb[85] = 2.71666e-06;
  npuFall17_41c3fb[86] = 9.23657e-07;
  npuFall17_41c3fb[87] = 4.82355e-07;
  npuFall17_41c3fb[88] = 3.38698e-07;
  npuFall17_41c3fb[89] = 6.77563e-07;
  npuFall17_41c3fb[90] = 7.60007e-07;
  npuFall17_41c3fb[91] = 2.96848e-07;
  npuFall17_41c3fb[92] = 2.32212e-05;
  npuFall17_41c3fb[93] = 6.46543e-07;
  npuFall17_41c3fb[94] = 3.96447e-07;
  npuFall17_41c3fb[95] = 1.83168e-08;
  npuFall17_41c3fb[96] = 3.61036e-07;
  npuFall17_41c3fb[97] = 1.18532e-07;
  npuFall17_41c3fb[98] = 7.6332e-09;
  npuFall17_41c3fb[99] = 5.02201e-09;

  for(int k = 0 ; k < 100 ; ++k)
    hT2017_MCFall17_Data41c3fb.push_back(npuFall17_41c3fb[k]) ;

  // RUN2ANALYSIS MC Fall2017 target Data 41.3/fb - Full Run 2017
  // Special weights for each sample read from external txt file
  if (filename == TString(""))
  {
    for(int k = 0 ; k < 100 ; ++k)
      hT2017_MCFall17_Data41c3fb_special.push_back(0.0) ; // if no filename passe, initialize a vector of 0s
  }
  else
    hT2017_MCFall17_Data41c3fb_special = LoadExternalWeights(theFilename);


  // RUN2ANALYSIS MC Spring2016 target Data 2.6/fb
  double npuSpring16_2c6fb[100] ;
  npuSpring16_2c6fb[0] = 1.10942e-05;
  npuSpring16_2c6fb[1] = 8.7385e-05;
  npuSpring16_2c6fb[2] = 0.000235451;
  npuSpring16_2c6fb[3] = 0.000460293;
  npuSpring16_2c6fb[4] = 0.00053997;
  npuSpring16_2c6fb[5] = 0.000643108;
  npuSpring16_2c6fb[6] = 0.000806518;
  npuSpring16_2c6fb[7] = 0.00409823;
  npuSpring16_2c6fb[8] = 0.0103187;
  npuSpring16_2c6fb[9] = 0.0119524;
  npuSpring16_2c6fb[10] = 0.0130906;
  npuSpring16_2c6fb[11] = 0.0161581;
  npuSpring16_2c6fb[12] = 0.0221365;
  npuSpring16_2c6fb[13] = 0.030481;
  npuSpring16_2c6fb[14] = 0.0388027;
  npuSpring16_2c6fb[15] = 0.044809;
  npuSpring16_2c6fb[16] = 0.0478983;
  npuSpring16_2c6fb[17] = 0.0476594;
  npuSpring16_2c6fb[18] = 0.0442376;
  npuSpring16_2c6fb[19] = 0.0389583;
  npuSpring16_2c6fb[20] = 0.0327539;
  npuSpring16_2c6fb[21] = 0.026543;
  npuSpring16_2c6fb[22] = 0.0207051;
  npuSpring16_2c6fb[23] = 0.015336;
  npuSpring16_2c6fb[24] = 0.0107381;
  npuSpring16_2c6fb[25] = 0.00707373;
  npuSpring16_2c6fb[26] = 0.00440416;
  npuSpring16_2c6fb[27] = 0.00259124;
  npuSpring16_2c6fb[28] = 0.00145504;
  npuSpring16_2c6fb[29] = 0.000788986;
  npuSpring16_2c6fb[30] = 0.00041218;
  npuSpring16_2c6fb[31] = 0.000210234;
  npuSpring16_2c6fb[32] = 0.000106008;
  npuSpring16_2c6fb[33] = 5.39633e-05;
  npuSpring16_2c6fb[34] = 2.92773e-05;
  npuSpring16_2c6fb[35] = 1.8728e-05;
  npuSpring16_2c6fb[36] = 1.48676e-05;
  npuSpring16_2c6fb[37] = 1.45913e-05;
  npuSpring16_2c6fb[38] = 1.64934e-05;
  npuSpring16_2c6fb[39] = 1.99875e-05;
  npuSpring16_2c6fb[40] = 2.50058e-05;
  npuSpring16_2c6fb[41] = 3.2628e-05;
  npuSpring16_2c6fb[42] = 4.34942e-05;
  npuSpring16_2c6fb[43] = 5.97589e-05;
  npuSpring16_2c6fb[44] = 8.26555e-05;
  npuSpring16_2c6fb[45] = 0.000118052;
  npuSpring16_2c6fb[46] = 0.000166901;
  npuSpring16_2c6fb[47] = 0.000239219;
  npuSpring16_2c6fb[48] = 0.000347361;
  npuSpring16_2c6fb[49] = 0.000507212;
  npuSpring16_2c6fb[50] = 0.000796241;
  npuSpring16_2c6fb[51] = 0.00109752;
  npuSpring16_2c6fb[52] = 0.00161868;
  npuSpring16_2c6fb[53] = 0.00267864;
  npuSpring16_2c6fb[54] = 0.00377844;
  npuSpring16_2c6fb[55] = 0.00701859;
  npuSpring16_2c6fb[56] = 0.00848053;
  npuSpring16_2c6fb[57] = 0.016081;
  npuSpring16_2c6fb[58] = 0.0324173;
  npuSpring16_2c6fb[59] = 0.0348507;
  npuSpring16_2c6fb[60] = 0;
  npuSpring16_2c6fb[61] = 0;
  npuSpring16_2c6fb[62] = 0;
  npuSpring16_2c6fb[63] = 0;
  npuSpring16_2c6fb[64] = 0;
  npuSpring16_2c6fb[65] = 0;
  npuSpring16_2c6fb[66] = 0;
  npuSpring16_2c6fb[67] = 0;
  npuSpring16_2c6fb[68] = 0;
  npuSpring16_2c6fb[69] = 0;
  npuSpring16_2c6fb[70] = 0;
  npuSpring16_2c6fb[71] = 0;
  npuSpring16_2c6fb[72] = 0;
  npuSpring16_2c6fb[73] = 0;
  npuSpring16_2c6fb[74] = 0;
  npuSpring16_2c6fb[75] = 0;
  npuSpring16_2c6fb[76] = 0;
  npuSpring16_2c6fb[77] = 0;
  npuSpring16_2c6fb[78] = 0;
  npuSpring16_2c6fb[79] = 0;
  npuSpring16_2c6fb[80] = 0;
  npuSpring16_2c6fb[81] = 0;
  npuSpring16_2c6fb[82] = 0;
  npuSpring16_2c6fb[83] = 0;
  npuSpring16_2c6fb[84] = 0;
  npuSpring16_2c6fb[85] = 0;
  npuSpring16_2c6fb[86] = 0;
  npuSpring16_2c6fb[87] = 0;
  npuSpring16_2c6fb[88] = 0;
  npuSpring16_2c6fb[89] = 0;
  npuSpring16_2c6fb[90] = 0;
  npuSpring16_2c6fb[91] = 0;
  npuSpring16_2c6fb[92] = 0;
  npuSpring16_2c6fb[93] = 0;
  npuSpring16_2c6fb[94] = 0;
  npuSpring16_2c6fb[95] = 0;
  npuSpring16_2c6fb[96] = 0;
  npuSpring16_2c6fb[97] = 0;
  npuSpring16_2c6fb[98] = 0;
  npuSpring16_2c6fb[99] = 0;
  for(int k = 0 ; k < 100 ; ++k)
    hT2016_MCSpring16_Data2c6fb.push_back(npuSpring16_2c6fb[k]) ;

  // RUN2ANALYSIS MC Summer2016 target Data 36.4/fb - Full Run 2016
  // Special weights for each sample read from external txt file
  if (filename == TString(""))
  {
    for(int k = 0 ; k < 100 ; ++k)
      hT2016_MCSummer16_Data36c4fb_special.push_back(0.0) ; // if no filename passe, initialize a vector of 0s
  }
  else
    hT2016_MCSummer16_Data36c4fb_special = LoadExternalWeights(theFilename);


  // RUN2ANALYSIS MC Spring2016 target Data 4.0/fb
  double npuSpring16_4c0fb[100] ;
  npuSpring16_4c0fb[0] = 8.7422e-06;
  npuSpring16_4c0fb[1] = 7.63583e-05;
  npuSpring16_4c0fb[2] = 0.00033404;
  npuSpring16_4c0fb[3] = 0.000636716;
  npuSpring16_4c0fb[4] = 0.000771597;
  npuSpring16_4c0fb[5] = 0.000792934;
  npuSpring16_4c0fb[6] = 0.000892549;
  npuSpring16_4c0fb[7] = 0.00352036;
  npuSpring16_4c0fb[8] = 0.00888222;
  npuSpring16_4c0fb[9] = 0.0119397;
  npuSpring16_4c0fb[10] = 0.0160619;
  npuSpring16_4c0fb[11] = 0.0213887;
  npuSpring16_4c0fb[12] = 0.0281328;
  npuSpring16_4c0fb[13] = 0.0370348;
  npuSpring16_4c0fb[14] = 0.0459453;
  npuSpring16_4c0fb[15] = 0.0516731;
  npuSpring16_4c0fb[16] = 0.0539392;
  npuSpring16_4c0fb[17] = 0.0534552;
  npuSpring16_4c0fb[18] = 0.0506146;
  npuSpring16_4c0fb[19] = 0.0462617;
  npuSpring16_4c0fb[20] = 0.040696;
  npuSpring16_4c0fb[21] = 0.0346134;
  npuSpring16_4c0fb[22] = 0.0284331;
  npuSpring16_4c0fb[23] = 0.0222798;
  npuSpring16_4c0fb[24] = 0.0165486;
  npuSpring16_4c0fb[25] = 0.0115209;
  npuSpring16_4c0fb[26] = 0.00747978;
  npuSpring16_4c0fb[27] = 0.00448272;
  npuSpring16_4c0fb[28] = 0.00248583;
  npuSpring16_4c0fb[29] = 0.00128658;
  npuSpring16_4c0fb[30] = 0.000621414;
  npuSpring16_4c0fb[31] = 0.000285809;
  npuSpring16_4c0fb[32] = 0.00012797;
  npuSpring16_4c0fb[33] = 5.75304e-05;
  npuSpring16_4c0fb[34] = 2.77107e-05;
  npuSpring16_4c0fb[35] = 1.60887e-05;
  npuSpring16_4c0fb[36] = 1.20626e-05;
  npuSpring16_4c0fb[37] = 1.15813e-05;
  npuSpring16_4c0fb[38] = 1.30154e-05;
  npuSpring16_4c0fb[39] = 1.57539e-05;
  npuSpring16_4c0fb[40] = 1.97052e-05;
  npuSpring16_4c0fb[41] = 2.57109e-05;
  npuSpring16_4c0fb[42] = 3.42733e-05;
  npuSpring16_4c0fb[43] = 4.70898e-05;
  npuSpring16_4c0fb[44] = 6.51323e-05;
  npuSpring16_4c0fb[45] = 9.30244e-05;
  npuSpring16_4c0fb[46] = 0.000131517;
  npuSpring16_4c0fb[47] = 0.000188504;
  npuSpring16_4c0fb[48] = 0.000273719;
  npuSpring16_4c0fb[49] = 0.000399681;
  npuSpring16_4c0fb[50] = 0.000627436;
  npuSpring16_4c0fb[51] = 0.00086484;
  npuSpring16_4c0fb[52] = 0.00127551;
  npuSpring16_4c0fb[53] = 0.00211076;
  npuSpring16_4c0fb[54] = 0.0029774;
  npuSpring16_4c0fb[55] = 0.00553063;
  npuSpring16_4c0fb[56] = 0.00668263;
  npuSpring16_4c0fb[57] = 0.0126717;
  npuSpring16_4c0fb[58] = 0.0255447;
  npuSpring16_4c0fb[59] = 0.0274622;
  npuSpring16_4c0fb[60] = 0;
  npuSpring16_4c0fb[61] = 0;
  npuSpring16_4c0fb[62] = 0;
  npuSpring16_4c0fb[63] = 0;
  npuSpring16_4c0fb[64] = 0;
  npuSpring16_4c0fb[65] = 0;
  npuSpring16_4c0fb[66] = 0;
  npuSpring16_4c0fb[67] = 0;
  npuSpring16_4c0fb[68] = 0;
  npuSpring16_4c0fb[69] = 0;
  npuSpring16_4c0fb[70] = 0;
  npuSpring16_4c0fb[71] = 0;
  npuSpring16_4c0fb[72] = 0;
  npuSpring16_4c0fb[73] = 0;
  npuSpring16_4c0fb[74] = 0;
  npuSpring16_4c0fb[75] = 0;
  npuSpring16_4c0fb[76] = 0;
  npuSpring16_4c0fb[77] = 0;
  npuSpring16_4c0fb[78] = 0;
  npuSpring16_4c0fb[79] = 0;
  npuSpring16_4c0fb[80] = 0;
  npuSpring16_4c0fb[81] = 0;
  npuSpring16_4c0fb[82] = 0;
  npuSpring16_4c0fb[83] = 0;
  npuSpring16_4c0fb[84] = 0;
  npuSpring16_4c0fb[85] = 0;
  npuSpring16_4c0fb[86] = 0;
  npuSpring16_4c0fb[87] = 0;
  npuSpring16_4c0fb[88] = 0;
  npuSpring16_4c0fb[89] = 0;
  npuSpring16_4c0fb[90] = 0;
  npuSpring16_4c0fb[91] = 0;
  npuSpring16_4c0fb[92] = 0;
  npuSpring16_4c0fb[93] = 0;
  npuSpring16_4c0fb[94] = 0;
  npuSpring16_4c0fb[95] = 0;
  npuSpring16_4c0fb[96] = 0;
  npuSpring16_4c0fb[97] = 0;
  npuSpring16_4c0fb[98] = 0;
  npuSpring16_4c0fb[99] = 0;
  for(int k = 0 ; k < 100 ; ++k)
    hT2016_MCSpring16_Data4c0fb.push_back(npuSpring16_4c0fb[k]) ;

// RUN2ANALYSIS MC Spring2016 target Data 9.2/fb
  double npuSpring16_9c2fb[100];
  npuSpring16_9c2fb[0] = 7.99397e-06;
  npuSpring16_9c2fb[1] = 0.0001837;
  npuSpring16_9c2fb[2] = 0.000476029;
  npuSpring16_9c2fb[3] = 0.000783864;
  npuSpring16_9c2fb[4] = 0.000862171;
  npuSpring16_9c2fb[5] = 0.000904729;
  npuSpring16_9c2fb[6] = 0.00142236;
  npuSpring16_9c2fb[7] = 0.00522872;
  npuSpring16_9c2fb[8] = 0.0120961;
  npuSpring16_9c2fb[9] = 0.0206096;
  npuSpring16_9c2fb[10] = 0.0271844;
  npuSpring16_9c2fb[11] = 0.0319611;
  npuSpring16_9c2fb[12] = 0.0371995;
  npuSpring16_9c2fb[13] = 0.0420228;
  npuSpring16_9c2fb[14] = 0.0457301;
  npuSpring16_9c2fb[15] = 0.0480489;
  npuSpring16_9c2fb[16] = 0.048993;
  npuSpring16_9c2fb[17] = 0.0484794;
  npuSpring16_9c2fb[18] = 0.0467976;
  npuSpring16_9c2fb[19] = 0.0443421;
  npuSpring16_9c2fb[20] = 0.0409662;
  npuSpring16_9c2fb[21] = 0.0372117;
  npuSpring16_9c2fb[22] = 0.0332663;
  npuSpring16_9c2fb[23] = 0.02881;
  npuSpring16_9c2fb[24] = 0.0239567;
  npuSpring16_9c2fb[25] = 0.0189997;
  npuSpring16_9c2fb[26] = 0.0144513;
  npuSpring16_9c2fb[27] = 0.0105358;
  npuSpring16_9c2fb[28] = 0.00739642;
  npuSpring16_9c2fb[29] = 0.00501586;
  npuSpring16_9c2fb[30] = 0.00325624;
  npuSpring16_9c2fb[31] = 0.00204339;
  npuSpring16_9c2fb[32] = 0.00124549;
  npuSpring16_9c2fb[33] = 0.00073489;
  npuSpring16_9c2fb[34] = 0.000418875;
  npuSpring16_9c2fb[35] = 0.000233045;
  npuSpring16_9c2fb[36] = 0.000124262;
  npuSpring16_9c2fb[37] = 6.5332e-05;
  npuSpring16_9c2fb[38] = 3.5447e-05;
  npuSpring16_9c2fb[39] = 2.1382e-05;
  npuSpring16_9c2fb[40] = 1.60027e-05;
  npuSpring16_9c2fb[41] = 1.5921e-05;
  npuSpring16_9c2fb[42] = 1.92115e-05;
  npuSpring16_9c2fb[43] = 2.58401e-05;
  npuSpring16_9c2fb[44] = 3.60058e-05;
  npuSpring16_9c2fb[45] = 5.22807e-05;
  npuSpring16_9c2fb[46] = 7.53206e-05;
  npuSpring16_9c2fb[47] = 0.000110057;
  npuSpring16_9c2fb[48] = 0.000163025;
  npuSpring16_9c2fb[49] = 0.000242939;
  npuSpring16_9c2fb[50] = 0.000389336;
  npuSpring16_9c2fb[51] = 0.000547988;
  npuSpring16_9c2fb[52] = 0.000825449;
  npuSpring16_9c2fb[53] = 0.00139539;
  npuSpring16_9c2fb[54] = 0.00201104;
  npuSpring16_9c2fb[55] = 0.00381725;
  npuSpring16_9c2fb[56] = 0.00471394;
  npuSpring16_9c2fb[57] = 0.00913689;
  npuSpring16_9c2fb[58] = 0;
  npuSpring16_9c2fb[59] = 0;
  npuSpring16_9c2fb[60] = 0;
  npuSpring16_9c2fb[61] = 0;
  npuSpring16_9c2fb[62] = 0;
  npuSpring16_9c2fb[63] = 0;
  npuSpring16_9c2fb[64] = 0;
  npuSpring16_9c2fb[65] = 0;
  npuSpring16_9c2fb[66] = 0;
  npuSpring16_9c2fb[67] = 0;
  npuSpring16_9c2fb[68] = 0;
  npuSpring16_9c2fb[69] = 0;
  npuSpring16_9c2fb[70] = 0;
  npuSpring16_9c2fb[71] = 0;
  npuSpring16_9c2fb[72] = 0;
  npuSpring16_9c2fb[73] = 0;
  npuSpring16_9c2fb[74] = 0;
  npuSpring16_9c2fb[75] = 0;
  npuSpring16_9c2fb[76] = 0;
  npuSpring16_9c2fb[77] = 0;
  npuSpring16_9c2fb[78] = 0;
  npuSpring16_9c2fb[79] = 0;
  npuSpring16_9c2fb[80] = 0;
  npuSpring16_9c2fb[81] = 0;
  npuSpring16_9c2fb[82] = 0;
  npuSpring16_9c2fb[83] = 0;
  npuSpring16_9c2fb[84] = 0;
  npuSpring16_9c2fb[85] = 0;
  npuSpring16_9c2fb[86] = 0;
  npuSpring16_9c2fb[87] = 0;
  npuSpring16_9c2fb[88] = 0;
  npuSpring16_9c2fb[89] = 0;
  npuSpring16_9c2fb[90] = 0;
  npuSpring16_9c2fb[91] = 0;
  npuSpring16_9c2fb[92] = 0;
  npuSpring16_9c2fb[93] = 0;
  npuSpring16_9c2fb[94] = 0;
  npuSpring16_9c2fb[95] = 0;
  npuSpring16_9c2fb[96] = 0;
  npuSpring16_9c2fb[97] = 0;
  npuSpring16_9c2fb[98] =   0;
  npuSpring16_9c2fb[99] = 0;
  for(int k = 0 ; k < 100 ; ++k)
    hT2016_MCSpring16_Data9c2fb.push_back(npuSpring16_9c2fb[k]) ;

  // RUN2ANALYSIS MC Spring2016 target Data 12.9/fb

  // Old version not synced with other groups (bad MinBias XS?)
  /*double npuSpring16_12c9fb[100];
    npuSpring16_12c9fb[0] = 9.23591e-06;
    npuSpring16_12c9fb[1] = 0.000255013;
    npuSpring16_12c9fb[2] = 0.000547704;
    npuSpring16_12c9fb[3] = 0.000874676;
    npuSpring16_12c9fb[4] = 0.000896299;
    npuSpring16_12c9fb[5] = 0.000985639;
    npuSpring16_12c9fb[6] = 0.00138007;
    npuSpring16_12c9fb[7] = 0.00434416;
    npuSpring16_12c9fb[8] = 0.00979431;
    npuSpring16_12c9fb[9] = 0.0171594;
    npuSpring16_12c9fb[10] = 0.0256192;
    npuSpring16_12c9fb[11] = 0.034788;
    npuSpring16_12c9fb[12] = 0.042003;
    npuSpring16_12c9fb[13] = 0.0466772;
    npuSpring16_12c9fb[14] = 0.0501062;
    npuSpring16_12c9fb[15] = 0.052079;
    npuSpring16_12c9fb[16] = 0.0527129;
    npuSpring16_12c9fb[17] = 0.0519845;
    npuSpring16_12c9fb[18] = 0.0501612;
    npuSpring16_12c9fb[19] = 0.0477159;
    npuSpring16_12c9fb[20] = 0.0444941;
    npuSpring16_12c9fb[21] = 0.0409428;
    npuSpring16_12c9fb[22] = 0.0371571;
    npuSpring16_12c9fb[23] = 0.0327908;
    npuSpring16_12c9fb[24] = 0.0279854;
    npuSpring16_12c9fb[25] = 0.0229831;
    npuSpring16_12c9fb[26] = 0.0182312;
    npuSpring16_12c9fb[27] = 0.013901;
    npuSpring16_12c9fb[28] = 0.0101914;
    npuSpring16_12c9fb[29] = 0.00718466;
    npuSpring16_12c9fb[30] = 0.00481241;
    npuSpring16_12c9fb[31] = 0.00308214;
    npuSpring16_12c9fb[32] = 0.0018929;
    npuSpring16_12c9fb[33] = 0.00111257;
    npuSpring16_12c9fb[34] = 0.000627268;
    npuSpring16_12c9fb[35] = 0.000344694;
    npuSpring16_12c9fb[36] = 0.000182029;
    npuSpring16_12c9fb[37] = 9.49072e-05;
    npuSpring16_12c9fb[38] = 5.03845e-05;
    npuSpring16_12c9fb[39] = 2.83432e-05;
    npuSpring16_12c9fb[40] = 1.83473e-05;
    npuSpring16_12c9fb[41] = 1.53675e-05;
    npuSpring16_12c9fb[42] = 1.64617e-05;
    npuSpring16_12c9fb[43] = 2.09364e-05;
    npuSpring16_12c9fb[44] = 2.85643e-05;
    npuSpring16_12c9fb[45] = 4.1185e-05;
    npuSpring16_12c9fb[46] = 5.9207e-05;
    npuSpring16_12c9fb[47] = 8.64583e-05;
    npuSpring16_12c9fb[48] = 0.000128047;
    npuSpring16_12c9fb[49] = 0.000190806;
    npuSpring16_12c9fb[50] = 0.000305784;
    npuSpring16_12c9fb[51] = 0.000430388;
    npuSpring16_12c9fb[52] = 0.000648305;
    npuSpring16_12c9fb[53] = 0.00109594;
    npuSpring16_12c9fb[54] = 0.00157946;
    npuSpring16_12c9fb[55] = 0.00299806;
    npuSpring16_12c9fb[56] = 0.00370231;
    npuSpring16_12c9fb[57] = 0.00717608;
    npuSpring16_12c9fb[58] = 0;
    npuSpring16_12c9fb[59] = 0;
    npuSpring16_12c9fb[60] = 0;
    npuSpring16_12c9fb[61] = 0;
    npuSpring16_12c9fb[62] = 0;
    npuSpring16_12c9fb[63] = 0;
    npuSpring16_12c9fb[64] = 0;
    npuSpring16_12c9fb[65] = 0;
    npuSpring16_12c9fb[66] = 0;
    npuSpring16_12c9fb[67] = 0;
    npuSpring16_12c9fb[68] = 0;
    npuSpring16_12c9fb[69] = 0;
    npuSpring16_12c9fb[70] = 0;
    npuSpring16_12c9fb[71] = 0;
    npuSpring16_12c9fb[72] = 0;
    npuSpring16_12c9fb[73] = 0;
    npuSpring16_12c9fb[74] = 0;
    npuSpring16_12c9fb[75] = 0;
    npuSpring16_12c9fb[76] = 0;
    npuSpring16_12c9fb[77] = 0;
    npuSpring16_12c9fb[78] = 0;
    npuSpring16_12c9fb[79] = 0;
    npuSpring16_12c9fb[80] = 0;
    npuSpring16_12c9fb[81] = 0;
    npuSpring16_12c9fb[82] = 0;
    npuSpring16_12c9fb[83] = 0;
    npuSpring16_12c9fb[84] = 0;
    npuSpring16_12c9fb[85] = 0;
    npuSpring16_12c9fb[86] = 0;
    npuSpring16_12c9fb[87] = 0;
    npuSpring16_12c9fb[88] = 0;
    npuSpring16_12c9fb[89] = 0;
    npuSpring16_12c9fb[90] = 0;
    npuSpring16_12c9fb[91] = 0;
    npuSpring16_12c9fb[92] = 0;
    npuSpring16_12c9fb[93] = 0;
    npuSpring16_12c9fb[94] = 0;
    npuSpring16_12c9fb[95] = 0;
    npuSpring16_12c9fb[96] = 0;
    npuSpring16_12c9fb[97] = 0;
    npuSpring16_12c9fb[98] = 0;
    npuSpring16_12c9fb[99] = 0;*/

  double npuSpring16_12c9fb[100];
  npuSpring16_12c9fb[0] = 0.000195304;
  npuSpring16_12c9fb[1] = 0.00984055;
  npuSpring16_12c9fb[2] = 0.0139812;
  npuSpring16_12c9fb[3] = 0.02361;
  npuSpring16_12c9fb[4] = 0.0393765;
  npuSpring16_12c9fb[5] = 0.0309974;
  npuSpring16_12c9fb[6] = 0.0335424;
  npuSpring16_12c9fb[7] = 0.0694468;
  npuSpring16_12c9fb[8] = 0.164972;
  npuSpring16_12c9fb[9] = 0.316651;
  npuSpring16_12c9fb[10] = 0.498313;
  npuSpring16_12c9fb[11] = 0.723282;
  npuSpring16_12c9fb[12] = 0.973598;
  npuSpring16_12c9fb[13] = 1.15938;
  npuSpring16_12c9fb[14] = 1.26109;
  npuSpring16_12c9fb[15] = 1.21929;
  npuSpring16_12c9fb[16] = 1.14045;
  npuSpring16_12c9fb[17] = 1.21277;
  npuSpring16_12c9fb[18] = 1.1834;
  npuSpring16_12c9fb[19] = 1.27557;
  npuSpring16_12c9fb[20] = 1.1353;
  npuSpring16_12c9fb[21] = 1.05513;
  npuSpring16_12c9fb[22] = 1.07051;
  npuSpring16_12c9fb[23] = 1.11291;
  npuSpring16_12c9fb[24] = 1.06956;
  npuSpring16_12c9fb[25] = 1.1489;
  npuSpring16_12c9fb[26] = 1.01616;
  npuSpring16_12c9fb[27] = 1.00315;
  npuSpring16_12c9fb[28] = 0.837579;
  npuSpring16_12c9fb[29] = 0.753319;
  npuSpring16_12c9fb[30] = 0.638572;
  npuSpring16_12c9fb[31] = 0.533641;
  npuSpring16_12c9fb[32] = 0.576996;
  npuSpring16_12c9fb[33] = 0.560222;
  npuSpring16_12c9fb[34] = 0.732517;
  npuSpring16_12c9fb[35] = 1.18053;
  npuSpring16_12c9fb[36] = 2.41915;
  npuSpring16_12c9fb[37] = 3.41194;
  npuSpring16_12c9fb[38] = 0;
  npuSpring16_12c9fb[39] = 0;
  npuSpring16_12c9fb[40] = 0;
  npuSpring16_12c9fb[41] = 0;
  npuSpring16_12c9fb[42] = 0;
  npuSpring16_12c9fb[43] = 0;
  npuSpring16_12c9fb[44] = 0;
  npuSpring16_12c9fb[45] = 0;
  npuSpring16_12c9fb[46] = 0;
  npuSpring16_12c9fb[47] = 0;
  npuSpring16_12c9fb[48] = 0;
  npuSpring16_12c9fb[49] = 0;
  npuSpring16_12c9fb[50] = 0;
  npuSpring16_12c9fb[51] = 0;
  npuSpring16_12c9fb[52] = 0;
  npuSpring16_12c9fb[53] = 0;
  npuSpring16_12c9fb[54] = 0;
  npuSpring16_12c9fb[55] = 0;
  npuSpring16_12c9fb[56] = 0;
  npuSpring16_12c9fb[57] = 0;
  npuSpring16_12c9fb[58] = 0;
  npuSpring16_12c9fb[59] = 0;
  npuSpring16_12c9fb[60] = 0;
  npuSpring16_12c9fb[61] = 0;
  npuSpring16_12c9fb[62] = 0;
  npuSpring16_12c9fb[63] = 0;
  npuSpring16_12c9fb[64] = 0;
  npuSpring16_12c9fb[65] = 0;
  npuSpring16_12c9fb[66] = 0;
  npuSpring16_12c9fb[67] = 0;
  npuSpring16_12c9fb[68] = 0;
  npuSpring16_12c9fb[69] = 0;
  npuSpring16_12c9fb[70] = 0;
  npuSpring16_12c9fb[71] = 0;
  npuSpring16_12c9fb[72] = 0;
  npuSpring16_12c9fb[73] = 0;
  npuSpring16_12c9fb[74] = 0;
  npuSpring16_12c9fb[75] = 0;
  npuSpring16_12c9fb[76] = 0;
  npuSpring16_12c9fb[77] = 0;
  npuSpring16_12c9fb[78] = 0;
  npuSpring16_12c9fb[79] = 0;
  npuSpring16_12c9fb[80] = 0;
  npuSpring16_12c9fb[81] = 0;
  npuSpring16_12c9fb[82] = 0;
  npuSpring16_12c9fb[83] = 0;
  npuSpring16_12c9fb[84] = 0;
  npuSpring16_12c9fb[85] = 0;
  npuSpring16_12c9fb[86] = 0;
  npuSpring16_12c9fb[87] = 0;
  npuSpring16_12c9fb[88] = 0;
  npuSpring16_12c9fb[89] = 0;
  npuSpring16_12c9fb[90] = 0;
  npuSpring16_12c9fb[91] = 0;
  npuSpring16_12c9fb[92] = 0;
  npuSpring16_12c9fb[93] = 0;
  npuSpring16_12c9fb[94] = 0;
  npuSpring16_12c9fb[95] = 0;
  npuSpring16_12c9fb[96] = 0;
  npuSpring16_12c9fb[97] = 0;
  npuSpring16_12c9fb[98] = 0;
  npuSpring16_12c9fb[99] = 0;

  for(int k = 0 ; k < 100 ; ++k)
    hT2016_MCSpring16_Data12c9fb.push_back(npuSpring16_12c9fb[k]) ;

  // RUN2ANALYSIS MC Spring2016 target Data 36.4/fb

  double npuSpring16_36c4fb[100];
  npuSpring16_36c4fb[0] = 0.00790511;
  npuSpring16_36c4fb[1] = 0.0182519;
  npuSpring16_36c4fb[2] = 0.018532;
  npuSpring16_36c4fb[3] = 0.0209577;
  npuSpring16_36c4fb[4] = 0.0320782;
  npuSpring16_36c4fb[5] = 0.0248311;
  npuSpring16_36c4fb[6] = 0.0232158;
  npuSpring16_36c4fb[7] = 0.0372245;
  npuSpring16_36c4fb[8] = 0.0702474;
  npuSpring16_36c4fb[9] = 0.12634;
  npuSpring16_36c4fb[10] = 0.228679;
  npuSpring16_36c4fb[11] = 0.400977;
  npuSpring16_36c4fb[12] = 0.589868;
  npuSpring16_36c4fb[13] = 0.744094;
  npuSpring16_36c4fb[14] = 0.867251;
  npuSpring16_36c4fb[15] = 0.893295;
  npuSpring16_36c4fb[16] = 0.86212;
  npuSpring16_36c4fb[17] = 0.922547;
  npuSpring16_36c4fb[18] = 0.898629;
  npuSpring16_36c4fb[19] = 0.972743;
  npuSpring16_36c4fb[20] = 0.886346;
  npuSpring16_36c4fb[21] = 0.862668;
  npuSpring16_36c4fb[22] = 0.928278;
  npuSpring16_36c4fb[23] = 1.02934;
  npuSpring16_36c4fb[24] = 1.06564;
  npuSpring16_36c4fb[25] = 1.25514;
  npuSpring16_36c4fb[26] = 1.24672;
  npuSpring16_36c4fb[27] = 1.42187;
  npuSpring16_36c4fb[28] = 1.41333;
  npuSpring16_36c4fb[29] = 1.55807;
  npuSpring16_36c4fb[30] = 1.66432;
  npuSpring16_36c4fb[31] = 1.80189;
  npuSpring16_36c4fb[32] = 2.59852;
  npuSpring16_36c4fb[33] = 3.46815;
  npuSpring16_36c4fb[34] = 6.42475;
  npuSpring16_36c4fb[35] = 15.1079;
  npuSpring16_36c4fb[36] = 46.4544;
  npuSpring16_36c4fb[37] = 100.782;
  npuSpring16_36c4fb[38] = 0;
  npuSpring16_36c4fb[39] = 0;
  npuSpring16_36c4fb[40] = 0;
  npuSpring16_36c4fb[41] = 0;
  npuSpring16_36c4fb[42] = 0;
  npuSpring16_36c4fb[43] = 0;
  npuSpring16_36c4fb[44] = 0;
  npuSpring16_36c4fb[45] = 0;
  npuSpring16_36c4fb[46] = 0;
  npuSpring16_36c4fb[47] = 0;
  npuSpring16_36c4fb[48] = 0;
  npuSpring16_36c4fb[49] = 0;
  npuSpring16_36c4fb[50] = 0;
  npuSpring16_36c4fb[51] = 0;
  npuSpring16_36c4fb[52] = 0;
  npuSpring16_36c4fb[53] = 0;
  npuSpring16_36c4fb[54] = 0;
  npuSpring16_36c4fb[55] = 0;
  npuSpring16_36c4fb[56] = 0;
  npuSpring16_36c4fb[57] = 0;
  npuSpring16_36c4fb[58] = 0;
  npuSpring16_36c4fb[59] = 0;
  npuSpring16_36c4fb[60] = 0;
  npuSpring16_36c4fb[61] = 0;
  npuSpring16_36c4fb[62] = 0;
  npuSpring16_36c4fb[63] = 0;
  npuSpring16_36c4fb[64] = 0;
  npuSpring16_36c4fb[65] = 0;
  npuSpring16_36c4fb[66] = 0;
  npuSpring16_36c4fb[67] = 0;
  npuSpring16_36c4fb[68] = 0;
  npuSpring16_36c4fb[69] = 0;
  npuSpring16_36c4fb[70] = 0;
  npuSpring16_36c4fb[71] = 0;
  npuSpring16_36c4fb[72] = 0;
  npuSpring16_36c4fb[73] = 0;
  npuSpring16_36c4fb[74] = 0;
  npuSpring16_36c4fb[75] = 0;
  npuSpring16_36c4fb[76] = 0;
  npuSpring16_36c4fb[77] = 0;
  npuSpring16_36c4fb[78] = 0;
  npuSpring16_36c4fb[79] = 0;
  npuSpring16_36c4fb[80] = 0;
  npuSpring16_36c4fb[81] = 0;
  npuSpring16_36c4fb[82] = 0;
  npuSpring16_36c4fb[83] = 0;
  npuSpring16_36c4fb[84] = 0;
  npuSpring16_36c4fb[85] = 0;
  npuSpring16_36c4fb[86] = 0;
  npuSpring16_36c4fb[87] = 0;
  npuSpring16_36c4fb[88] = 0;
  npuSpring16_36c4fb[89] = 0;
  npuSpring16_36c4fb[90] = 0;
  npuSpring16_36c4fb[91] = 0;
  npuSpring16_36c4fb[92] = 0;
  npuSpring16_36c4fb[93] = 0;
  npuSpring16_36c4fb[94] = 0;
  npuSpring16_36c4fb[95] = 0;
  npuSpring16_36c4fb[96] = 0;
  npuSpring16_36c4fb[97] = 0;
  npuSpring16_36c4fb[98] = 0;
  npuSpring16_36c4fb[99] = 0;

  for(int k = 0 ; k < 100 ; ++k)
    hT2016_MCSpring16_Data36c4fb.push_back(npuSpring16_36c4fb[k]) ;


  // Run II analysis - Summer16 MC - full 2016 dataset (~36/fb)
  hT2016_MCSummer16_Data36c4fb.resize(100);
  hT2016_MCSummer16_Data36c4fb[0] = 0.00894537;
  hT2016_MCSummer16_Data36c4fb[1] = 0.023167;
  hT2016_MCSummer16_Data36c4fb[2] = 0.0298831;
  hT2016_MCSummer16_Data36c4fb[3] = 0.0240736;
  hT2016_MCSummer16_Data36c4fb[4] = 0.0280743;
  hT2016_MCSummer16_Data36c4fb[5] = 0.0291664;
  hT2016_MCSummer16_Data36c4fb[6] = 0.0196959;
  hT2016_MCSummer16_Data36c4fb[7] = 0.0123776;
  hT2016_MCSummer16_Data36c4fb[8] = 0.0185592;
  hT2016_MCSummer16_Data36c4fb[9] = 0.0221088;
  hT2016_MCSummer16_Data36c4fb[10] = 0.0241107;
  hT2016_MCSummer16_Data36c4fb[11] = 0.0268053;
  hT2016_MCSummer16_Data36c4fb[12] = 0.0281658;
  hT2016_MCSummer16_Data36c4fb[13] = 0.0294374;
  hT2016_MCSummer16_Data36c4fb[14] = 0.0300953;
  hT2016_MCSummer16_Data36c4fb[15] = 0.030248;
  hT2016_MCSummer16_Data36c4fb[16] = 0.030042;
  hT2016_MCSummer16_Data36c4fb[17] = 0.0296216;
  hT2016_MCSummer16_Data36c4fb[18] = 0.0286588;
  hT2016_MCSummer16_Data36c4fb[19] = 0.0274544;
  hT2016_MCSummer16_Data36c4fb[20] = 0.0266721;
  hT2016_MCSummer16_Data36c4fb[21] = 0.0263141;
  hT2016_MCSummer16_Data36c4fb[22] = 0.0263468;
  hT2016_MCSummer16_Data36c4fb[23] = 0.0263206;
  hT2016_MCSummer16_Data36c4fb[24] = 0.0262594;
  hT2016_MCSummer16_Data36c4fb[25] = 0.0264999;
  hT2016_MCSummer16_Data36c4fb[26] = 0.0268332;
  hT2016_MCSummer16_Data36c4fb[27] = 0.0271126;
  hT2016_MCSummer16_Data36c4fb[28] = 0.0274069;
  hT2016_MCSummer16_Data36c4fb[29] = 0.0277553;
  hT2016_MCSummer16_Data36c4fb[30] = 0.0273923;
  hT2016_MCSummer16_Data36c4fb[31] = 0.0271316;
  hT2016_MCSummer16_Data36c4fb[32] = 0.026088;
  hT2016_MCSummer16_Data36c4fb[33] = 0.0246702;
  hT2016_MCSummer16_Data36c4fb[34] = 0.0227779;
  hT2016_MCSummer16_Data36c4fb[35] = 0.0205672;
  hT2016_MCSummer16_Data36c4fb[36] = 0.0179364;
  hT2016_MCSummer16_Data36c4fb[37] = 0.0152783;
  hT2016_MCSummer16_Data36c4fb[38] = 0.0125975;
  hT2016_MCSummer16_Data36c4fb[39] = 0.0101432;
  hT2016_MCSummer16_Data36c4fb[40] = 0.00774467;
  hT2016_MCSummer16_Data36c4fb[41] = 0.0057138;
  hT2016_MCSummer16_Data36c4fb[42] = 0.00409726;
  hT2016_MCSummer16_Data36c4fb[43] = 0.00283222;
  hT2016_MCSummer16_Data36c4fb[44] = 0.00193442;
  hT2016_MCSummer16_Data36c4fb[45] = 0.00127609;
  hT2016_MCSummer16_Data36c4fb[46] = 0.000797952;
  hT2016_MCSummer16_Data36c4fb[47] = 0.000503393;
  hT2016_MCSummer16_Data36c4fb[48] = 0.000306307;
  hT2016_MCSummer16_Data36c4fb[49] = 0.000185814;
  hT2016_MCSummer16_Data36c4fb[50] = 0.000109825;
  hT2016_MCSummer16_Data36c4fb[51] = 6.52416e-05;
  hT2016_MCSummer16_Data36c4fb[52] = 3.93734e-05;
  hT2016_MCSummer16_Data36c4fb[53] = 2.43154e-05;
  hT2016_MCSummer16_Data36c4fb[54] = 1.82969e-05;
  hT2016_MCSummer16_Data36c4fb[55] = 1.68709e-05;
  hT2016_MCSummer16_Data36c4fb[56] = 1.83602e-05;
  hT2016_MCSummer16_Data36c4fb[57] = 2.36165e-05;
  hT2016_MCSummer16_Data36c4fb[58] = 3.35728e-05;
  hT2016_MCSummer16_Data36c4fb[59] = 4.6298e-05;
  hT2016_MCSummer16_Data36c4fb[60] = 7.93232e-05;
  hT2016_MCSummer16_Data36c4fb[61] = 9.8746e-05;
  hT2016_MCSummer16_Data36c4fb[62] = 0.000121905;
  hT2016_MCSummer16_Data36c4fb[63] = 0.000129194;
  hT2016_MCSummer16_Data36c4fb[64] = 0.000135008;
  hT2016_MCSummer16_Data36c4fb[65] = 0.000139488;
  hT2016_MCSummer16_Data36c4fb[66] = 0.000124604;
  hT2016_MCSummer16_Data36c4fb[67] = 0.0001077;
  hT2016_MCSummer16_Data36c4fb[68] = 9.67475e-05;
  hT2016_MCSummer16_Data36c4fb[69] = 8.67088e-05;
  hT2016_MCSummer16_Data36c4fb[70] = 7.77805e-05;
  hT2016_MCSummer16_Data36c4fb[71] = 6.93644e-05;
  hT2016_MCSummer16_Data36c4fb[72] = 5.83042e-05;
  hT2016_MCSummer16_Data36c4fb[73] = 4.97382e-05;
  hT2016_MCSummer16_Data36c4fb[74] = 4.35502e-05;
  hT2016_MCSummer16_Data36c4fb[75] = 0;
  hT2016_MCSummer16_Data36c4fb[76] = 0;
  hT2016_MCSummer16_Data36c4fb[77] = 0;
  hT2016_MCSummer16_Data36c4fb[78] = 0;
  hT2016_MCSummer16_Data36c4fb[79] = 0;
  hT2016_MCSummer16_Data36c4fb[80] = 0;
  hT2016_MCSummer16_Data36c4fb[81] = 0;
  hT2016_MCSummer16_Data36c4fb[82] = 0;
  hT2016_MCSummer16_Data36c4fb[83] = 0;
  hT2016_MCSummer16_Data36c4fb[84] = 0;
  hT2016_MCSummer16_Data36c4fb[85] = 0;
  hT2016_MCSummer16_Data36c4fb[86] = 0;
  hT2016_MCSummer16_Data36c4fb[87] = 0;
  hT2016_MCSummer16_Data36c4fb[88] = 0;
  hT2016_MCSummer16_Data36c4fb[89] = 0;
  hT2016_MCSummer16_Data36c4fb[90] = 0;
  hT2016_MCSummer16_Data36c4fb[91] = 0;
  hT2016_MCSummer16_Data36c4fb[92] = 0;
  hT2016_MCSummer16_Data36c4fb[93] = 0;
  hT2016_MCSummer16_Data36c4fb[94] = 0;
  hT2016_MCSummer16_Data36c4fb[95] = 0;
  hT2016_MCSummer16_Data36c4fb[96] = 0;
  hT2016_MCSummer16_Data36c4fb[97] = 0;
  hT2016_MCSummer16_Data36c4fb[98] = 0;
  hT2016_MCSummer16_Data36c4fb[99] = 0;

  // RUN2ANALYSIS MC 25ns target Data25ns
  // from https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData#2015_Pileup_JSON_Files 13-10-2015 done with mb=69mb Lumi=1260
  double npuWinter15_25ns[52] ;
  npuWinter15_25ns[0] = 0.09672924 ;
  npuWinter15_25ns[1] = 0.6018639 ;
  npuWinter15_25ns[2] = 0.1982618 ;
  npuWinter15_25ns[3] = 0.04426294 ;
  npuWinter15_25ns[4] = 0.01555679 ;
  npuWinter15_25ns[5] = 0.002250036 ;
  npuWinter15_25ns[6] = 0.001632626 ;
  npuWinter15_25ns[7] = 0.003994533 ;
  npuWinter15_25ns[8] = 0.00634282 ;
  npuWinter15_25ns[9] = 0.006385341 ;
  npuWinter15_25ns[10] = 0.006274029 ;
  npuWinter15_25ns[11] = 0.005998706 ;
  npuWinter15_25ns[12] = 0.004751215 ;
  npuWinter15_25ns[13] = 0.003026687 ;
  npuWinter15_25ns[14] = 0.001546196 ;
  npuWinter15_25ns[15] = 0.0006348245 ;
  npuWinter15_25ns[16] = 0.0002329903 ;
  npuWinter15_25ns[17] = 0.0001029755 ;
  npuWinter15_25ns[18] = 6.678185e-05 ;
  npuWinter15_25ns[19] = 4.525077e-05 ;
  npuWinter15_25ns[20] = 2.500376e-05 ;
  npuWinter15_25ns[21] = 1.071974e-05 ;
  npuWinter15_25ns[22] = 3.512034e-06 ;
  npuWinter15_25ns[23] = 8.834002e-07 ;
  npuWinter15_25ns[24] = 1.682074e-07 ;
  npuWinter15_25ns[25] = 2.370267e-08 ;
  npuWinter15_25ns[26] = 2.504465e-09 ;
  npuWinter15_25ns[27] = 1.951381e-10 ;
  npuWinter15_25ns[28] = 1.149427e-11 ;
  npuWinter15_25ns[29] = 4.720787e-13 ;
  npuWinter15_25ns[30] = 1.194543e-14 ;
  npuWinter15_25ns[31] = 2.021363e-16 ;
  npuWinter15_25ns[32] = 1.841988e-18 ;
  npuWinter15_25ns[33] = 0 ;
  npuWinter15_25ns[34] = 0 ;
  npuWinter15_25ns[35] = 0 ;
  npuWinter15_25ns[36] = 0 ;
  npuWinter15_25ns[37] = 0 ;
  npuWinter15_25ns[38] = 0 ;
  npuWinter15_25ns[39] = 0 ;
  npuWinter15_25ns[40] = 0 ;
  npuWinter15_25ns[41] = 0 ;
  npuWinter15_25ns[42] = 0 ;
  npuWinter15_25ns[43] = 0 ;
  npuWinter15_25ns[44] = 0 ;
  npuWinter15_25ns[45] = 0 ;
  npuWinter15_25ns[46] = 0 ;
  npuWinter15_25ns[47] = 0 ;
  npuWinter15_25ns[48] = 0 ;
  npuWinter15_25ns[49] = 0 ;
  npuWinter15_25ns[50] = 0 ;
  npuWinter15_25ns[51] = 0 ;

  // from https://twiki.cern.ch/twiki/bin/view/CMS/ExoDiBosonResonancesRun2#PU_weights old recipe 27-10-2015 done with mb=80mb
  /*
    double npuWinter15_25ns[52] ;
    npuWinter15_25ns[0] = 4.8551E-07;
    npuWinter15_25ns[1] = 1.74806E-06;
    npuWinter15_25ns[2] = 3.30868E-06;
    npuWinter15_25ns[3] = 1.62972E-05;
    npuWinter15_25ns[4] = 4.95667E-05;
    npuWinter15_25ns[5] = 0.000606966;
    npuWinter15_25ns[6] = 0.003307249;
    npuWinter15_25ns[7] = 0.010340741;
    npuWinter15_25ns[8] = 0.022852296;
    npuWinter15_25ns[9] = 0.041948781;
    npuWinter15_25ns[10] = 0.058609363;
    npuWinter15_25ns[11] = 0.067475755;
    npuWinter15_25ns[12] = 0.072817826;
    npuWinter15_25ns[13] = 0.075931405;
    npuWinter15_25ns[14] = 0.076782504;
    npuWinter15_25ns[15] = 0.076202319;
    npuWinter15_25ns[16] = 0.074502547;
    npuWinter15_25ns[17] = 0.072355135;
    npuWinter15_25ns[18] = 0.069642102;
    npuWinter15_25ns[19] = 0.064920999;
    npuWinter15_25ns[20] = 0.05725576;
    npuWinter15_25ns[21] = 0.047289348;
    npuWinter15_25ns[22] = 0.036528446;
    npuWinter15_25ns[23] = 0.026376131;
    npuWinter15_25ns[24] = 0.017806872;
    npuWinter15_25ns[25] = 0.011249422;
    npuWinter15_25ns[26] = 0.006643385;
    npuWinter15_25ns[27] = 0.003662904;
    npuWinter15_25ns[28] = 0.001899681;
    npuWinter15_25ns[29] = 0.00095614;
    npuWinter15_25ns[30] = 0.00050028;
    npuWinter15_25ns[31] = 0.000297353;
    npuWinter15_25ns[32] = 0.000208717;
    npuWinter15_25ns[33] = 0.000165856;
    npuWinter15_25ns[34] = 0.000139974;
    npuWinter15_25ns[35] = 0.000120481;
    npuWinter15_25ns[36] = 0.000103826;
    npuWinter15_25ns[37] = 8.88868E-05;
    npuWinter15_25ns[38] = 7.53323E-05;
    npuWinter15_25ns[39] = 6.30863E-05;
    npuWinter15_25ns[40] = 5.21356E-05;
    npuWinter15_25ns[41] = 4.24754E-05;
    npuWinter15_25ns[42] = 3.40876E-05;
    npuWinter15_25ns[43] = 2.69282E-05;
    npuWinter15_25ns[44] = 2.09267E-05;
    npuWinter15_25ns[45] = 1.5989E-05;
    npuWinter15_25ns[46] = 4.8551E-06;
    npuWinter15_25ns[47] = 2.42755E-06;
    npuWinter15_25ns[48] = 4.8551E-07;
    npuWinter15_25ns[49] = 2.42755E-07;
    npuWinter15_25ns[50] = 1.21378E-07;
    npuWinter15_25ns[51] = 4.8551E-08;
  */
  for(int k = 0 ; k < 52 ; ++k)
    hT2015_MC25ns_Data25ns.push_back(npuWinter15_25ns[k]) ;

  // RUN2ANALYSIS  MC 25ns target Data50ns
  for(int k = 0 ; k < 300 ; ++k)
    hT2015_MC25ns_Data50ns.push_back(1.) ;

  // RUN2ANALYSIS  MC 50ns target Data25ns
  for(int k = 0 ; k < 300 ; ++k)
    hT2015_MC50ns_Data25ns.push_back(1.) ;

  // RUN2ANALYSIS  MC 50ns target Data50ns
  for(int k = 0 ; k < 300 ; ++k)
    hT2015_MC50ns_Data50ns.push_back(1.) ;

  // RUN1ANALYSIS 2011 taken from HZZ4l FIXME _check_
  float w2011[50];
  w2011[0]= 0.212929;
  w2011[1]= 0.0208114;
  w2011[2]= 0.0584048;
  w2011[3]= 0.538898;
  w2011[4]= 1.357;
  w2011[5]= 1.49913;
  w2011[6]= 1.42247;
  w2011[7]= 1.35904;
  w2011[8]= 1.29946;
  w2011[9]= 1.27925;
  w2011[10]= 1.37845;
  w2011[11]= 1.71246;
  w2011[12]= 1.5291;
  w2011[13]= 1.35234;
  w2011[14]= 1.22215;
  w2011[15]= 1.0155;
  w2011[16]= 1.01137;
  w2011[17]= 0.395465;
  w2011[18]= 0.230984;
  w2011[19]= 0.109883;
  w2011[20]= 0.0433739;
  w2011[21]= 0.0111497;
  w2011[22]= 0.00408801;
  w2011[23]= 0.00115678;
  w2011[24]= 0.000365505;
  w2011[25]= 0.000112391;
  w2011[26]= 3.83894e-05;
  w2011[27]= 1.60651e-05;
  w2011[28]= 4.81412e-06;
  w2011[29]= 1.39717e-06;
  w2011[30]= 1.92368e-06;
  w2011[31]= 4.10748e-06;
  w2011[32]= 2.33157e-05;
  w2011[33]= 4.0181e-05;
  w2011[34]= 4.87786e-05;
  w2011[35]= 0.00194128;
  w2011[36]= 8.97414e-05;
  w2011[37]= 1;
  w2011[38]= 1;
  w2011[39]= 0.000162709;
  w2011[40]= 1;
  w2011[41]= 1;
  w2011[42]= 1;
  w2011[43]= 1;
  w2011[44]= 1;
  w2011[45]= 1;
  w2011[46]= 1;
  w2011[47]= 1;
  w2011[48]= 1;
  w2011[49]= 1;
  for(int k=0 ;k<50 ;++k)
    hT2011.push_back(w2011[k]) ;

  // RUN1ANALYSIS 2012 FIXME taken from HZZ4l FIXME _check_
  float w2012[240];
  w2012[0]= 1;
  w2012[1]= 1;
  w2012[2]= 0.222451;
  w2012[3]= 0.0658851;
  w2012[4]= 1;
  w2012[5]= 1;
  w2012[6]= 0.150902;
  w2012[7]= 0.202205;
  w2012[8]= 1;
  w2012[9]= 1;
  w2012[10]= 1.50116;
  w2012[11]= 2.79375;
  w2012[12]= 0.198341;
  w2012[13]= 0.246893;
  w2012[14]= 0.28116;
  w2012[15]= 0.449377;
  w2012[16]= 0.553276;
  w2012[17]= 1.48919;
  w2012[18]= 2.15249;
  w2012[19]= 3.62415;
  w2012[20]= 4.33041;
  w2012[21]= 3.57192;
  w2012[22]= 4.99603;
  w2012[23]= 7.79303;
  w2012[24]= 8.04276;
  w2012[25]= 8.05557;
  w2012[26]= 12.9364;
  w2012[27]= 9.9036;
  w2012[28]= 14.6975;
  w2012[29]= 13.3387;
  w2012[30]= 10.9734;
  w2012[31]= 12.6077;
  w2012[32]= 11.5617;
  w2012[33]= 10.8107;
  w2012[34]= 14.5043;
  w2012[35]= 17.8497;
  w2012[36]= 11.8817;
  w2012[37]= 9.6805;
  w2012[38]= 12.2255;
  w2012[39]= 10.1117;
  w2012[40]= 10.2482;
  w2012[41]= 11.5398;
  w2012[42]= 9.35737;
  w2012[43]= 9.90259;
  w2012[44]= 9.19216;
  w2012[45]= 7.57377;
  w2012[46]= 7.94847;
  w2012[47]= 7.15578;
  w2012[48]= 5.63016;
  w2012[49]= 5.35972;
  w2012[50]= 5.05791;
  w2012[51]= 3.35313;
  w2012[52]= 3.60582;
  w2012[53]= 3.35256;
  w2012[54]= 2.49496;
  w2012[55]= 2.28219;
  w2012[56]= 2.21227;
  w2012[57]= 1.76362;
  w2012[58]= 1.68533;
  w2012[59]= 1.62149;
  w2012[60]= 1.34263;
  w2012[61]= 1.30646;
  w2012[62]= 1.21918;
  w2012[63]= 1.10347;
  w2012[64]= 1.08544;
  w2012[65]= 1.0251;
  w2012[66]= 0.907123;
  w2012[67]= 0.905997;
  w2012[68]= 0.869217;
  w2012[69]= 0.816708;
  w2012[70]= 0.76043;
  w2012[71]= 0.714367;
  w2012[72]= 0.679723;
  w2012[73]= 0.665294;
  w2012[74]= 0.609956;
  w2012[75]= 0.586386;
  w2012[76]= 0.548999;
  w2012[77]= 0.521088;
  w2012[78]= 0.4929;
  w2012[79]= 0.453545;
  w2012[80]= 0.44546;
  w2012[81]= 0.406266;
  w2012[82]= 0.378486;
  w2012[83]= 0.347898;
  w2012[84]= 0.337097;
  w2012[85]= 0.313674;
  w2012[86]= 0.291392;
  w2012[87]= 0.283346;
  w2012[88]= 0.25272;
  w2012[89]= 0.244178;
  w2012[90]= 0.228673;
  w2012[91]= 0.211327;
  w2012[92]= 0.19084;
  w2012[93]= 0.179408;
  w2012[94]= 0.169234;
  w2012[95]= 0.157131;
  w2012[96]= 0.143818;
  w2012[97]= 0.140968;
  w2012[98]= 0.124021;
  w2012[99]= 0.118273;
  w2012[100]= 0.109751;
  w2012[101]= 0.0977754;
  w2012[102]= 0.0967206;
  w2012[103]= 0.0870401;
  w2012[104]= 0.0826372;
  w2012[105]= 0.0746777;
  w2012[106]= 0.0698592;
  w2012[107]= 0.0656062;
  w2012[108]= 0.0601853;
  w2012[109]= 0.057892;
  w2012[110]= 0.0517871;
  w2012[111]= 0.0512109;
  w2012[112]= 0.0465423;
  w2012[113]= 0.0403982;
  w2012[114]= 0.0443631;
  w2012[115]= 0.0399185;
  w2012[116]= 0.0338933;
  w2012[117]= 0.0354274;
  w2012[118]= 0.0310775;
  w2012[119]= 0.0261122;
  w2012[120]= 0.0280878;
  w2012[121]= 0.0264014;
  w2012[122]= 0.021299;
  w2012[123]= 0.0245197;
  w2012[124]= 0.0221076;
  w2012[125]= 0.0189236;
  w2012[126]= 0.0202148;
  w2012[127]= 0.0177248;
  w2012[128]= 0.0163634;
  w2012[129]= 0.0188307;
  w2012[130]= 0.0144512;
  w2012[131]= 0.0134599;
  w2012[132]= 0.0143315;
  w2012[133]= 0.0130668;
  w2012[134]= 0.0108666;
  w2012[135]= 0.0162516;
  w2012[136]= 0.0126035;
  w2012[137]= 0.0102154;
  w2012[138]= 0.0154442;
  w2012[139]= 0.00959973;
  w2012[140]= 0.0106827;
  w2012[141]= 0.0146624;
  w2012[142]= 0.0155156;
  w2012[143]= 0.00761674;
  w2012[144]= 0.0187999;
  w2012[145]= 0.0135013;
  w2012[146]= 0.0160794;
  w2012[147]= 0.0180586;
  w2012[148]= 0.00905508;
  w2012[149]= 0.00869858;
  w2012[150]= 0.0193968;
  w2012[151]= 0.0209201;
  w2012[152]= 0.0084405;
  w2012[153]= 0.0407657;
  w2012[154]= 0.0109116;
  w2012[155]= 0.0262218;
  w2012[156]= 0.0104767;
  w2012[157]= 1;
  w2012[158]= 1;
  w2012[159]= 1;
  w2012[160]= 1;
  w2012[161]= 0.00658031;
  w2012[162]= 0.0051814;
  w2012[163]= 1;
  w2012[164]= 1;
  w2012[165]= 1;
  w2012[166]= 1;
  w2012[167]= 1;
  w2012[168]= 1;
  w2012[169]= 1;
  w2012[170]= 1;
  w2012[171]= 1;
  w2012[172]= 1;
  w2012[173]= 1;
  w2012[174]= 1;
  w2012[175]= 1;
  w2012[176]= 1;
  w2012[177]= 1;
  w2012[178]= 1;
  w2012[179]= 1;
  w2012[180]= 1;
  w2012[181]= 1;
  w2012[182]= 1;
  w2012[183]= 1;
  w2012[184]= 1;
  w2012[185]= 1;
  w2012[186]= 1;
  w2012[187]= 1;
  w2012[188]= 1;
  w2012[189]= 1;
  w2012[190]= 1;
  w2012[191]= 1;
  w2012[192]= 1;
  w2012[193]= 1;
  w2012[194]= 1;
  w2012[195]= 1;
  w2012[196]= 1;
  w2012[197]= 1;
  w2012[198]= 1;
  w2012[199]= 1;
  w2012[200]= 1;
  w2012[201]= 1;
  w2012[202]= 1;
  w2012[203]= 1;
  w2012[204]= 1;
  w2012[205]= 1;
  w2012[206]= 1;
  w2012[207]= 1;
  w2012[208]= 1;
  w2012[209]= 1;
  w2012[210]= 1;
  w2012[211]= 1;
  w2012[212]= 1;
  w2012[213]= 1;
  w2012[214]= 1;
  w2012[215]= 1;
  w2012[216]= 1;
  w2012[217]= 1;
  w2012[218]= 1;
  w2012[219]= 1;
  w2012[220]= 1;
  w2012[221]= 1;
  w2012[222]= 1;
  w2012[223]= 1;
  w2012[224]= 1;
  w2012[225]= 1;
  w2012[226]= 1;
  w2012[227]= 1;
  w2012[228]= 1;
  w2012[229]= 1;
  w2012[230]= 1;
  w2012[231]= 1;
  w2012[232]= 1;
  w2012[233]= 1;
  w2012[234]= 1;
  w2012[235]= 1;
  w2012[236]= 1;
  w2012[237]= 1;
  w2012[238]= 1;
  w2012[239]= 1;
  for(int k=0 ;k<240 ;k++)
    hT2012.push_back(w2012[k]) ;
  }

PUReweight::~PUReweight() {}
