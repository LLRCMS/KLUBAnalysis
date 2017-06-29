#### this is a fix to convert outPlotter.root to repalce some histograms with others to pass them to cardmaker.py

from ROOT import *
import argparse
import sys
import os
from array import *

parser = argparse.ArgumentParser(description='Command line parser of parsing options')
parser.add_argument('--dir', dest='dir', help='dir name', default=None)
parser.add_argument('--nonres', dest='nonres', help='dir name', action='store_true', default=False)
parser.add_argument('--onebin', dest='onebin', help='do just one bin for boosted mass?', action='store_true', default=False)
parser.add_argument('--norebinboost', dest='rebinboost', help='do not rebin in boosted cat.', action='store_false', default=True)
args = parser.parse_args()

if not args.dir:
    print "No folder set"
    sys.exit()

outplotName = args.dir + "/outPlotter.root"
if not os.path.isfile(outplotName) :
    print "Cannot open file: " , outplotName
    sys.exit()

fIn = TFile.Open(outplotName)
fOut = TFile.Open(args.dir + "/outPlotter3.root", "recreate")

array4Bin = array('d',[250, 500, 600, 700, 850, 1000, 1600]) ##6 bin
array1Bin = array('d',[250, 500, 1000, 1600]) ##3 bin

print "USING ONE BIN FOR BOOSTED HHMASS? " , args.onebin
print "REBINNING FOR BOOSTED HHMASS? " , args.rebinboost

for key in fIn.GetListOfKeys():
        kname = key.GetName()
        # print key, kname
        obj = fIn.Get(kname)
        
        ### the normal plots cane be directly saved
        # if not "resolved_2b0j_Mcut" in kname : #and not "HHKin_mass_raw_copy" in kname:
        # if not "resolved_2b0j_Mcut" in kname and not "boosted_Mcut" in kname: 
        #     fOut.cd()
        #     obj.Write()

        # else:
        #     if 
        #     print kname

        ### take the shape of "resolved_1b1j_Mcut" and put here instead of 2b0j
        if "resolved_2b0j_Mcut" in kname and "HHKin_mass_raw" in kname and "QCD" in kname and not args.nonres:
            h_2b0j = obj
            name_1b1j = kname.replace("resolved_2b0j_Mcut", "resolved_1b1j_Mcut")
            h_1b1j = fIn.Get(name_1b1j)
            
            # if h_1b1j.Integral() <= 0 and h_2b0j.Integral() > 0:
            #     print "WARNING!! histo " , name_1b1j, " has integral " , h_1b1j.Integral(), "WTF?" , h_2b0j.Integral()
            hnewShape = h_1b1j.Clone(kname)
            newInt = h_2b0j.Integral()/h_1b1j.Integral() if h_1b1j.Integral() > 0 else 0
            hnewShape.Scale (newInt)
            fOut.cd()
            hnewShape.Write()

        elif "defaultBtagLLNoIsoBBTTCut45" in kname and ("HH_mass_raw" in kname or "MT2" in kname) and "QCD" in kname and not "1tag" in kname and args.nonres:
            h_2b0jnonres = obj
            name_1b1jnonres = kname.replace("defaultBtagLLNoIsoBBTTCut45", "defaultBtagLLNoIsoBBTTCut451tag")
            print name_1b1jnonres
            h_1b1jnonres = fIn.Get(name_1b1jnonres)
            
            # if h_1b1jnonres.Integral() <= 0 and h_2b0j.Integral() > 0:
            #     print "WARNING!! histo " , name_1b1j, " has integral " , h_1b1jnonres.Integral(), "WTF?" , h_2b0j.Integral()
            hnewShapenonres = h_1b1jnonres.Clone(kname)
            newIntnonres = h_2b0jnonres.Integral()/h_1b1jnonres.Integral() if h_1b1jnonres.Integral() > 0 else 0
            hnewShapenonres.Scale (newIntnonres)
            fOut.cd()
            hnewShapenonres.Write()

        elif "boosted_Mcut" in kname and "HHKin_mass_raw" in kname and args.rebinboost:
            # obj
            ## rebin everything up to 500 in a single bin -- they are bins 1,2,3,4,5,6
            err = Double(0)
            err2 = Double(0)
            integ = obj.IntegralAndError(1, 6, err)
            integ2 = obj.IntegralAndError(7, 10, err2)

            if args.onebin:
                h_boosted = obj.Rebin(3, "TEMP", array1Bin)
            else:
                h_boosted = obj.Rebin(6, "TEMP", array4Bin)

            h_boosted.SetName(kname)
            h_boosted.SetTitle(kname)

            fOut.cd()
            h_boosted.Write()

            # if args.onebin:
            #     h_boosted = TH1F (kname, kname, array1Bin, 3)
            # else:
            #     h_boosted = TH1F (kname, kname, array4Bin, 6)

            # ## replace first bins content
            # h_boosted.SetBinContent(1, integ)
            # h_boosted.SetBinError(1, err)

            # ## also rebin everything from bin 7 to 10 (last visible bin) in a single bin
            # if args.onebin:
            #     h_boosted.SetBinContent(1, integ)
            #     h_boosted.SetBinError(1, err)
            # else:
            #     # 250, 275, 300, 350, 400, 450, 500, 600, 700, 850, 1000, 1600
            #     h_boosted.SetBinContent(2, obj.GetBinContent(1+6))
            #     h_boosted.SetBinContent(2, obj.GetBinContent(2+6))
            #     h_boosted.SetBinContent(2, obj.GetBinContent(3+6))
            #     h_boosted.SetBinContent(2, obj.GetBinContent(4+6))
            #     h_boosted.SetBinError(1, err)


        else:
            fOut.cd()
            obj.Write()
