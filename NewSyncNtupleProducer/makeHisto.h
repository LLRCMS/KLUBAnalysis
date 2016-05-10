/*
 * File:   jetVeto.h
 * Author: abdollah
 *
 * Created on July 21, 2010, 3:39 PM
 */

#ifndef MAKEHISTO_H
#define	MAKEHISTO_H




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
//#include "myevent.h"
//#include "LinkDef.h"
#include "TMath.h" //M_PI is in TMath
#include "TRandom3.h"
using namespace std;

//****************************************************
map<string, TH1F*>* myMap1;
map<string, TH2F*>* myMap2;
//**********************************************

TH1F* nplot1(string name) {
    if (myMap1->find(name) != myMap1->end())
        return (*myMap1)[name];
    else
        return 0;
}

TH2F* nplot2(string name) {
    if (myMap2->find(name) != myMap2->end())
        return (*myMap2)[name];
    else
        return 0;
}
//****************************************************

void plotFill(string name, float x, int nx, float nxmin, float nxmax, double weight=1) {
    if (myMap1->find(name) == myMap1->end())
        (*myMap1)[name] = new TH1F(name.c_str(), name.c_str(), nx, nxmin, nxmax);
    (*myMap1)[name]->Fill(x,weight);
}

void plotFill(string name, float x, float y, int nx, float nxmin, float nxmax, int ny, float nymin, float nymax, double weight=1) {
    if (myMap2->find(name) == myMap2->end())
        (*myMap2)[name] = new TH2F(name.c_str(), name.c_str(), nx, nxmin, nxmax, ny, nymin, nymax);
    (*myMap2)[name]->Fill(x, y,weight);
}
//****************************************************
//Transverse Mass
//double TMass(double et1,double et2, double px1, double px2, double py1, double py2){
//    return sqrt(pow(et1+et2,2)-pow(px1+px2,2)-pow(py1+py2,2));
//};
//M_T_tauMet = sqrt(((ittau->et) + (Met.front().et))*((ittau->et) + (Met.front().et)) -
//        (ittau->px + Met.front().px)*(ittau->px + Met.front().px) -
//        (ittau->py + Met.front().py)*(ittau->py + Met.front().py));

#endif	/* _JETVETO_H */

