#include "AnalysisHelper.h"
#include "TString.h" // just for Form, else use string
#include "TTreeFormulaGroup.h"
#include "TTreeFormula.h"
#include <iomanip>

using namespace std;

#define DEBUG false

AnalysisHelper::AnalysisHelper(string cfgname)
{
    TH1::SetDefaultSumw2(true);

    nominal_name_ = "NOMINAL"; // used for nominal systematics

    cout << "@@ Parsing main config : " << cfgname << endl;
    mainCfg_ = unique_ptr<CfgParser>(new CfgParser(cfgname));
    bool success = readMainInfo();
    if (!success)
        cerr << "** AnalysisHelper : error : some information could not be retrieved from config" << endl;
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
    
    if (!(mainCfg_->hasOpt("general::outputFolder"))) return false;
    outputFolder_ = mainCfg_->readStringOpt("general::outputFolder");   
    cout << "@@ output folder       : " << outputFolder_<< endl;   

    outputFileName_ = "outPlotter.root"; // override default only if specified
    if (mainCfg_->hasOpt("general::outputFileName"))
        outputFileName_ = mainCfg_->readStringOpt("general::outputFileName");
    cout << "@@ output file  name   : " << outputFileName_<< endl;       
    
    return true;
}

void AnalysisHelper::saveOutputsToFile()
{
    string outFile = outputFolder_ + "/" + outputFileName_ ;
    cout << "@@ Saving all plots to file : " << outFile << endl;
    system (Form("mkdir %s", outputFolder_.c_str()));

    system (Form("cp %s %s", (mainCfg_  ->getCfgName()).c_str() , outputFolder_.c_str()));
    system (Form("cp %s %s", (cutCfg_   ->getCfgName()).c_str() , outputFolder_.c_str()));
    system (Form("cp %s %s", (sampleCfg_->getCfgName()).c_str() , outputFolder_.c_str()));

    TFile* fOut = TFile::Open(outFile.c_str(), "recreate");
    vector <ordered_map <std::string, std::shared_ptr<Sample>> *> allToSave;
    allToSave.push_back(&data_samples_); 
    allToSave.push_back(&sig_samples_); 
    allToSave.push_back(&bkg_samples_); 
    
    // nesting orderd: type of events --> sample --> selection --> variable --> systematics

    fOut->cd();
    for (uint itype = 0; itype < allToSave.size(); ++itype)        
    {
        // cout << "itype " << itype << "/" << allToSave.size() << endl;
        for (uint isample = 0; isample < allToSave.at(itype)->size(); ++isample)
        {
            // cout << "isample " << isample << "/" << allToSave.at(itype)->size() << endl;
            Sample::selColl& plotSet = allToSave.at(itype)->at(isample)->plots();
            for (uint isel = 0; isel < plotSet.size(); ++isel)
            {
                // cout << "isel " << isel << "/" << plotSet.size() << endl;
                for (uint ivar = 0; ivar < plotSet.at(isel).size(); ++ivar)
                {
                    // cout << "ivar " << ivar << "/" << plotSet.at(isel).size() << endl;
                    for (uint isyst = 0; isyst < plotSet.at(isel).at(ivar).size(); ++isyst)
                    {
                        // cout << "isyst " << isyst << "/" << plotSet.at(isel).at(ivar).size() << endl;
                        plotSet.at(isel).at(ivar).at(isyst)->Write();
                        // cout << "DONE" << endl;
                    }
                }
            }
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
        data_samples_.append(name, smp);
        // cout << " " << name;
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
        if (DEBUG){
            cout << " ..........DEBUG: read bkg sample: " << smp->getName() << " nweights: " << smp->getWeights().size() << endl;
            for (uint iww = 0; iww < smp->getWeights().size(); ++iww)
                cout << " ..........DEBUG:    >> wname: " << smp->getWeights().at(iww).getName() << " nsyst: " << smp->getWeights().at(iww).getNSysts() << endl;
        }
        // cout << " " << name;
    }
    cout << endl;

    // printSamples(true);

    return;
}

shared_ptr<Sample> AnalysisHelper::openSample(string sampleName)
{
    if (DEBUG) cout << " ..........DEBUG: entering AnalysisHelper::openSample for sample " << sampleName << endl;

    string filename = sampleCfg_->readStringOpt(Form("samples::%s",sampleName.c_str()));
    shared_ptr<Sample> sample (new Sample(sampleName, filename + string("/goodfiles.txt")));
    
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
    variables_ = mainCfg_->readStringListOpt("general::variables");

    cout << "@@ Variables : reading variables : ";       
    for (string var : variables_)
        cout << " " << var;
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

                    varcoll.append(varName, Sample::systColl());
                    Sample::systColl& systcoll = varcoll.back();
                    
                    bool   hasUserBinning = cutCfg_->hasOpt(Form("binning::%s", varName.c_str()));
                    int    nbins = -1;
                    float  xlow = -1.;
                    float  xup = -1.;
                    float* binning = 0;

                    if (hasUserBinning)
                    {
                        vector<float> vBins = cutCfg_->readFloatListOpt(Form("binning::%s", varName.c_str()));
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
                    string selName    = currSel.getName();

                    string hname = formHistoName (sampleName, selName, varName, nominal_name_);
                    std::shared_ptr<TH1F> hist;
                    if (hasUserBinning) hist = make_shared<TH1F> (hname.c_str(), hname.c_str(), nbins, binning);
                    else                hist = make_shared<TH1F> (hname.c_str(), hname.c_str(), nbins, xlow, xup);
                    systcoll.append(hname, hist);

                    if (hasUserBinning) delete[] binning ; // was allocated with new

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
                                systcoll.append(hname, histS);
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
                                systcoll.append(hname, histS);
                            }
                        }
                    }

                    // set Sumw2() and other stuff for all the histos
                    for (uint ih = 0; ih < systcoll.size(); ++ih)
                    {
                        if (doselW.at(ismpc) != 1) // is data
                            systcoll.at(ih)->Sumw2();
                        else
                            systcoll.at(ih)->SetBinErrorOption(TH1::kPoisson);   
                    }

                } // end loop on variables
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

    return s;
}

