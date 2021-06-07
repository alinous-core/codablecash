/*
 * AndLogicalScanColumn.h
 *
 *  Created on: 2020/08/20
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_LOGICAL_ANDSCANCOLUMNTARGET_H_
#define SCAN_COLUMNS_LOGICAL_ANDSCANCOLUMNTARGET_H_

#include "scan_select/scan_columns/scan_columns_logical/AbstractLogicalScanColumnTarget.h"

#include "base/ArrayList.h"


using namespace alinous;

namespace codablecash {

class AndScanColumnTarget : public AbstractLogicalScanColumnTarget {
public:
	AndScanColumnTarget();
	virtual ~AndScanColumnTarget();

	void addCondition(AbstractScanColumnsTarget* cond) noexcept;

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);
private:
	ArrayList<AbstractScanColumnsTarget> list;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_LOGICAL_ANDSCANCOLUMNTARGET_H_ */
