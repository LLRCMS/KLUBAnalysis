import ROOT
import argparse


parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--mass', dest='mass', help='mass or reweight point', default=None)
parser.add_argument('--nonres', dest='nonres', help='non resonant sample', action='store_true', default=False)
args = parser.parse_args()

folder = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_HH_lambdarew_' if args.nonres else '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_GluGluToRadionToHHTo2B2Tau_M-'
folder += args.mass

fOutName = 'results/purity_'
fOutName += ('nonresonant_' if args.nonres else 'resonant_')
fOutName += (args.mass + '.root')

ch = ROOT.TChain('HTauTauTree')

printReport = True

###### open files and add to tchain
print '... opening files in folder', folder
fList = open(folder + '/' + 'goodfiles.txt')
nfiles = 0
for line in fList:
    line = line.strip()
    if not line: continue
    ch.Add(line)
    nfiles += 1
print '... opened', nfiles, 'files'

###### make the histograms for the 3 final states
histos = [
    ROOT.TH1F('hDecayMode0', 'hDecayMode0', 6, 0, 6),
    ROOT.TH1F('hDecayMode1', 'hDecayMode1', 6, 0, 6),
    ROOT.TH1F('hDecayMode2', 'hDecayMode2', 6, 0, 6),
]

histoUnscaled = [ ## for errors later..
    ROOT.TH1F('unscaled_hDecayMode0', 'unscaled_hDecayMode0', 6, 0, 6),
    ROOT.TH1F('unscaled_hDecayMode1', 'unscaled_hDecayMode1', 6, 0, 6),
    ROOT.TH1F('unscaled_hDecayMode2', 'unscaled_hDecayMode2', 6, 0, 6),
]

# non ha effetto sugli errori...
# for h in histos:
#     h.Sumw2()

channelNameLatex = [
    '#mu#tau_{h}',
    'e#tau_{h}',
    '#tau_{h}#tau_{h}',
]

channelName = [
    'MuTau',
    'ETau',
    'TauTau'
]


names = [
    '#mu#tau_{h}',
    'e#tau_{h}',
    '#tau_{h}#tau_{h}',
    '#mu#mu',
    'ee',
    'e#mu',
]

selections = [
    'pairType == 0 && dau1_pt > 23 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && isOS != 0 && dau1_iso < 0.15 && dau2_MVAiso >= 3', ## mutau
    'pairType == 1 && dau1_pt > 27 && abs(dau1_eta) < 2.1 && dau2_pt > 20 && abs(dau2_eta) < 2.3 && nleps == 0 && isOS != 0 && dau1_iso < 0.1 && dau2_MVAiso >= 3', ## etau
    'pairType == 2 && dau1_pt > 45 && abs (dau1_eta) < 2.1 && dau2_pt > 45 && abs (dau2_eta) < 2.1 && nleps == 0 && isOS != 0 && dau1_MVAiso >= 3 && dau2_MVAiso >= 3', ## tautau
]

weights = 'MC_weight * IdAndIsoSF * trigSF * PUReweight'

##### fill the histos
ROOT.gROOT.SetBatch(True)
ch.Draw('genDecMode1 + genDecMode2 - 8 >> hDecayMode0', weights + ' * (' + selections[0] + ')')
ch.Draw('genDecMode1 + genDecMode2 - 8 >> hDecayMode1', weights + ' * (' + selections[1] + ')')
ch.Draw('genDecMode1 + genDecMode2 - 8 >> hDecayMode2', weights + ' * (' + selections[2] + ')')

ch.Draw('genDecMode1 + genDecMode2 - 8 >> unscaled_hDecayMode0', selections[0])
ch.Draw('genDecMode1 + genDecMode2 - 8 >> unscaled_hDecayMode1', selections[1])
ch.Draw('genDecMode1 + genDecMode2 - 8 >> unscaled_hDecayMode2', selections[2])


# hControl1 = ROOT.TH1F('hControl1', 'hControl1', 6, 0, 6)
# hControl2 = ROOT.TH1F('hControl2', 'hControl2', 6, 0, 6)

# ch.Draw('genDecMode1 + genDecMode2 - 8 >> hControl1', 'MC_weight * IdAndIsoSF * trigSF * PUReweight * (' + selections[2] + ')')
# ttf = ROOT.TTreeFormula('ttf', selections[2], ch)
# ch.SetNotify(ttf)
# iev = 0
# while True:
#     got = ch.GetEntry(iev)
#     if got == 0: break
#     iev += 1
#     if not ttf.EvalInstance(): continue
#     val = ch.genDecMode1 + ch.genDecMode2 - 8
#     ww = ch.MC_weight * ch.IdAndIsoSF * ch.trigSF * ch.PUReweight
#     hControl2.Fill(val, ww)

# hControl1.Scale(1./hControl1.Integral())
# hControl2.Scale(1./hControl2.Integral())

# print "from chain draw"
# for i in range(1, 7):
#     print i, hControl1.GetBinContent(i)
# print "from chain fill"
# for i in range(1, 7):
#     print i, hControl2.GetBinContent(i)

for idx, h in enumerate(histos):
    for i in range(1, 7):
        h.GetXaxis().SetBinLabel(i, names[i-1])

    # histoUnscaled.append(h.Clone('unscaled_' + h.GetName()))
    
    if h.Integral() > 0:
        h.Scale(1./h.Integral())

    
    if printReport:
        print "** CHANNEL: ", channelName[idx]
        for i in range(1, h.GetNbinsX()+1):
            frac   = '%.2f' % (100.*h.GetBinContent(i))
            Err    = '%.2f' % (100.*h.GetBinError(i))
            print '  .. ', h.GetXaxis().GetBinLabel(i), frac, '+/-', Err
        print '\n'


print ' ... output name is', fOutName
fOut = ROOT.TFile.Open(fOutName, 'recreate')
fOut.cd()
for h in histos:
    h.Write()
for h in histoUnscaled:
    h.Write()