void AnalysisHelper::printSelections(bool printWeights)
{
    cout << "@@ Selection list ------- " << endl;
    for (unsigned int i = 0; i < selections_.size(); ++i)
    {
        cout << " >> " << selections_.at(i).getName() << " " << selections_.at(i).getValue().GetTitle() << endl;
        if (printWeights)
        {
            cout << "     ~~~> ";
            for (uint iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
            {
                cout << selections_.at(i).getWeights().at(iw).getName() << " ";
            }
            cout << endl;
        }

        // // syst of weights
        // for (int iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
        // {
        //     cout << selections_.at(i).getWeights().at(iw).getName() << " ::: ";
        //     for (int isys = 0; isys < selections_.at(i).getWeights().at(iw).nSysts(); ++isys)
        //         cout << selections_.at(i).getWeights().at(iw).getSystName(isys) <<"="<<selections_.at(i).getWeights().at(iw).getSyst(isys) << " , ";
        //     cout << endl;
        // }
    }
    cout << "   ---------------------- " << endl;
}

void AnalysisHelper::printSamples(bool printWeights)
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

        // // syst of weights
        // for (int iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
        // {
        //     cout << selections_.at(i).getWeights().at(iw).getName() << " ::: ";
        //     for (int isys = 0; isys < selections_.at(i).getWeights().at(iw).nSysts(); ++isys)
        //         cout << selections_.at(i).getWeights().at(iw).getSystName(isys) <<"="<<selections_.at(i).getWeights().at(iw).getSyst(isys) << " , ";
        //     cout << endl;
        // }
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

        // // syst of weights
        // for (int iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
        // {
        //     cout << selections_.at(i).getWeights().at(iw).getName() << " ::: ";
        //     for (int isys = 0; isys < selections_.at(i).getWeights().at(iw).nSysts(); ++isys)
        //         cout << selections_.at(i).getWeights().at(iw).getSystName(isys) <<"="<<selections_.at(i).getWeights().at(iw).getSyst(isys) << " , ";
        //     cout << endl;
        // }
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

        // // syst of weights
        // for (int iw = 0; iw < selections_.at(i).getWeights().size(); ++iw)
        // {
        //     cout << selections_.at(i).getWeights().at(iw).getName() << " ::: ";
        //     for (int isys = 0; isys < selections_.at(i).getWeights().at(iw).nSysts(); ++isys)
        //         cout << selections_.at(i).getWeights().at(iw).getSystName(isys) <<"="<<selections_.at(i).getWeights().at(iw).getSyst(isys) << " , ";
        //     cout << endl;
        // }
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

void AnalysisHelper::fillHistosSample(Sample& sample)
{
    cout << "@@ Filling histograms of sample " << sample.getName() << endl;

    activateBranches(sample);
/*
    TChain* tree = sample.getTree();
    long long int nEvts = sample.getEntries();
    
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

    // setup address for variables to be filled and for weights
    vector<int> floatVarsIdxs;
    vector<int> intVarsIdxs;
    vector <int> vType; // 0: int, 1: float

    // vector<int> doubleVarsIdxs;
    TObjArray *branchList = tree->GetListOfBranches();
    for (unsigned int ivar = 0; ivar < variables_.size(); ++ivar)
    {
        TBranch* br = (TBranch*) branchList->FindObject(variables_.at(ivar).c_str());
        string brName = br->GetTitle();
        if (brName.find(string("/F")) != string::npos) // F : a 32 bit floating point (Float_t)
        {
            floatVarsIdxs.push_back(ivar);
            vType.push_back(1);
        }
        else if (brName.find(string("/I")) != string::npos) // I : a 32 bit signed integer (Int_t)
        {
            intVarsIdxs.push_back(ivar);
            vType.push_back(0);
        }
        else
        {
            cerr << "** AnalysisHelper : error : could not detect the type of var " << variables_.at(ivar)
            << " (title: " << br->GetTitle() << " , name: " << br->GetName() << " , className: " << br->GetClassName() << ")" << endl;
            cerr << "   ... assuming float, but errors could happen" << endl;
            floatVarsIdxs.push_back(ivar);
            vType.push_back(1);
        }
    }
*/
/*
    // create vectors and set branch addresses - as long as vectors are not resized addresses will stay constant
    vector<float> floatVars(floatVarsIdxs.size());
    vector<int>   intVars(intVarsIdxs.size());

    for (uint iv = 0; iv < floatVarsIdxs.size(); ++iv)
    {
        string vname = variables_.at(floatVarsIdxs.at(iv));
        tree->SetBranchAddress(vname.c_str(), &(floatVars.at(iv)));
    }

    for (uint iv = 0; iv < intVarsIdxs.size(); ++iv)
    {
        string vname = variables_.at(intVarsIdxs.at(iv));
        tree->SetBranchAddress(vname.c_str(), &(intVars.at(iv)));
    }
*/

    /*
    // create structure to retrieve weight value by setBranchAddres without resetting address at every event/selection
    // this is done by creating a double structure:
    // 1- vector of size N that will contain the weight value
    // 2- map that remaps weight name to index in the vector
    // NOTE: this requires all weights to be of the same type (float in this case)

    vector<float> floatWeights;
    unordered_map<string, int> weightMap;
    int nweights = 0; // unique weights used for this sample
    
    // sample
    for (uint iw = 0; iw < sample.getWeights().size(); ++iw)
    {
        const Weight& currW = sample.getWeights().at(iw);
        string wname = currW.getName();
        if (weightMap.find(wname) == weightMap.end())
        {
            weightMap[wname] = nweights;
            ++nweights;
            for (int isys = 0; isys < currW.getNSysts(); ++isys)
            {
                string sysName = currW.getSyst(isys);
                if (weightMap.find(sysName) == weightMap.end())
                {
                    weightMap[sysName] = nweights;
                    ++nweights;
                }
            }
        }
    }

    // selection -- probably not the most efficient as many w are shared so will be chacked many times
    // but this function is called only a few times
    for (uint isel = 0; isel < selections_.size(); ++isel)
    {
        const Selection& currSel = selections_.at(isel);
        for (uint iw = 0; iw < currSel.getWeights().size(); ++iw)
        {
            const Weight& currW = currSel.getWeights().at(iw);
            string wname = currW.getName();
            if (weightMap.find(wname) == weightMap.end())
            {
                weightMap[wname] = nweights;
                ++nweights;
                for (int isys = 0; isys < currW.getNSysts(); ++isys)
                {
                    string sysName = currW.getSyst(isys);
                    if (weightMap.find(sysName) == weightMap.end())
                    {
                        weightMap[sysName] = nweights;
                        ++nweights;
                    }
                }
            }
        }
    }
    // finally, I have a structure containing all the weight names that I need -- let's set branchaddress
    floatWeights.resize(nweights);
    for (auto el : weightMap)
    {
        tree->SetBranchAddress(el.first.c_str(), &(floatWeights.at(el.second)));
    }
    */
    
    /*
    for (uint iw = 0; iw < sample.getWeights().size(); ++iw)
    {
        Weight& currW = sample.getWeights().at(iw);
        tree->SetBranchAddress(currW.getName().c_str(), &currW.getRefToWeightValue());
        for (int isys = 0; isys < currW.getNSysts(); ++isys)
        {
            string sysName = currW.getSyst(isys);
            tree->SetBranchAddress(sysName.c_str(), &(currW.getSystsValues().at(isys)));
        }
    }
    */



    //////////////////////////////////////
    ////////////////////// loop on entries
/*
    Sample::selColl& plots = sample.plots();
    for (uint iEv = 0; true; ++iEv)
    {


        int got = tree->GetEntry(iEv);
        if (got == 0) break; // end of the chain
        if (iEv % 500000 == 0) cout << "   ... reading " << iEv << " / " << nEvts << endl;

        double wEvSample = 1.0;
        // get the product of all the event weights -- sample
        for (uint iw = 0; iw < sample.getWeights().size(); ++iw)
        {
            wEvSample *= sample.getWeights().at(iw).getWeightValue();
        }

        for (uint isel = 0; isel < selections_.size(); ++isel)
        {

            if (!(vTTF.at(isel)->EvalInstance())) continue;


            double wEvSel = 1.0;
            const Selection& currSel = selections_.at(isel);
            for (uint iw = 0; iw < currSel.getWeights().size(); ++iw)
            {   
                wEvSel *= currSel.getWeights().at(iw).getWeightValue();
            }
            
            // loop on all vars to fill
            for (uint ivar = 0; ivar < variables_.size(); ++ivar)
            {
                plots.at(isel).at(ivar).at(0)->Fill(1, wEvSample*wEvSel);
                
                // now up/down syst -- FIXME : TO DO
                // for 
                // {

                // }
            }
        }
    }
*/
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

// list all the information analysis helper knows
void AnalysisHelper::dump()
{
    cout << " ========= dumping AnalysisHelper information =========" << endl;
    cout << endl;
    
    cout << "@@@@@@@@ GENERAL @@@@@@@@" << endl;
    cout << "@ lumi           : " << lumi_ << endl;
    cout << "@ main cfg       : " << mainCfg_->getCfgName() << endl;
    cout << "@ sample cfg     : " << sampleCfg_->getCfgName() << endl;
    cout << "@ sel. cfg       : " << cutCfg_->getCfgName() << endl;
    cout << "@ n. selections  : " << selections_.size() << endl;
    cout << "@ n. variables   : " << variables_.size() << endl;
    cout << "@ n. data samples: " << data_samples_.size() << endl;
    cout << "@ n. sig samples : " << sig_samples_.size() << endl;
    cout << "@ n. bkg samples : " << bkg_samples_.size() << endl;
    cout << endl;

    cout << "@@@@@@@@ VARIABLES @@@@@@@@" << endl;
    cout << "@ variable list: " << endl;
    for (uint iv = 0; iv < variables_.size(); ++iv)
    {
        cout << "  " << iv << " >> " << variables_.at(iv) << endl;
    }
    cout << endl;

    cout << "@@@@@@@@ SELECTIONS @@@@@@@@" << endl;
    cout << "@ selection list: " << endl;
    for (uint is = 0; is < selections_.size(); ++is)
        cout << "  " << is << " >> " << setw(25) << left << selections_.at(is).getName() << setw(13) << " nweights: " << selections_.at(is).getWeights().size() << endl;
    cout << "@ printing selections... " << endl;
    printSelections(true);
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
    cout << "@ printing details... " << endl;
    printSamples(true);


    cout << " ================== end of printouts ==================" << endl;

}