#include "../../HHKinFit2/HHKinFit2/include/exceptions/HHInvMConstraintException.h"
#include "../../HHKinFit2/HHKinFit2/include/exceptions/HHEnergyRangeException.h"
#include "../../HHKinFit2/HHKinFit2/include/exceptions/HHEnergyConstraintException.h"
#include "../../HHKinFit2/HHKinFit2/include/exceptions/HHLimitSettingException.h"
#include "../../HHKinFit2/HHKinFit2/include/HHKinFitMasterHeavyHiggs.h"
#include "kinfitter.h"

KinFitter::KinFitter(TLorentzVector tlv_firstBjet,
                     TLorentzVector tlv_secondBjet,
                     TLorentzVector tlv_firstLepton,
                     TLorentzVector tlv_secondLepton,
                     TVector2 ptmiss,
                     TMatrixD stableMetCov,
                     double bjet1_JER,
                     double bjet2_JER) {
    
    tlv_l1 = tlv_firstBjet;
    tlv_l2 = tlv_secondBjet;
    tlv_b1 = tlv_firstLepton;
    tlv_b2 = tlv_secondLepton;
    ptmiss = ptmiss;
    metcov = stableMetCov;
    b1_JER = bjet1_JER;
    b2_JER = bjet2_JER;

    KinFit = HHKinFit2::HHKinFitMasterHeavyHiggs(tlv_b1, tlv_b2, tlv_l1, tlv_l2, ptmiss, metcov, b1_JER, b2_JER);
}

KinFitter::~KinFitter() {}

