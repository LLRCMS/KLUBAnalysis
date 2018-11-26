#include "bTagSF.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TMath.h"
#include <stdio.h>
#include <iostream>

using namespace std;

#define DEBUG false

// bTagSF::bTagSF(std::string SFfilename) : 
    
//     m_calib("CSVv2", SFfilename.c_str()) ,
//     m_reader {
//     BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "central"),
//     BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "central"),
//     BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "central")} ,
//     m_reader_up {
//     BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "up"),
//     BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "up"),
//     BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "up")} ,
//     m_reader_do {
//     BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "down"),
//     BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "down"),
//     BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "down")}
// {
//     m_fileEff = 0;
// }


bTagSF::bTagSF(std::string SFfilename, std::string effFileName, std::string effHistoTag, std::string WPset) :
    
    //m_calib("CSVv2", SFfilename.c_str()) ,
    m_calib("DeepCSV", SFfilename.c_str()) ,
    m_readers {
        BTagCalibrationReader(BTagEntry::OP_LOOSE,  "central", {"up", "down"}),
        BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", {"up", "down"}),
        BTagCalibrationReader(BTagEntry::OP_TIGHT,  "central", {"up", "down"})}
    // m_reader {
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "central"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "central"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "central")} ,
    // m_reader_up {
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "up"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "up"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "up")} ,
    // m_reader_do {
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "down"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "down"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "down")} ,
    
    // m_reader_c {
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "central"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "central"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "central")} ,
    // m_reader_c_up {
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "up"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "up"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "up")} ,
    // m_reader_c_do {
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "down"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "down"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "down")} ,

    // m_reader_udsg {
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "incl", "central"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "incl", "central"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "incl", "central")} ,
    // m_reader_udsg_up {
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "incl", "up"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "incl", "up"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "incl", "up")} ,
    // m_reader_udsg_do {
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "incl", "down"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "incl", "down"),
    // BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "incl", "down")} 
{
    // // [b, c, udsg] [central, up, down] [loose, medium, tight]        = & _up//down [loose, medium, tight]

    // m_readers[0][0][0] = & (m_reader[0]) ;
    // m_readers[0][0][1] = & (m_reader[1]) ;
    // m_readers[0][0][2] = & (m_reader[2]) ;

    // m_readers[0][1][0] = & (m_reader_up[0]) ;
    // m_readers[0][1][1] = & (m_reader_up[1]) ;
    // m_readers[0][1][2] = & (m_reader_up[2]) ;

    // m_readers[0][2][0] = & (m_reader_do[0]) ;
    // m_readers[0][2][1] = & (m_reader_do[1]) ;
    // m_readers[0][2][2] = & (m_reader_do[2]) ;


    // m_readers[1][0][0] = & (m_reader_c[0]) ;
    // m_readers[1][0][1] = & (m_reader_c[1]) ;
    // m_readers[1][0][2] = & (m_reader_c[2]) ;

    // m_readers[1][1][0] = & (m_reader_c_up[0]) ;
    // m_readers[1][1][1] = & (m_reader_c_up[1]) ;
    // m_readers[1][1][2] = & (m_reader_c_up[2]) ;

    // m_readers[1][2][0] = & (m_reader_c_do[0]) ;
    // m_readers[1][2][1] = & (m_reader_c_do[1]) ;
    // m_readers[1][2][2] = & (m_reader_c_do[2]) ;


    // m_readers[2][0][0] = & (m_reader_udsg[0]) ;
    // m_readers[2][0][1] = & (m_reader_udsg[1]) ;
    // m_readers[2][0][2] = & (m_reader_udsg[2]) ;

    // m_readers[2][1][0] = & (m_reader_udsg_up[0]) ;
    // m_readers[2][1][1] = & (m_reader_udsg_up[1]) ;
    // m_readers[2][1][2] = & (m_reader_udsg_up[2]) ;
        
    // m_readers[2][2][0] = & (m_reader_udsg_do[0]) ;
    // m_readers[2][2][1] = & (m_reader_udsg_do[1]) ;
    // m_readers[2][2][2] = & (m_reader_udsg_do[2]) ;

    // load readers [loose, medium, tight]
    m_readers[0].load(m_calib, BTagEntry::FLAV_B, "comb");
    m_readers[0].load(m_calib, BTagEntry::FLAV_C, "comb");
    m_readers[0].load(m_calib, BTagEntry::FLAV_UDSG, "incl");

    m_readers[1].load(m_calib, BTagEntry::FLAV_B, "comb");
    m_readers[1].load(m_calib, BTagEntry::FLAV_C, "comb");
    m_readers[1].load(m_calib, BTagEntry::FLAV_UDSG, "incl");

    m_readers[2].load(m_calib, BTagEntry::FLAV_B, "comb");
    m_readers[2].load(m_calib, BTagEntry::FLAV_C, "comb");
    m_readers[2].load(m_calib, BTagEntry::FLAV_UDSG, "incl");

    m_fileEff = new TFile (effFileName.c_str());
 
    TString flavs[3]   = {"b", "c", "udsg"};
    TString chnames[4] = {"MuTau", "EleTau", "TauTau", "ALL"};
    TString WPnames[3] = {"L", "M", "T"};
 
    for (int iWP = 0; iWP < 3; iWP++)
    {
        for (int flav = 0; flav < 3; flav++)
        {
        for (int channel = 0; channel < 4; channel++)
            {
                TString name = Form("eff_%s_%s_%s", flavs[flav].Data(), WPnames[iWP].Data(), chnames[channel].Data());
                //cout << "Tmps name " << name << endl;
                m_hEff [iWP][flav][channel] = (TH1F*) m_fileEff->Get(name.Data());
                //cout << " --> " << m_hEff [iWP][flav][channel] -> GetName() << endl;
            }
        }
    }

    SetWPset(WPset);
}

