/*
 * MET phi modulation correction for MC and Data, Run 2 UL.
 *
 * Info: https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETRun2Corrections#xy_Shift_Correction_MET_phi_modu
 * Adapted from: https://lathomas.web.cern.ch/lathomas/METStuff/XYCorrections/XYMETCorrection_withUL17andUL18andUL16.h
 */

#include <iostream>
#include <cmath>

enum PhiCorrRunEra {
    yNONE,
    // data
    yUL2016B,
    yUL2016C,
    yUL2016D,
    yUL2016E,
    yUL2016F,
    yUL2016Flate,
    yUL2016G,
    yUL2016H,
    yUL2017B,
    yUL2017C,
    yUL2017D,
    yUL2017E,
    yUL2017F,
    yUL2018A,
    yUL2018B,
    yUL2018C,
    yUL2018D,
    // mc
    yUL2016MCAPV,
    yUL2016MCnonAPV,
    yUL2017MC,
    yUL2018MC
};

std::pair<double, double> met_phi_correction_pxpy(
    double met_px, double met_py, int npv, int runnb, const std::string& period, bool isMC
) {
    // limit npv
    if (npv > 100) {
        npv = 100;
    }

    // determine the run era
    PhiCorrRunEra runera = yNONE;
    if (isMC) {
        if (period == "2016preVFP") {
            runera = yUL2016MCAPV;
        } else if (period == "2016postVFP") {
            runera = yUL2016MCnonAPV;
        } else if (period == "2017") {
            runera = yUL2017MC;
        } else if (period == "2018") {
            runera = yUL2018MC;
        }
    } else {
        if (runnb >= 315252 && runnb <= 316995) {
            runera = yUL2018A;
        } else if (runnb >= 316998 && runnb <= 319312) {
            runera = yUL2018B;
        } else if (runnb >= 319313 && runnb <= 320393) {
            runera = yUL2018C;
        } else if (runnb >= 320394 && runnb <= 325273) {
            runera = yUL2018D;
        } else if (runnb >= 297020 && runnb <= 299329) {
            runera = yUL2017B;
        } else if (runnb >= 299337 && runnb <= 302029) {
            runera = yUL2017C;
        } else if (runnb >= 302030 && runnb <= 303434) {
            runera = yUL2017D;
        } else if (runnb >= 303435 && runnb <= 304826) {
            runera = yUL2017E;
        } else if (runnb >= 304911 && runnb <= 306462) {
            runera = yUL2017F;
        } else if (runnb >= 272007 && runnb <= 275376) {
            runera = yUL2016B;
        } else if (runnb >= 275657 && runnb <= 276283) {
            runera = yUL2016C;
        } else if (runnb >= 276315 && runnb <= 276811) {
            runera = yUL2016D;
        } else if (runnb >= 276831 && runnb <= 277420) {
            runera = yUL2016E;
        } else if (((runnb >= 277772 && runnb <= 278768) || runnb == 278770)) {
            runera = yUL2016F;
        } else if (((runnb >= 278801 && runnb <= 278808) || runnb == 278769)) {
            runera = yUL2016Flate;
        } else if (runnb >= 278820 && runnb <= 280385) {
            runera = yUL2016G;
        } else if (runnb >= 280919 && runnb <= 284044) {
            runera = yUL2016H;
        }
    }
    if (runera == yNONE) {
        throw std::runtime_error("Unknown MC period and/or data run number");
    }

    // determine parameters of the linear correction function
    double met_px_diff = 0.0;
    double met_py_diff = 0.0;
    if (runera == yUL2017B) {
        met_px_diff = -(-0.211161 * npv + 0.419333);
        met_py_diff = -(0.251789 * npv + -1.28089);
    } else if (runera == yUL2017C) {
        met_px_diff = -(-0.185184 * npv + -0.164009);
        met_py_diff = -(0.200941 * npv + -0.56853);
    } else if (runera == yUL2017D) {
        met_px_diff = -(-0.201606 * npv + 0.426502);
        met_py_diff = -(0.188208 * npv + -0.58313);
    } else if (runera == yUL2017E) {
        met_px_diff = -(-0.162472 * npv + 0.176329);
        met_py_diff = -(0.138076 * npv + -0.250239);
    } else if (runera == yUL2017F) {
        met_px_diff = -(-0.210639 * npv + 0.72934);
        met_py_diff = -(0.198626 * npv + 1.028);
    } else if (runera == yUL2017MC) {
        met_px_diff = -(-0.300155 * npv + 1.90608);
        met_py_diff = -(0.300213 * npv + -2.02232);
    } else if (runera == yUL2018A) {
        met_px_diff = -(0.263733 * npv + -1.91115);
        met_py_diff = -(0.0431304 * npv + -0.112043);
    } else if (runera == yUL2018B) {
        met_px_diff = -(0.400466 * npv + -3.05914);
        met_py_diff = -(0.146125 * npv + -0.533233);
    } else if (runera == yUL2018C) {
        met_px_diff = -(0.430911 * npv + -1.42865);
        met_py_diff = -(0.0620083 * npv + -1.46021);
    } else if (runera == yUL2018D) {
        met_px_diff = -(0.457327 * npv + -1.56856);
        met_py_diff = -(0.0684071 * npv + -0.928372);
    } else if (runera == yUL2018MC) {
        met_px_diff = -(0.183518 * npv + 0.546754);
        met_py_diff = -(0.192263 * npv + -0.42121);
    } else if (runera == yUL2016B) {
        met_px_diff = -(-0.0214894 * npv + -0.188255);
        met_py_diff = -(0.0876624 * npv + 0.812885);
    } else if (runera == yUL2016C) {
        met_px_diff = -(-0.032209 * npv + 0.067288);
        met_py_diff = -(0.113917 * npv + 0.743906);
    } else if (runera == yUL2016D) {
        met_px_diff = -(-0.0293663 * npv + 0.21106);
        met_py_diff = -(0.11331 * npv + 0.815787);
    } else if (runera == yUL2016E) {
        met_px_diff = -(-0.0132046 * npv + 0.20073);
        met_py_diff = -(0.134809 * npv + 0.679068);
    } else if (runera == yUL2016F) {
        met_px_diff = -(-0.0543566 * npv + 0.816597);
        met_py_diff = -(0.114225 * npv + 1.17266);
    } else if (runera == yUL2016Flate) {
        met_px_diff = -(0.134616 * npv + -0.89965);
        met_py_diff = -(0.0397736 * npv + 1.0385);
    } else if (runera == yUL2016G) {
        met_px_diff = -(0.121809 * npv + -0.584893);
        met_py_diff = -(0.0558974 * npv + 0.891234);
    } else if (runera == yUL2016H) {
        met_px_diff = -(0.0868828 * npv + -0.703489);
        met_py_diff = -(0.0888774 * npv + 0.902632);
    } else if (runera == yUL2016MCnonAPV) {
        met_px_diff = -(-0.153497 * npv + -0.231751);
        met_py_diff = -(0.00731978 * npv + 0.243323);
    } else if (runera == yUL2016MCAPV) {
        met_px_diff = -(-0.188743 * npv + 0.136539);
        met_py_diff = -(0.0127927 * npv + 0.117747);
    }

    return std::make_pair(met_px + met_px_diff, met_py + met_py_diff);
}

std::pair<double, double> met_phi_correction_ptphi(
    double met_et, double met_phi, int npv, int runnb, const std::string& year, bool isMC
) {
    // apply the correction
    std::pair<double, double> pxpy_corr = met_phi_correction_pxpy(
        met_et * cos(met_phi),
        met_et * sin(met_phi),
        npv, runnb, year, isMC
    );

    // convert back to pt and phi
    double met_pt_correced = sqrt(pxpy_corr.first * pxpy_corr.first + pxpy_corr.second * pxpy_corr.second);
    double met_phi_corrected = atan2(pxpy_corr.second, pxpy_corr.first);

    return std::make_pair(met_pt_correced, met_phi_corrected);
}
