
#ifndef PNetSFInterface_h
#define PNetSFInterface_h

// ---------------------------------------------------------------------------------------------- //
//                                                                                                //
//   class PNetSFInterface                                                                        //
//                                                                                                //
//   Class to compute ParticleNet ScaleFactors. Modified from original implementation in          //
//    - https://github.com/LLRCMS/KLUBAnalysis/pull/371                                           //
//                                                                                                // 
//   Improved implementation taken from CCLUB framework , made by Francesco                       //
//    - https://gitlab.cern.ch/cclubbtautau/AnalysisCore/-/merge_requests/7                       //
//                                                                                                //
//   Scale factor values from:                                                                    //
//    - http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2021_005_v10.pdf (page 76)      //
//    - Values to be applied ONLY to H->bb events                                                 //
//                                                                                                //
//   Author: Francesco Brivio, Simona Palluotto (INFN Milano-Bicocca)                             //
//   Date  : June 2024                                                                            //
//                                                                                                //
// ---------------------------------------------------------------------------------------------- //

// Standard libraries
#include <string>
#include <vector>
#include <stdexcept> // include this for std::logic_error

class PNetSFInterface {

    public:
        PNetSFInterface(const std::string period);
        ~PNetSFInterface();
        std::vector<float> getSFvec(const float pT, const std::string sampleType);

    private:
        // Vectors with SF values:
        //  - 0/1/2 : HP wp -> central, up, down
        //  - 3/4/5 : MP wp -> central, up, down
        //  - 6/7/8 : LP wp -> central, up, down

        // SampleType: HH-like
        std::vector<float> scaleFactorsHH_lowPt_;
        std::vector<float> scaleFactorsHH_medPt_;
        std::vector<float> scaleFactorsHH_highPt_;

        // SampleType: TT-like
        std::vector<float> scaleFactorsTT_lowPt_;
        std::vector<float> scaleFactorsTT_medPt_;
        std::vector<float> scaleFactorsTT_highPt_;

        // SampleType: DY-like
        std::vector<float> scaleFactorsDY_lowPt_;
        std::vector<float> scaleFactorsDY_medPt_;
        std::vector<float> scaleFactorsDY_highPt_;

        // Methods to initialiaze the SF maps
        void FillHHlikeSFs(const std::string period);
        void FillTTlikeSFs(const std::string period);
        void FillDYlikeSFs(const std::string period);
};

#endif // PNetSFInterface_h
