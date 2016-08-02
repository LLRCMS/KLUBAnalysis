#!/usr/bin/env python
import re
import os.path
from math import *
from ROOT import *

# cards_Combined_2016_07_26

# folder = "2016_07_26_out3" #bis = 3cat, ter = 2cat
# outString = "_25_jul"

folder = "2016_07_26" #bis = 3cat, ter = 2cat
outString = "_25_jul"

outFormats = [".pdf",".png",".root",".C"]
benchmark = 0 # -1: 1507 points, 0: lambda, 1: benchmark, by default we do not plot the 1507 points
addObserved = True
#scale=1000*37.9*0.073
scale=10000.0/100.0 #/37.9/0.073
categories = ["defaultBtagLLNoIsoBBTTCutKine80HH_mass_raw","defaultBtagLLNoIsoBBTTCutKine80HH_mass_raw", "defaultBtagLLNoIsoBBTTCut45HH_mass_raw","COMBINED"]
#categoriesNames = ["Resolved 2b0j","Resolved 1b1j", "Boosted","2015-like","Combined"]
#masses = [250, 260, 270, 280, 300, 320, 340, 400, 450, 500, 550, 600, 650, 700, 750, 800, 900]
channels = ["ETau","MuTau","TauTau","Combined"]
channelsName = ["bb e#tau_{h} channel","bb #mu#tau_{h} channel","bb #tau_{h}#tau_{h} channel","bb #mu#tau_{h} + bb e#tau_{h} + bb #tau_{h}#tau_{h}"]
#channels = "COMBINED"
#colors = [2,3,4,6]
pointNumbers=[]
#observed2d=[[],[],[],[]]
#expected2d=[[],[],[],[]]

def getExpValue( kl,  yt): 
	BR =1 
	return (2.09*yt*yt*yt*yt +   0.28*yt*yt*kl*kl  -1.37*yt*yt*yt*kl)*2.44185/BR;

