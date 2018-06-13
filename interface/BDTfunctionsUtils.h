#ifndef BDTfunctionsUtils_h
#define BDTfunctionsUtils_h

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <cmath>

#include <Math/PtEtaPhiE4D.h>
#include <Math/PtEtaPhiM4D.h>
#include <Math/PxPyPzE4D.h>
#include <Math/LorentzVector.h>
#include <Math/SMatrix.h>
#include <Math/VectorUtil.h>
#include <TVector3.h>
#include <TH1.h>
#include <TH2.h>
#include <TMatrixD.h>
#include <TLorentzVector.h>



// -----------------------------------------
// Transform a TLorentzVector to a LorentzVector
using LorentzVectorXYZ = ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double>>;

LorentzVectorXYZ getLVfromTLV (TLorentzVector tlv)
{
    LorentzVectorXYZ lv (tlv.Px(), tlv.Py(), tlv.Pz(),  tlv.E());
    return lv;
}


// -----------------------------------------
// Various functions for BDT variables

template<typename LVector1, typename LVector2, typename LVector3>
double Calculate_Pzeta(const LVector1& l1_p4, const LVector2& l2_p4, const LVector3& met_p4)
{
    const auto ll_p4 = l1_p4 + l2_p4;
    const TVector2 ll_p2(ll_p4.Px(), ll_p4.Py());
    const TVector2 met_p2(met_p4.Px(), met_p4.Py());
    const TVector2 ll_s = ll_p2 + met_p2;
    const TVector2 l1_u(std::cos(l1_p4.Phi()), std::sin(l1_p4.Phi()));
    const TVector2 l2_u(std::cos(l2_p4.Phi()), std::sin(l2_p4.Phi()));
    const TVector2 ll_u = l1_u + l2_u;
    const double ll_u_met = ll_s * ll_u;
    const double ll_mod = ll_u.Mod();
    return ll_u_met / ll_mod;
}


template<typename LVector1, typename LVector2>
double Calculate_visiblePzeta(const LVector1& l1_p4, const LVector2& l2_p4)
{
    const auto ll_p4 = l1_p4 + l2_p4;
    const TVector2 ll_p2(ll_p4.Px(), ll_p4.Py());
    const TVector2 l1_u(std::cos(l1_p4.Phi()), std::sin(l1_p4.Phi()));
    const TVector2 l2_u(std::cos(l2_p4.Phi()), std::sin(l2_p4.Phi()));
    const TVector2 ll_u = l1_u + l2_u;
    const double ll_p2u = ll_p2 * ll_u;
    const double ll_mod = ll_u.Mod();
    return ll_p2u / ll_mod;
}


template<typename LVector1, typename LVector2>
double Calculate_MT(const LVector1& lepton_p4, const LVector2& met_p4)
{
    const double delta_phi = TVector2::Phi_mpi_pi(lepton_p4.Phi() - met_p4.Phi());
    return std::sqrt( 2.0 * lepton_p4.Pt() * met_p4.Pt() * ( 1.0 - std::cos(delta_phi) ) );
}


template<typename LVector1, typename LVector2, typename LVector3>
double Calculate_TotalMT(const LVector1& lepton1_p4, const LVector2& lepton2_p4, const LVector3& met_p4)
{
    const double mt_1 = Calculate_MT(lepton1_p4, met_p4);
    const double mt_2 = Calculate_MT(lepton2_p4, met_p4);
    const double mt_ll = Calculate_MT(lepton1_p4, lepton2_p4);
    return std::sqrt(std::pow(mt_1, 2) + std::pow(mt_2, 2) + std::pow(mt_ll, 2));
}


