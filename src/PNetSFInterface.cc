#include "PNetSFInterface.h"

// Constructor
// 3 pT bins have been chosen to maintain sufficient statistics in the bins, and the choice of bins is based on the distribution of events in the variable.
// e.g. https://indico.cern.ch/event/1360981/#173-ak8-particlenet-correction

PNetSFInterface::PNetSFInterface(const std::string period) :
    // First initialize all SF vectors to 1...
    scaleFactorsHH_lowPt_ (9, 1.),
    scaleFactorsHH_medPt_ (9, 1.),
    scaleFactorsHH_highPt_(9, 1.),
    scaleFactorsTT_lowPt_ (9, 1.),
    scaleFactorsTT_medPt_ (9, 1.),
    scaleFactorsTT_highPt_(9, 1.),
    scaleFactorsDY_lowPt_ (9, 1.),
    scaleFactorsDY_medPt_ (9, 1.),
    scaleFactorsDY_highPt_(9, 1.)
{
  if (period == "2016preVFP" || period == "2016postVFP" || period == "2017" || period == "2018")
    {
      // ...then fill them with actual values depending on Period
      FillHHlikeSFs(period);
      FillTTlikeSFs(period);
      FillDYlikeSFs(period);
    }
  else
    {
      std::string errorMessage = "PNseSFInterface - Invalid period: "
	+ period
	+ " [options are: 2016preVFP/2016postVFP/2017/2018]";
      throw std::logic_error(errorMessage);
    }
}

// Destructor
PNetSFInterface::~PNetSFInterface() {}

