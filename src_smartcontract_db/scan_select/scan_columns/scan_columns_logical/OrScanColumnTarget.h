/*
 * OrlScanColumnTarget.h
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_LOGICAL_ORSCANCOLUMNTARGET_H_
#define SCAN_COLUMNS_LOGICAL_ORSCANCOLUMNTARGET_H_

#include "scan_select/scan_columns/scan_columns_logical/AbstractLogicalScanColumnTarget.h"

#include "base/ArrayList.h"

namespace codablecash {

class OrScanCondition;

class OrScanColumnTarget: public AbstractLogicalScanColumnTarget {
public:
	OrScanColumnTarget();
	virtual ~OrScanColumnTarget();

	void addCondition(AbstractScanColumnsTarget* cond) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);
	virtual IValueProvider* toValueProvider();

private:
	ArrayList<AbstractScanColumnsTarget> list;

	OrScanCondition* cond;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_LOGICAL_ORSCANCOLUMNTARGET_H_ */
