#include "AnalysisHelper.h"
#include "TString.h" // just for Form, else use string
#include "TTreeFormulaGroup.h"
#include "TTreeFormula.h"
#include "TMath.h"
#include <iomanip>
#include <boost/variant.hpp>
#include <unordered_map>
#include <sstream>

using namespace std;

#define DEBUG false

AnalysisHelper::AnalysisHelper(string cfgname, string merge_section)
{
  TH1::SetDefaultSumw2(true);

  nominal_name_ = "NOMINAL"; // used for nominal systematics
  merge_section_ = merge_section;

  nsplit_   = 1; // default: only 1 job
  idxsplit_ = 0;

  cout << "@@ Parsing main config : " << cfgname << endl;
  mainCfg_ = unique_ptr<CfgParser>(new CfgParser(cfgname));
  bool success = readMainInfo();
  if (!success) {
    cerr << "** AnalysisHelper : error : some information could not be retrieved from config" << endl;
    std::exit(1);
  }
}

AnalysisHelper::~AnalysisHelper()
{}

bool AnalysisHelper::readMainInfo()
{
  if (!mainCfg_->hasOpt("configs::cutCfg") || !mainCfg_->hasOpt("configs::sampleCfg"))
	{
	  cerr << "** AnalysisHelper : error : configs for sample and cut not provided in main cfg" << endl;
	  return false;
	}
  string sampleCfgName = mainCfg_->readStringOpt("configs::sampleCfg");
  string cutCfgName = mainCfg_->readStringOpt("configs::cutCfg");
  cout << "@@ sample cfg          : " << sampleCfgName << endl;
  cout << "@@ selection cfg       : " << cutCfgName << endl;

  cutCfg_ = unique_ptr<CfgParser>(new CfgParser(cutCfgName));
  sampleCfg_ = unique_ptr<CfgParser>(new CfgParser(sampleCfgName));

  if (!(mainCfg_->hasOpt("general::lumi"))) return false;
  lumi_ = mainCfg_->readFloatOpt("general::lumi");
  cout << "@@ lumi                : " << lumi_ << endl;

  if(merge_section_ == "merge_plots") {
    outputFileName_ = "outPlots.root";
  }
  else if(merge_section_ == "merge_limits") {
    outputFileName_ = "outLimits.root";
  }
  else {
	cout << "wrong option: " << merge_section_ <<  endl;
	exit(1);
  }
  cout << "@@ output file  name   : " << outputFileName_<< endl;

  if (mainCfg_->hasSect(merge_section_))
	{
	  cout << "@@ will merge these samples [" << merge_section_ << "]" << " : " << endl;
	  vector<string> samps_to_merge = mainCfg_->readListOfOpts(merge_section_);
	  for (string s : samps_to_merge)
		{
		  std::cout << "==== " << s << std::endl;
		  sample_merge_list_.append(s, mainCfg_->readStringListOpt(merge_section_+string("::")+s));
		  cout << "   -- " << s << "   <==   ";
		  for (unsigned int idx = 0; idx < sample_merge_list_.at(s).size(); ++idx)
			{
			  cout << sample_merge_list_.at(s).at(idx);
			  if (idx != sample_merge_list_.at(s).size() -1 ) cout << ", ";
			  else cout << endl;
			}
		}
	}
  return true;
}

void AnalysisHelper::saveOutputsToFile(std::string outFolder)
{
  string outFile = outFolder + "/" + outputFileName_;
  system (Form("cp %s %s", (mainCfg_  ->getCfgName()).c_str() , outFolder.c_str()));
  system (Form("cp %s %s", (cutCfg_   ->getCfgName()).c_str() , outFolder.c_str()));
  system (Form("cp %s %s", (sampleCfg_->getCfgName()).c_str() , outFolder.c_str()));

  TFile* fOut = TFile::Open(outFile.c_str(), "recreate");
  vector <ordered_map <std::string, std::shared_ptr<Sample>> *> allToSave;
  allToSave.push_back(&data_samples_);
  allToSave.push_back(&sig_samples_);
  allToSave.push_back(&bkg_samples_);

  // nesting order: type of events --> sample --> selection --> variable --> systematics
  fOut->cd();
  for (uint itype = 0; itype < allToSave.size(); ++itype) {
	for (uint isample = 0; isample < allToSave.at(itype)->size(); ++isample) {

	  Sample::selColl& plotSet = allToSave.at(itype)->at(isample)->plots();
	  for (uint isel = 0; isel < plotSet.size(); ++isel) {
		for (uint ivar = 0; ivar < plotSet.at(isel).size(); ++ivar) {
		  for (uint isyst = 0; isyst < plotSet.at(isel).at(ivar).size(); ++isyst) {
			plotSet.at(isel).at(ivar).at(isyst)->Write();
		  }}}

	  Sample::selColl2D& plotSet2D = allToSave.at(itype)->at(isample)->plots2D();
	  for (uint isel = 0; isel < plotSet2D.size(); ++isel) {
		for (uint ivar = 0; ivar < plotSet2D.at(isel).size(); ++ivar) {
		  for (uint isyst = 0; isyst < plotSet2D.at(isel).at(ivar).size(); ++isyst) {
			plotSet2D.at(isel).at(ivar).at(isyst)->Write();
		  }}}

	}
  }

  cout << "@@ ... saving completed, closing output file" << endl;
  fOut->Close();
  return;
}

void AnalysisHelper::readSamples()
{
  vector<string> dataSampleNameList = mainCfg_->readStringListOpt("general::data");
  vector<string> sigSampleNameList  = mainCfg_->readStringListOpt("general::signals");
  vector<string> bkgSampleNameList  = mainCfg_->readStringListOpt("general::backgrounds");
  
  cout << "@@ Samples : reading samples DATA : " << endl;
  for (string name : dataSampleNameList)
	{
	  shared_ptr<Sample> smp = openSample(name);
	  smp->setType(Sample::kData);
	  smp->clearWeights(); // no weights should be applied on data -- remove manually all weights read
	  smp->clearExtWeights(); // no weights should be applied on data -- remove manually all weights read
	  data_samples_.append(name, smp);
	}
  cout << endl;

  cout << "@@ Samples : reading samples sig  : " << endl;
  for (string name : sigSampleNameList)
	{
	  shared_ptr<Sample> smp = openSample(name);
	  smp->setType(Sample::kSig);
	  sig_samples_.append(name, smp);
	  // cout << " " << name;
	}
  cout << endl;

  cout << "@@ Samples : reading samples bkg  : " << endl;
  for (string name : bkgSampleNameList)
	{
	  shared_ptr<Sample> smp = openSample(name);
	  smp->setType(Sample::kBkg);
	  bkg_samples_.append(name, smp);

	  if (DEBUG) {
		cout << " ..........DEBUG: read bkg sample: " << smp->getName() <<
		  " (" << name << ")" << " nweights: " << smp->getWeights().size() << endl;
		for (uint iww = 0; iww < smp->getWeights().size(); ++iww) {
		  cout << " ..........DEBUG:    >> wname: " << smp->getWeights().at(iww).getName() <<
			" nsyst: " << smp->getWeights().at(iww).getNSysts() << endl;
		}
	  }
	}
  cout << endl;

  // printSamples(true);

  return;
}

shared_ptr<Sample> AnalysisHelper::openSample(string sampleName)
{
  if (DEBUG) {
	cout << " ..........DEBUG: entering AnalysisHelper::openSample for sample " << sampleName << endl;
  }
  
  string filename = sampleCfg_->readStringOpt(Form("samples::%s",sampleName.c_str()));
  string sampleCfgName = mainCfg_->readStringOpt("configs::sampleCfg");
  string list_pattern = "goodfiles";
  if (mainCfg_->hasOpt("configs::pattern")){
    list_pattern = mainCfg_->readStringOpt("configs::pattern");
  }
  shared_ptr<Sample> sample (new Sample(sampleName, filename + string("/") + list_pattern + string(".txt")));
  if (sampleCfg_->hasOpt(Form("userEffBin::%s",sampleName.c_str())))
	{
	  int ubin = sampleCfg_->readIntOpt(Form("userEffBin::%s",sampleName.c_str()));
	  sample->setEffBin(ubin);
	}
  bool success = sample->openFileAndTree();
  if (!success)
	{
	  throw std::runtime_error("cannot open input file for sample " + sampleName);
	}

  // for the moment stored in selection cfg -- could be stored in sample cfg instead
  // but I prefer to have all the weights at the same place

  if (!cutCfg_->hasOpt(Form("sampleWeights::%s", sampleName.c_str())))
    return sample;

  if (DEBUG) cout << " ..........DEBUG: " << sampleName << " has weights associated, will be listed" << endl;
  vector<string> weights = cutCfg_->readStringListOpt(Form("sampleWeights::%s", sampleName.c_str()));
  for (string wname : weights)
	{
	  // cout << " +++ adding " << wname << endl;
	  if (DEBUG) cout << " ..........DEBUG: -- " << wname << endl;
	  Weight w (wname);
	  vector<pair<string, string> > wsyst = readWeightSysts(wname, "systematics");
	  if (DEBUG){
		cout << " ..........DEBUG:    > nsyst: " << wsyst.size() << endl;
		for (auto pp : wsyst) cout << "................>> DEBUG: " << pp.first << " " << pp.second << endl;
	  }
	  w.addSysts(wsyst); // can be empty as well
	  sample->addWeight(w);
	}

  return sample;
}

