#include "../interface/StatTools.h"

// Can directly call with different years with the command:
// root -l printDYsyst.C'("2017")'
void printDYsyst (string const &myYearName = "2016")
{

  std::cout << " ** DEBUG: year: " << myYearName << std::endl;

  // General names of the DY split samples
  //std::vector<std::string> DYsamples = {
  //  "DY_0b_0JPt", "DY_0b_10JPt", "DY_0b_50JPt", "DY_0b_80JPt", "DY_0b_110JPt", "DY_0b_190JPt",
  //  "DY_1b_0JPt", "DY_1b_10JPt", "DY_1b_50JPt", "DY_1b_80JPt", "DY_1b_110JPt", "DY_1b_190JPt",
  //  "DY_2b_0JPt", "DY_2b_10JPt", "DY_2b_50JPt", "DY_2b_80JPt", "DY_2b_110JPt", "DY_2b_190JPt"
  //};
  //if (myYearName == "2017" || myYearName == "2018")
  //{
  //  DYsamples = {
  //  "DY_0b_0JPt", "DY_0b_10JPt", "DY_0b_30JPt", "DY_0b_50JPt", "DY_0b_100JPt", "DY_0b_200JPt",
  //  "DY_1b_0JPt", "DY_1b_10JPt", "DY_1b_30JPt", "DY_1b_50JPt", "DY_1b_100JPt", "DY_1b_200JPt",
  //  "DY_2b_0JPt", "DY_2b_10JPt", "DY_2b_30JPt", "DY_2b_50JPt", "DY_2b_100JPt", "DY_2b_200JPt"
  //  };
  //}
  // Use same names for all three years
  std::vector<std::string> DYsamples = {
    "DY_0b_1Pt", "DY_0b_2Pt", "DY_0b_3Pt", "DY_0b_4Pt", "DY_0b_5Pt", "DY_0b_6Pt",
    "DY_1b_1Pt", "DY_1b_2Pt", "DY_1b_3Pt", "DY_1b_4Pt", "DY_1b_5Pt", "DY_1b_6Pt",
    "DY_2b_1Pt", "DY_2b_2Pt", "DY_2b_3Pt", "DY_2b_4Pt", "DY_2b_5Pt", "DY_2b_6Pt"
  };

  // Read input file
  string fileName = Form("../weights/DY_SF_Legacy/DY_Scale_factor_LO_NbjetBins_ptBins_mtt_%s_prod_v6.root",myYearName.c_str());
  std::cout << " ** DEBUG: fileName: " << fileName << std::endl;

  //TFile* dy_file  = TFile::Open("DY_Scale_factor_LO_NbjetBins_ptBins_mtt_2016_prod_v6.root");
  TFile* dy_file  = TFile::Open(fileName.c_str());
  TH2D* dy_matrix = (TH2D*)dy_file->Get("NbjetBins_ptBins/covariance_matrix");

  const size_t DYUncDim = dy_matrix->GetNbinsX();
  std::cout << " ** DEBUG --> DYUncDim: " << DYUncDim << std::endl;

  TMatrixD dy_unc_cov(DYUncDim, DYUncDim);
  for(size_t i = 1; i <= DYUncDim; ++i)
  {
    for(size_t j = i; j <= DYUncDim; ++j)
    {
      dy_unc_cov[i-1][j-1] = dy_unc_cov[j-1][i-1] = dy_matrix->GetBinContent(i,j);
    }
  }

  TVectorD dy_sf(DYUncDim);
  TH1D* sf_histo = (TH1D*)dy_file->Get("NbjetBins_ptBins/scale_factors");

  for(size_t i = 1; i <= DYUncDim; ++i)
  {
    dy_sf[i-1] = sf_histo->GetBinContent(i);
  }

  auto dy_w_inv = stat_tools::ComputeWhiteningMatrix(dy_unc_cov).Invert();
  std::cout << " ** DEBUG: inverse whitening matrix for DY sf covariance matrix" << std::endl;
  dy_w_inv.Print();

  std::cout << Form("//Copy the following lines in systematics_DY%s.cfg",myYearName.c_str()) << std::endl;
  for(size_t n = 0; n < DYUncDim; ++n)
  {
      std::cout << std::endl;
      std::cout << "[CMS_hbbhtautau_" << myYearName << "_DYSFunc" << n+1 << "]" << std::endl;
      std::cout << "type = lnN" << std::endl;
      for(size_t k = 0; k < DYsamples.size(); ++k)
      {
        const double unc_value = dy_w_inv[k][n] / dy_sf[k];
        //std::cout << DYsamples.at(k) << " = " << unc_value << std::endl;

        const double unc_value_rounded = std::round(unc_value*1000) / 1000 ;
        //std::cout << DYsamples.at(k) << " = " << unc_value_rounded << std::endl;

        if (std::abs(unc_value_rounded) < 0.001) continue;

        std::string DYname = DYsamples.at(k);
        std::printf("%-13s = %g\n", DYname.c_str(), 1+unc_value_rounded);
      }
  }

}
