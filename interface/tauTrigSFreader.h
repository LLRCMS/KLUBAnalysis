#include <string>
#include "TFile.h"
#include "TF1.h"

class tauTrigSFreader {
    public:
        tauTrigSFreader(std::string rfilename);
        ~tauTrigSFreader();
        double getSF(double pt, int decayMode);
    private:
        TFile* fIn_;
        
        // 3 TF1 per decay mode 0, 1, and 10
        TF1* fMC_[3];
        TF1* fDataBG_[3];
        TF1* fDataH_[3];
        
        double fracBG_;
        double fracH_;

        double xmin_;
        double xmax_;
};