bTagSF::~bTagSF()
{
    if (m_fileEff) delete m_fileEff;
}

void bTagSF::SetWPset(std::string WPset)
{
    if (WPset == "80X_ICHEP_2016")
        SetWPset(0.460, 0.800, 0.935);
    
    else if (WPset == "80X_MORIOND_2017")
        SetWPset(0.5426, 0.8484, 0.9535);
    
    else if (WPset =="94X_DeepCSV_V1")
        SetWPset(0.1522, 0.4941, 0.8001);

    else
    {
        cout << "bTagSF :: WARNING! SF set string " << WPset << " not recognized, going to use values from 80X_MORIOND_2017" << endl;
        SetWPset(0.460, 0.800, 0.935);        
    }

    printf("bTagSF :: info : WP set is %s i.e.;%.3f %.3f %.3f\n", WPset.c_str(), _WPtag[0], _WPtag[1], _WPtag[2]);
}

void bTagSF::SetWPset(double loose, double medium, double tight)
{
    _WPtag[0] = loose;
    _WPtag[1] = medium;
    _WPtag[2] = tight;
}

float bTagSF::getSF (WP wpt, SFsyst syst, int jetFlavor, float pt, float eta)
{
    // if (DEBUG) cout << "   ~~ requesting SF for WP=" << wpt << " SFsyst=" << syst << " jetFlavor=" << jetFlavor << " pt=" << pt << " eta=" << eta << endl;

    float SF = 1.0;
    
    BTagEntry::JetFlavor flav;
    // int myFlavIndex = -1; // indexes in the m_readers array
    int mySystIndex = (int) syst;
    int myWPIndex   = (int) wpt;
    float mypt = pt;
    if (mypt < 30.) mypt = 30.;
    if (mypt > 670.) mypt = 670.;

    if (abs(jetFlavor) == 5)
    {
        flav = BTagEntry::FLAV_B;
        // myFlavIndex = 0;
    }
    else if (abs(jetFlavor) == 4)
    {
        flav = BTagEntry::FLAV_C;
        // myFlavIndex = 1;
    }
    else
    {
        flav = BTagEntry::FLAV_UDSG;
        // myFlavIndex = 2;
    }

    if (DEBUG) cout << "   ~~ requesting SF for WP=" << wpt << "," << myWPIndex << " SFsyst=" << syst << "," << mySystIndex << " jetFlavor=" << jetFlavor << " pt=" << pt << " eta=" << eta << endl;

    // SF = m_readers[myFlavIndex][mySystIndex][myWPIndex] -> eval(flav, eta, pt);
    string systName[3] = {"central", "up", "down"}; // like SFsyst enum;
    SF = m_readers[myWPIndex].eval_auto_bounds(systName[mySystIndex], flav, eta, pt);

    // if (syst == central)
    //     SF = m_reader[(int)wpt].eval(flav, eta, pt);   
    // else if (syst == up)
    //     SF = m_reader_up[(int)wpt].eval(flav, eta, pt);
    // else if (syst == down)
    //         SF = m_reader_do[(int)wpt].eval(flav, eta, pt);
    // double uncertainty up/down if out of some boundaries
    // now automatically returned by BTAG reader
    // and not just double the scale
    // if (syst != central)
    // {
    //     if (flav == BTagEntry::FLAV_B || flav == BTagEntry::FLAV_C)
    //     {
    //         if (pt < 30.0 || pt > 670.0 ) SF *= 2.0;
    //     }
    //     if (flav == BTagEntry::FLAV_UDSG)
    //     {
    //         if (pt < 20.0 || pt > 1000.0 ) SF *= 2.0;
    //     }
    //     SF *= 1.0;
    // }

    if (DEBUG) cout << "   ~~ returning " << SF << endl;
    return SF;
}