// Fill SFs for HH-like samples
void PNetSFInterface::FillHHlikeSFs(const std::string period)
{
    // Set scale factor values depending on the Era
    if (period == "2016preVFP")
    {
        // pT < 500
        scaleFactorsHH_lowPt_[0] = (1.054);            // HP central
        scaleFactorsHH_lowPt_[1] = (1.054 + ( 0.080)); // HP up
        scaleFactorsHH_lowPt_[2] = (1.054 + (-0.077)); // HP down
        scaleFactorsHH_lowPt_[3] = (1.052);            // MP central
        scaleFactorsHH_lowPt_[4] = (1.052 + ( 0.087)); // MP up
        scaleFactorsHH_lowPt_[5] = (1.052 + (-0.081)); // MP down
        scaleFactorsHH_lowPt_[6] = (1.032);            // LP central
        scaleFactorsHH_lowPt_[7] = (1.032 + ( 0.096)); // LP up
        scaleFactorsHH_lowPt_[8] = (1.032 + (-0.090)); // LP down

        // 500 <= pT < 600
        scaleFactorsHH_medPt_[0] = (1.139);            // HP central
        scaleFactorsHH_medPt_[1] = (1.139 + ( 0.083)); // HP up
        scaleFactorsHH_medPt_[2] = (1.139 + (-0.081)); // HP down
        scaleFactorsHH_medPt_[3] = (1.068);            // MP central
        scaleFactorsHH_medPt_[4] = (1.068 + ( 0.078)); // MP up
        scaleFactorsHH_medPt_[5] = (1.068 + (-0.073)); // MP down
        scaleFactorsHH_medPt_[6] = (1.062);            // LP central
        scaleFactorsHH_medPt_[7] = (1.062 + ( 0.092)); // LP up
        scaleFactorsHH_medPt_[8] = (1.062 + (-0.082)); // LP down

        // pT >= 600
        scaleFactorsHH_highPt_[0] = (1.049);            // HP central
        scaleFactorsHH_highPt_[1] = (1.049 + ( 0.133)); // HP up
        scaleFactorsHH_highPt_[2] = (1.049 + (-0.130)); // HP down
        scaleFactorsHH_highPt_[3] = (0.996);            // MP central
        scaleFactorsHH_highPt_[4] = (0.996 + ( 0.101)); // MP up
        scaleFactorsHH_highPt_[5] = (0.996 + (-0.097)); // MP down
        scaleFactorsHH_highPt_[6] = (1.002);            // LP central
        scaleFactorsHH_highPt_[7] = (1.002 + ( 0.106)); // LP up
        scaleFactorsHH_highPt_[8] = (1.002 + (-0.101)); // LP down
    }
    else if (period == "2016postVFP")
    {
        // pT < 500
        scaleFactorsHH_lowPt_[0] = (1.031);            // HP central
        scaleFactorsHH_lowPt_[1] = (1.031 + ( 0.050)); // HP up
        scaleFactorsHH_lowPt_[2] = (1.031 + (-0.046)); // HP down
        scaleFactorsHH_lowPt_[3] = (1.029);            // MP central
        scaleFactorsHH_lowPt_[4] = (1.029 + ( 0.051)); // MP up
        scaleFactorsHH_lowPt_[5] = (1.029 + (-0.045)); // MP down
        scaleFactorsHH_lowPt_[6] = (1.031);            // LP central
        scaleFactorsHH_lowPt_[7] = (1.031 + ( 0.058)); // LP up
        scaleFactorsHH_lowPt_[8] = (1.031 + (-0.050)); // LP down

        // 500 <= pT < 600
        scaleFactorsHH_medPt_[0] = (1.055);            // HP central
        scaleFactorsHH_medPt_[1] = (1.055 + ( 0.069)); // HP up
        scaleFactorsHH_medPt_[2] = (1.055 + (-0.067)); // HP down
        scaleFactorsHH_medPt_[3] = (1.070);            // MP central
        scaleFactorsHH_medPt_[4] = (1.070 + ( 0.066)); // MP up
        scaleFactorsHH_medPt_[5] = (1.070 + (-0.062)); // MP down
        scaleFactorsHH_medPt_[6] = (1.089);            // LP central
        scaleFactorsHH_medPt_[7] = (1.089 + ( 0.076)); // LP up
        scaleFactorsHH_medPt_[8] = (1.089 + (-0.068)); // LP down

        // pT >= 600
        scaleFactorsHH_highPt_[0] = (1.088);            // HP central
        scaleFactorsHH_highPt_[1] = (1.088 + ( 0.076)); // HP up
        scaleFactorsHH_highPt_[2] = (1.088 + (-0.072)); // HP down
        scaleFactorsHH_highPt_[3] = (1.077);            // MP central
        scaleFactorsHH_highPt_[4] = (1.077 + ( 0.067)); // MP up
        scaleFactorsHH_highPt_[5] = (1.077 + (-0.059)); // MP down
        scaleFactorsHH_highPt_[6] = (1.057);            // LP central
        scaleFactorsHH_highPt_[7] = (1.057 + ( 0.077)); // LP up
        scaleFactorsHH_highPt_[8] = (1.057 + (-0.056)); // LP down
    }
    else if (period == "2017")
    {
        // pT < 500
        scaleFactorsHH_lowPt_[0] = (1.055);            // HP central
        scaleFactorsHH_lowPt_[1] = (1.055 + ( 0.057)); // HP up
        scaleFactorsHH_lowPt_[2] = (1.055 + (-0.054)); // HP down
        scaleFactorsHH_lowPt_[3] = (1.006);            // MP central
        scaleFactorsHH_lowPt_[4] = (1.006 + ( 0.052)); // MP up
        scaleFactorsHH_lowPt_[5] = (1.006 + (-0.052)); // MP down
        scaleFactorsHH_lowPt_[6] = (0.966);            // LP central
        scaleFactorsHH_lowPt_[7] = (0.966 + ( 0.055)); // LP up
        scaleFactorsHH_lowPt_[8] = (0.966 + (-0.057)); // LP down

        // 500 <= pT < 600
        scaleFactorsHH_medPt_[0] = (1.067);            // HP central
        scaleFactorsHH_medPt_[1] = (1.067 + ( 0.057)); // HP up
        scaleFactorsHH_medPt_[2] = (1.067 + (-0.055)); // HP down
        scaleFactorsHH_medPt_[3] = (1.051);            // MP central
        scaleFactorsHH_medPt_[4] = (1.051 + ( 0.056)); // MP up
        scaleFactorsHH_medPt_[5] = (1.051 + (-0.055)); // MP down
        scaleFactorsHH_medPt_[6] = (1.021);            // LP central
        scaleFactorsHH_medPt_[7] = (1.021 + ( 0.053)); // LP up
        scaleFactorsHH_medPt_[8] = (1.021 + (-0.052)); // LP down

        // pT >= 600
        scaleFactorsHH_highPt_[0] = (1.045);            // HP central
        scaleFactorsHH_highPt_[1] = (1.045 + ( 0.045)); // HP up
        scaleFactorsHH_highPt_[2] = (1.045 + (-0.046)); // HP down
        scaleFactorsHH_highPt_[3] = (0.991);            // MP central
        scaleFactorsHH_highPt_[4] = (0.991 + ( 0.038)); // MP up
        scaleFactorsHH_highPt_[5] = (0.991 + (-0.043)); // MP down
        scaleFactorsHH_highPt_[6] = (0.979);            // LP central
        scaleFactorsHH_highPt_[7] = (0.979 + ( 0.035)); // LP up
        scaleFactorsHH_highPt_[8] = (0.979 + (-0.038)); // LP down
    }
    else if (period == "2018")
    {
        // pT < 500
        scaleFactorsHH_lowPt_[0] = (0.994);            // HP central
        scaleFactorsHH_lowPt_[1] = (0.994 + ( 0.064)); // HP up
        scaleFactorsHH_lowPt_[2] = (0.994 + (-0.064)); // HP down
        scaleFactorsHH_lowPt_[3] = (0.966);            // MP central
        scaleFactorsHH_lowPt_[4] = (0.966 + ( 0.056)); // MP up
        scaleFactorsHH_lowPt_[5] = (0.966 + (-0.057)); // MP down
        scaleFactorsHH_lowPt_[6] = (0.921);            // LP central
        scaleFactorsHH_lowPt_[7] = (0.921 + ( 0.071)); // LP up
        scaleFactorsHH_lowPt_[8] = (0.921 + (-0.077)); // LP down

        // 500 <= pT < 600
        scaleFactorsHH_medPt_[0] = (1.072);            // HP central
        scaleFactorsHH_medPt_[1] = (1.072 + ( 0.041)); // HP up
        scaleFactorsHH_medPt_[2] = (1.072 + (-0.036)); // HP down
        scaleFactorsHH_medPt_[3] = (1.033);            // MP central
        scaleFactorsHH_medPt_[4] = (1.033 + ( 0.030)); // MP up
        scaleFactorsHH_medPt_[5] = (1.033 + (-0.025)); // MP down
        scaleFactorsHH_medPt_[6] = (1.006);            // LP central
        scaleFactorsHH_medPt_[7] = (1.006 + ( 0.024)); // LP up
        scaleFactorsHH_medPt_[8] = (1.006 + (-0.026)); // LP down

        // pT >= 600
        scaleFactorsHH_highPt_[0] = (1.046);            // HP central
        scaleFactorsHH_highPt_[1] = (1.046 + ( 0.038)); // HP up
        scaleFactorsHH_highPt_[2] = (1.046 + (-0.038)); // HP down
        scaleFactorsHH_highPt_[3] = (1.010);            // MP central
        scaleFactorsHH_highPt_[4] = (1.010 + ( 0.030)); // MP up
        scaleFactorsHH_highPt_[5] = (1.010 + (-0.035)); // MP down
        scaleFactorsHH_highPt_[6] = (1.001);            // LP central
        scaleFactorsHH_highPt_[7] = (1.001 + ( 0.035)); // LP up
        scaleFactorsHH_highPt_[8] = (1.001 + (-0.037)); // LP down
    }
    else
    {
        std::string errorMessage = "PNseSFInterface - Invalid period: "
                                 + period
                                 + " [options are: 2016preVFP/2016postVFP/2017/2018]";
        throw std::logic_error(errorMessage);
    }
}

