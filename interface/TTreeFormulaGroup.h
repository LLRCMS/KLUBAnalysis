/*
 * TTreeFormulaGroup.h
 *
 *  Created on: Dec 10, 2013
 *      Author: kaess
 */

#ifndef TTREEFORMULAGROUP_H_
#define TTREEFORMULAGROUP_H_

#include <TObject.h>
#include <TTreeFormula.h>
#include <TList.h>

class TTreeFormulaGroup: public TObject {
   ClassDef(TTreeFormulaGroup, 1);
public:
   //Constructor: If setOwner==kTRUE, deletes member TTreeFormulas on delete.
   TTreeFormulaGroup(bool setOwner=kTRUE);

   //Destructor: Self-explanatory.
   virtual ~TTreeFormulaGroup();

   //Notify: Calls UpdateFormulaLeaves on all member TTreeFormulas
   //Also calls Notify for each member TTreeFormula
   //Returns true if every member Notify returns true
   bool Notify(void);

   //SetNotify: Adds passed TTreeFormula to group
   //Returns true if TTreeFormula successfully added.
   bool SetNotify(TTreeFormula *);

   //UnsetNotify: Removes passed TTreeFormula from group.
   //Returns true if TTreeFormula successfully removed.
   //Returns false if it was not a member.
   bool UnsetNotify(TTreeFormula *);

private:
   //Pay no attention to the TList behind the curtain.
   TList * tlist;

};

#endif /* TTREEFORMULAGROUP_H_ */