def getExpLine(c,  xmin,  xmax,  yt):
	BR = 1
	myFunc =  TF1("myFunc","(2.09*[0]*[0]*[0]*[0] + 0.28*[0]*[0]*x*x -1.37*[0]*[0]*[0]*x)*2.44185/[1]",xmin,xmax);
	myFunc.SetParameter(0,yt); 
	myFunc.SetParameter(1,BR); 
	graph = TGraph(myFunc);
	ci = TColor.GetColor("#ff0000");
	c.cd();
	graph.SetLineColor(ci);
	graph.SetLineWidth(3);
	graph.Draw("l");
	nP = int((xmax-xmin)*10.0)
	#Graph_syst_Scale_x = double[nP] 
	#Graph_syst_Scale_y= double[nP] 
	#Graph_syst_Scale_x_err = double[nP] 
	#Graph_syst_Scale_y_errup=  double[nP] 
	#Graph_syst_Scale_y_errdown=  double[nP] 
	Graph_syst_Scale =  TGraphAsymmErrors(nP)
	for i in range(nP) : 
		Graph_syst_Scale_x=(xmin+(i*1.)/10.)
		Graph_syst_Scale_y=(getExpValue(xmin+(i*1.)/10.,yt)) #; //(2.11+0.29*(-4.+(i*1.)/10.)*(-4.+(i*1.)/10.)-1.40*(-4.+(i*1.)/10.))*2.5039)
		Graph_syst_Scale_x_err=(0)
		Graph_syst_Scale_y_errup=(  (2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.44185*0.053/BR)
		Graph_syst_Scale_y_errdown=((2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.44185*0.067/BR)
		Graph_syst_Scale.SetPoint(i,Graph_syst_Scale_x,Graph_syst_Scale_y)
		Graph_syst_Scale.SetPointError(i,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown)
	#Graph_syst_Scale =  TGraphAsymmErrors(nP,Graph_syst_Scale_x,Graph_syst_Scale_y,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown);
	Graph_syst_Scale.SetLineColor(kRed)
	Graph_syst_Scale.SetFillColor(kRed)
	Graph_syst_Scale.SetFillStyle(3001)
	Graph_syst_Scale.DrawClone("e3");

gROOT.SetBatch(True)

gEtau = TMultiGraph()
gMutau = TMultiGraph()
gTautau = TMultiGraph()
gCombined = TMultiGraph()

mg = [gEtau,gMutau,gTautau,gCombined] 

gtemp = [TGraphAsymmErrors(),TGraphAsymmErrors(),TGraphAsymmErrors(),TGraphAsymmErrors()]
g95 = [TGraphAsymmErrors(),TGraphAsymmErrors(),TGraphAsymmErrors(),TGraphAsymmErrors()]

#ichan= 0
#for i in channels :
#	name = i
#	icat = 0
#	#if i == "COMBINED" : folder = "2016_06_28_ter"
#	#else : folder = "2016_06_28_bis"
#	for c in categories:
#		g = TGraphAsymmErrors()
#		g.SetTitle(i+" "+categoriesNames[icat])
#		g.SetName(i+" "+categoriesNames[icat])		
#		g.SetLineColor(colors[icat])
#		g.SetMarkerColor(colors[icat])
#		g.SetMarkerStyle(20)
#		g.SetFillStyle(0)	
#		if icat==3 :
#			g.SetLineStyle(3)
#			g.SetMarkerStyle(24)
#		#elif icat == 4 :
#		#	g.SetLineColor(1)
#		#	g.SetMarkerColor(1)	
#		#	name = "COMBINED"	
#		
#		for m in masses:
#			#if i == "COMBINED" :
#			#	fin = TFile.Open("cards_"+i+"_"+folder+"/Radion"+str(m)+"COMBINED"+c+"/higgsCombineRadion"+str(m)+"_forLim.Asymptotic.mH125.root")
#			#else :
#			fin = TFile.Open("cards_"+name+"_"+folder+"/Radion"+str(m)+c+"HHKin_mass_raw/higgsCombineRadion"+str(m)+"_forLim.Asymptotic.mH125.root")
#			tree = fin.Get("limit")
#			high=0
#			low=0
#			limit = 0
#			for entry in tree :
#				if tree.quantileExpected == 0.5 : 
#					limit = tree.limit*scale
#				elif tree.quantileExpected > 0.83 and tree.quantileExpected < 0.85 : high = tree.limit*scale
#				elif tree.quantileExpected > 0.15 and tree.quantileExpected < 0.17 : low = tree.limit*scale
#			if limit>0 : #and limit < 50 : 
#				g.SetPoint(g.GetN(),m,limit)
#				g.SetPointError(g.GetN()-1,0,0,limit-low,high-limit)
#		if icat == 3 :
#			gtemp[ichan] = g
#		else :
#			mg[ichan].Add(g)
#		icat = icat+1
#	ichan=ichan+1

for c in range(len(channels)) :
	gAll = TGraphAsymmErrors()
	gAll.SetTitle("Combined categories")
	gAll.SetName("Combined categories")
	gAll.SetFillStyle(0)
	gObs = TGraph()
	gObs.SetTitle("Observed")
	gObs.SetName("Observed")
	gObs.SetLineColor(1)
	gObs.SetMarkerColor(1)
	gObs.SetMarkerStyle(20)
	gObs.SetFillStyle(0)
	if benchmark == 1 : 
		npoints = 12
		app = "benchrew"
	elif benchmark == 0 :
		npoints = 52
		app = "lambdarew"
	elif benchmark < 0:
		npoints = 1507
		app = "5Dplane"
	for m in range(0,npoints):
		fileLocation = "/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_"+channels[c]+"_"+folder+"/HH"+app+str(m)+categories[c]+"/higgsCombineHH"+app+str(m)+"_forLim.Asymptotic.mH125.root"
		# print "FILE: " , fileLocation
		if not os.path.isfile(fileLocation) : continue
		if benchmark<0 : pointNumbers.append(m)
		fin = TFile.Open(fileLocation)
		tree = fin.Get("limit")
		if not tree :
			print "MALEDETTO TREE", fileLocation, fin, tree
			continue
		high=0
		low=0
		limit = 0
		obs = 0
		offsetX = 20
		errX = 0.3
		if benchmark == 1 : 
			offsetX = -1
			errX = 0.15
		for entry in tree :
			if tree.quantileExpected < 0 :
				obs = tree.limit*scale
			elif tree.quantileExpected == 0.5 : 
				limit = tree.limit*scale
			elif tree.quantileExpected > 0.83 and tree.quantileExpected < 0.85 : 
				high = tree.limit*scale
			elif tree.quantileExpected > 0.15 and tree.quantileExpected < 0.17: 
				low = tree.limit*scale 
			elif tree.quantileExpected > 0.024 and tree.quantileExpected < 0.026 : 
				low95 = tree.limit*scale
			elif tree.quantileExpected > 0.974 and tree.quantileExpected < 0.976 : 
				high95 = tree.limit*scale

		if limit>0 : #and limit < 50 : 
			gAll.SetPoint(gAll.GetN(),m-offsetX,limit)
			gAll.SetPointError(gAll.GetN()-1,0+errX,0+errX,limit-low,high-limit)
			g95[c].SetPoint(g95[c].GetN(),m-offsetX,limit)
			g95[c].SetPointError(g95[c].GetN()-1,0+errX,0+errX,limit-low95,high95-limit)
			gObs.SetPoint(gObs.GetN(),m-offsetX,obs)	
			# print "OBS: " , m-offsetX,obs
			# print "  - EXP: " , m-offsetX,limit
			# print "\n"
			#if benchmark < 0:
			#	observed2d[c].append(obs)
			#	expected2d[c].append(limit)
			#g95[c].SetPoint(g95[c].GetN(),m,limit-low95)
			#g95[c].SetPoint(g95[c].GetN(),m,high95-limit)
			#g68[c].SetPoint(g68[c].GetN(),m,limit-low)
			#g68[c].SetPoint(g68[c].GetN(),m,high-limit)
	print "adding gall", gAll.GetN()
	mg[c].Add(gAll)
	mg[c].Add(gtemp[c])
	mg[c].Add(gObs)

#c = [TCanvas("ETau"),TCanvas("MuTau"),TCanvas("TauTau"),TCanvas("COMBINED")]
#for ic in range(len(channels)):
#	c[ic].cd()
#	mg[ic].Draw("ALP")
#	mg[ic].GetYaxis().SetRangeUser(0.001*scale,100*scale)
#	mg[ic].GetYaxis().SetTitle("95% CL on #sigma#times BR(H#rightarrow H#rightarrow hh#rightarrow bb#tau#tau) [pb]")
#	mg[ic].GetXaxis().SetTitle("m_{H} [GeV]")
#	c[ic].BuildLegend()
#	c[ic].SetLogy()
#	c[ic].SaveAs("limitResonant_"+channels[ic]+outString+".pdf")

cNice = [
	TCanvas("ETauFinal", "ETauFinal", 650, 500),
	TCanvas("MuTauFinal", "MuTauFinal", 650, 500),
	TCanvas("TauTauFinal", "TauTauFinal", 650, 500),
	TCanvas("COMBINEDFinal", "COMBINEDFinal", 650, 500)
	]

for ic in range(len(channels)):
	print "DOING CHANNEL", channels[ic]
	cNice[ic].cd()
	cNice[ic].SetFrameLineWidth(3)
	cNice[ic].SetBottomMargin (0.15)
	cNice[ic].SetRightMargin (0.05)
	cNice[ic].SetLeftMargin (0.15)

	#mg[ic].GetListOfGraphs().ls()
	gexp = mg[ic].GetListOfGraphs().FindObject("Combined categories").Clone()
	gexp.SetTitle("Expected CLs")
	gexp.SetMarkerStyle(24)
	gexp.SetMarkerColor(4)
	gexp.SetMarkerSize(0.8)
	gexp.SetLineColor(4)
	gexp.SetLineStyle(2)
	gexp.SetFillColor(0)
	g68 = mg[ic].GetListOfGraphs().FindObject("Combined categories").Clone()
	g68.SetTitle("Expected #pm 1#sigma")
	g68.SetMarkerStyle(0)
	g68.SetMarkerColor(3)
	g68.SetFillColor(TColor.GetColor(0, 220, 60))
	g68.SetLineColor(TColor.GetColor(0, 220, 60))
	g68.SetFillStyle(1001)
	g95[ic].SetTitle("Expected #pm 2#sigma")
	g95[ic].SetName(channels[ic])
	g95[ic].SetMarkerStyle(0)
	g95[ic].SetMarkerColor(5)
	g95[ic].SetFillColor(TColor.GetColor(254, 234, 0))
	g95[ic].SetLineColor(TColor.GetColor(254, 234, 0))
	g95[ic].SetFillStyle(1001)
	g95[ic].GetYaxis().SetRangeUser(1,100000)
	gObs = mg[ic].GetListOfGraphs().FindObject("Observed").Clone()

	g95[ic].GetYaxis().SetTitleSize(0.047)
	g95[ic].GetXaxis().SetTitleSize(0.055)
	g95[ic].GetYaxis().SetLabelSize(0.045)
	g95[ic].GetXaxis().SetLabelSize(0.045)
	g95[ic].GetXaxis().SetLabelOffset(0.012)

	g95[ic].GetYaxis().SetTitleOffset(1.15)
	g95[ic].GetXaxis().SetTitleOffset(1.1)

	#plot in the format to be passed to the 2D Xanda's macro
	if benchmark < 0:
		print " "
		ipoint = 0
		#gObs.Print()
		#print observed2d,expected2d
		#o2 = gObs.GetY()
		#e = gexp.GetY()
		for point in range(1507):
			if point in pointNumbers :
				o1, o2, e = Double(0), Double(0), Double(0)
				gObs.GetPoint(ipoint,Double(0),o2)
				gexp.GetPoint(ipoint,Double(0),e)
				print point+1, "{0:.4f}".format(o2), "{0:.4f}".format(o2), 0, 0, 0, 0
				#print point, "{0:.4f}".format(observed2d[ic][ipoint]), "{0:.4f}".format(expected2d[ic][ipoint]), 0, 0, 0, 0
				ipoint = ipoint +1
			else :
				print point+1, 9999, 9999, 0, 0, 0, 0

	if benchmark == 1 : 
		g95[ic].GetYaxis().SetRangeUser(1,100000)
		g95[ic].GetXaxis().SetRangeUser(-0.49+1.,11.49+1.) 
	else : g95[ic].GetXaxis().SetRangeUser(-21,32) 
	g95[ic].GetYaxis().SetTitle("95% CL on #sigma#times BR(hh#rightarrow bb#tau#tau) [fb]")
	if benchmark == 1 : 
		g95[ic].GetXaxis().SetTitle("Benchmark number") 
		g95[ic].GetXaxis().SetNdivisions(13) 
	else : g95[ic].GetXaxis().SetTitle("k_{#lambda}")
	gStyle.SetOptTitle(0)
	if benchmark == 1 : 
		g95[ic].Draw("A2")
		g68.Draw("2SAME")
		gexp.Draw("PXSAME")
	elif benchmark == 0 :
		g95[ic].Draw("A2")
		g68.Draw("2SAME")
		gexp.Draw("PXSAME")
	if addObserved and benchmark>-1:
		gObs.Draw("PSAME")
	legend = TLegend()#	cNice[ic].BuildLegend()
	legend.SetFillStyle(0)
	legend.SetBorderSize(0)
	legend.SetX1(0.15)
	legend.SetY1(0.181)
	legend.SetX2(0.546)
	legend.SetY2(0.372)
	if benchmark == 1:
		legend.SetX1(0.630)
		legend.SetY1(0.171)
		legend.SetX2(1.026)
		legend.SetY2(0.362)		
	if addObserved : legend.AddEntry(gObs,"Observed","PL")
	legend.AddEntry(gexp, gexp.GetTitle(), "lp")
	legend.AddEntry(g68, g68.GetTitle(), "f")
	legend.AddEntry(g95[ic], g95[ic].GetTitle(), "f")
	
	# fakePlot = TGraphAsymmErrors ("fakePlot", "fakePlot", 100, 0, 100);
	fakePlot = TGraphAsymmErrors()
	
	fakePlot.SetFillColor(kRed)
	fakePlot.SetFillStyle(3001)
	fakePlot.SetLineColor(kRed)
	fakePlot.SetLineWidth(3)
	if benchmark == 0:
		legend.AddEntry(fakePlot, "Theory prediction", "lf")
	if benchmark>-1 : legend.Draw()
	cNice[ic].SetLogy()
	cNice[ic].SetGridy(1)
	cNice[ic].SetGridx(1)

	pt = TPaveText(0.1663218,0.7966102,0.3045977,0.8898305,"brNDC")
	pt.SetBorderSize(0)
	pt.SetTextAlign(12)
	pt.SetTextFont(62)
	pt.SetTextSize(0.05)
	pt.SetFillColor(0)
	pt.SetFillStyle(0)
	pt.AddText("CMS" )
	pt.AddText("#font[52]{preliminary}")
	pt2 = TPaveText(0.79,0.9066667,0.8997773,0.957037,"brNDC")
	pt2.SetBorderSize(0)
	pt2.SetFillColor(0)
	pt2.SetTextSize(0.040)
	pt2.SetTextFont(42)
	pt2.SetFillStyle(0)
	pt2.AddText("12.9 fb^{-1} (13 TeV)")

	pt4 = TPaveText(0.4819196,0.7780357,0.9008929,0.8675595,"brNDC")
	pt4.SetTextAlign(12)
	pt4.SetFillColor(0)
	pt4.SetFillStyle(0)
	pt4.SetTextFont(42)
	pt4.SetTextSize(0.05)
	pt4.SetBorderSize(0)
	pt4.SetTextAlign(32)
	pt4.AddText(channelsName[ic]) 
	if ic == 3 : pt4.AddText("#scale[0.8]{Combined channels}")
	if benchmark > -1:
		pt.Draw()
		pt2.Draw()
		pt4.Draw()


	#getExpLine(cNice[ic],-20,30,1)
	xmin=-20.4
	xmax=31.4
	yt=1
	BR = 1
	myFunc =  TF1("myFunc","(2.09*[0]*[0]*[0]*[0] + 0.28*[0]*[0]*x*x -1.37*[0]*[0]*[0]*x)*2.44185/[1]",xmin,xmax);
	myFunc.SetParameter(0,yt); 
	myFunc.SetParameter(1,BR); 
	graph = TGraph(myFunc);
	ci = TColor.GetColor("#ff0000");
	cNice[ic].cd();
	graph.SetLineColor(ci);
	graph.SetLineWidth(3);
	if benchmark ==0 : graph.Draw("l");
	nP = int((xmax-xmin)*10.0)
	#Graph_syst_Scale_x = double[nP] 
	#Graph_syst_Scale_y= double[nP] 
	#Graph_syst_Scale_x_err = double[nP] 
	#Graph_syst_Scale_y_errup=  double[nP] 
	#Graph_syst_Scale_y_errdown=  double[nP] 
	Graph_syst_Scale =  TGraphAsymmErrors(nP)
	for i in range(nP) : 
		Graph_syst_Scale_x=(xmin+(i*1.)/10.)
		Graph_syst_Scale_y=(getExpValue(xmin+(i*1.)/10.,yt)) #; //(2.11+0.29*(-4.+(i*1.)/10.)*(-4.+(i*1.)/10.)-1.40*(-4.+(i*1.)/10.))*2.5039)
		Graph_syst_Scale_x_err=(0)
		Graph_syst_Scale_y_errup=(  (2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.44185*0.053/BR)
		Graph_syst_Scale_y_errdown=((2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.44185*0.067/BR)
		Graph_syst_Scale.SetPoint(i,Graph_syst_Scale_x,Graph_syst_Scale_y)
		Graph_syst_Scale.SetPointError(i,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown)
	#Graph_syst_Scale =  TGraphAsymmErrors(nP,Graph_syst_Scale_x,Graph_syst_Scale_y,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown);
	Graph_syst_Scale.SetLineColor(kRed)
	Graph_syst_Scale.SetFillColor(kRed)
	Graph_syst_Scale.SetFillStyle(3001)
	if benchmark == 0 : 
		Graph_syst_Scale.Draw("e3");
		app2 = "Final"
	elif benchmark == 1 : app2 = "Benchmark"

	if benchmark>-1:
		for ext in outFormats : cNice[ic].SaveAs("plots/preApp_01_jul/limit"+app2+"_"+channels[ic]+outString+ext)
		print "SAVED IN " , "plots/preApp_01_jul/limit"+app2+"_"+channels[ic]+outString+ext
raw_input()

   #return ;


#c2 = TCanvas()
#c2.cd()
#mg[1].Draw("ALP")
#c2.BuildLegend()
#c3 = TCanvas()
#c3.cd()
#mg[2].Draw("ALP")
#c3.BuildLegend()
