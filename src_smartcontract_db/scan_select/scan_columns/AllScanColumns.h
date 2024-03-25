/*
 * AllScanColumns.h
 *
 *  Created on: 2020/08/19
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_ALLSCANCOLUMNS_H_
#define SCAN_COLUMNS_ALLSCANCOLUMNS_H_

#include "scan_select/scan_columns/AbstractScanColumnsTarget.h"

namespace codablecash {

class AllScanColumns : public AbstractScanColumnsTarget {
public:
	AllScanColumns();
	virtual ~AllScanColumns();

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);

	virtual void buildNameList(ArrayList<UnicodeString>* list, const ScanResultMetadata* metadata) const noexcept;
	virtual IValueProvider* toValueProvider();
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_ALLSCANCOLUMNS_H_ */
