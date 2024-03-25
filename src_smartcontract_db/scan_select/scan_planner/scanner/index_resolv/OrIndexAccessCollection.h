/*
 * OrIndexWrapperCollection.h
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_ORINDEXACCESSCOLLECTION_H_
#define SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_ORINDEXACCESSCOLLECTION_H_

#include "base/ArrayList.h"
#include "scan_select/scan_planner/scanner/index_resolv/AbstractColumnsIndexAccess.h"

using alinous::ArrayList;

namespace codablecash {

class OrIndexAccessCollection : public AbstractColumnsIndexAccess {
public:
	OrIndexAccessCollection(const OrIndexAccessCollection& inst);
	OrIndexAccessCollection();
	virtual ~OrIndexAccessCollection();

	virtual bool isOr() const noexcept {
		return true;
	}
	virtual int size() const noexcept;
	virtual bool hasIndex(SchemaManager* schemaManager, VirtualMachine* vm);

	virtual AbstractColumnsIndexAccess* clone() const noexcept;

	virtual const UnicodeString* toCodeString(VirtualMachine* vm) noexcept;

	void add(AbstractColumnsIndexAccess* index) noexcept;
	AbstractColumnsIndexAccess* get(int i) const noexcept;

private:
	void makeCodeString(VirtualMachine* vm) noexcept;

private:
	ArrayList<AbstractColumnsIndexAccess> list;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_ORINDEXACCESSCOLLECTION_H_ */