float bTagSF::getEff (WP wpt, int jetFlavor, int channel, float pt, float eta)
{
    // use always "ALL" measurement (last in the array)
    //if (channel >= 3) return 0; 
    channel = 3; // "ALL"

    int flav;
    if (abs(jetFlavor) == 5) flav = 0;
    else if (abs(jetFlavor) == 4) flav = 1;
    else flav = 2;

    TH1F* h = m_hEff [(int) wpt] [flav] [channel];
    float aEta = TMath::Abs(eta);

    int binglobal = h->FindBin (pt, aEta);
    int binx, biny, binz;
    h->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
    int nx = h->GetNbinsX();
    int ny = h->GetNbinsY();
    
    // under-overflows
    if (binx < 1) binx = 1;
    if (biny < 1) biny = 1;
    if (binx > nx) binx = nx;
    if (biny > ny) biny = ny;

    /*
    float minPt = h->GetXaxis()->GetBinLowEdge(1);
    float maxPt = h->GetXaxis()->GetBinLowEdge(nx+1);

    float minEta = h->GetYaxis()->GetBinLowEdge(1);
    float maxEta = h->GetYaxis()->GetBinLowEdge(ny+1);
    */

    float eff = h->GetBinContent (binx, biny);

    // protection againts wrongly measured efficiencies (low stat) --> reduce pT bin
    while (eff < 0.00000000001 && binx > 0)
    {
        binx-- ;
        eff = h->GetBinContent (binx, biny);
    }

    return eff;
}

