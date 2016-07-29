#!/usr/bin/env python
import re
import os.path
from math import *
from ROOT import *

folder = "2016_07_26_out1" #bis = 3cat, ter = 2cat
outString = "_25_jul"
addObserved = True
#folder = "2016_07_01_nr" #bis = 3cat, ter = 2cat
#outString = "_01_jul"

outFormats = [".pdf",".png",".root",".C"]
scale=10.0/100.0
categories = ["resolved_2b0j_Mcut","resolved_1b1j_Mcut", "boosted_Mcut"] #,"defaultBtagMMNoIsoBBTTCut"]
categoriesNames = ["Resolved 2b0j","Resolved 1b1j", "Boosted","Combined"]#,"2015-like","Combined"]
masses = [250, 260, 270, 280, 300, 320, 340, 400, 450, 500, 550, 600, 650, 700, 750, 800, 900]
channels = ["ETau","MuTau","TauTau","Combined"]
channelsName = ["bb e#tau_{h} channel","bb #mu#tau_{h} channel","bb #tau_{h}#tau_{h} channel","bb #mu#tau_{h} + bb e#tau_{h} + bb #tau_{h}#tau_{h}"]
#channels = "COMBINED"
colors = [2,3,4,6]

gEtau = TMultiGraph()
gMutau = TMultiGraph()
gTautau = TMultiGraph()
gCombined = TMultiGraph()

mg = [gEtau,gMutau,gTautau,gCombined] 

gtemp = [TGraphAsymmErrors(),TGraphAsymmErrors(),TGraphAsymmErrors(),TGraphAsymmErrors()]
g95 = [TGraphAsymmErrors(),TGraphAsymmErrors(),TGraphAsymmErrors(),TGraphAsymmErrors()]

ichan= 0
for i in channels :
	name = i
	icat = 0
	#if i == "COMBINED" : folder = "2016_06_28_ter"
	#else : folder = "2016_06_28_bis"
	for c in categories:
		g = TGraphAsymmErrors()
		g.SetTitle(i+" "+categoriesNames[icat])
		g.SetName(i+" "+categoriesNames[icat])		
		g.SetLineColor(colors[icat])
		g.SetMarkerColor(colors[icat])
		g.SetMarkerStyle(20)
		g.SetFillStyle(0)	
		if icat==3 :
			g.SetLineStyle(3)
			g.SetMarkerStyle(24)
		#elif icat == 4 :
		#	g.SetLineColor(1)
		#	g.SetMarkerColor(1)	
		#	name = "COMBINED"	
		
		for m in masses:
			#if i == "COMBINED" :
			#	fin = TFile.Open("cards_"+i+"_"+folder+"/Radion"+str(m)+"COMBINED"+c+"/higgsCombineRadion"+str(m)+"_forLim.Asymptotic.mH125.root")
			#else :
			fin = TFile.Open("/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_"+name+"_"+folder+"/Radion"+str(m)+c+"HHKin_mass_raw/higgsCombineRadion"+str(m)+"_forLim.Asymptotic.mH125.root")
			tree = fin.Get("limit")
			high=0
			low=0
			limit = 0
			for entry in tree :
				if tree.quantileExpected == 0.5 : 
					limit = tree.limit*scale
				elif tree.quantileExpected > 0.83 and tree.quantileExpected < 0.85 : high = tree.limit*scale
				elif tree.quantileExpected > 0.15 and tree.quantileExpected < 0.17 : low = tree.limit*scale
			if limit>0 : #and limit < 50 : 
				g.SetPoint(g.GetN(),m,limit)
				g.SetPointError(g.GetN()-1,0,0,limit-low,high-limit)
		if icat == 3 :
			gtemp[ichan] = g
		else :
			mg[ichan].Add(g)
		icat = icat+1
	ichan=ichan+1

