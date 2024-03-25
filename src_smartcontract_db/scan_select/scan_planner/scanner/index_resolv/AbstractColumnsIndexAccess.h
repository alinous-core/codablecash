/*
 * AbstractColumnsIndexWrapper.h
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_ABSTRACTCOLUMNSINDEXACCESS_H_
#define SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_ABSTRACTCOLUMNSINDEXACCESS_H_

namespace alinous {
class UnicodeString;
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class SchemaManager;
class CdbTableColumn;
class AbstractScanTableTarget;
class CdbTableIndex;
class IValueProvider;

class AbstractColumnsIndexAccess {
public:
	AbstractColumnsIndexAccess(const AbstractColumnsIndexAccess& inst);
	AbstractColumnsIndexAccess();
	virtual ~AbstractColumnsIndexAccess();

	virtual bool isOr() const noexcept {
		return false;
	}
	virtual bool isRange(VirtualMachine* vm) const noexcept {
		return false;
	}
	virtual bool isBottomEq(VirtualMachine* vm) const noexcept{return false;}
	virtual bool isTopEq(VirtualMachine* vm) const noexcept{ return false; }
	virtual const IValueProvider* getTopValue(VirtualMachine* vm) const noexcept { return nullptr; }
	virtual const IValueProvider* getValue(VirtualMachine* vm) const noexcept { return nullptr; }

	virtual int size() const noexcept = 0;

	virtual bool hasIndex(SchemaManager* schemaManager, VirtualMachine* vm) = 0;

	virtual AbstractColumnsIndexAccess* clone() const noexcept = 0;

	virtual const UnicodeString* toCodeString(VirtualMachine* vm) noexcept = 0;

	const AbstractScanTableTarget* getTarget() const {
		return target;
	}

	void setTarget(const AbstractScanTableTarget* target) {
		this->target = target;
	}

	CdbTableIndex* getIndex() const noexcept {
		return index;
	}

protected:
	void resetStr() noexcept;

protected:
	const AbstractScanTableTarget* target;
	CdbTableIndex* index;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_ABSTRACTCOLUMNSINDEXACCESS_H_ */
