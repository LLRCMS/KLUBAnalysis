Fit parameters for HLT_DoubleMedium(Combined)IsoPFTau35_Trk1_eta2p1_Reg trigger, expressed per single tau leg.  

The measurement is performed using the entire 2016 (PromptReco) dataset, but it's split for different periods:
- for Run2016 BCDEFG, we've had HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg trigger
- for Run2016 H, we moved to HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg, which includes updated L1 and HLT isolations. As a result, efficiency curves do differ from BCDEFG
- the trigger simulation corresponds to HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg

Scale factors can be computed as the weighed average (by integrated luminosity) of data curves divided by MC curve.  

Parameters are to be passed to this fit function  
https://github.com/rmanzoni/triggertools/blob/master/objects/FitFunctions.py#L68  
that corresponds to an (approximate) convolution of a CrystalBall resolution and a step function.  

For the moment, everything is computed for genuine taus, the temporary recommendation is to apply the same scale factors to fake taus too.  


Scale factors for both genuine and fake taus, using the ReReco data and the latest MC are being produced.
