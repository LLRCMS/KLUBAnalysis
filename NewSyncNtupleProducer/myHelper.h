#ifndef MYHELPER_H
#define	MYHELPER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
#include <TLorentzVector.h>

float norm_F(float x, float y){
    return sqrt(x*x+y*y);
}

float TMass_F(float pt3lep, float px3lep, float py3lep, float met, float metPhi) {
    return sqrt(pow(pt3lep + met, 2) - pow(px3lep + met * cos(metPhi), 2) - pow(py3lep + met * sin(metPhi), 2));
}

float deltaPhi(float a, float b) {
    float result = a - b;
    while (result > M_PI) result -= 2 * M_PI;
    while (result <= -M_PI) result += 2 * M_PI;
    return fabs(result);
}

float dPhi(TLorentzVector a, TLorentzVector b) {
    float result = a.Phi() - b.Phi();
    while (result > M_PI) result -= 2 * M_PI;
    while (result <= -M_PI) result += 2 * M_PI;
    return fabs(result);
}

float dR(float l1eta, float l1phi, float l2eta, float l2phi) {
    float deta = l1eta - l2eta;
    float dphi = deltaPhi(l1phi, l2phi);
    return sqrt(deta * deta + dphi * dphi);
}

bool Overlap_3(TLorentzVector l1, TLorentzVector l2, TLorentzVector l3){
    if (dR(l1.Eta(),l1.Phi(),l3.Eta(),l3.Phi())<0.5) return true;
    if (dR(l3.Eta(),l3.Phi(),l2.Eta(),l2.Phi())<0.5) return true;
    else return false;
}

bool Overlap_2(TLorentzVector l1, TLorentzVector l2){
    if (dR(l1.Eta(),l1.Phi(),l2.Eta(),l2.Phi())<0.5) return true;
    else return false;
}

bool Overlap_015(TLorentzVector l1, TLorentzVector l2){
    if (dR(l1.Eta(),l1.Phi(),l2.Eta(),l2.Phi())<0.15) return true;
    else return false;
}

bool Overlap_0p3(TLorentzVector l1, TLorentzVector l2){
    if (dR(l1.Eta(),l1.Phi(),l2.Eta(),l2.Phi())<0.30) return true;
    else return false;
}

bool NewOverLap(float l1eta, float l1phi, float l2eta, float l2phi, float l3eta, float l3phi, float l4eta, float l4phi) {
    bool over_12 = dR(l1eta, l1phi, l2eta, l2phi) > 0.3;
    bool over_13 = dR(l1eta, l1phi, l3eta, l3phi) > 0.3;
    bool over_14 = dR(l1eta, l1phi, l4eta, l4phi) > 0.3;
    bool over_23 = dR(l2eta, l2phi, l3eta, l3phi) > 0.3;
    bool over_24 = dR(l2eta, l2phi, l4eta, l4phi) > 0.3;
    bool over_34 = dR(l3eta, l3phi, l4eta, l4phi) > 0.3;

    if (over_12 && over_13 && over_14 && over_23 && over_24 && over_34)
        return true;
    else
        return false;
}

float TMass_F(float et1, float et2, float px1, float px2, float py1, float py2) {
    //std::cout<<"mt "<<pow(et1 + et2, 2)<<" "<<pow(px1 + px2, 2)<<" "<<pow(py1 + py2, 2)<<" "<<pow(et1 + et2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2)<<std::endl;
    return sqrt(pow(et1 + et2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2));

}

double InvarMass_F(float e1, float e2, float px1, float px2, float py1, float py2, float pz1, float pz2) {
    return sqrt(pow(e1 + e2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2) - pow(pz1 + pz2, 2));
}

#endif	/* MYHELPER_H */

