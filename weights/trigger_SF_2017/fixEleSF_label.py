from ROOT import *
from array import *

#fname = "Electron_Ele32orEle35"
#fname = "Electron_EleTau_Ele24"
fname = "Electron_Ele35"

inFile = TFile.Open(fname+".root")

inFile.ls()
if not inFile:
    print fname, " not found"


graphs = []
for key in inFile.GetListOfKeys():
    
    name  = key.GetName()
    print name    
    if "etaBinsH" in name:
        continue
    graph = inFile.Get(name)
    graph.SetName(name)    
    graphs.append(graph)

oldHisto = inFile.Get("etaBinsH")

labels = []
binEdge = []
for i in range(oldHisto.GetNbinsX()):
    
    label = oldHisto.GetXaxis().GetBinLabel(i+1)
    print i+1, label
    labels.append(label)

for label in labels:
    if "Lt" in label:
        binEdge.append(float(0))
        label = label.replace("EtaLt","")
        

    if "to" in label:
        edges = label.split("to")
        print edges
        label = edges[1] 

    binEdge.append(float(label.replace("p",".")))
print binEdge 

binEdge = sorted(binEdge)
binEdge = array('d',binEdge)


newHisto = TH1F("etaBinsH", "", len(binEdge)-1, binEdge)

for i, label in enumerate(labels):
    print i, label
    newHisto.GetXaxis().SetBinLabel(i+1, label)


outFile = TFile.Open(fname+'_fix.root',"recreate")

for graph in graphs:
    graph.Write()
newHisto.Write()
outFile.ls()
