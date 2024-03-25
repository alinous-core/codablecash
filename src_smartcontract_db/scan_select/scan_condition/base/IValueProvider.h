/*
 * IValueProvider.h
 *
 *  Created on: 2020/07/21
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_IVALUEPROVIDER_H_
#define SCAN_CONDITION_IVALUEPROVIDER_H_

namespace alinous {
class VirtualMachine;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class SelectScanPlanner;
class FilterConditionDitector;
class AbstractCdbValue;
class CdbRecord;
class ScanResultMetadata;
class NullScanParam;
class InfinityScanParam;

class IValueProvider {
public:
	static const NullScanParam NULLPARAM;
	static const InfinityScanParam INFINITY_PARAM;

	IValueProvider();
	virtual ~IValueProvider();

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) = 0;
	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) = 0;
	virtual bool isAvailable(VirtualMachine* vm) const = 0;
	virtual bool hasPlaceholder() const noexcept = 0;

	virtual IValueProvider* clone() const noexcept = 0;
	virtual bool isColumn() const noexcept;
	virtual bool hasColumnId() const noexcept;
	virtual bool isRecordIndependent() const noexcept = 0;
	virtual bool isFilterable(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept = 0;

	virtual const UnicodeString* toStringCode() noexcept = 0;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_IVALUEPROVIDER_H_ */
