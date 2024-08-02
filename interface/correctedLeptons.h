#ifndef correctedLeptons_h
#define correctedLeptons_h

#include <vector>
#include "bigTree.h"

using vecf = std::vector<float>;

class correctedLeptons { 
public:
  correctedLeptons(const unsigned, bigTree*);
  std::pair<float, float> correctMET(float, float);
  
  float px(unsigned j) { return mPx[j]; };
  float py(unsigned j) { return mPy[j]; };
  float pz(unsigned j) { return mPz[j]; };
  float en(unsigned j) { return mEn[j]; };

  float metx() { return mMETx; };
  float mety() { return mMETy; };
  
 private:
  unsigned mSize = 0;
  bigTree* mBigTree;
  
  vecf mPx;
  vecf mPy;
  vecf mPz;
  vecf mEn;
  float mMETx;
  float mMETy;
  
  void mCorrect4Vector();
};

#endif
