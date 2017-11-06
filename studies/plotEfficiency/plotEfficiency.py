import ROOT

def openFileList(filelistname):
    
    # KEY: TH1F h_effSummary;1  h_effSummary
    # KEY: TH1F h_effSummary_MuTau;1    h_effSummary_MuTau
    # KEY: TH1F h_effSummary_ETau;1 h_effSummary_ETau
    # KEY: TH1F h_effSummary_TauTau;1   h_effSummary_TauTau
    # KEY: TH1F h_effSummary_MuMu;1 h_effSummary_MuMu
    # KEY: TH1F h_effSummary_EE;1   h_effSummary_EE
    # KEY: TH1F h_effSummary_EMu;1  h_effSummary_EMu

    histosName = ['h_effSummary', 'h_effSummary_MuTau', 'h_effSummary_ETau', 'h_effSummary_TauTau']
    histos = []

    print '... creating TChain from: ' , filelistname
    
    tree = ROOT.TChain('HTauTauTree')
    nfiles = 0
    with open (filelistname) as fIn:
        for line in fIn:
            line = (line.split("#")[0]).strip()
            if line:
                
                ## add to the tchain
                tree.Add(line)
                
                ## sum up the summary histos of preselections
                rfile = ROOT.TFile.Open(line)
                htmp = [rfile.Get(h) for h in histosName]
                for idx, h in enumerate(htmp):
                    # print idx
                    if len(histos) < len(histosName):
                        histos.append(h.Clone('heff_' + h.GetName()))
                        histos[-1].SetDirectory(0)
                    else:
                        histos[idx].Add(h)
                rfile.Close()
    
    result = (tree,) + tuple(histos)
    return result

#############################################################################################
#############################################################################################
#############################################################################################

folderIn = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_HH_lambdarew_21'


########
if folderIn[-1] != '/':
    folderIn += '/'


####### make the tchain
data = openFileList(folderIn + 'goodfiles.txt')
# print data

tree = data[0]
effTot = data[1]

fOut = ROOT.TFile('histos_debug.root', 'recreate')
for i in range(1, len(data)):
    data[i].Write()