// the collection jets_and_btag in input contain all the final list of jets, already cleaned from PU and leptons
// returns a collection of weights according to the tested WP
vector<float> bTagSF::getEvtWeight (std::vector <std::pair <int, float> >& jets_and_btag, std::vector<float> *jets_px, std::vector<float> *jets_py, std::vector<float> *jets_pz, std::vector<float> *jets_e, std::vector<int> *jets_HadronFlavour, int channel)
{

    vector<double> P_MC   (3, 1.0); // 0 = L, 1 = M, 2 = T
    vector<double> P_Data (3, 1.0); // 0 = L, 1 = M, 2 = T
    
    TLorentzVector vJet (0,0,0,0);
    // float WPtag[3] = {0.605, 0.89, 0.97}; // L, M, T
    // double WPtag[3] = {0.460, 0.800, 0.935}; // L, M, T -- 80X 4 inv fb
    // double WPtag[3] = {0.5426, 0.8484, 0.9535}; // L, M, T -- 80X for Moriond 2017, 23SepReReco

    for (unsigned int ijet = 0; ijet < jets_and_btag.size(); ijet++)
    {
        if (DEBUG) cout << "DEB: ijet " << ijet << " , size = " << jets_and_btag.size() << endl;

        int idx = jets_and_btag.at(ijet).first;
        vJet.SetPxPyPzE (jets_px->at(idx), jets_py->at(idx), jets_pz->at(idx), jets_e->at(idx));
        
        int flav = jets_HadronFlavour->at(idx);
        double SF[3];
        SF[0] = getSF (loose,  central, flav, vJet.Pt(), vJet.Eta());
        SF[1] = getSF (medium, central, flav, vJet.Pt(), vJet.Eta());
        SF[2] = getSF (tight,  central, flav, vJet.Pt(), vJet.Eta());
        if (DEBUG) cout << "  >> DEB: SFs " << SF[0] << " " << SF[1] << " " << SF[2] << endl;

        double effBTag[3];
        effBTag[0] = getEff (static_cast<WP> (0), flav, channel, vJet.Pt(), vJet.Eta()) ;
        effBTag[1] = getEff (static_cast<WP> (1), flav, channel, vJet.Pt(), vJet.Eta()) ;
        effBTag[2] = getEff (static_cast<WP> (2), flav, channel, vJet.Pt(), vJet.Eta()) ;
        if (DEBUG) cout << "  >> DEB: EFFs " << effBTag[0] << " " << effBTag[1] << " " << effBTag[2] << endl;

        double CSV = jets_and_btag.at(ijet).second;
        bool tagged[3];
        tagged[0] = (CSV > _WPtag[0]);
        tagged[1] = (CSV > _WPtag[1]);
        tagged[2] = (CSV > _WPtag[2]);
        if (DEBUG) cout << "  >> DEB: tagged " << tagged[0] << " " << tagged[1] << " " << tagged[2] << endl;
        for (int iWP = 0; iWP < 3; iWP++)
        {
            //cout << " ---> WP: " << iWP << endl;
            double tmpMC   = P_MC.at(iWP);
            double tmpData = P_Data.at(iWP);

            if (tagged[iWP])
            {
                tmpMC *= effBTag[iWP];  
                tmpData *= effBTag[iWP]*SF[iWP];  
            } 
            else
            {
                tmpMC *= (1. - effBTag[iWP]);  
                tmpData *= (1. - (effBTag[iWP]*SF[iWP]) );  
            }

            //cout << "       effBTag: " << effBTag[iWP] << endl;
            //cout << "       SF: " << SF[iWP] << endl;
            //cout << "       tmpMC: " << tmpMC << endl;
            //cout << "       tmpData: " << tmpData << endl;
        
            P_MC.at(iWP)  = tmpMC;
            P_Data.at(iWP) = tmpData;

            // if (iWP == 0) 
            // {
            //     cout << ijet << " pt, eta: " << vJet.Pt() << " " << vJet.Eta() << " jet flav: " << flav << " //// SF= " << SF[0] << " eff= " << effBTag[0] << endl;
            // }
        
        }
    }
    // return ratio
    vector<float> weight (3);
    weight.at(0) = P_Data.at(0) / P_MC.at(0);
    weight.at(1) = P_Data.at(1) / P_MC.at(1);
    weight.at(2) = P_Data.at(2) / P_MC.at(2);
    
    if (weight.at(0) < 0.05) 
    {
        cout << "------ ERROR Null B-TAG weight!!" << endl;
        cout << "    >>> " << P_Data.at(0) << " / " << P_MC.at(0) << " , "
                           << P_Data.at(1) << " / " << P_MC.at(1) << " , "
                           << P_Data.at(2) << " / " << P_MC.at(2) << " , "
                           << endl;
    }
    //cout << "weights: " << weight.at(0) << " " << weight.at(1) << " " << weight.at(2) << endl;
    return weight;
}