// Fill SFs for TT-like samples
void PNetSFInterface::FillTTlikeSFs(const std::string period)
{
    // Set scale factor values depending on the Era
    if (period == "2016preVFP")
    {
        // pT < 300
        scaleFactorsTT_lowPt_[6] = (0.4927);                         // LP central
        scaleFactorsTT_lowPt_[7] = (0.4927 + (0.3802));              // LP up
        scaleFactorsTT_lowPt_[8] = (0.4927 + (-0.3802));             // LP down

        // 300 <= pT < 400
        scaleFactorsTT_medPt_[6] = (1.0216);                         // LP central
        scaleFactorsTT_medPt_[7] = (1.0216 + (0.6653));              // LP up
        scaleFactorsTT_medPt_[8] = (1.0216 + (-0.6653));             // LP down

        // pT >= 400
        scaleFactorsTT_highPt_[6] = (0.3795);                        // LP central
        scaleFactorsTT_highPt_[7] = (0.3795 + (1.5452));             // LP up
        scaleFactorsTT_highPt_[8] = (0.3795 + (-1.5452));            // LP down
    }
    else if (period == "2016postVFP")
    {
        // pT < 300
        scaleFactorsTT_lowPt_[6] = (0.8377);                         // LP central
        scaleFactorsTT_lowPt_[7] = (0.8377 + (0.5155));              // LP up
        scaleFactorsTT_lowPt_[8] = (0.8377 + (-0.5155));             // LP down

        // 300 <= pT < 400
        scaleFactorsTT_medPt_[6] = (0.571);                          // LP central
        scaleFactorsTT_medPt_[7] = (0.571 + (0.7731));               // LP up
        scaleFactorsTT_medPt_[8] = (0.571 + (-0.7731));              // LP down

        // pT >= 400
        scaleFactorsTT_highPt_[6] = (1.2948);                        // LP central
        scaleFactorsTT_highPt_[7] = (1.2948 + (3.1934));             // LP up
        scaleFactorsTT_highPt_[8] = (1.2948 + (-3.1934));            // LP down
    }
    else if (period == "2017")
    {
        // pT < 300
        scaleFactorsTT_lowPt_[6] = (1.6759);                         // LP central
        scaleFactorsTT_lowPt_[7] = (1.6759 + (0.4351));              // LP up
        scaleFactorsTT_lowPt_[8] = (1.6759 + (-0.4351));             // LP down

        // 300 <= pT < 400
        scaleFactorsTT_medPt_[6] = (0.5925);                         // LP central
        scaleFactorsTT_medPt_[7] = (0.5925 + (0.3974));              // LP up
        scaleFactorsTT_medPt_[8] = (0.5925 + (-0.3974));             // LP down

        // pT >= 400
        scaleFactorsTT_highPt_[6] = (0.8383);                        // LP central
        scaleFactorsTT_highPt_[7] = (0.8383 + (1.1402));             // LP up
        scaleFactorsTT_highPt_[8] = (0.8383 + (-1.1402));            // LP down
    }
    else if (period == "2018")
    {
        // pT < 300
        scaleFactorsTT_lowPt_[6] = (0.8782);                         // LP central
        scaleFactorsTT_lowPt_[7] = (0.8782 + (0.3081));              // LP up
        scaleFactorsTT_lowPt_[8] = (0.8782 + (-0.3081));             // LP down

        // 300 <= pT < 400
        scaleFactorsTT_medPt_[6] = (0.6941);                         // LP central
        scaleFactorsTT_medPt_[7] = (0.6941 + (0.3251));              // LP up
        scaleFactorsTT_medPt_[8] = (0.6941 + (-0.3251));             // LP down

        // pT >= 400
        scaleFactorsTT_highPt_[6] = (0.8692);                        // LP central
        scaleFactorsTT_highPt_[7] = (0.8692 + (0.6044));             // LP up
        scaleFactorsTT_highPt_[8] = (0.8692 + (-0.6044));            // LP down
    }
    else
    {
        std::string errorMessage = "PNseSFInterface - Invalid period: "
                                 + period
                                 + " [options are: 2016preVFP/2016postVFP/2017/2018]";
        throw std::logic_error(errorMessage);
    }
}