void AnalysisHelper::readSelections()
{
  if (DEBUG) cout << " ..........DEBUG: entering AnalysisHelper::readSelections" << endl;

  vector<string> selListNames = mainCfg_->readStringListOpt("general::selections");
  vector<Selection> selList;
  cout << "@@ Selections : reading selections : ";
  for (string sel : selListNames)
	{
	  cout << " " << sel;
	  selList.push_back(readSingleSelection(sel));
	}
  cout << endl;

  vector<string> CRListNames = mainCfg_->readStringListOpt("general::regions");
  vector<Selection> CRList;
  cout << "@@ Selections : reading regions    : ";
  for (string sel : CRListNames)
	{
	  cout << " " << sel;
	  CRList.push_back(readSingleSelection(sel));
	}
  cout << endl;

  // combine
  for (Selection& sel : selList)
	{
	  for (Selection& CR : CRList)
		{
		  Selection s (sel.getName() + "_" + CR.getName(), "");
		  s.extend(sel);
		  s.extend(CR);
		  selections_.push_back(s);
		}
	}

  if (DEBUG)
	{
	  cout << " ..........DEBUG: printing selections" << endl;
	  printSelections(true);
	}
  return;
}

void AnalysisHelper::readVariables()
{
  if (mainCfg_->hasOpt("general::variables"))
	{
	  variables_ = mainCfg_->readStringListOpt("general::variables");
	}

  if (mainCfg_->hasOpt("general::variables2D"))
	{
	  std::vector<string> variables2DPacked = mainCfg_->readStringListOpt("general::variables2D");
	  for (string spack : variables2DPacked)
		variables2D_.push_back(unpack2DName(spack));
	}

  cout << "@@ Variables : reading variables : ";
  for (string var : variables_)
    cout << " " << var;
  cout << endl;

  cout << "@@ Variables : reading 2D variables : ";
  for (auto var : variables2D_)
    cout << " " << var.first << ":" << var.second;
  cout << endl;

  return;
}


void AnalysisHelper::prepareSamplesHistos()
{
  // to loop all in once
  vector <sampleColl*> allToInit;
  allToInit.push_back(&data_samples_);
  allToInit.push_back(&sig_samples_);
  allToInit.push_back(&bkg_samples_);

  vector<int> doselW;
  doselW.push_back(0); // no sel W for data!
  doselW.push_back(1);
  doselW.push_back(1);

  for (uint ismpc = 0; ismpc < allToInit.size(); ++ismpc) // loop on (data, sig, bkg)
	{
	  sampleColl* samcoll = allToInit.at(ismpc);
	  for (uint isample = 0; isample < samcoll->size(); ++isample) // loop on samples
		{
		  Sample::selColl& selcoll = samcoll->at(isample)->plots();
		  for (uint isel = 0; isel < selections_.size(); ++isel)
			{
			  selcoll.append(selections_.at(isel).getName(), Sample::varColl());
			  Sample::varColl& varcoll = selcoll.back();
			  for (uint ivar = 0; ivar < variables_.size(); ++ivar)
				{
				  string varName = variables_.at(ivar);
				  string selName = selections_.at(isel).getName();
				  string sel_var = selName + ":" + varName;

				  varcoll.append(varName, Sample::systColl());
				  Sample::systColl& systcoll = varcoll.back();

				  bool   hasUserBinning = cutCfg_->hasOpt(Form("binning::%s", varName.c_str()));
				  bool   hasUserBinning_thisSel = cutCfg_->hasOpt(Form("binning::%s", sel_var.c_str()));
				  int    nbins = -1;
				  float  xlow = -1.;
				  float  xup = -1.;
				  float* binning = 0;

				  if (hasUserBinning || hasUserBinning_thisSel )
					{
					  vector<float> vBins;
					  if (hasUserBinning_thisSel)
						vBins = cutCfg_->readFloatListOpt(Form("binning::%s", sel_var.c_str()));
					  else if (hasUserBinning)
						vBins = cutCfg_->readFloatListOpt(Form("binning::%s", varName.c_str()));

					  if (DEBUG)
						{
						  cout << " ..........DEBUG: printing special binnings for selections:" << endl;

						  cout << " ---> Selection: " << sel_var << endl;
						  cout << "      Binning: ";
						  for (uint i=0; i<vBins.size(); i++) cout << " " << vBins.at(i);
						  cout << endl;
						}

					  nbins = vBins.size() -1;

					  if (nbins < 1) // wrong
						{
						  cerr << "** AnalysisHelper : prepareSamplesHistos : error : binning of " << varName << " must have at least 2 numbers, dummy one used" << endl;
						  vBins.clear();
						  vBins.push_back(0.);
						  vBins.push_back(1.);
						  nbins = 1;
						}

					  binning = new float[nbins+1] ;
					  for (uint ibin = 0; ibin < vBins.size(); ++ibin) binning[ibin] = vBins.at(ibin);
					}
				  else // no user binning
					{
					  if (!cutCfg_->hasOpt(Form("histos::%s", varName.c_str())))
						{
						  cerr << "** AnalysisHelper : prepareSamplesHistos : error : did not find binning for var " << varName << " , dummy one used" << endl;
						  nbins = 1;
						  xlow = 0;
						  xup = 0;
						}
					  else
						{
						  vector<float> vBins  = cutCfg_->readFloatListOpt(Form("histos::%s", varName.c_str()));
						  nbins = (int) (vBins.at(0) + 0.5); // just to avoid numerical errors
						  xlow = vBins.at(1);
						  xup  = vBins.at(2);
						}
					}

				  // prepare histos -- first one is always the nominal one
				  const Sample& currSample = *(samcoll->at(isample));
				  const Selection& currSel = selections_.at(isel);
				  string sampleName = currSample.getName();

				  string hname = formHistoName (sampleName, selName, varName, nominal_name_);
				  std::shared_ptr<TH1F> hist;
				  if (hasUserBinning) hist = make_shared<TH1F> (hname.c_str(), (hname+string(";")+varName+string(";events")).c_str(), nbins, binning);
				  else                hist = make_shared<TH1F> (hname.c_str(), (hname+string(";")+varName+string(";events")).c_str(), nbins, xlow, xup);
				  systcoll.append(nominal_name_, hist);

				  // now loop over available syst and create more histos
				  // skip the up/down variables and only add the systematics to the nominal variables
				  std::vector<string> shiftVarNames {"tauup", "taudown", "eleup", "eledown", "muup", "mudown", "jetup", "jetdown"};
				  bool isVarShifted = false;
				  for (uint idx=0; idx<shiftVarNames.size(); idx++)
					{
					  if (varName.find(shiftVarNames.at(idx)) != string::npos)
						{
						  isVarShifted = true;
						  break;
						}
					}
				  if (DEBUG) std::cout << "************* varName: " << varName << " --> isVarShifted: " << isVarShifted << std::endl;
				  if (doselW.at(ismpc) == 1 && !isVarShifted)
					{
					  // sample
					  for (uint iw = 0; iw < currSample.getWeights().size(); ++iw)
						{
						  const Weight& currW = currSample.getWeights().at(iw);
						  for (int isys = 0; isys < currW.getNSysts(); ++isys)
							{
							  hname = formHistoName (sampleName, selName, varName, currW.getSystName(isys));
							  std::shared_ptr<TH1F> histS;
							  if (hasUserBinning) histS = make_shared<TH1F> (hname.c_str(), (hname+string(";")+varName+string(";events")).c_str(), nbins, binning);
							  else                histS = make_shared<TH1F> (hname.c_str(), (hname+string(";")+varName+string(";events")).c_str(), nbins, xlow, xup);
							  systcoll.append(currW.getSystName(isys), histS);
							}
						}

					  // selection
					  for (uint iw = 0; iw < currSel.getWeights().size(); ++iw)
						{
						  const Weight& currW = currSel.getWeights().at(iw);
						  for (int isys = 0; isys < currW.getNSysts(); ++isys)
							{
							  hname = formHistoName (sampleName, selName, varName, currW.getSystName(isys));
							  std::shared_ptr<TH1F> histS;
							  if (hasUserBinning) histS = make_shared<TH1F> (hname.c_str(), (hname+string(";")+varName+string(";events")).c_str(), nbins, binning);
							  else                histS = make_shared<TH1F> (hname.c_str(), (hname+string(";")+varName+string(";events")).c_str(), nbins, xlow, xup);
							  systcoll.append(currW.getSystName(isys), histS);
							}
						}
					}

				  if (hasUserBinning || hasUserBinning_thisSel) delete[] binning ; // was allocated with new

				  // set Sumw2() and other stuff for all the histos
				  for (uint ih = 0; ih < systcoll.size(); ++ih)
					{
					  if (doselW.at(ismpc) != 1) // is data
						systcoll.at(ih)->Sumw2();
					  else
						systcoll.at(ih)->SetBinErrorOption(TH1::kPoisson);
					}

				} // end loop on 1D variables
			} // end loop on selections
		} // end loop on samples
	} // end loop on (data, sig, bkg)
}


