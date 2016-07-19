#!/usr/bin/env python
import re
import os.path
from math import *
from ROOT import *

folder = "2016_07_02_nr" #bis = 3cat, ter = 2cat
outString = "_02_jul"

outFormats = [".pdf",".png",".root",".C"]
benchmark = False
#scale=1000*37.9*0.073
scale=10000 #/37.9/0.073
categories = ["defaultBtagLLNoIsoBBTTCutKine80HH_mass_raw","defaultBtagLLNoIsoBBTTCutKine80HH_mass_raw", "defaultBtagLLNoIsoBBTTCut45HH_mass_raw","COMBINED"]
#categoriesNames = ["Resolved 2b0j","Resolved 1b1j", "Boosted","2015-like","Combined"]
#masses = [250, 260, 270, 280, 300, 320, 340, 400, 450, 500, 550, 600, 650, 700, 750, 800, 900]
channels = ["ETau","MuTau","TauTau","Combined"]
channelsName = ["bb e#tau_{h} channel","bb #mu#tau_{h} channel","bb #tau_{h}#tau_{h} channel","bb #mu#tau_{h} + bb e#tau_{h} + bb #tau_{h}#tau_{h}"]
#channels = "COMBINED"
#colors = [2,3,4,6]

def getExpValue( kl,  yt): 
	BR =1 
	return (2.11*yt*yt*yt*yt +   0.29*yt*yt*kl*kl  -1.40*yt*yt*yt*kl)*2.7667/BR;

