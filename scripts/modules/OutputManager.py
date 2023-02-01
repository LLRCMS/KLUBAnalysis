import ROOT
import collections
import fnmatch
import array
import math

from VBFReweightModules import inputSample, VBFReweight, printProgressBar

def makeHistoName(sample, sel, var, syst=""):
    name = sample +  "_" + sel + "_" + var
    if syst:
        name += "_"
        name += syst
    return name;

def makeHisto2DName(sample, sel, var1, var2, syst=""):
    name = sample +  "_" + sel + "_" + var1 + "_" + var2
    if syst:
        name += "_"
        name += syst
    return name;

def matchInDictionary(diction, pattern):
    matches = []
    for key in diction:
        if fnmatch.fnmatch(key, pattern):
            matches.append(key)
    return matches

def checkBinningCompatibility (newbinning, oldbinning):
    """ oldbinning must include newbinning boundaries """
    for x in newbinning:
        if not x in oldbinning: return False
    return True

class OutputManager:
    """ Handles the input from AnalysisHelper and manages the output to a file
    to be used for datacards and analysis"""
    def __init__(self):
        self.histos      = collections.OrderedDict()
        self.histos2D    = collections.OrderedDict()
        
        self.selections  = []
        self.sel_def     = []
        self.sel_regions = []
        self.variables   = []
        self.variables2D = []
        self.systList    = []
        # self.samples     = []
        self.data        = []
        self.bkgs        = []
        self.sigs        = []

    def readAll (self, rootfile):
        """ read all histograms from rootfile """

        for key in rootfile.GetListOfKeys():
            kname = key.GetName()
            # print key, kname
            obj = rootfile.Get(kname)

            ## 1D plots
            if isinstance(obj, ROOT.TH1):
                self.histos[kname] = obj

            ## 2D plots
            if isinstance(obj, ROOT.TH2):
                self.histos2D[kname] = obj

        # if check:
        #     print '.... checking reading'
        #     for sample in self.samples:
        #         for sel in self.selections:
        #             for var in self.variables:
        #                 pass

    def groupTogether (self, newName, partsList):
        """ regroup the samples in partsList into a new sample that replaces old ones"""
        
        print "... replacing" , newName , " <== " , partsList

        if len(partsList) == 0:
            print "** ERROR: while grouping together histos, input list has size 0"
            raise ValueError

        ## 1D
        for sel in self.selections:
            for var in self.variables:
                newHistoName = makeHistoName(newName, sel, var)
                
                if newHistoName in self.histos:
                    print "** ERROR: while grouping together histos, name", newName, "already exists in input as", newHistoName, "cannot continue..."
                    raise ValueError

                for idx, oldName in enumerate(partsList):
                    oldHistoName = makeHistoName(oldName, sel, var)
                    if idx == 0:
                        self.histos[newHistoName] = self.histos[oldHistoName].Clone(newHistoName)
                        self.histos[newHistoName].SetTitle(newHistoName)
                    else:
                        self.histos[newHistoName].Add(self.histos[oldHistoName])
                    del self.histos[oldHistoName]

                ### now do systs - use the 1st one of the list to get the syst prototype
                ### NOTE: it supposes that all the histos grouped together have the same syst
                protoName = makeHistoName(partsList[0], sel, var)
                allSysts = matchInDictionary(self.histos, protoName+'_*')
                allSysts = [x.replace(protoName+'_', '') for x in allSysts]
                # if 'TT' in partsList[0] and var == 'MT2' and 'defaultBtagLLNoIsoBBTTCut_SR' in sel: print protoName, allSysts
                for syst in allSysts:
                    newHistoName = makeHistoName(newName, sel, var, syst)
                    for idx, oldName in enumerate(partsList):
                        oldHistoName = makeHistoName(oldName, sel, var, syst)
                        if idx == 0:
                            self.histos[newHistoName] = self.histos[oldHistoName].Clone(newHistoName)
                            self.histos[newHistoName].SetTitle(newHistoName)
                        else:
                            self.histos[newHistoName].Add(self.histos[oldHistoName])
                        del self.histos[oldHistoName]

        ## 2D
        for sel in self.selections:
            for var in self.variables2D:
                var1 = var.rsplit(':', 1)[0]
                var2 = var.rsplit(':', 1)[1]
                newHistoName = makeHisto2DName(newName, sel, var1, var2)
                
                if newHistoName in self.histos2D:
                    print "** ERROR: while grouping together histos, name", newName, "already exists in input as", newHistoName, "cannot continue..."
                    raise ValueError

                for idx, oldName in enumerate(partsList):
                    oldHistoName = makeHisto2DName(oldName, sel, var1, var2)
                    if idx == 0:
                        self.histos2D[newHistoName] = self.histos2D[oldHistoName].Clone(newHistoName)
                        self.histos2D[newHistoName].SetTitle(newHistoName)
                    else:
                        self.histos2D[newHistoName].Add(self.histos2D[oldHistoName])
                    del self.histos2D[oldHistoName]

                ### now do systs - FIXME
                ### now do systs - use the 1st one of the list to get the syst prototype
                ### NOTE: it supposes that all the histos grouped together have the same syst
                protoName = makeHisto2DName(partsList[0], sel, var1, var2)
                allSysts = matchInDictionary(self.histos2D, protoName)
                allSysts = [x.replace(protoName+'_', '') for x in allSysts]
                # print allSysts
        
        ## replace entries in data/sig/bkg with their sum
        theList = None
        if partsList[0]   in self.bkgs: theList = self.bkgs
        elif partsList[0] in self.data: theList = self.data
        elif partsList[0] in self.sigs: theList = self.sigs

        for sample in partsList:
            theList.remove(sample) ## ok since all occurences are unique
        theList.append(newName)


    # def setShape(self, fromSel, toSel, fromSample, toSample=None, fromVar='*', toVar=None, forSysts='*', do2D=False):
    #     """ take the shape from a (sample, selection, variable) and use it in another (sample, selection, variable)
    #     None values mean that the same as fromX field is used"""

    #     if not toSample:
    #         toSample = fromSample
    #     if not toVar:
    #         toVar = fromVar

    #     ## 1D
    #     for sel in self.selections:
    #         if not fnmatch.fnmatch(sel, toSel): continue
            
    #         for var in self.variables:
    #             if not fnmatch.fnmatch(var, toVar): continue
            
    #             for sample in self.samples:
    #                 if not fnmatch.fnmatch(sample, toSample): continue

    #                 hTo = self.histos[makeHistoName(sample, sel, var)]
    #                 hFrom = self.histos[makeHistoName(sample, sel, var)]


    # Check if the integral of "histo" is compatible with "value"
    # only stat uncertainties are considered: errors of each bin are
    # summed in quadrature to get the total statistical uncertainty
    def isIntegralCompatible(self, histo, value, debug=False):

        # original integral of histo
        origIntegral = histo.Integral()

        # get the stat. uncertainty by looping on all bins
        unc = 0.0
        for ibin in range(1, histo.GetNbinsX()+1):
            err = histo.GetBinError(ibin)
            unc = unc + (err*err)
        sq_unc = math.sqrt(unc)

        # check if compatible with value
        delta = origIntegral - value
        isCompatible = abs(delta) < sq_unc

        if (debug):
            print '---> Original Integral:', origIntegral
            print '---> Unc:', unc, '  - sq_unc:', sq_unc
            print '---> delta:', delta
            print '---> isCompatible:', isCompatible

        return isCompatible


    def makeQCD_SBtoSR (self, regionC, regionD, sel, var, syst = "", removeNegBins = True):
        print "... computing C/D factor for QCD from: C =", regionC, ", D =", regionD, "in region ",sel
        for idx, data in enumerate (self.data):
            hnameC = makeHistoName (data, sel+"_"+regionC, var)
            hnameD = makeHistoName (data, sel+"_"+regionD, var)
            print hnameC
            print hnameD
            if idx == 0: 
                hregC = self.histos[hnameC].Clone(makeHistoName('regC',sel+'_'+regionC, var, syst))
                hregC.SetTitle(hregC.GetName())
                hregD = self.histos[hnameD].Clone(makeHistoName('regD',sel+'_'+regionD, var, syst))
                hregD.SetTitle(hregD.GetName())
            else:
                hregC.Add(self.histos[hnameC])
                hregD.Add(self.histos[hnameD])
                    # subtract bkg
        for bkg in self.bkgs:
                hnameC = makeHistoName(bkg, sel+'_'+regionC, var, syst)
                hregC.Add(self.histos[hnameC], -1.)
                hnameD = makeHistoName(bkg, sel+'_'+regionD, var, syst)
                hregD.Add(self.histos[hnameD], -1.)

        # Negative bins should be preserved in order to have
        # correct integrals to compute SBtoSR ---> so these lines are commented out
        #if removeNegBins:
        #        for ibin in range(1, hregC.GetNbinsX()+1):
        #            if hregC.GetBinContent(ibin) < 0:
        #                hregC.SetBinContent(ibin, 1.e-6)
        #            if hregD.GetBinContent(ibin) < 1.e-6:
        #                hregD.SetBinContent(ibin, 1.e-6)

        # Check if total integrals (without removing negative bins) are <=0
        # if yes, return 0 --> no QCD contribution is present for this variable/selection
        # (if is compatible with 0 (but >0) we keep it, the uncertainty will be large anyway)
        intC = hregC.Integral()
        intD = hregD.Integral()
        #if intC <= 0.0 or self.isIntegralCompatible(hregC,0):
        if intC <= 0.0:
            #print '*** WARNING: integral of numerator is negative or compatible with Zero! Returning SBtoSRdyn = 0 !'
            print '*** WARNING: integral of numerator is negative! Returning SBtoSRdyn = 0 !'
            return 0.0
        #if intD <= 0.0 or self.isIntegralCompatible(hregD,0):
        if intD <= 0.0:
            #print '*** WARNING: integral of denominator is negative or compatible with Zero! Returning SBtoSRdyn = 0 !'
            print '*** WARNING: integral of denominator is negative! Returning SBtoSRdyn = 0 !'
            return 0.0

        SBtoSRdyn = hregC.Integral()/hregD.Integral()
        print "... C/D = ", SBtoSRdyn                  
        return SBtoSRdyn

                    # if var == 'MT2' and sel == 'defaultBtagLLNoIsoBBTTCut' : print ">> -- bkg - SHAPE: " , hname, hQCD.Integral()
    ## FIXME: how to treat systematics properly ? Do we need to do ann alternative QCD histo for every syst?
    def makeQCD (self, SR, yieldSB, shapeSB, SBtoSRfactor, regionC, regionD, doFitIf='False', fitFunc='[0] + [1]*x', computeSBtoSR = True, QCDname = 'QCD',
                 removeNegBins = True, doUpDown = False, boostSBtoSR = 1., classSBtoSR = 1.):
        
        print "... building QCD w/ name:", QCDname, ". SR:" , SR, " yieldSB:", yieldSB, " shapeSB:", shapeSB, " SBtoSRfactor:", SBtoSRfactor, " doUpDown:", doUpDown
        print "    >> recompute SBtoSR dynamically? "
        if computeSBtoSR: 
            print "    >>  yes"
        else:
            print "    >>  no"
        print "    >> doFitIf:", doFitIf , "fitFunction:", fitFunc

        # Since we always use the nominal QCD histogram we do not need to
        # loop on all the systematics to compute the shifted QCD histograms.
        # Can easily be changed by setting: allSysts = self.systList
        allSysts = [""]

        # Do actual QCD computation: loop on vars --> selections --> systs
        for var in self.variables:
            for sel in self.sel_def:
                for syst in allSysts:

                    # If var already contains one of the shape systs (tauup, taudown, jetup...)
                    # do not create the shifted QCD template
                    if 'tauup' in var or 'taudown' in var or \
                       'eleup' in var or 'eledown' in var or \
                       'muup'  in var or 'mudown'  in var or \
                       'jetup' in var or 'jetdown' in var:
                       continue

                    # If var already contains one of the shape systs (tauup, taudown, jetup...)
                    # skip the syst from allSysts (otherwise it look for histograms with two systematics
                    # like 'var_sel_SR_tauup_PUjetIDUp' which do not make sense)
                    doubleSyst = False
                    namesToBeRemoved = ["tauup", "taudown", "eleup", "eledown", "muup", "mudown", "jetup", "jetdown"]
                    for doubleName in namesToBeRemoved:
                        if doubleName in var:
                            doubleSyst = True
                            break
                    if syst != '' and doubleSyst: continue

                    # for boosted category we use 'L' bTag WP
                    if 'boost' in sel:
                        if 'bTagSF' in syst:
                            syst = syst.replace('M','L')

                    ## make shape hist
                    for idx, data in enumerate(self.data):
                        hname = makeHistoName(data, sel+'_'+shapeSB, var)
                        if idx == 0:
                            hQCD = self.histos[hname].Clone(makeHistoName(QCDname, sel+'_'+SR, var, syst)) ## use SR name as this is where QCD refers to
                            hQCD.SetTitle(hQCD.GetName())
                        else:
                            hQCD.Add(self.histos[hname])

                    # subtract bkg
                    for bkg in self.bkgs:
                        hname = makeHistoName(bkg, sel+'_'+shapeSB, var, syst)
                        hQCD.Add(self.histos[hname], -1.)

                    ## remove negative bins if needed
                    if removeNegBins:
                        for ibin in range(1, hQCD.GetNbinsX()+1):
                            if hQCD.GetBinContent(ibin) < 0:
                                hQCD.SetBinContent(ibin, 1.e-6)

                    ## now compute yield to be set
                    for idx, data in enumerate(self.data):
                        hname = makeHistoName(data, sel+'_'+yieldSB, var)
                        if idx == 0:
                            hyieldQCD = self.histos[hname].Clone(makeHistoName(QCDname+'yield', sel+'_'+yieldSB, var, syst))
                        else:
                            hyieldQCD.Add(self.histos[hname])

                    for bkg in self.bkgs:
                        hname = makeHistoName(bkg, sel+'_'+yieldSB, var, syst)
                        hyieldQCD.Add(self.histos[hname], -1)

                    ## now scale
                    qcdYield = hyieldQCD.Integral()

                    # SBtoSR ratio computation is dynamic for res1b and res2b, while it's
                    # hard-coded for boosted and VBF multicategories, for more details see:
                    # https://indico.cern.ch/event/987490/contributions/4157106/attachments/2165647/3655229/201218_QCD_JLH_posMeeting.pdf
                    if computeSBtoSR:
                        # If category is boosted use boost B/D (no-bTag)
                        if 'boost' in sel and boostSBtoSR != 1.:
                            SBtoSRfactor = boostSBtoSR
                            print "... static boosted C/D = ", boostSBtoSR
                        # Else is one of the VBF multiclasses use B/D from VBf inclusive
                        elif 'class' in sel and classSBtoSR != 1.:
                            SBtoSRfactor = classSBtoSR
                            print "... static VBF multiclass C/D = ", classSBtoSR
                        # else (res1b/res2b) compute dynamically the QCD
                        else:
                            SBtoSRfactor = self.makeQCD_SBtoSR(regionC, regionD, sel, var, syst, removeNegBins)

                    # Check if original integral of shape histo (without removing negative bins) is <= 0
                    # if yes, return 0 --> no QCD contribution is present for this variable/selection
                    # (if is compatible with 0 (but >0) we keep it, the uncertainty will be large anyway)
                    # ( if qcdYield <= 0.0 or self.isIntegralCompatible(hyieldQCD,0): )
                    if qcdYield <= 0.0:
                        #print '*** WARNING: integral of shapeQCD is negative or compatible with Zero! Setting QCD = 0 !'
                        print '*** WARNING: integral of shapeQCD is negative! Setting QCD = 0 !'
                        sc = 0.0
                    else:
                        sc = SBtoSRfactor*qcdYield/hQCD.Integral() if hQCD.Integral() > 0 else 0.0

                    hQCD.Scale(sc)

                    if eval(doFitIf):
                        try:
                            self.QCDfits
                        except AttributeError:
                            self.QCDfits = collections.OrderedDict()
                            self.QCDfitresults = collections.OrderedDict()

                        # the previous QCD histogram is still stored but as 'uncorr', and the new one gets QCDname
                        hQCD.SetName('uncorr'+hQCD.GetName())
                        hQCDCorr = hQCD.Clone(makeHistoName(QCDname, sel+'_'+SR, var, syst))
                        hQCDCorr.Scale(1./hQCDCorr.Integral())
                        hQCDNum = hyieldQCD.Clone(makeHistoName(QCDname+'FIT', sel+'_'+SR, var, syst))
                        hQCDNum.Scale(1./hQCDNum.Integral()) ## both num and denom are normalized to 1

                        hQCDNum.Divide(hQCDCorr)
                        fFitFunc = ROOT.TF1('QCDFitFunc', fitFunc, hQCD.GetXaxis().GetXmin(), hQCD.GetXaxis().GetXmax())
                        fitresult = hQCDNum.Fit(fFitFunc, "S")
                        fitresult.SetName(makeHistoName(QCDname+'fitresult', sel+'_'+SR, var, syst))

                        self.QCDfits[hQCDNum.GetName()] = hQCDNum
                        self.QCDfitresults[fitresult.GetName()] = fitresult
                        # the fit gets attached to the histo, so that one can retrieve parameters as TF1* f = histo->GetFunction("QCDFitFunc")

                        ## scale the QCD template according to the fit function at the bin center
                        normaliz = hQCD.Integral()
                        hQCDCorr.Multiply(fFitFunc) # NOTE: multiplication is done in the function range, so it's important to set this properly before. Errors are propagated.
                        hQCDCorr.Scale(normaliz/hQCDCorr.Integral())
                        if doUpDown:
                            hQCDCorrup   = hQCD.Clone(hQCDCorr.GetName()+"_Up")
                            hQCDCorrdown = hQCD.Clone(hQCDCorr.GetName()+"_Down")
                            self.histos[hQCDCorr.GetName()+"_Up"]   = hQCDCorrup
                            self.histos[hQCDCorr.GetName()+"_Down"] = hQCDCorrdown
                        else:
                            self.histos[hQCDCorr.GetName()] = hQCDCorr

                    ## store hQCD - is either 'QCD' if no fit was done or uncorrQCD if fit was done, in any case is the final one to plot
                    if doUpDown:
                        hQCDup   = hQCD.Clone(hQCD.GetName()+"_Up")
                        hQCDdown = hQCD.Clone(hQCD.GetName()+"_Down")
                        self.histos[hQCD.GetName()+"_Up"]   = hQCDup
                        self.histos[hQCD.GetName()+"_Down"] = hQCDdown
                    else:
                        self.histos[hQCD.GetName()] = hQCD
            

        ### FIXME: now do 2D histos

    def rebin(self, var, sel, newbinning):        
        print '... rebinning histos for var:' , var, 'sel:', sel
        newbinning_a = array.array('d', newbinning)
        for idx, s in enumerate(self.data + self.bkgs + self.sigs):
            htorebin_name = makeHistoName(s, sel, var)
            h = self.histos[htorebin_name]
            if idx == 0: # all histos have the same binning, don't waste time
                # for i in range(1, h.GetNbinsX()+2): print var, i, h.GetBinLowEdge(i)
                oldbinning = [h.GetBinLowEdge(i) for i in range(1, h.GetNbinsX()+2)]
            if not checkBinningCompatibility (newbinning, oldbinning):
                print "** OutputManager : rebin : warning: binnings are not compatible, won't rebin", var, sel
                print "old:", oldbinning, "new:", newbinning
                continue
            h.SetName('prerebin_'+htorebin_name)
            hnew = h.Rebin(len(newbinning)-1, htorebin_name, newbinning_a)
            # print sel, var, hnew.GetNbinsX(), hnew.GetName()
            self.histos[hnew.GetName()] = hnew

            ## the following is the correct one, but slow
            # protoName = makeHistoName(s, sel, var)
            # allSysts = matchInDictionary(self.histos, protoName+'_*')
            # allSysts = [x.replace(protoName+'_', '') for x in allSysts]

            ## I should build this once
            # # now systematics
            # if not hasattr(self, 'systMap'):
            #     self.buildSystMap()

            # for the moment, only top pt rew has a systematic, let's speed up and use this information
            #### FIXME: to be generalized! (with the previous function)

            allSysts = []
            #if s == 'TT':
            #    allSysts = ['topUp', 'topDown']
            #    print '.. this is a TT sample, hence the only one with systs', allSysts
            
            for syst in allSysts:
                htorebin_name = makeHistoName(s, sel, var, syst)
                h = self.histos[htorebin_name]
                h.SetName('prerebin_'+htorebin_name)
                hnew = h.Rebin(len(newbinning)-1, htorebin_name, newbinning_a)
                self.histos[hnew.GetName()] = hnew

    def saveToFile(self, fOut, saveQCDFit=True):
        fOut.cd()
        for elem in self.histos:
            #print elem, self.histos[elem]
            self.histos[elem].Write()
        for elem in self.histos2D:
            self.histos2D[elem].Write()
        if hasattr(self, 'QCDfits') and saveQCDFit:
            for elem in self.QCDfits:
                self.QCDfits[elem].Write()
            for elem in self.QCDfitresults:
                self.QCDfitresults[elem].Write()

    def buildSystList(self):
        """ make a list of the suffixes of all shape systematics excluding:
            tauup, taudown, eleup, eledown, muup, mudown, jetup, jetdown
            which are already included in the variable names  """

        # Build systematics list:
        print '... creating systematics list'

        # Use first element of self values just to build the protoName
        protoProcess = self.bkgs[0]
        protoSel     = self.sel_def[0]
        protoRegion  = self.sel_regions[0]
        protoVar     = self.variables[0]
        protoName    = makeHistoName(protoProcess, protoSel+'_'+protoRegion, protoVar)
        print "    >> Using as protoname: ", protoName

        # Get the actual syst names
        allSysts = matchInDictionary(self.histos, protoName+'_*')
        allSysts = [x.replace(protoName+'_', '') for x in allSysts]

        # Exclude TES/EES/MES/JES from the list of systematics
        namesToBeRemoved = ["tauup", "taudown", "eleup", "eledown", "muup", "mudown", "jetup", "jetdown"]
        allSysts = [ x for x in allSysts if not any(b in x for b in namesToBeRemoved) ]

        # Add empty string to get the nominal histo
        allSysts.insert(0, "")

        print "    >> systematics list: ", allSysts
        self.systList = allSysts

    def scaleHistos(self,strBkg, factor, strSel = None):
        print '... scaling histos for bkg: ' , strBkg, " ",strSel, ' by factor : ', factor
        for sel in self.selections:
            if strSel:
                if not strSel in sel: continue 
            for var in self.variables:
                for syst in self.systList:
                    # If var already contains one of the shape systs (tauup, taudown, jetup...)
                    # skip the syst from allSysts (otherwise it look for histograms with two systematics
                    # like 'var_sel_SR_tauup_PUjetIDUp' which do not make sense)
                    doubleSyst = False
                    namesToBeRemoved = ["tauup", "taudown", "eleup", "eledown", "muup", "mudown", "jetup", "jetdown"]
                    for doubleName in namesToBeRemoved:
                        if doubleName in var:
                            doubleSyst = True
                            break
                    if syst != '' and doubleSyst: continue

                    for idx, s in enumerate(self.bkgs):
                        #if (strBkg in s):
                        if (strBkg == s):
                            htoscale_name = makeHistoName(s, sel, var, syst)
                            print htoscale_name
                            h = self.histos[htoscale_name]
                            h.Scale(factor)

    def addHistos(self, strBkg, fExt):
        print '... taking histos for bkg: ' , strBkg, 'from file : ', fExt
        inFile = ROOT.TFile.Open(fExt)
        for sel in self.selections:
            for var in self.variables:
                htoadd_name = makeHistoName(strBkg, sel, var)
                #print htoadd_name
                htoadd = inFile.Get(htoadd_name)
                self.histos[htoadd_name] = htoadd.Clone(htoadd_name)
                return self.histos[htoadd_name]

    # VBF Reweighting
    # Inputs:
    # - inputSigs  : name of 6 input samples
    # - target_kl  : vector of kl target values (float or int)
    # - target_cv  : vector of cv target values (float or int)
    # - target_c2v : vector of c2v target values (float or int)
    # - target_xs  : target value of the output xs (float)
    #                use target_xs = -1 if you want the output normalized to (integratedLumi * weights * correct xs * ...)
    #                if not specified in the config, defaults to target_xs = -1
    #
    # Output:
    # - one histogram for each target couplings combination and for each variable/selection (SignalRegion only)
    def makeVBFrew(self, inputSigs, target_kl, target_cv, target_c2v, target_xs):

        print "-- VBF reweighting --"
        print "Input samples:", inputSigs
        print "Target kl    :", target_kl
        print "Target cv    :", target_cv
        print "Target c2v   :", target_c2v
        print "Target Cross Section:", target_xs, "[pb]"

        totIterations = len(target_kl) * len(target_cv) * len(target_c2v) * len(self.variables) * len(self.sel_def)
        nIteration = 0

        # Loop on variables and selections to reweight all the histograms
        for var in self.variables:
            for sel in self.sel_def:

                # For each slection/variable get the six input signal histograms
                s0_name = makeHistoName(inputSigs[0], sel+'_SR', var)
                s1_name = makeHistoName(inputSigs[1], sel+'_SR', var)
                s2_name = makeHistoName(inputSigs[2], sel+'_SR', var)
                s3_name = makeHistoName(inputSigs[3], sel+'_SR', var)
                s4_name = makeHistoName(inputSigs[4], sel+'_SR', var)
                s5_name = makeHistoName(inputSigs[5], sel+'_SR', var)

                h_s0 = self.histos[s0_name].Clone(makeHistoName('S0', sel+'_SR', var))
                h_s1 = self.histos[s1_name].Clone(makeHistoName('S1', sel+'_SR', var))
                h_s2 = self.histos[s2_name].Clone(makeHistoName('S2', sel+'_SR', var))
                h_s3 = self.histos[s3_name].Clone(makeHistoName('S3', sel+'_SR', var))
                h_s4 = self.histos[s4_name].Clone(makeHistoName('S4', sel+'_SR', var))
                h_s5 = self.histos[s5_name].Clone(makeHistoName('S5', sel+'_SR', var))

                # Create a list of inputSamples
                inputList = [
                  inputSample(  1, 1, 1, 0.001499, h_s0 ), # node 1
                  inputSample(  1, 1, 0, 0.003947, h_s1 ), # node 2
                  inputSample(  1, 1, 2, 0.001243, h_s2 ), # node 3
                  inputSample(  1, 2, 1, 0.012719, h_s3 ), # node 4
                  inputSample(1.5, 1, 1, 0.057943, h_s4 ), # node 5
                  inputSample(  1, 0, 2,   0.0178, h_s5 ), # node 19
                ]

                # Instantiate a VBFReweight object
                VBFreweighter = VBFReweight(inputList)

                # Get the modeled histogram for each (cv,c2v,kl) target point
                for cv in target_cv:
                    for c2v in target_c2v:
                        for kl in target_kl:

                            modeled_xs, modeled_histo = VBFreweighter.modelSignal(cv,c2v,kl,target_xs)

                            newName = makeHistoName(modeled_histo.GetName(), sel+'_SR', var)
                            modeled_histo.SetNameTitle(newName,newName)

                            self.histos[modeled_histo.GetName()] = modeled_histo

                            # Print a progression bar
                            nIteration +=1
                            printProgressBar(nIteration, totIterations, 'VBF Reweighting:', 'Done', 0, 50)