for c in range(len(channels)) :
	gAll = TGraphAsymmErrors()
	gAll.SetTitle("Combined categories")
	gAll.SetName("Combined categories")
	gAll.SetLineColor(1)
	gAll.SetMarkerColor(1)
	gAll.SetMarkerStyle(20)
	gAll.SetFillStyle(0)
	gObs = TGraph()
	gObs.SetTitle("Observed")
	gObs.SetName("Observed")
	gObs.SetLineColor(1)
	gObs.SetMarkerColor(1)
	gObs.SetLineWidth(2)
	gObs.SetMarkerStyle(8)
	gObs.SetFillStyle(0)
	
	print "DOING CHANNEL: " , channelsName[c] , "mass, obs, exp"
	for m in masses:
		fin = TFile.Open("/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_"+channels[c]+"_"+folder+"/Radion"+str(m)+"/higgsCombineRadion"+str(m)+"_forLim.Asymptotic.mH125.root")
		tree = fin.Get("limit")
		high=0
		low=0
		limit = 0
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

		if limit>0  : 
			gAll.SetPoint(gAll.GetN(),m,limit)
			gAll.SetPointError(gAll.GetN()-1,0,0,limit-low,high-limit)
			g95[c].SetPoint(g95[c].GetN(),m,limit)
			g95[c].SetPointError(g95[c].GetN()-1,0,0,limit-low95,high95-limit)			
			gObs.SetPoint(gAll.GetN(),m,obs)
			# print m, obs, limit
			print m , '%s' % float('%.2g' % obs), '%s' % float('%.2g' % limit)

			#g95[c].SetPoint(g95[c].GetN(),m,limit-low95)
			#g95[c].SetPoint(g95[c].GetN(),m,high95-limit)
			#g68[c].SetPoint(g68[c].GetN(),m,limit-low)
			#g68[c].SetPoint(g68[c].GetN(),m,high-limit)
	print "adding gall", gAll.GetN()
	mg[c].Add(gAll)
	mg[c].Add(gObs)
	mg[c].Add(gtemp[c])

#c = [TCanvas("ETau"),TCanvas("MuTau"),TCanvas("TauTau"),TCanvas("COMBINED")]
#for ic in range(len(channels)):
#	c[ic].cd()
#	mg[ic].Draw("ALP")
#	mg[ic].GetYaxis().SetRangeUser(0.001*scale,1000*scale)
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
	gexp.SetLineColor(kBlue+2)
	gexp.SetLineStyle(2)
	gexp.SetLineWidth(2)
	gexp.SetFillColor(0)
	g68 = mg[ic].GetListOfGraphs().FindObject("Combined categories").Clone()
	g68.SetTitle("Expected #pm 1#sigma")
	g68.SetMarkerStyle(0)
	g68.SetMarkerColor(3)
	# colG = TColor()
	# colG.SetRGB(0, 255, 0); #r, g, b
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
	g95[ic].GetYaxis().SetRangeUser(0.01*scale,1000*scale)
	g95[ic].GetXaxis().SetRangeUser(250,900)
	g95[ic].GetYaxis().SetTitle("95% CL on #sigma#times BR(H#rightarrow hh#rightarrow bb#tau#tau) [pb]")
	g95[ic].GetXaxis().SetTitle("m_{H} [GeV]")

	g95[ic].GetYaxis().SetTitleSize(0.047)
	g95[ic].GetXaxis().SetTitleSize(0.055)
	g95[ic].GetYaxis().SetLabelSize(0.045)
	g95[ic].GetXaxis().SetLabelSize(0.045)
	g95[ic].GetXaxis().SetLabelOffset(0.012)

	g95[ic].GetYaxis().SetTitleOffset(1.15)
	g95[ic].GetXaxis().SetTitleOffset(1.1)


	gStyle.SetOptTitle(0)
	g95[ic].Draw("A3")
	g68.Draw("3SAME")
	gexp.Draw("PLXSAME")
	if addObserved :
		gObs = mg[ic].GetListOfGraphs().FindObject("Observed").Clone()
		gObs.Draw("PLSAME")

	legend = TLegend()#	cNice[ic].BuildLegend()
	legend.SetFillStyle(0)
	legend.SetLineColor(0)
	legend.SetX1(0.164)
	legend.SetY1(0.1992373)
	legend.SetX2(0.56)
	legend.SetY2(0.39)
	if addObserved : legend.AddEntry(gObs,"Observed","PL")
	legend.AddEntry(gexp, gexp.GetTitle(), "lp")
	legend.AddEntry(g68, g68.GetTitle(), "f")
	legend.AddEntry(g95[ic], g95[ic].GetTitle(), "f")
	legend.Draw()
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
	pt.Draw()
	pt2.Draw()
	pt4.Draw()
	for ext in outFormats : cNice[ic].SaveAs("plots/preApp_01_jul/ResonantFinal_"+channels[ic]+outString+ext)

raw_input()

#c2 = TCanvas()
#c2.cd()
#mg[1].Draw("ALP")
#c2.BuildLegend()
#c3 = TCanvas()
#c3.cd()
#mg[2].Draw("ALP")
#c3.BuildLegend()