std::pair<HHKinFit2::HHKinFitMasterHeavyHiggs,float> KinFitter::_fit(int mh1_hp, int mh2_hp) {
    HHKinFit2::HHKinFitMasterHeavyHiggs localFit = HHKinFit2::HHKinFitMasterHeavyHiggs(tlv_b1, tlv_b2, tlv_l1, tlv_l2, ptmiss, metcov);
    localFit.addHypo(mh1_hp, mh2_hp);

    float localChi2 = -999;
    bool DEBUG = false;

    bool wrongFit=false;
    try { localFit.fit(); }
    catch (HHKinFit2::HHInvMConstraintException const& e) {
        std::cout<<"THIS FIT DID NOT CONVERGE: INV MASS CONSTRAIN EXCEPTION (mh1_hp = " << mh1_hp << " , mh2_hp = " << mh2_hp << ")" << std::endl;
        if (DEBUG) {            
            std::cout<<"INVME Tau1"<<std::endl;
            std::cout<<"INVME (E,Px,Py,Pz,M) "<< tlv_l1.E() <<","<< tlv_l1.Px() <<","<< tlv_l1.Py() <<","<< tlv_l1.Pz() <<","<< tlv_l1.M() << std::endl;
            std::cout<<"INVME Tau2"<<std::endl;
            std::cout<<"INVME (E,Px,Py,Pz,M) "<< tlv_l2.E() <<","<< tlv_l2.Px() <<","<< tlv_l2.Py() <<","<< tlv_l2.Pz() <<","<< tlv_l2.M() << std::endl;
            std::cout<<"INVME B1"<<std::endl;
            std::cout<<"INVME (E,Px,Py,Pz,M) "<< tlv_b1.E() <<","<< tlv_b1.Px() <<","<< tlv_b1.Py() <<","<< tlv_b1.Pz() <<","<< tlv_b1.M() << std::endl;
            std::cout<<"INVME B2"<<std::endl;
            std::cout<<"INVME (E,Px,Py,Pz,M) "<< tlv_b2.E() <<","<< tlv_b2.Px() <<","<< tlv_b2.Py() <<","<< tlv_b2.Pz() <<","<< tlv_b2.M() << std::endl;
            std::cout<<"INVME MET"<<std::endl;
            std::cout<<"INVME (E,Px,Py,Pz,M) "<<","<< ptmiss.Px() <<","<< ptmiss.Py() << std::endl;
            std::cout<<"INVME METCOV "<<std::endl;
            std::cout<<"INVME "<< metcov (0,0) <<"  "<< metcov (0,1) << std::endl;
            std::cout<<"INVME "<< metcov (1,0) <<"  "<< metcov (1,1) << std::endl;
            std::cout<<"INVME tau1, tau2, b1, b2"<<std::endl;
            std::cout<<"INVME ";
            tlv_l1.Print();
            std::cout<<"INVME ";
            tlv_l2.Print();
            std::cout<<"INVME ";
            tlv_b1.Print();
            std::cout<<"INVME ";
            tlv_b2.Print();
        }
        wrongFit=true;
    }
    catch (HHKinFit2::HHEnergyRangeException const& e) {
        std::cout<<"THIS FIT DID NOT CONVERGE: INV MASS CONSTRAIN EXCEPTION (mh1_hp = " << mh1_hp << " , mh2_hp = " << mh2_hp << ")" << std::endl;
        if (DEBUG) {
            std::cout<<"ERANGE Tau1"<<std::endl;
            std::cout<<"ERANGE (E,Px,Py,Pz,M) "<< tlv_l1.E() <<","<< tlv_l1.Px() <<","<< tlv_l1.Py() <<","<< tlv_l1.Pz() <<","<< tlv_l1.M() << std::endl;
            std::cout<<"ERANGE Tau2"<<std::endl;
            std::cout<<"ERANGE (E,Px,Py,Pz,M) "<< tlv_l2.E() <<","<< tlv_l2.Px() <<","<< tlv_l2.Py() <<","<< tlv_l2.Pz() <<","<< tlv_l2.M() << std::endl;
            std::cout<<"ERANGE B1"<<std::endl;
            std::cout<<"ERANGE (E,Px,Py,Pz,M) "<< tlv_b1.E() <<","<< tlv_b1.Px() <<","<< tlv_b1.Py() <<","<< tlv_b1.Pz() <<","<< tlv_b1.M() << std::endl;
            std::cout<<"ERANGE B2"<<std::endl;
            std::cout<<"ERANGE (E,Px,Py,Pz,M) "<< tlv_b2.E() <<","<< tlv_b2.Px() <<","<< tlv_b2.Py() <<","<< tlv_b2.Pz() <<","<< tlv_b2.M() << std::endl;
            std::cout<<"ERANGE MET"<<std::endl;
            std::cout<<"ERANGE (E,Px,Py,Pz,M) "<<","<< ptmiss.Px() <<","<< ptmiss.Py() <<std::endl;
            std::cout<<"ERANGE METCOV "<<std::endl;
            std::cout<<"ERANGE "<< metcov (0,0) <<"  "<< metcov (0,1) << std::endl;
            std::cout<<"ERANGE "<< metcov (1,0) <<"  "<< metcov (1,1) << std::endl;
            std::cout<<"ERANGE tau1, tau2, b1, b2"<<std::endl;
            std::cout<<"ERANGE ";
            tlv_l1.Print();
            std::cout<<"ERANGE ";
            tlv_l2.Print();
            std::cout<<"ERANGE ";
            tlv_b1.Print();
            std::cout<<"ERANGE ";
            tlv_b2.Print();
        }
        wrongFit=true;
    }
    catch (HHKinFit2::HHEnergyConstraintException const& e) {
        std::cout<<"THIS FIT DID NOT CONVERGE: ENERGY CONSTRAIN EXCEPTION (mh1_hp = " << mh1_hp << " , mh2_hp = " << mh2_hp << ")" << std::endl;
        if (DEBUG) {
            std::cout<<"ECON Tau1"<<std::endl;
            std::cout<<"ECON (E,Px,Py,Pz,M) "<< tlv_l1.E() <<","<< tlv_l1.Px() <<","<< tlv_l1.Py() <<","<< tlv_l1.Pz() <<","<< tlv_l1.M() << std::endl;
            std::cout<<"ECON Tau2"<<std::endl;
            std::cout<<"ECON (E,Px,Py,Pz,M) "<< tlv_l2.E() <<","<< tlv_l2.Px() <<","<< tlv_l2.Py() <<","<< tlv_l2.Pz() <<","<< tlv_l2.M() << std::endl;
            std::cout<<"ECON B1"<<std::endl;
            std::cout<<"ECON (E,Px,Py,Pz,M) "<< tlv_b1.E() <<","<< tlv_b1.Px() <<","<< tlv_b1.Py() <<","<< tlv_b1.Pz() <<","<< tlv_b1.M() << std::endl;
            std::cout<<"ECON B2"<<std::endl;
            std::cout<<"ECON (E,Px,Py,Pz,M) "<< tlv_b2.E() <<","<< tlv_b2.Px() <<","<< tlv_b2.Py() <<","<< tlv_b2.Pz() <<","<< tlv_b2.M() << std::endl;
            std::cout<<"ECON MET"<<std::endl;
            std::cout<<"ECON (E,Px,Py,Pz,M) "<<","<< ptmiss.Px() <<","<< ptmiss.Py() <<std::endl;
            std::cout<<"ECON METCOV "<<std::endl;
            std::cout<<"ECON "<< metcov (0,0) <<"  "<< metcov (0,1) << std::endl;
            std::cout<<"ECON "<< metcov (1,0) <<"  "<< metcov (1,1) << std::endl;
            std::cout<<"ECON tau1, tau2, b1, b2"<<std::endl;
            std::cout<<"ECON ";
            tlv_l1.Print();
            std::cout<<"ECON ";
            tlv_l2.Print();
            std::cout<<"ECON ";
            tlv_b1.Print();
            std::cout<<"ECON ";
            tlv_b2.Print();
        }
        wrongFit=true;
    }
    if(!wrongFit) {
        localChi2 = localFit.getChi2(mh1_hp, mh2_hp);
        if (localChi2 < 0 || std::isnan(localChi2)) {
            localChi2 = std::nanf("1");
        }
    }
    else {
        localChi2 = std::nanf("1");
    }

    return std::pair(localFit, localChi2);
}