// Fill SFs for DY-like samples
void PNetSFInterface::FillDYlikeSFs(const std::string period)
{
    // Set scale factor values depending on the Era
    if (period == "2016preVFP")
    {
        // pT < 300
        scaleFactorsDY_lowPt_[6] = (1.0743);                // LP central
        scaleFactorsDY_lowPt_[7] = (1.0743 + ( 0.2764));   // LP up
        scaleFactorsDY_lowPt_[8] = (1.0743 + (-0.2764));   // LP down

        // 300 <= pT < 400
        scaleFactorsDY_medPt_[6] = (1.2709);                // LP central
        scaleFactorsDY_medPt_[7] = (1.2709 + ( 0.3487));   // LP up
        scaleFactorsDY_medPt_[8] = (1.2709 + (-0.3487));   // LP down

        // pT >= 400
        scaleFactorsDY_highPt_[6] = (1.5753);                // LP central
        scaleFactorsDY_highPt_[7] = (1.5753 + ( 0.6695));   // LP up
        scaleFactorsDY_highPt_[8] = (1.5753 + (-0.6695));   // LP down
    }
    else if (period == "2016postVFP")
    {
        // pT < 300
        scaleFactorsDY_lowPt_[6] = (1.6456);                // LP central
        scaleFactorsDY_lowPt_[7] = (1.6456 + ( 0.4316));   // LP up
        scaleFactorsDY_lowPt_[8] = (1.6456 + (-0.4316));   // LP down

        // 300 <= pT < 400
        scaleFactorsDY_medPt_[6] = (0.8931);                // LP central
        scaleFactorsDY_medPt_[7] = (0.8931 + ( 0.4125));   // LP up
        scaleFactorsDY_medPt_[8] = (0.8931 + (-0.4125));   // LP down

        // pT >= 400
        scaleFactorsDY_highPt_[6] = (0.6539);                // LP central
        scaleFactorsDY_highPt_[7] = (0.6539 + ( 0.763));    // LP up
        scaleFactorsDY_highPt_[8] = (0.6539 + (-0.763));    // LP down
    }
    else if (period == "2017")
    {
        // pT < 300
        scaleFactorsDY_lowPt_[6] = (1.0085);                // LP central
        scaleFactorsDY_lowPt_[7] = (1.0085 + ( 0.1877));   // LP up
        scaleFactorsDY_lowPt_[8] = (1.0085 + (-0.1877));   // LP down

        // 300 <= pT < 400
        scaleFactorsDY_medPt_[6] = (1.4333);                // LP central
        scaleFactorsDY_medPt_[7] = (1.4333 + ( 0.3412));   // LP up
        scaleFactorsDY_medPt_[8] = (1.4333 + (-0.3412));   // LP down

        // pT >= 400
        scaleFactorsDY_highPt_[6] = (1.4411);                // LP central
        scaleFactorsDY_highPt_[7] = (1.4411 + ( 0.5479));   // LP up
        scaleFactorsDY_highPt_[8] = (1.4411 + (-0.5479));   // LP down
    }
    else if (period == "2018")
    {
        // pT < 300
        scaleFactorsDY_lowPt_[6] = (1.1462);                // LP central
        scaleFactorsDY_lowPt_[7] = (1.1462 + ( 0.2));      // LP up
        scaleFactorsDY_lowPt_[8] = (1.1462 + (-0.2));      // LP down

        // 300 <= pT < 400
        scaleFactorsDY_medPt_[6] = (1.2728);                // LP central
        scaleFactorsDY_medPt_[7] = (1.2728 + ( 0.2966));   // LP up
        scaleFactorsDY_medPt_[8] = (1.2728 + (-0.2966));   // LP down

        // pT >= 400
        scaleFactorsDY_highPt_[6] = (0.952);                 // LP central
        scaleFactorsDY_highPt_[7] = (0.952 + ( 0.3111));    // LP up
        scaleFactorsDY_highPt_[8] = (0.952 + (-0.3111));    // LP down
    }
    else
    {
        std::string errorMessage = "PNetSFInterface - Invalid period: "
                                 + period
                                 + " [options are: 2016preVFP/2016postVFP/2017/2018]";
        throw std::logic_error(errorMessage);
    }
}


