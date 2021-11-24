/*
 * TTreeFormulaGroup.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: kaess
 */

#include "TTreeFormulaGroup.h"
#include <TIterator.h>

TTreeFormulaGroup::TTreeFormulaGroup(bool setOwner) {
  tlist = new TList();
  tlist->SetOwner(setOwner);
}

bool TTreeFormulaGroup::Notify(void) {

  bool success=true;

  TIter ti =TIter(tlist);
  TObject * temp;

  while((temp=ti.Next())) {
    ((TTreeFormula *)temp)->UpdateFormulaLeaves();
    success=temp->Notify()&&success;
  }

  return success;
}

bool TTreeFormulaGroup::SetNotify(TTreeFormula * ttf) {

  Int_t n = tlist->GetEntries();
  tlist->Add(ttf);

  return (tlist->GetEntries()==n+1);
}

bool TTreeFormulaGroup::UnsetNotify(TTreeFormula *ttf) {

  Int_t n = tlist->GetEntries();
  while(tlist->Remove(ttf));//Removes all copies of ttf

  return (tlist->GetEntries()<n);
}

TTreeFormulaGroup::~TTreeFormulaGroup() {
  delete tlist;
}