template<typename LVector1, typename LVector2, typename LVector3, typename LVector4, typename LVector5 >
std::pair<double, double> Calculate_topPairMasses(const LVector1& lepton1_p4, const LVector2& lepton2_p4, const LVector3& bjet_1, const LVector4& bjet_2, const LVector5& met_p4){
    static constexpr double mass_top = 172.5;
    std::vector<std::pair<double, double>> vector_mass_top = {
        { (lepton1_p4 + bjet_1 + met_p4).mass(), (lepton2_p4 + bjet_2).mass() },
        { (lepton1_p4 + bjet_1).mass(), (lepton2_p4 + bjet_2 + met_p4).mass() },
        { (lepton1_p4 + bjet_2 + met_p4).mass(), (lepton2_p4 + bjet_1).mass() },
        { (lepton1_p4 + bjet_2).mass(), (lepton2_p4 + bjet_1 + met_p4).mass() }
    };
    std::vector<std::pair<size_t, double>> distance;
    for (size_t i = 0; i < vector_mass_top.size(); ++i) {
        distance.emplace_back(i, pow(vector_mass_top[i].first - mass_top,2)
                              + pow(vector_mass_top[i].second - mass_top,2));
    }
    std::sort(distance.begin(), distance.end(), [](const std::pair<size_t, double>& el1,const std::pair<size_t, double>& el2){
        return el1.second < el2.second;
    });
    return vector_mass_top.at(distance.front().first);
}


template<typename LVector1, typename LVector2, typename LVector3, typename LVector4, typename LVector5>
double Calculate_MX(const LVector1& lepton1, const LVector2& lepton2, const LVector3& bjet1, const LVector4& bjet2, const LVector5& met){

    static constexpr double shift = 250.;
    auto mass_4 = (lepton1 + lepton2 + met + bjet1 + bjet2).M();
    auto mass_ll =  (lepton1 + lepton2 +  met).M();
    auto mass_bb =  (bjet1 +  bjet2).M();
    return mass_4 - mass_ll - mass_bb + shift;
}


template<typename LVector1, typename LVector2, typename LVector3, typename LVector4, typename LVector5,  typename LVector6>
double Calculate_phi(const LVector1& lepton1, const LVector2& lepton2, const LVector3& bjet1, const LVector4& bjet2, const LVector5& ll, const LVector6& bb){
    const auto H = bb + ll;
    const auto boosted_l1 = ROOT::Math::VectorUtil::boost(lepton1, H.BoostToCM());
    const auto boosted_l2 = ROOT::Math::VectorUtil::boost(lepton2, H.BoostToCM());
    const auto boosted_j1 = ROOT::Math::VectorUtil::boost(bjet1, H.BoostToCM());
    const auto boosted_j2 = ROOT::Math::VectorUtil::boost(bjet2, H.BoostToCM());
    const auto n1 = boosted_l1.Vect().Cross(boosted_l2.Vect());
    const auto n2 = boosted_j1.Vect().Cross(boosted_j2.Vect());
    return ROOT::Math::VectorUtil::Angle(n1, n2);
}


template<typename LVector1, typename LVector2, typename LVector3, typename LVector4>
double Calculate_phi1(const LVector1& object1, const LVector2& object2, const LVector3& ll, const LVector4& bb){
    const auto H = bb + ll;
    const auto boosted_1 = ROOT::Math::VectorUtil::boost(object1, H.BoostToCM());
    const auto boosted_2 = ROOT::Math::VectorUtil::boost(object2, H.BoostToCM());
    const auto boosted_h = ROOT::Math::VectorUtil::boost(ll, H.BoostToCM());
    ROOT::Math::Cartesian3D<> z_axis(0, 0, 1);
    const auto n1 = boosted_1.Vect().Cross(boosted_2.Vect());
    const auto n3 = boosted_h.Vect().Cross(z_axis);
    return ROOT::Math::VectorUtil::Angle(n1,n3);
}


template<typename LVector1, typename LVector2>
double Calculate_cosTheta_2bodies(const LVector1& object1, const LVector2&  hh){
    const auto boosted_object1 = ROOT::Math::VectorUtil::boost(object1, hh.BoostToCM());
    return  ROOT::Math::VectorUtil::CosTheta(boosted_object1, hh);
}

#endif