bool KinFitter::fit(std::string sgnHp) {
    int mh1_hp = 0;
    int mh2_hp = 0;

    if (sgnHp == "HH")
    {
        mh1_hp = H_MASS;
        mh2_hp = H_MASS;
    }
    else if (sgnHp == "ZZ") {
        mh1_hp = Z_MASS;
        mh2_hp = Z_MASS;
    }
    else if (sgnHp == "ZH") {
        mh1_hp = Z_MASS;
        mh2_hp = H_MASS;
    }
    else {
        std::cout << "**ERROR: only HH/ZH/ZZ supported in KinFitter class, " << sgnHp << " entered." << std::endl;
        std::cout << "         defaulting to HH hypothesis!" <<std::endl;
        mh1_hp = H_MASS;
        mh2_hp = H_MASS;
    }

    if (mh1_hp == mh2_hp) {
        std::pair<HHKinFit2::HHKinFitMasterHeavyHiggs,float> res = _fit(mh1_hp, mh2_hp);
        if (std::isnan(res.second)) fitsNotConverging = true;
        KinFit = res.first;
        mh1_hpFit = mh1_hp;
        mh2_hpFit = mh2_hp;
    }
    else {
        std::pair<HHKinFit2::HHKinFitMasterHeavyHiggs,float> right_fit = _fit(mh1_hp, mh2_hp);
        std::pair<HHKinFit2::HHKinFitMasterHeavyHiggs,float> left_fit  = _fit(mh2_hp, mh1_hp);

        if (!std::isnan(right_fit.second) && !std::isnan(left_fit.second )) {
            if (right_fit.second < left_fit.second) {
                KinFit = right_fit.first;
                mh1_hpFit = mh1_hp;
                mh2_hpFit = mh2_hp;
            }
            else {
                KinFit = left_fit.first;
                mh1_hpFit = mh2_hp;
                mh2_hpFit = mh1_hp;
            }
        }
        else if (std::isnan(right_fit.second)) {
            KinFit = left_fit.first;
            mh1_hpFit = mh2_hp;
            mh2_hpFit = mh1_hp;
        }
        else if (std::isnan(left_fit.second)) {
            KinFit = right_fit.first;
            mh1_hpFit = mh1_hp;
            mh2_hpFit = mh2_hp;
        }
        else fitsNotConverging = true;;
    }

    return fitsNotConverging;
}

double KinFitter::getMH() {
    return KinFit.getMH(mh1_hpFit, mh2_hpFit);
}

double KinFitter::getChi2() {
    return KinFit.getChi2(mh1_hpFit, mh2_hpFit);
}

int KinFitter::getConvergence() {
    return KinFit.getConvergence(mh1_hpFit, mh2_hpFit);
}

double KinFitter::getFitProb() {
    return KinFit.getFitProb(mh1_hpFit, mh2_hpFit);
}

TLorentzVector KinFitter::getFittedBJet1() {
    return KinFit.getFittedBJet1(mh1_hpFit, mh2_hpFit);
}

TLorentzVector KinFitter::getFittedBJet2() {
    return KinFit.getFittedBJet2(mh1_hpFit, mh2_hpFit);
}
