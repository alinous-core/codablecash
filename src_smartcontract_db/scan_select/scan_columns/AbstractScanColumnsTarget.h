/*
 * AbstractScanColumns.h
 *
 *  Created on: 2020/08/19
 *      Author: iizuka
 */

#ifndef SCAN_COLUMNS_ABSTRACTSCANCOLUMNSTARGET_H_
#define SCAN_COLUMNS_ABSTRACTSCANCOLUMNSTARGET_H_

#include "base/ArrayList.h"

namespace alinous {
class UnicodeString;
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class SelectScanPlanner;
class CdbRecord;
class ScanResultMetadata;
class IValueProvider;

class AbstractScanColumnsTarget {
public:
	AbstractScanColumnsTarget();
	virtual ~AbstractScanColumnsTarget();

	void setAsName(const UnicodeString* asName) noexcept;

	virtual const UnicodeString* toStringCode() noexcept = 0;

	virtual void resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) = 0;

	virtual void scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) = 0;
	virtual void buildNameList(ArrayList<UnicodeString>* list, const ScanResultMetadata* metadata) const noexcept;

	virtual IValueProvider* toValueProvider() = 0;

protected:
	void resetStr() noexcept;

protected:
	UnicodeString* asName;

	UnicodeString* str;
};

} /* namespace codablecash */

#endif /* SCAN_COLUMNS_ABSTRACTSCANCOLUMNSTARGET_H_ */
