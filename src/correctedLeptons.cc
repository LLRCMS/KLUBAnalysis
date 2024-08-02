#include "correctedLeptons.h"

correctedLeptons::correctedLeptons(const unsigned N, bigTree* theBigTree)
  : mSize(N), mBigTree(theBigTree)
{  
  mPx.reserve(mSize);
  mPy.reserve(mSize);
  mPz.reserve(mSize);
  mEn.reserve(mSize);
  
  mCorrect4Vector();
}

void correctedLeptons::mCorrect4Vector() {
  for (unsigned iLep=0; iLep<mSize; ++iLep)
	{
	  TLorentzVector tlv_corr(mBigTree->daughters_px->at(iLep),
							  mBigTree->daughters_py->at(iLep),
							  mBigTree->daughters_pz->at(iLep),
							  mBigTree->daughters_e->at(iLep));
	  double uncorrEn = tlv_corr.E();
	  if(mBigTree->particleType->at(iLep) == 1) {
		double corr = mBigTree->daughters_ecalTrkEnergyPostCorr->at(iLep);
		if (corr == -999.) {
		  throw std::runtime_error("ERROR: unchanged electron energy scale!");
		}
		tlv_corr *= corr / uncorrEn;
	  }
	  mPx[iLep] = tlv_corr.Px();
	  mPy[iLep] = tlv_corr.Py();
	  mPz[iLep] = tlv_corr.Pz();
	  mEn[iLep] = tlv_corr.E();
	}
}

std::pair<float, float> correctedLeptons::correctMET(float metx, float mety) {
  for (unsigned iLep=0; iLep<mSize; ++iLep)
	{
	  // remove the original, old four-vector
	  metx += mBigTree->daughters_px->at(iLep);
	  mety += mBigTree->daughters_py->at(iLep);

	  // add the new four-vector
	  metx -= mPx[iLep];
	  mety -= mPy[iLep];
	}

  mMETx = metx;
  mMETy = mety;
  return std::make_pair(mMETx, mMETy);
}
