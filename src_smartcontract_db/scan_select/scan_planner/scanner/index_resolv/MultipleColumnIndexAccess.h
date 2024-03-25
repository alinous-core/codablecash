/*
 * MultipleColumnIndex.h
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_MULTIPLECOLUMNINDEXACCESS_H_
#define SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_MULTIPLECOLUMNINDEXACCESS_H_

#include "base/ArrayList.h"
#include "scan_select/scan_planner/scanner/index_resolv/AbstractColumnsIndexAccess.h"

using alinous::ArrayList;

namespace codablecash {

class SingleColumnIndexAccess;
class MultipleValueProvidorScanParam;
class CdbOid;

class MultipleColumnIndexAccess : public AbstractColumnsIndexAccess {
public:
	MultipleColumnIndexAccess(const MultipleColumnIndexAccess& inst);
	MultipleColumnIndexAccess();
	virtual ~MultipleColumnIndexAccess();

	virtual bool hasIndex(SchemaManager* schemaManager, VirtualMachine* vm);

	virtual int size() const noexcept;

	virtual bool isRange(VirtualMachine* vm) const noexcept;
	virtual bool isBottomEq(VirtualMachine* vm) const noexcept;
	virtual bool isTopEq(VirtualMachine* vm) const noexcept;
	virtual const IValueProvider* getTopValue(VirtualMachine* vm) const noexcept;
	virtual const IValueProvider* getValue(VirtualMachine* vm) const noexcept;

	virtual AbstractColumnsIndexAccess* clone() const noexcept;

	virtual const UnicodeString* toCodeString(VirtualMachine* vm) noexcept;

	void add(SingleColumnIndexAccess* idx) noexcept;

private:
	void makeLongerIndexAccess(VirtualMachine* vm);
	void makeIndexAccess(VirtualMachine* vm);
	void makeRange(VirtualMachine* vm);

	SingleColumnIndexAccess* getIndexAccess(const CdbOid* colId) const noexcept;

	void makeCodeString(VirtualMachine* vm) noexcept;

private:
	ArrayList<SingleColumnIndexAccess> list;

	mutable MultipleValueProvidorScanParam* value;
	mutable MultipleValueProvidorScanParam* topValue;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_MULTIPLECOLUMNINDEXACCESS_H_ */