void AnalysisHelper::prepareSamples2DHistos()
{
  // to loop all in once
  vector <sampleColl*> allToInit;
  allToInit.push_back(&data_samples_);
  allToInit.push_back(&sig_samples_);
  allToInit.push_back(&bkg_samples_);

  vector<int> doselW;
  doselW.push_back(0); // no sel W for data!
  doselW.push_back(1);
  doselW.push_back(1);

  for (uint ismpc = 0; ismpc < allToInit.size(); ++ismpc) // loop on (data, sig, bkg)
	{
	  sampleColl* samcoll = allToInit.at(ismpc);
	  for (uint isample = 0; isample < samcoll->size(); ++isample) // loop on samples
		{
		  Sample::selColl2D& selcoll = samcoll->at(isample)->plots2D();
		  for (uint isel = 0; isel < selections_.size(); ++isel)
			{
			  selcoll.append(selections_.at(isel).getName(), Sample::varColl2D());
			  Sample::varColl2D& varcoll = selcoll.back();
			  for (uint ivar = 0; ivar < variables2D_.size(); ++ivar)
				{
				  auto pairName = variables2D_.at(ivar);
				  string varName1 = pairName.first;
				  string varName2 = pairName.second;
				  string packedVarName = pack2DName(varName1, varName2);

				  varcoll.append(packedVarName, Sample::systColl2D());
				  Sample::systColl2D& systcoll = varcoll.back();

				  bool hasUserBinning1 = cutCfg_->hasOpt(Form("binning2D::%s@%s", packedVarName.c_str(), varName1.c_str()));
				  bool hasUserBinning2 = cutCfg_->hasOpt(Form("binning2D::%s@%s", packedVarName.c_str(), varName2.c_str()));

				  vector<float> binning;
				  if (!hasUserBinning1 || !hasUserBinning2) // at least one must have been specified
					binning = cutCfg_->readFloatListOpt(Form("histos2D::%s", packedVarName.c_str()));

				  int    nbins1   = -1;
				  float  xlow1    = -1.;
				  float  xup1     = -1.;
				  double* binning1 = 0;

				  int    nbins2   = -1;
				  float  xlow2    = -1.;
				  float  xup2     = -1.;
				  double* binning2 = 0;

				  if (hasUserBinning1)
					{
					  vector<float> vBins = cutCfg_->readFloatListOpt(Form("binning2D::%s@%s", packedVarName.c_str(), varName1.c_str()));
					  nbins1 = vBins.size() -1;

					  if (nbins1 < 1) // wrong
						{
						  cerr << "** AnalysisHelper : prepareSamples2DHistos : error : binning of " << packedVarName << "@" << varName1 << " must have at least 2 numbers, dummy one used" << endl;
						  vBins.clear();
						  vBins.push_back(0.);
						  vBins.push_back(1.);
						  nbins1 = 1;
						}

					  binning1 = new double[nbins1+1] ;
					  for (uint ibin = 0; ibin < vBins.size(); ++ibin) binning1[ibin] = vBins.at(ibin);
					}
				  else
					{
					  nbins1 =  (int) (binning.at(0) + 0.5); // just to avoid numerical errors
					  xlow1  =  binning.at(1);
					  xup1   =  binning.at(2);
					}

				  if (hasUserBinning2)
					{
					  vector<float> vBins = cutCfg_->readFloatListOpt(Form("binning2D::%s@%s", packedVarName.c_str(), varName2.c_str()));
					  nbins2 = vBins.size() -1;

					  if (nbins2 < 1) // wrong
						{
						  cerr << "** AnalysisHelper : prepareSamples2DHistos : error : binning of " << packedVarName << "@" << varName2 << " must have at least 2 numbers, dummy one used" << endl;
						  vBins.clear();
						  vBins.push_back(0.);
						  vBins.push_back(1.);
						  nbins2 = 1;
						}

					  binning2 = new double[nbins2+1] ;
					  for (uint ibin = 0; ibin < vBins.size(); ++ibin) binning2[ibin] = vBins.at(ibin);
					}
				  else
					{
					  nbins2 =  (int) (binning.at(3) + 0.5); // just to avoid numerical errors
					  xlow2  =  binning.at(4);
					  xup2   =  binning.at(5);
					}

				  // prepare histos -- first one is always the nominal one
				  const Sample& currSample = *(samcoll->at(isample));
				  const Selection& currSel = selections_.at(isel);
				  string sampleName = currSample.getName();
				  string selName    = currSel.getName();

				  string hname = formHisto2DName (sampleName, selName, varName1, varName2, nominal_name_);
				  std::shared_ptr<TH2F> hist;
				  if (hasUserBinning1 && hasUserBinning2)        hist = make_shared<TH2F> (hname.c_str(), (hname+string(";")+varName1+string(";")+varName2).c_str(), nbins1, binning1, nbins2, binning2);
				  else if (hasUserBinning1 && !hasUserBinning2)  hist = make_shared<TH2F> (hname.c_str(), (hname+string(";")+varName1+string(";")+varName2).c_str(), nbins1, binning1, nbins2, xlow2, xup2);
				  else if (!hasUserBinning1 && hasUserBinning2)  hist = make_shared<TH2F> (hname.c_str(), (hname+string(";")+varName1+string(";")+varName2).c_str(), nbins1, xlow1, xup1, nbins2, binning2);
				  else                                           hist = make_shared<TH2F> (hname.c_str(), (hname+string(";")+varName1+string(";")+varName2).c_str(), nbins1, xlow1, xup1, nbins2, xlow2, xup2);
				  systcoll.append(nominal_name_, hist);

				  /*
				  // now loop over available syst and create more histos
				  if (doselW.at(ismpc) == 1)
				  {
				  // sample
				  for (uint iw = 0; iw < currSample.getWeights().size(); ++iw)
				  {
				  const Weight& currW = currSample.getWeights().at(iw);
				  for (int isys = 0; isys < currW.getNSysts(); ++isys)
				  {
				  hname = formHistoName (sampleName, selName, varName, currW.getSystName(isys));
				  std::shared_ptr<TH1F> histS;
				  if (hasUserBinning) histS = make_shared<TH1F> (hname.c_str(), hname.c_str(), nbins, binning);
				  else                histS = make_shared<TH1F> (hname.c_str(), hname.c_str(), nbins, xlow, xup);
				  systcoll.append(nominal_name_, histS);
				  }
				  }

				  // selection
				  for (uint iw = 0; iw < currSel.getWeights().size(); ++iw)
				  {
				  const Weight& currW = currSel.getWeights().at(iw);
				  for (int isys = 0; isys < currW.getNSysts(); ++isys)
				  {
				  hname = formHistoName (sampleName, selName, varName, currW.getSystName(isys));
				  std::shared_ptr<TH1F> histS;
				  if (hasUserBinning) histS = make_shared<TH1F> (hname.c_str(), hname.c_str(), nbins, binning);
				  else                histS = make_shared<TH1F> (hname.c_str(), hname.c_str(), nbins, xlow, xup);
				  systcoll.append(currW.getSystName(isys), histS);
				  }
				  }
				  }
				  */
				  if (hasUserBinning1) delete[] binning1 ; // was allocated with new
				  if (hasUserBinning2) delete[] binning2 ; // was allocated with new

				  // set Sumw2() and other stuff for all the histos
				  for (uint ih = 0; ih < systcoll.size(); ++ih)
					{
					  if (doselW.at(ismpc) != 1) // is data
						systcoll.at(ih)->Sumw2();
					  else
						systcoll.at(ih)->SetBinErrorOption(TH1::kPoisson);
					}

				} // end loop on 1D variables
			} // end loop on selections
		} // end loop on samples
	} // end loop on (data, sig, bkg)
}


