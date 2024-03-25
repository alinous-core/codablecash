/*
 * UpdateColumnPolicy.h
 *
 *  Created on: 2022/01/08
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_PLANNER_BASE_UPDATECOLUMNPOLICY_H_
#define SCAN_SELECT_SCAN_PLANNER_BASE_UPDATECOLUMNPOLICY_H_

namespace alinous {
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class AbstractScanConditionElement;
class ScanResultMetadata;
class ScanResultFieldMetadata;
class CdbRecord;

class UpdateColumnPolicy {
public:
	UpdateColumnPolicy();
	virtual ~UpdateColumnPolicy();

	void setFieldMetadata(ScanResultFieldMetadata* fld) noexcept;
	void setValueProvider(AbstractScanConditionElement* valueProvider) noexcept;

	void updateRecord(VirtualMachine* vm, CdbRecord* record, const ScanResultMetadata* metadata);

private:
	AbstractScanConditionElement* valueProvider;
	ScanResultFieldMetadata* fld;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_PLANNER_BASE_UPDATECOLUMNPOLICY_H_ */
