/*
 * SingleColumnIndex.h
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_SINGLECOLUMNINDEXACCESS_H_
#define SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_SINGLECOLUMNINDEXACCESS_H_

#include "scan_select/scan_planner/scanner/index_resolv/AbstractColumnsIndexAccess.h"

namespace codablecash {

class IValueProvider;
class CdbTableColumn;
class AbstractScanTableTarget;

class SingleColumnIndexAccess : public AbstractColumnsIndexAccess {
public:
	SingleColumnIndexAccess(const SingleColumnIndexAccess& inst);
	SingleColumnIndexAccess();
	virtual ~SingleColumnIndexAccess();

	const CdbTableColumn* getColumn() const noexcept {
		return column;
	}
	void setColumn(const CdbTableColumn* column) noexcept {
		this->column = column;
	}

	virtual int size() const noexcept;

	virtual AbstractColumnsIndexAccess* clone() const noexcept;

	virtual bool isBottomEq(VirtualMachine* vm) const noexcept;

	void setBottomEq(bool bottomEq) noexcept {
		this->bottomEq = bottomEq;
	}
	virtual bool isTopEq(VirtualMachine* vm) const noexcept;

	void setTopEq(bool topEq) noexcept {
		this->topEq = topEq;
	}

	virtual const IValueProvider* getTopValue(VirtualMachine* vm) const noexcept;

	void setTopValue(const IValueProvider* topValue) noexcept;

	virtual const IValueProvider* getValue(VirtualMachine* vm) const noexcept;

	void setValue(const IValueProvider* value) noexcept;

	virtual bool isRange(VirtualMachine* vm) const noexcept;
	void setRange(bool range, VirtualMachine* vm) noexcept;

	virtual bool hasIndex(SchemaManager* schemaManager, VirtualMachine* vm);

	virtual const UnicodeString* toCodeString(VirtualMachine* vm) noexcept;

	void toRangeAccess() noexcept;

	bool hasPlaceHolder() const noexcept;

private:
	void makeRangeString(VirtualMachine* vm) noexcept;
	void makeString() noexcept;

private:
	const CdbTableColumn* column;

	IValueProvider* topValue;
	bool topEq;

	IValueProvider* value;
	bool bottomEq;

	bool range;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_SCANNER_INDEX_RESOLV_SINGLECOLUMNINDEXACCESS_H_ */