vector<pair<string, string> > AnalysisHelper::readWeightSysts(std::string name, std::string section)
{
  vector<pair<string, string>> systs;
  if (!cutCfg_->hasOpt(Form("%s::%s", section.c_str(), name.c_str())))
	{
	  return systs;
	}

  vector<string> v = cutCfg_->readStringListOpt(Form("%s::%s", section.c_str(), name.c_str()));
  for (string elem : v)
	{
	  std::string delimiter = ":";
	  size_t pos = 0;
	  vector<std::string> tokens;
	  while ((pos = elem.find(delimiter)) != std::string::npos)
		{
		  tokens.push_back(elem.substr(0, pos));
		  elem.erase(0, pos + delimiter.length());
		}
	  tokens.push_back(elem); // last part splitted
	  if (tokens.size() != 2)
		{
		  cerr << "** AnalysisHelper : readWeightSyst : error : could not parse entry " << elem << " of " << section << "::" << name << " , skipping..." << endl;
		  continue;
		}

	  systs.push_back(make_pair(tokens.at(0), tokens.at(1)));
	}
  return systs;

}

Selection AnalysisHelper::readSingleSelection (std::string name)
{
  if (!cutCfg_->hasOpt(Form("selections::%s",name.c_str())))
	{
	  cerr << "** AnalysisHelper : readSingleSelection : error : could not find selection " << name << endl;
	  return Selection("dummy", "1==0");
	}

  Selection s(name, "");
  vector<string> selDef = cutCfg_->readStringListOpt(Form("selections::%s", name.c_str())) ;
  for (string part : selDef)
	{
	  if (cutCfg_->hasOpt(Form("selections::%s", part.c_str())))
		s.extend(readSingleSelection(part));
	  else
		s.extend(part.c_str()); // was a block of selection in TCut form
	}

  // now fetch weights if any and update
  if (!cutCfg_->hasOpt(Form("selectionWeights::%s", name.c_str())))
    return s;

  vector<string> weights = cutCfg_->readStringListOpt(Form("selectionWeights::%s", name.c_str()));
  for (string wname : weights)
	{
	  // cout << " +++ adding " << wname << endl;
	  Weight w (wname);
	  vector<pair<string, string> > wsyst = readWeightSysts(wname, "systematics");
	  w.addSysts(wsyst); // can be empty as well
	  s.addWeight(w);
	}


  if (!cutCfg_->hasOpt(Form("selectionWeights_ext::%s", name.c_str())))
    return s;

  vector<pair<string, float> > weights_ext = cutCfg_->readStringFloatListOpt(Form("selectionWeights_ext::%s", name.c_str()));
  for (auto wfloat : weights_ext)
	{
	  //cout << " +++ adding " << wfloat.first <<" ... "<< wfloat.second<<endl;
	  Weight_ext w_ext (wfloat.first, wfloat.second);
	  //vector<pair<string, string> > wsyst = readWeightSysts(wname, "systematics");
	  //w.addSysts(wsyst); // can be empty as well
	  s.addWeightExt(w_ext);
	}


  return s;
}