def getExpLine(c,  xmin,  xmax,  yt):
	BR = 1
	myFunc =  TF1("myFunc","(2.09*[0]*[0]*[0]*[0] + 0.28*[0]*[0]*x*x -1.37*[0]*[0]*[0]*x)*2.7667/[1]",xmin,xmax);
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
		Graph_syst_Scale_y_errup=(  (2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.7667*0.1301/BR)
		Graph_syst_Scale_y_errdown=((2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.7667*0.137/BR)
		Graph_syst_Scale.SetPoint(i,Graph_syst_Scale_x,Graph_syst_Scale_y)
		Graph_syst_Scale.SetPointError(i,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown)
	#Graph_syst_Scale =  TGraphAsymmErrors(nP,Graph_syst_Scale_x,Graph_syst_Scale_y,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown);
	Graph_syst_Scale.SetLineColor(kRed)
	Graph_syst_Scale.SetFillColor(kRed)
	Graph_syst_Scale.SetFillStyle(3001)
	Graph_syst_Scale.DrawClone("e3");

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
	gAll.SetLineColor(1)
	gAll.SetMarkerColor(1)
	gAll.SetMarkerStyle(20)
	gAll.SetFillStyle(0)
	if benchmark : 
		npoints = 12
		app = "bench"
	else :
		npoints = 52
		app = "lambda"
	for m in range(0,npoints):
		fin = TFile.Open("cards_"+channels[c]+"_"+folder+"/HH"+app+"rew"+str(m)+categories[c]+"/higgsCombineHH"+app+"rew"+str(m)+"_forLim.Asymptotic.mH125.root")
		tree = fin.Get("limit")
		high=0
		low=0
		limit = 0
		offsetX = 20
		errX = 0
		if benchmark : 
			offsetX = 0
			errX = 0.15
		for entry in tree :
			if tree.quantileExpected == 0.5 : 
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
			#g95[c].SetPoint(g95[c].GetN(),m,limit-low95)
			#g95[c].SetPoint(g95[c].GetN(),m,high95-limit)
			#g68[c].SetPoint(g68[c].GetN(),m,limit-low)
			#g68[c].SetPoint(g68[c].GetN(),m,high-limit)
	print "adding gall", gAll.GetN()
	mg[c].Add(gAll)
	mg[c].Add(gtemp[c])

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

cNice = [TCanvas("ETauFinal"),TCanvas("MuTauFinal"),TCanvas("TauTauFinal"),TCanvas("COMBINEDFinal")]
for ic in range(len(channels)):
	cNice[ic].cd()
	#mg[ic].GetListOfGraphs().ls()
	gexp = mg[ic].GetListOfGraphs().FindObject("Combined categories").Clone()
	gexp.SetTitle("Expected CLs")
	gexp.SetMarkerStyle(20)
	gexp.SetMarkerColor(4)
	gexp.SetLineColor(4)
	gexp.SetLineStyle(2)
	gexp.SetFillColor(0)
	g68 = mg[ic].GetListOfGraphs().FindObject("Combined categories").Clone()
	g68.SetTitle("Expected #pm 1#sigma")
	g68.SetMarkerStyle(0)
	g68.SetMarkerColor(3)
	g68.SetFillColor(3)
	g68.SetLineColor(3)
	g68.SetFillStyle(1001)
	g95[ic].SetTitle("Expected #pm 2#sigma")
	g95[ic].SetName(channels[ic])
	g95[ic].SetMarkerStyle(0)
	g95[ic].SetMarkerColor(5)
	g95[ic].SetFillColor(5)
	g95[ic].SetLineColor(5)
	g95[ic].SetFillStyle(1001)
	g95[ic].GetYaxis().SetRangeUser(1,100000)
	if benchmark : 
		g95[ic].GetYaxis().SetRangeUser(1,100000)
		g95[ic].GetXaxis().SetRangeUser(-0.49,12) 
	else : g95[ic].GetXaxis().SetRangeUser(-20,30) #-20,30)
	g95[ic].GetYaxis().SetTitle("95% CL on #sigma#times BR(hh#rightarrow bb#tau#tau) [fb]")
	if benchmark : 
		g95[ic].GetXaxis().SetTitle("Benchmark number") 
		g95[ic].GetXaxis().SetNdivisions(13) 
	else : g95[ic].GetXaxis().SetTitle("k_{#lambda} [GeV]")
	gStyle.SetOptTitle(0)
	if benchmark : 
		g95[ic].Draw("A2")
		g68.Draw("2SAME")
		gexp.Draw("PXSAME")
	else :
		g95[ic].Draw("A3")
		g68.Draw("3SAME")
		gexp.Draw("PLXSAME")

	legend = TLegend()#	cNice[ic].BuildLegend()
	legend.SetLineColor(0)
	legend.SetX1(0.1192529)
	legend.SetY1(0.1292373)
	legend.SetX2(0.4755)
	legend.SetY2(0.2733)
	legend.AddEntry(gexp)
	legend.AddEntry(g68)
	legend.AddEntry(g95[ic])
	legend.Draw()
	cNice[ic].SetLogy()
	cNice[ic].SetGridy(1)
	cNice[ic].SetGridx(1)

	pt = TPaveText(0.1063218,0.7966102,0.3045977,0.8898305,"brNDC")
	pt.SetBorderSize(0)
	pt.SetTextAlign(12)
	pt.SetTextFont(62)
	pt.SetTextSize(0.05)
	pt.SetFillColor(0)
	pt.SetFillStyle(0)
	pt.AddText("CMS" )
	pt.AddText("#font[52]{preliminary}")
	pt2 = TPaveText(0.7,0.9066667,0.8997773,0.957037,"brNDC")
	pt2.SetBorderSize(0)
	pt2.SetFillColor(0)
	pt2.SetTextSize(0.035)
	pt2.SetTextFont(42)
	pt2.SetFillStyle(0)
	pt2.AddText("L = 2.6 fb^{-1} (13 TeV)")

	pt4 = TPaveText(0.3819196,0.7780357,0.9008929,0.8675595,"brNDC")
	pt4.SetTextAlign(12)
	pt4.SetFillColor(0)
	pt4.SetFillStyle(0)
	pt4.SetTextFont(42)
	pt4.SetBorderSize(0)
	pt4.AddText(channelsName[ic]) 
	if ic == 3 : pt4.AddText("Combined channels")
	pt.Draw()
	pt2.Draw()
	pt4.Draw()


	#getExpLine(cNice[ic],-20,30,1)
	xmin=-20
	xmax=30
	yt=1
	BR = 1
	myFunc =  TF1("myFunc","(2.09*[0]*[0]*[0]*[0] + 0.28*[0]*[0]*x*x -1.37*[0]*[0]*[0]*x)*2.7667/[1]",xmin,xmax);
	myFunc.SetParameter(0,yt); 
	myFunc.SetParameter(1,BR); 
	graph = TGraph(myFunc);
	ci = TColor.GetColor("#ff0000");
	cNice[ic].cd();
	graph.SetLineColor(ci);
	graph.SetLineWidth(3);
	if not benchmark : graph.Draw("l");
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
		Graph_syst_Scale_y_errup=(  (2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.7667*0.1301/BR)
		Graph_syst_Scale_y_errdown=((2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.7667*0.137/BR)
		Graph_syst_Scale.SetPoint(i,Graph_syst_Scale_x,Graph_syst_Scale_y)
		Graph_syst_Scale.SetPointError(i,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown)
	#Graph_syst_Scale =  TGraphAsymmErrors(nP,Graph_syst_Scale_x,Graph_syst_Scale_y,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown);
	Graph_syst_Scale.SetLineColor(kRed)
	Graph_syst_Scale.SetFillColor(kRed)
	Graph_syst_Scale.SetFillStyle(3001)
	if not benchmark : 
		Graph_syst_Scale.Draw("e3");
		app2 = "Final"
	else : app2 = "Benchmark"
	


	for ext in outFormats : cNice[ic].SaveAs("plots/preApp_01_jul/limit"+app2+"_"+channels[ic]+outString+ext)



   #return ;


#c2 = TCanvas()
#c2.cd()
#mg[1].Draw("ALP")
#c2.BuildLegend()
#c3 = TCanvas()
#c3.cd()
#mg[2].Draw("ALP")
#c3.BuildLegend()