// Get the SF vector based on jet pT and sampleType
std::vector<float> PNetSFInterface::getSFvec(const float pT, const std::string sampleType)
{
    // Initialize output SF vector
    std::vector<float> SFvec(9, 1.);

    // HH-like samples SF
    if (sampleType == "HHlike")
    {
        // set the pT-dependent SF
        if (pT < 500)
        {
            SFvec = scaleFactorsHH_lowPt_;
        }
        else if (pT >= 500 && pT < 600)
        {
            SFvec = scaleFactorsHH_medPt_;
        }
        else /* pT >= 600 */
        {
            SFvec = scaleFactorsHH_highPt_;
        }
    }

    // TT-like samples SF
    if (sampleType == "TTlike")
    {
        // Set the pT-dependent SF
        if (pT < 300)
        {
            SFvec = scaleFactorsTT_lowPt_;
        }
        else if (pT >= 300 && pT < 400)
        {
            SFvec = scaleFactorsTT_medPt_;
        }
        else /* pT >= 400 */
        {
            SFvec = scaleFactorsTT_highPt_;
        }
    }

    // DY-like samples SF
    if (sampleType == "DYlike")
    {
        // Set the pT-dependent SF
        if (pT < 300)
        {
            SFvec = scaleFactorsDY_lowPt_;
        }
        else if (pT >= 300 && pT < 400)
        {
            SFvec = scaleFactorsDY_medPt_;
        }
        else /* pT >= 400 */
        {
            SFvec = scaleFactorsDY_highPt_;
        }
    }

    // Return final SF vector
    return SFvec;
}