void AnalysisHelper::printSelections(bool printWeights, bool printSysts)
{
  cout << "@@ Selection list ------- " << endl;
  for (unsigned int i = 0; i < selections_.size(); ++i)
	{
	  cout << " >> " << selections_.at(i).getName() << " " << selections_.at(i).getValue().GetTitle() << endl;
	  if (printWeights)
		{
		  cout << "     ~~~> ";
		  for (unsigned int iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
			{
			  cout << selections_.at(i).getWeights().at(iw).getName() << " ";
			}
		  cout << endl;
		  for (unsigned int iw = 0; iw < selections_.at(i).getWeightsExt().size(); ++iw)
			{
			  cout << selections_.at(i).getWeightsExt().at(iw).getFormula() << ":" << selections_.at(i).getWeightsExt().at(iw).getValue()<< " ";
			}
		  cout << endl;


		}

	  if (printSysts)
		{
		  // syst of weights
		  for (unsigned int iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
			{
			  cout << selections_.at(i).getWeights().at(iw).getName() << " ::: ";
			  for (int isys = 0; isys < selections_.at(i).getWeights().at(iw).getNSysts(); ++isys)
				cout << selections_.at(i).getWeights().at(iw).getSystName(isys) <<"="<<selections_.at(i).getWeights().at(iw).getSyst(isys) << " , ";
			  cout << endl;
			}
		}
	}
  cout << "   ---------------------- " << endl;
}

void AnalysisHelper::printSamples(bool printWeights, bool printSysts)
{
  cout << "@@ Samples list ------- " << endl;
  cout << "   Data:" << endl;
  for (unsigned int i = 0; i < data_samples_.size(); ++i)
	{
	  cout << "   >> " << data_samples_.at(i)->getName() << endl;
	  if (printWeights)
		{
		  cout << "       ~~~> ";
		  for (unsigned int iw = 0; iw < data_samples_.at(i)->getWeights().size(); ++iw)
			{
			  cout << data_samples_.at(i)->getWeights().at(iw).getName() << " ";
			}
		  cout << endl;
		}

	  if (printSysts)
		{
		  // syst of weights
		  for (unsigned int iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
			{
			  cout << selections_.at(i).getWeights().at(iw).getName() << " ::: ";
			  for (int isys = 0; isys < selections_.at(i).getWeights().at(iw).getNSysts(); ++isys)
				cout << selections_.at(i).getWeights().at(iw).getSystName(isys) <<"="<<selections_.at(i).getWeights().at(iw).getSyst(isys) << " , ";
			  cout << endl;
			}
		}
	}

  cout << "   Signal:" << endl;
  for (unsigned int i = 0; i < sig_samples_.size(); ++i)
	{
	  cout << "   >> " << sig_samples_.at(i)->getName() << endl;
	  if (printWeights)
		{
		  cout << "       ~~~> ";
		  for (unsigned int iw = 0; iw < sig_samples_.at(i)->getWeights().size(); ++iw)
			{
			  cout << sig_samples_.at(i)->getWeights().at(iw).getName() << " ";
			}
		  cout << endl;
		}

	  if (printSysts)
		{
		  // syst of weights
		  for (unsigned int iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
			{
			  cout << selections_.at(i).getWeights().at(iw).getName() << " ::: ";
			  for (int isys = 0; isys < selections_.at(i).getWeights().at(iw).getNSysts(); ++isys)
				cout << selections_.at(i).getWeights().at(iw).getSystName(isys) <<"="<<selections_.at(i).getWeights().at(iw).getSyst(isys) << " , ";
			  cout << endl;
			}
		}
	}

  cout << "   Background:" << endl;
  for (unsigned int i = 0; i < bkg_samples_.size(); ++i)
	{
	  cout << "   >> " << bkg_samples_.at(i)->getName() << endl;
	  if (printWeights)
		{
		  cout << "       ~~~> ";
		  for (unsigned int iw = 0; iw < bkg_samples_.at(i)->getWeights().size(); ++iw)
			{
			  cout << bkg_samples_.at(i)->getWeights().at(iw).getName() << " ";
			}
		  cout << endl;
		}

	  if (printSysts)
		{
		  // syst of weights
		  for (unsigned int iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
			{
			  cout << selections_.at(i).getWeights().at(iw).getName() << " ::: ";
			  for (int isys = 0; isys < selections_.at(i).getWeights().at(iw).getNSysts(); ++isys)
				cout << selections_.at(i).getWeights().at(iw).getSystName(isys) <<"="<<selections_.at(i).getWeights().at(iw).getSyst(isys) << " , ";
			  cout << endl;
			}
		}
	}

  cout << "   ---------------------- " << endl;
}

string AnalysisHelper::formHistoName (string sample, string sel, string var, string syst)
{
  string name = "";
  name += sample;
  name += "_";
  name += sel;
  name += "_";
  name += var;
  if (syst != nominal_name_)
	{
	  name += "_";
	  name += syst;
	}
  return name;
}

string AnalysisHelper::formHisto2DName (string sample, string sel, string var1, string var2, string syst)
{
  string name = "";
  name += sample;
  name += "_";
  name += sel;
  name += "_";
  name += var1;
  name += "_";
  name += var2;
  if (syst != nominal_name_)
	{
	  name += "_";
	  name += syst;
	}
  return name;
}
void AnalysisHelper::fillHistosSample(Sample& sample)
{
  cout << "@@ Filling histograms of sample " << sample.getName() << endl;

  activateBranches(sample);

  TChain* tree = sample.getTree();

  // setup selection group
  shared_ptr<TTreeFormulaGroup> fg = make_shared<TTreeFormulaGroup>(true);
  vector<TTreeFormula*> vTTF;
  for (unsigned int isel = 0; isel < selections_.size(); ++isel)
	{
	  // note: no need to call later delete, because fg is set as the owner of the member TTF
	  // cout << selections_.at(isel).getValue().GetTitle() << endl;
	  TTreeFormula* TTF = new TTreeFormula (Form("TTF_%i", isel), selections_.at(isel).getValue().GetTitle(), tree);
	  vTTF.push_back(TTF);
	  fg->SetNotify(TTF);
	}
  tree->SetNotify(fg.get());

  // prepare container for the variables and weights
  // data structure used: a unordered_map (fast lookup) with
  // map[name][variant]
  // the value is a variant type that can contain float, double, int, bools...
  // the name is the weight/variable name (same as used in setBranchAddress)
  // in the map for external weights, map[TTreeFormula][variant]
  //
  // NOTE: boost::variant also available in C++17, but no adequate compiler in CMSSW_7_4_7
  // could be in principle changed to std::variant if a newer release is used
  //
  // A second structure systMap is used to remap the systematics names in the histos to the
  // nominal and shifted weight names. Stored as systMap[systName as in histo] = <nominal_name in tree, syst_name in tree>
  // when histos systs are applied, retrieve the two names using the key that is stored in the histogram name

  typedef boost::variant<bool, int, float, double> varType;
  unordered_map<string, varType> valuesMap;
  unordered_map<string, varType >  extValuesMap;
  unordered_map<string, pair<string, string> > systMap;


  if (DEBUG) cout << " ..........DEBUG: AnalysisHelper : fillHistosSample : going to setup map for SetBranchAddress --- VARS" << endl;
  // loop over all variables and weights to initialize the map
  for (unsigned int ivar = 0; ivar < variables_.size(); ++ivar)
	{
	  if (valuesMap.find(variables_.at(ivar)) == valuesMap.end())
		{
		  valuesMap[variables_.at(ivar)] = float(0); // after will change to the proper type
		  if (DEBUG) cout << " .......... >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " , adding var " << variables_.at(ivar) << endl;
		}
	  else
		cout << "** Warning: AnalysisHelper::fillHistosSample : sample : " << sample.getName() << " , variable " << variables_.at(ivar) << " was already added to valuesMap, duplicated?" << endl;
	}

  if (DEBUG) cout << " ..........DEBUG: AnalysisHelper : fillHistosSample : going to setup map for SetBranchAddress --- VARS 2D" << endl;
  for (unsigned int ivar = 0; ivar < variables2D_.size(); ++ivar)
	{
	  string var1 = variables2D_.at(ivar).first;
	  string var2 = variables2D_.at(ivar).second;
	  if (valuesMap.find(var1) == valuesMap.end())
		{
		  valuesMap[var1] = float(0); // after will change to the proper type
		  if (DEBUG) cout << " .......... >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " , adding var 2D " << var1 << endl;
		}
	  if (valuesMap.find(var2) == valuesMap.end())
		{
		  valuesMap[var2] = float(0); // after will change to the proper type
		  if (DEBUG) cout << " .......... >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " , adding var 2D " << var2 << endl;
		}
	}


  // loop over all variables and external weights to initialize the map
  for (unsigned int ivar = 0; ivar < variables_.size(); ++ivar)
	{
	  if (extValuesMap.find(variables_.at(ivar)) == extValuesMap.end())
		{
		  extValuesMap[variables_.at(ivar)] = float(0); // after will change to the proper type
		  if (DEBUG) cout << " .......... >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " , adding var " << variables_.at(ivar) << "to extValuesMap"<<endl;
		}
	  else
		cout << "** Warning: AnalysisHelper::fillHistosSample : sample : " << sample.getName() << " , variable " << variables_.at(ivar) << " was already added to extValuesMap, duplicated?" << endl;
	}


  if (DEBUG) cout << " ..........DEBUG: AnalysisHelper : fillHistosSample : going to setup map for SetBranchAddress --- SAMPLE" << endl;
  // weoghts - sample
  for (uint iw = 0; iw < sample.getWeights().size(); ++iw)
	{
	  const Weight& currW = sample.getWeights().at(iw);
	  string wname = currW.getName();
	  if (valuesMap.find(wname) == valuesMap.end())
		{
		  if (DEBUG) cout << " .......... >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " , adding sample weight " << wname << endl;
		  valuesMap[wname] = float(0);
		  for (int isys = 0; isys < currW.getNSysts(); ++isys)
			{
			  string sysName = currW.getSyst(isys);
			  if (valuesMap.find(sysName) == valuesMap.end())
				{
				  if (DEBUG) cout << " .......... >> >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " , adding syst weight " << sysName << endl;
				  if (DEBUG) cout << " .......... >> >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " , adding syst map    " << currW.getSystName(isys) << " = [" << wname << " , " << sysName << "]" << endl;
				  valuesMap[sysName] = float(0);
				  systMap[currW.getSystName(isys)] = make_pair(wname, sysName);
				}
			  else
				cout << "** Warning: AnalysisHelper::fillHistosSample : sample : " << sample.getName() << " , syst " << sysName << " from weight " << wname << " was already added to valuesMap, duplicated?" << endl;
			}
		}
	  else
		cout << "** Warning: AnalysisHelper::fillHistosSample : sample : " << sample.getName() << " , weight " << wname << " was already added to valuesMap, duplicated?" << endl;
	}

  if (DEBUG) cout << " ..........DEBUG: AnalysisHelper : fillHistosSample : going to setup map for SetBranchAddress --- SELECTIONS" << endl;
  // selection -- probably not the most efficient as many w are shared so will be chacked many times
  // but this function is called only a few time

  for (uint isel = 0; isel < selections_.size(); ++isel)
	{
	  const Selection& currSel = selections_.at(isel);
	  for (uint iw = 0; iw < currSel.getWeights().size(); ++iw)
		{
		  const Weight& currW = currSel.getWeights().at(iw);
		  string wname = currW.getName();
		  if (valuesMap.find(wname) == valuesMap.end())
			{
			  if (DEBUG) cout << " .......... >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " : sel : " << currSel.getName() << " , adding selection weight " << wname << endl;
			  valuesMap[wname] = float(0);
			  for (int isys = 0; isys < currW.getNSysts(); ++isys)
				{
				  string sysName = currW.getSyst(isys);
				  if (valuesMap.find(sysName) == valuesMap.end())
					{
					  if (DEBUG) cout << " .......... >> >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " : sel : " << currSel.getName() << " , adding selection syst weight " << sysName << endl;
					  if (DEBUG) cout << " .......... >> >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " : sel : " << currSel.getName() << " , adding selection syst map    " << currW.getSystName(isys) << " = [" << wname << " , " << sysName << "]" << endl;
					  valuesMap[sysName] = float(0);
					  systMap[currW.getSystName(isys)] = make_pair(wname, sysName);
					}
				}
			}
		}
	  //external weights

	  for (uint iw = 0; iw < currSel.getWeightsExt().size(); ++iw)
		{
		  const Weight_ext& currW = currSel.getWeightsExt().at(iw);
		  // setup selection group
		  if (extValuesMap.find(currW.getFormula()) == extValuesMap.end())
			{
			  if (DEBUG) cout << " .......... >> DEBUG: AnalysisHelper : fillHistosSample : sample : " << sample.getName() << " : sel : " << currSel.getName() << " , adding selection ext weight " << currW.getFormula() << endl;

			  extValuesMap[currW.getFormula()] = currW.getValue() ;

			}
		}
	}
  //tree->SetNotify(TTF_extWeight_g.get());
  if (DEBUG) cout << " ..........DEBUG: AnalysisHelper : fillHistosSample : valueMap created, going to assess branch types... " << endl;

  // decide types
  TObjArray *branchList = tree->GetListOfBranches();
  for (auto it = valuesMap.begin(); it != valuesMap.end(); ++it)
	{
	  TBranch* br = (TBranch*) branchList->FindObject(it->first.c_str());
	  if (!br)
		cerr << endl << "** ERROR: AnalysisHelper::fillHistosSample : sample : " << sample.getName() << " does not have branch " << it->first << ", expect a crash..." << endl;

	  string brName = br->GetTitle();
	  if (brName.find(string("/F")) != string::npos) // F : a 32 bit floating point (Float_t)
		{
		  it->second = float(0.0);
		  tree->SetBranchAddress(it->first.c_str(), &boost::get<float>(it->second));
		}
	  else if (brName.find(string("/I")) != string::npos) // I : a 32 bit signed integer (Int_t)
		{
		  it->second = int(0);
		  tree->SetBranchAddress(it->first.c_str(), &boost::get<int>(it->second));
		}

	  else if (brName.find(string("/D")) != string::npos) // D : a 64 bit floating point (Double_t)
		{
		  it->second = double(0.0);
		  tree->SetBranchAddress(it->first.c_str(), &boost::get<double>(it->second));
		}

	  else if (brName.find(string("/O")) != string::npos) // O : [the letter o, not a zero] a boolean (Bool_t)
		{
		  it->second = bool(false);
		  tree->SetBranchAddress(it->first.c_str(), &boost::get<bool>(it->second));
		}

	  else
		{
		  cerr << "** AnalysisHelper : error : could not detect the type of var " << it->first
			   << " (title: " << br->GetTitle() << " , name: " << br->GetName() << " , className: " << br->GetClassName() << ")" << endl;
		  cerr << "   ... assuming float, but errors could happen" << endl;
		  it->second = float(0.0);
		  tree->SetBranchAddress(it->first.c_str(), &boost::get<float>(it->second));
		}
	}




  //////////////////////////////////////
  ////////////////////// loop on entries

  long long int nEvts  = sample.getEntries();
  long long int nStep  = nEvts/nsplit_;
  long long int nStart = nStep*idxsplit_;
  long long int nStop  = nStart+nStep;

  // to avoid for numerical errors (summing 1 to a TH1F can give rounding errors --> nEvts != chain.getEnries())
  // if the splitted job is the last one, let it go until chain completion
  // at the same time, it ensures that the remainder of the integer division is not skipped

  bool isLast = (idxsplit_ == nsplit_-1 ? true : false);
  // the last jobs takes up all the remainder of division -- as long as nsplit is O(10-100) is not a big problem
  // if (idxsplit_ == nsplit_-1)
  //     nStop = nEvts;

  if (DEBUG) {
	cout << " .....DEBUG: AnalysisHelper : fillHistosSample : going to loop on tree entries... " << endl;
  }

  Sample::selColl& plots = sample.plots();
  Sample::selColl2D& plots2D = sample.plots2D();
  for (long long iEv = nStart; true; ++iEv)
	{
	  int got = tree->GetEntry(iEv);
	  if (!isLast && iEv >= nStop) break;
	  if (got == 0) break; // end of the chain
	  if (iEv % 500000 == 0) cout << "   ... reading " << iEv << " / " << nEvts << endl;

	  double wEvSample = 1.0;
	  // get the product of all the event weights -- sample
	  //if (sample.getType() != Sample::kData)       {
	  for (unsigned int iw = 0; iw < sample.getWeights().size(); ++iw)
		{
		  int isNAN = TMath::IsNaN(boost::apply_visitor(get_variant_as_double(), valuesMap[sample.getWeights().at(iw).getName()]));
		  if (isNAN== 1 )cout<<"NaN weight: "<< sample.getWeights().at(iw).getName()<< endl;
		  wEvSample *= ( (isNAN == 1)? 0 : boost::apply_visitor(get_variant_as_double(), valuesMap[sample.getWeights().at(iw).getName()]));
		}

	  for (unsigned int isel = 0; isel < selections_.size(); ++isel)
		{

		  if (!(vTTF.at(isel)->EvalInstance())) continue;


		  double wEvSel = 1.0;
		  const Selection& currSel = selections_.at(isel);
		  for (unsigned int iw = 0; iw < currSel.getWeights().size(); ++iw)
			{
			  int isNAN = TMath::IsNaN(boost::apply_visitor(get_variant_as_double(), valuesMap[currSel.getWeights().at(iw).getName()]));
			  if (isNAN== 1 ) cout<<"NaN weight: "<< currSel.getWeights().at(iw).getName()<<endl;
			  wEvSel *= ( (isNAN == 1 )? 0 :boost::apply_visitor(get_variant_as_double(), valuesMap[currSel.getWeights().at(iw).getName()]));

			  //         if (sample.getType() == Sample::kBkg)
			  //cout << "~~~~~~~  : ~~~ " << iEv << " / evt sel: " << currSel.getWeights().at(iw).getName() << " = " << boost::apply_visitor(get_variant_as_double(), valuesMap[currSel.getWeights().at(iw).getName()]) << endl;
			}


		  for (unsigned int iw = 0; iw < currSel.getWeightsExt().size(); ++iw)
			{
			  double thisExtWeight = boost::apply_visitor(get_variant_as_double(), extValuesMap[currSel.getWeightsExt().at(iw).getFormula()]);
			  if (sample.getType() != Sample::kData){
				TTreeFormula* TTF_extWeight = new TTreeFormula (Form("TTF_extWeight_%i", iw), currSel.getWeightsExt().at(iw).getFormula().c_str(), tree);
				if (TTF_extWeight->EvalInstance())  {
				  wEvSel *= thisExtWeight;
				}
			  }
			}

		  // loop on all vars to fill
		  for (unsigned int ivar = 0; ivar < variables_.size(); ++ivar)
			{
			  double varvalue = boost::apply_visitor( get_variant_as_double(), valuesMap[variables_.at(ivar)]);
			  if (sample.getType() == Sample::kData)
				plots.at(isel).at(ivar).at(0)->Fill(varvalue);
			  else
				plots.at(isel).at(ivar).at(0)->Fill(varvalue, wEvSample*wEvSel);

			  // if (sample.getType() == Sample::kBkg)
			  //     cout << ">>>>  : >>> " << iEv << " / FILLING " << plots.at(isel).at(ivar).at(0)->GetName() << " varvalue:" << varvalue << " wEvSample:" << wEvSample << " wEvSel:" << wEvSel << " new integral:" << plots.at(isel).at(ivar).at(0)->Integral() << endl;

			  if (sample.getType() != Sample::kData)
				{
				  for (unsigned int isyst = 1; isyst < plots.at(isel).at(ivar).size(); ++isyst) // start from 1, as 0 is nominal case
					{
					  auto names = systMap.at(plots.at(isel).at(ivar).key(isyst));
					  double wnom   = boost::apply_visitor( get_variant_as_double(), valuesMap[names.first]);
					  double wshift = boost::apply_visitor( get_variant_as_double(), valuesMap[names.second]);
					  double wnew   = ( wshift == 0 && wnom == 0 ? 0.0 : wEvSample*wEvSel*wshift/wnom); // a protection from null weights. FIXME: should I redo all the multiplication to avoid this effect?
					  plots.at(isel).at(ivar).at(isyst)->Fill(varvalue, wnew);
					  // cout << " :::::: DDDDD ::::: " << wEvSample*wEvSel*wshift/wnom << " " << wnew << " " << wEvSample << " " << wEvSel << " " << wshift << " " << wnom << " " << names.first << " " << names.second << " " << sample.getName() << " " << iEv << endl;
					}
				}
			}

		  // loop on all 2D vars to fill
		  // loop on all vars to fill
		  for (unsigned int ivar = 0; ivar < variables2D_.size(); ++ivar)
			{
			  string var1 = variables2D_.at(ivar).first;
			  string var2 = variables2D_.at(ivar).second;
			  double varvalue1 = boost::apply_visitor( get_variant_as_double(), valuesMap[var1]);
			  double varvalue2 = boost::apply_visitor( get_variant_as_double(), valuesMap[var2]);
			  if (sample.getType() == Sample::kData)
				plots2D.at(isel).at(ivar).at(0)->Fill(varvalue1, varvalue2);
			  else
				plots2D.at(isel).at(ivar).at(0)->Fill(varvalue1, varvalue2, wEvSample*wEvSel);

			  if (sample.getType() != Sample::kData)
				{
				  for (unsigned int isyst = 1; isyst < plots2D.at(isel).at(ivar).size(); ++isyst) // start from 1, as 0 is nominal case
					{
					  auto names = systMap.at(plots2D.at(isel).at(ivar).key(isyst));
					  double wnom   = boost::apply_visitor( get_variant_as_double(), valuesMap[names.first]);
					  double wshift = boost::apply_visitor( get_variant_as_double(), valuesMap[names.second]);
					  double wnew   = ( wshift == 0 && wnom == 0 ? 0.0 : wEvSample*wEvSel*wshift/wnom); // a protection from null weights. FIXME: should I redo all the multiplication to avoid this effect?
					  plots2D.at(isel).at(ivar).at(isyst)->Fill(varvalue1, varvalue2, wnew);
					  // cout << " :::::: DDDDD ::::: " << wEvSample*wEvSel*wshift/wnom << " " << wnew << " " << wEvSample << " " << wEvSel << " " << wshift << " " << wnom << " " << names.first << " " << names.second << " " << sample.getName() << " " << iEv << endl;
					}
				}
			}
		}
	}

  // filling is finished, scale to the sample denominator evt sum to include acceptance
  if (sample.getType() != Sample::kData){
    sample.scaleAll(lumi_/sample.getEffDenom());
    cout << "scaled by " << lumi_<<"/"<< sample.getEffDenom()<<endl;
  }

  cout << "  fillHistos();" << endl;
  if (DEBUG) {
	for (uint is = 0; is < bkg_samples_.size(); ++is) {
	  cout << "  " << is << " >> " << setw(25) << left << bkg_samples_.at(is)->getName() << setw(13) << " nweights: " << bkg_samples_.at(is)->getWeights().size() << endl;
	}
  }

}

void AnalysisHelper::activateBranches(Sample& sample)
{
  if (DEBUG) cout << " ..........DEBUG: entering AnalysisHelper::activateBranches" << endl;

  TChain* tree = sample.getTree();
  tree->SetBranchStatus("*", 0);

  // activate all vars
  for (string var : variables_)
	{
	  tree->SetBranchStatus(var.c_str(), 1);
	}

  for (auto var2d : variables2D_)
	{
	  tree->SetBranchStatus(var2d.first.c_str(), 1);
	  tree->SetBranchStatus(var2d.second.c_str(), 1);
	}

  if (DEBUG) cout << " ..........DEBUG: activated var branches" << endl;

  // activate all weights
  // sample
  for (uint iw = 0; iw < sample.getWeights().size(); ++iw)
	{
	  const Weight& currW = sample.getWeights().at(iw);
	  tree->SetBranchStatus(currW.getName().c_str(), 1);
	  for (int isys = 0; isys < currW.getNSysts(); ++isys)
		{
		  //currW.getSyst(isys); // <----- keep an eye on it. It happened to throw range
		  tree->SetBranchStatus(currW.getSyst(isys).c_str(), 1);
		}
	}
  if (DEBUG) cout << " ..........DEBUG: activated sample weights branches" << endl;

  // selection -- probably not the most efficient as many w are shared so will be chacked many times
  // but this function is called only a few times
  for (uint isel = 0; isel < selections_.size(); ++isel)
	{
	  const Selection& currSel = selections_.at(isel);
	  for (uint iw = 0; iw < currSel.getWeights().size(); ++iw)
		{
		  const Weight& currW = currSel.getWeights().at(iw);
		  tree->SetBranchStatus(currW.getName().c_str(), 1);
		  for (int isys = 0; isys < currW.getNSysts(); ++isys)
			{
			  tree->SetBranchStatus(currW.getSyst(isys).c_str(), 1);
			}
		}

	  //for (uint iw = 0; iw < currSel.getWeightsExt().size(); ++iw)
	  //  {
	  //    const Weight_ext& currW = currSel.getWeightsExt().at(iw);
	  //    vector <string> varWeights= mainCfg_->readVarOpt(currSel.getWeightsExt().at(iw).getFormula().c_str());
	  //    for (string actVar: varWeights)
	  //      {
	  //	tree->SetBranchStatus(actVar.c_str(), 1);
	  //      }
	  //
	  //  }

	}
  if (DEBUG) cout << " ..........DEBUG: activated selections weights branches" << endl;

  // // activate all variables for cuts
  TObjArray* branchList =  tree->GetListOfBranches();
  int nBranch = tree->GetNbranches(); // all trees in the chain are identical
  for (int iB = 0 ; iB < nBranch; ++iB)
	{
	  string bName = branchList->At(iB)->GetName();
	  for (uint isel = 0; isel < selections_.size(); isel++)
		{
		  string theCut = selections_.at(isel).getValue().GetTitle(); // gives the content of tCut as char*
		  if (theCut.find(bName) != string::npos)
			{
			  tree->SetBranchStatus (bName.c_str(), 1);
			}
		}
	}
  if (DEBUG) cout << " ..........DEBUG: activated cut variables branches" << endl;

  tree->SetBranchStatus("dau1_decayMode",1);
  tree->SetBranchStatus("dau2_decayMode",1);
  tree->SetBranchStatus("isTau1real",1);
  tree->SetBranchStatus("isTau2real",1);
  if (DEBUG) cout << " ..........DEBUG: activated selectionWeights_ext branches" << endl;
}

pair <string, string> AnalysisHelper::unpack2DName (string packedName)
{
  stringstream packedNameS(packedName);
  string segment;
  vector<string> unpackedNames;
  while(std::getline(packedNameS, segment, ':'))
    unpackedNames.push_back(segment);
  if (unpackedNames.size() != 2)
	{
	  cout << "** AnalysisHelper : unpack2DName : error : couldn't interpret 2D variable name " << packedName << " (expecting X:Y)" << endl;
	  return make_pair("", "");
	}
  return make_pair(unpackedNames.at(0), unpackedNames.at(1));
}

string AnalysisHelper::pack2DName (string name1, string name2)
{
  return (name1 + string(":") + name2);
}


// vector<const Weight*> AnalysisHelper::getWeightsWithSyst (const Selection& sel, const Sample& sample)
// {
//     vector<const Weight*> vWeights;

//     // sample
//     for (uint iw = 0; iw < sample.getWeights().size(); ++iw)
//     {
//         if (sample.getWeights().at(iw).hasSysts())
//             vWeights.push_back(&(sample.getWeights().at(iw)));
//     }

//     // selection
//     for (uint iw = 0; iw < sel.getWeights().size(); ++iw)
//     {
//         if (sel.getWeights().at(iw).hasSysts())
//             vWeights.push_back(&(sel.getWeights().at(iw)));
//     }

//     return vWeights;

//     // 1) const for pointers
//     // 2) pointer to element of a vector
// }

// bool AnalysisHelper::readSingleSelection (std::string name, bool isComposed)
// {
//     if (!cutCfg_->hasOpt(name))
//     {
//         cerr << "** AnalysisHelper : readSingleSelection : error : could not find selection " << name << endl;
//         return false;
//     }


// }

void AnalysisHelper::fillHistos()
{
  for (uint isample = 0; isample < data_samples_.size(); ++isample) // loop on samples
	{
	  fillHistosSample(*(data_samples_.at(isample)));
	}

  // sig
  for (uint isample = 0; isample < sig_samples_.size(); ++isample) // loop on samples
	{
	  fillHistosSample(*(sig_samples_.at(isample)));
	}

  // bkg
  for (uint isample = 0; isample < bkg_samples_.size(); ++isample) // loop on samples
	{
	  fillHistosSample(*(bkg_samples_.at(isample)));
	}
}

void AnalysisHelper::setSplitting (int idxsplit, int nsplit)
{
  if (idxsplit >= nsplit)
	{
	  cout << "** Warning: AnalysisHelper::setSplitting : cannot have idx splitting : " << idxsplit << " if nsplit is " << nsplit << ", skipping..." << endl;
	  return;
	}
  nsplit_ = nsplit;
  idxsplit_ = idxsplit;

  // replace output name with suffix
  string appendix = ".root";
  size_t start_pos = outputFileName_.find(appendix);
  if(start_pos == std::string::npos)
    outputFileName_ += std::to_string(idxsplit_);
  else
    outputFileName_.replace(start_pos, appendix.length(), (string("_")+std::to_string(idxsplit_)+appendix));

  cout << "@@ split idx set to  : " << idxsplit_ <<  " of ntotal: " << nsplit_ << endl;
  cout << "@@ new output name   : " << outputFileName_ << endl;
}

// list all the information analysis helper knows
void AnalysisHelper::dump(int detail)
{
  cout << " ========= dumping AnalysisHelper information =========" << endl;
  cout << endl;

  cout << "@@@@@@@@ GENERAL @@@@@@@@" << endl;
  cout << "@ lumi            : " << lumi_ << endl;
  cout << "@ main cfg        : " << mainCfg_->getCfgName() << endl;
  cout << "@ sample cfg      : " << sampleCfg_->getCfgName() << endl;
  cout << "@ sel. cfg        : " << cutCfg_->getCfgName() << endl;
  cout << "@ n. selections   : " << selections_.size() << endl;
  cout << "@ n. variables    : " << variables_.size() << endl;
  cout << "@ n. 2D variables : " << variables2D_.size() << endl;
  cout << "@ n. data samples : " << data_samples_.size() << endl;
  cout << "@ n. sig samples  : " << sig_samples_.size() << endl;
  cout << "@ n. bkg samples  : " << bkg_samples_.size() << endl;
  cout << endl;

  cout << "@@@@@@@@ VARIABLES @@@@@@@@" << endl;
  cout << "@ variable list: " << endl;
  for (uint iv = 0; iv < variables_.size(); ++iv)
	{
	  cout << "  " << iv << " >> " << variables_.at(iv) << endl;
	}
  cout << endl;

  cout << "@ 2D variable list: " << endl;
  for (uint iv = 0; iv < variables2D_.size(); ++iv)
	{
	  cout << "  " << iv << " >> " << variables2D_.at(iv).first << ":" << variables2D_.at(iv).second << endl;
	}
  cout << endl;


  cout << "@@@@@@@@ SELECTIONS @@@@@@@@" << endl;
  cout << "@ selection list: " << endl;
  for (uint is = 0; is < selections_.size(); ++is)
    cout << "  " << is << " >> " << setw(25) << left << selections_.at(is).getName() << setw(13) << " nweights: " << selections_.at(is).getWeights().size() << endl;
  if (detail >=1)
	{
	  cout << "@ printing selections... " << endl;
	  printSelections((detail >= 2 ? true : false), (detail >= 3 ? true : false));
	}
  cout << endl;

  cout << "@@@@@@@@ SAMPLES @@@@@@@@" << endl;
  cout << "@ data sample list: " << endl;
  for (uint is = 0; is < data_samples_.size(); ++is)
    cout << "  " << is << " >> " << setw(25) << left << data_samples_.at(is)->getName() << setw(13) << " nweights: " << data_samples_.at(is)->getWeights().size() << endl;

  cout << "@ sig sample list: " << endl;
  for (uint is = 0; is < sig_samples_.size(); ++is)
    cout << "  " << is << " >> " << setw(25) << left << sig_samples_.at(is)->getName() << setw(13) << " nweights: " << sig_samples_.at(is)->getWeights().size() << endl;

  cout << "@ bkg sample list: " << endl;
  for (uint is = 0; is < bkg_samples_.size(); ++is)
	{
	  cout << "  " << is << " >> " << setw(25) << left << bkg_samples_.at(is)->getName() << setw(13) << " nweights: " << bkg_samples_.at(is)->getWeights().size() << endl;
	}

  cout << endl;
  if (detail >=1)
	{
	  cout << "@ printing details... " << endl;
	  printSamples((detail >= 2 ? true : false), (detail >= 3 ? true : false));
	}

  cout << " ================== end of printouts ==================" << endl;

}

void AnalysisHelper::prepareHistos()
{
  prepareSamplesHistos();
  prepareSamples2DHistos();
}

void AnalysisHelper::mergeSamples()
{
  for (unsigned int isnew = 0; isnew < sample_merge_list_.size(); ++isnew)
	{
	  string newname = sample_merge_list_.key(isnew);
	  cout << "@@ Merging histograms into " << newname << endl;

	  // create an empty new sample.
	  // NOTE: call this method after you finished to fill histos, the new sample has no tree associated and can't be filled!
	  shared_ptr<Sample> snew (new Sample(newname, ""));

	  // take the first histo in the list of masters
	  string snamefirst = sample_merge_list_.at(isnew).at(0);
	  shared_ptr<Sample> smaster = nullptr;
	  ordered_map <std::string, std::shared_ptr<Sample>>* chosenMap = nullptr;
	  int type = -1;

	  if (data_samples_.has_key(snamefirst)){
		type = (int) Sample::kData;
		smaster = data_samples_.at(snamefirst);
		chosenMap = &data_samples_;
	  }

	  else if (bkg_samples_.has_key(snamefirst)){
		type = (int) Sample::kBkg;
		smaster = bkg_samples_.at(snamefirst);
		chosenMap = &bkg_samples_;
	  }

	  else if (sig_samples_.has_key(snamefirst)){
		type = (int) Sample::kSig;
		smaster = sig_samples_.at(snamefirst);
		chosenMap = &sig_samples_;
	  }
	  else {
		cerr << "** AnalysisHelper : mergeSamples : error : could not find the sample " << snamefirst << " to merge, won't merge" << endl;
		return;
	  }

	  if (DEBUG) cout << "   DEBUG: --- merging histos - type is: " << type << endl;

	  //////////////////////// -- 1D plots //////////////////////////////////
	  // clone the histogram structure from the master

	  Sample::selColl& plmaster = smaster->plots();
	  Sample::selColl& plnew    = snew->plots();

	  if (DEBUG) cout << "   DEBUG: --- merging histos - going to loop over 1d plot to make structure" << endl;

	  for (unsigned int isel = 0; isel < plmaster.size(); ++isel){
		string selName = plmaster.key(isel);
		plnew.append(selName, Sample::varColl());
		for (unsigned int ivar = 0; ivar < plmaster.at(isel).size(); ++ivar ){
		  string varName = plmaster.at(isel).key(ivar);
		  plnew.at(isel).append(varName, Sample::systColl());
	
		  for (unsigned int isyst = 0; isyst < plmaster.at(isel).at(ivar).size(); ++isyst ){
			string systName = plmaster.at(isel).at(ivar).key(isyst);
			string hname = formHistoName (newname, selName, varName, systName);
			std::shared_ptr<TH1F> hist ((TH1F*) plmaster.at(isel).at(ivar).at(isyst)->Clone(hname.c_str())) ;
			hist->SetTitle(hist->GetName());
			plnew.at(isel).at(ivar).append(systName, hist);
		  }
		}
	  }

	  if (DEBUG) cout << "   DEBUG: --- merging histos - going to add the other samples" << endl;

	  // now add the content of the other histos to merge
	  for (unsigned int idx = 1; idx < sample_merge_list_.at(isnew).size(); ++idx)
		{
		  string sname = sample_merge_list_.at(isnew).at(idx);

		  Sample::selColl& pltoadd = chosenMap->at(sname)->plots();
		  for (unsigned int isel = 0; isel < plnew.size(); ++isel){
			for (unsigned int ivar = 0; ivar < plnew.at(isel).size(); ++ivar ){
			  for (unsigned int isyst = 0; isyst < plnew.at(isel).at(ivar).size(); ++isyst ){
				plnew.at(isel).at(ivar).at(isyst)->Add(pltoadd.at(isel).at(ivar).at(isyst).get());
			  }
			}
		  }
		}

	  //////////////////////// -- 2D plots //////////////////////////////////
	  // clone the histogram structure from the master

	  Sample::selColl2D& pl2Dmaster = smaster->plots2D();
	  Sample::selColl2D& pl2Dnew    = snew->plots2D();

	  if (DEBUG) cout << "   DEBUG: --- merging histos - going to loop over 1d plot to make structure" << endl;

	  for (unsigned int isel = 0; isel < pl2Dmaster.size(); ++isel){
		string selName = pl2Dmaster.key(isel);
		pl2Dnew.append(selName, Sample::varColl2D());

		if (DEBUG) {
		  cout << "   DEBUG: --- 1. merging histos - " << selName << " appended to pl2Dnew" << endl;
		}
	  
		for (unsigned int ivar = 0; ivar < pl2Dmaster.at(isel).size(); ++ivar ){
		  string varName = pl2Dmaster.at(isel).key(ivar);
		  pl2Dnew.at(isel).append(varName, Sample::systColl2D());

		  if (DEBUG) {
			cout << "   DEBUG: ---   2. merging histos - " << selName << " at " <<
			  varName << " appended to pl2Dnew" << endl;
		  }

		  string varName1 = variables2D_.at(ivar).first;
		  string varName2 = variables2D_.at(ivar).second;
		  for (unsigned int isyst = 0; isyst < pl2Dmaster.at(isel).at(ivar).size(); ++isyst ){
			string systName = pl2Dmaster.at(isel).at(ivar).key(isyst);
			string hname = formHisto2DName (newname, selName, varName1, varName2, systName);

			if (DEBUG) {
			  cout << "   DEBUG: ---      .merging histos - new histo name is: " <<
				hname << " from: " << newname << " " <<  selName << " " <<  varName <<
				" " <<  systName << "-||-" << endl;
			}

			std::shared_ptr<TH2F> hist ((TH2F*) pl2Dmaster.at(isel).at(ivar).at(isyst)->Clone(hname.c_str())) ;
			hist->SetTitle(hist->GetName());
			pl2Dnew.at(isel).at(ivar).append(systName, hist);
			if (DEBUG) {
			  cout << "   DEBUG: ---     3. merging histos - " << selName << " at " <<
				varName << " at " << systName << " done new histo, nbins: " << hist->GetNbinsX() << endl;
			}
		  }   
		}
	  }

	  if (DEBUG) cout << "   DEBUG: --- merging histos - going to add the other samples" << endl;
	
	  // now add the content of the other histos to merge
	  for (unsigned int idx = 1; idx < sample_merge_list_.at(isnew).size(); ++idx)
		{
		  string sname = sample_merge_list_.at(isnew).at(idx);
		  Sample::selColl2D& pltoadd = chosenMap->at(sname)->plots2D();
		  for (unsigned int isel = 0; isel < pl2Dnew.size(); ++isel){
			for (unsigned int ivar = 0; ivar < pl2Dnew.at(isel).size(); ++ivar ){
			  for (unsigned int isyst = 0; isyst < pl2Dnew.at(isel).at(ivar).size(); ++isyst ){
				pl2Dnew.at(isel).at(ivar).at(isyst)->Add(pltoadd.at(isel).at(ivar).at(isyst).get());
			  }
			}
		  }
		}

	  //////////////////////// now add the freshly created sample to its list, and remove old ones //////////////////////////////////

	  if (DEBUG) cout << "   DEBUG: --- merging histos - appending new sample" << endl;
	  chosenMap->append(newname, snew);
	  if (DEBUG) cout << "   DEBUG: --- merging histos - deleting merged samples" << endl;
	  for (string s : sample_merge_list_.at(isnew))
		chosenMap->remove(s);
	  if (DEBUG) cout << "   DEBUG: --- merging histos - all done with sample " << newname << endl;
	  if (DEBUG){
		cout << "   DEBUG: --- list of new samples " << endl;
		for (unsigned int ii = 0; ii < chosenMap->size(); ++ii)
		  cout << "    ................ " << chosenMap->key(ii) << endl;
	  }
	}
}
