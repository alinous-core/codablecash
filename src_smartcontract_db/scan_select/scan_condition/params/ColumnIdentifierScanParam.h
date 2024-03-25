/*
 * ColumnIdentifierColumn.h
 *
 *  Created on: 2020/07/21
 *      Author: iizuka
 */

#ifndef SCAN_CONDITION_PARAMS_COLUMNIDENTIFIERSCANPARAM_H_
#define SCAN_CONDITION_PARAMS_COLUMNIDENTIFIERSCANPARAM_H_

#include "scan_select/scan_condition/params/AbstractScanConditionParameter.h"

namespace alinous {
class SQLColumnIdentifier;
}
using namespace alinous;

namespace codablecash {

class CdbTableColumn;
class ScanTargetNameResolver;
class AbstractScanTableTarget;
class ScanResultFieldMetadata;

class ColumnIdentifierScanParam : public AbstractScanConditionParameter {
public:
	ColumnIdentifierScanParam(const ColumnIdentifierScanParam& inst);
	explicit ColumnIdentifierScanParam(const SQLColumnIdentifier* sqlColId);
	virtual ~ColumnIdentifierScanParam();

	virtual const UnicodeString* toStringCode() noexcept;

	virtual void analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner);
	virtual bool isFilterable(VirtualMachine* vm, SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept;

	virtual IValueProvider* clone() const noexcept;
	virtual bool isColumn() const noexcept;
	virtual bool hasColumnId() const noexcept;
	virtual bool isRecordIndependent() const noexcept;

	const AbstractScanTableTarget* getSourceTarget() const noexcept {
		return sourceTarget;
	}
	const CdbTableColumn* getCdbColumn() const noexcept {
		return cdbColumn;
	}

	bool hasIndex() const noexcept;

	virtual AbstractCdbValue* evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata);

	bool isDistinct() const noexcept {
		return distinct;
	}

	void setDistinct(bool distinct) noexcept {
		this->distinct = distinct;
	}

private:
	bool resolveAlias(const UnicodeString* tableAlias, ScanTargetNameResolver* aliasResolver);

private:
	const SQLColumnIdentifier* sqlColId;

	const AbstractScanTableTarget* sourceTarget;
	const CdbTableColumn* cdbColumn;

	UnicodeString* str;

	const ScanResultFieldMetadata* fieldMetadata;
	bool distinct;
};

} /* namespace codablecash */

#endif /* SCAN_CONDITION_PARAMS_COLUMNIDENTIFIERSCANPARAM_H_ */
