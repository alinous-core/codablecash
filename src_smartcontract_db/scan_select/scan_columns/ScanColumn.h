/*
 * ScanColumn.h
 *
 *  Created on: 2020/08/19
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_SCANCOLUMN_H_
#define SCAN_COLUMNS_SCANCOLUMN_H_

#include "scan_select/scan_columns/AbstractScanColumnsTarget.h"

namespace alinous {
class SQLColumnIdentifier;
}
using namespace alinous;

namespace codablecash {

class ColumnIdentifierScanParam;

class ScanColumn : public AbstractScanColumnsTarget {
public:
	explicit ScanColumn(const SQLColumnIdentifier* sqlColumnId);
	virtual ~ScanColumn();

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner);

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord);

	const ColumnIdentifierScanParam* getScanParam() const noexcept {
		return scanParam;
	}

	bool isDistinct() const noexcept {
		return this->distinct;
	}

	void setDistinct(bool distinct) noexcept {
		this->distinct = distinct;
	}

	virtual IValueProvider* toValueProvider();

private:
	const SQLColumnIdentifier* sqlColumnId;
	bool distinct;

	ColumnIdentifierScanParam* scanParam;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_SCANCOLUMN_H_ */
