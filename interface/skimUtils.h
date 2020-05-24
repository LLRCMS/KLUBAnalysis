#ifndef skimUtils_h
#define skimUtils_h

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

#include "bigTree.h"

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
float getCvsL (bigTree &theBigTree, int idx)
{
    float prob_c   = theBigTree.bDeepFlavor_probc->at(idx);
    float prob_uds = theBigTree.bDeepFlavor_probuds->at(idx);
    float prob_g   = theBigTree.bDeepFlavor_probg->at(idx);

    return prob_c / (prob_c + prob_uds + prob_g);
}

// -----------------------------------------
float getCvsB (bigTree &theBigTree, int idx)
{
    float prob_c    = theBigTree.bDeepFlavor_probc   ->at(idx);
    float prob_b    = theBigTree.bDeepFlavor_probb   ->at(idx);
    float prob_bb   = theBigTree.bDeepFlavor_probbb  ->at(idx);
    float prob_lepb = theBigTree.bDeepFlavor_problepb->at(idx);

    return prob_c / (prob_c + prob_b + prob_bb + prob_lepb);
}


#endif
