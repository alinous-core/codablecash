/*
 * SingleColumnIndex.cpp
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTable.h"

#include "scan_select/scan_table/TableScanTarget.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "scan_select/scan_condition/params/NullScanParam.h"

#include "scan_select/scan_condition/params/InfinityScanParam.h"
#include "scan_select/scan_planner/scanner/index_resolv/SingleColumnIndexAccess.h"

#include "engine/CdbOid.h"

#include "scan_select/scan_planner/scanner/index_resolv/IndexScorer.h"
namespace codablecash {

SingleColumnIndexAccess::SingleColumnIndexAccess(const SingleColumnIndexAccess& inst) : AbstractColumnsIndexAccess(inst) {
	this->column = inst.column;
	this->topEq = inst.topEq;

	this->topValue = nullptr;
	setTopValue(inst.topValue);
	this->bottomEq = inst.bottomEq;

	this->value = nullptr;
	setValue(inst.value);

	this->range = inst.range;
}

SingleColumnIndexAccess::SingleColumnIndexAccess() {
	this->column = nullptr;
	this->topEq = false;
	this->topValue = nullptr;
	this->bottomEq = false;
	this->value = nullptr;

	this->range = false;
}

SingleColumnIndexAccess::~SingleColumnIndexAccess() {
	this->column = nullptr;
	delete this->topValue;
	delete this->value;
}

int SingleColumnIndexAccess::size() const noexcept {
	return 1;
}

bool SingleColumnIndexAccess::isRange(VirtualMachine* vm) const noexcept {
	if(!this->range && (this->value == nullptr || !this->value->isAvailable(vm))){
		return true;
	}

	return this->range;
}

void SingleColumnIndexAccess::setRange(bool range, VirtualMachine* vm) noexcept {
	this->range = range;

	if(this->value == nullptr && isRange(vm)){
		this->value = new NullScanParam();
	}
	if(this->topValue == nullptr && isRange(vm)){
		this->topValue = new InfinityScanParam();
	}
}

void SingleColumnIndexAccess::setValue(const IValueProvider* value) noexcept {
	delete this->value;
	this->value = value != nullptr ? value->clone() : nullptr;
}

void SingleColumnIndexAccess::setTopValue(const IValueProvider* topValue) noexcept {
	delete this->topValue;
	this->topValue = topValue != nullptr ? topValue->clone() : nullptr;
}

bool SingleColumnIndexAccess::hasIndex(SchemaManager* schemaManager, VirtualMachine* vm) {
	const TableScanTarget* tableTarget = dynamic_cast<const TableScanTarget*>(this->target);
	const CdbTable* cdbTable = tableTarget->getTable();


	IndexScorer scorer;
	scorer.add(this);

	this->index = cdbTable->findMostAvailableIndex(&scorer);

	return this->index != nullptr;
}

const UnicodeString* SingleColumnIndexAccess::toCodeString(VirtualMachine* vm) noexcept {
	if(this->str == nullptr){
		if(isRange(vm)){
			makeRangeString(vm);
		}
		else {
			makeString();
		}
	}

	return str;
}

void SingleColumnIndexAccess::makeRangeString(VirtualMachine* vm) noexcept {
	this->str = new UnicodeString(L"");

	if(this->value != nullptr){
		IValueProvider* vp = const_cast<IValueProvider*>(this->value);
		const UnicodeString* bottomStr = vp->toStringCode();
		this->str->append(bottomStr);

		this->str->append(L" <");
		if(isBottomEq(vm)){
			this->str->append(L"=");
		}
		this->str->append(L" ");
	}

	const UnicodeString* colStr = this->column->getName();
	this->str->append(colStr);

	if(this->topValue != nullptr){
		this->str->append(L" <");
		if(isTopEq(vm)){
			this->str->append(L"=");
		}
		this->str->append(L" ");

		IValueProvider* vp = const_cast<IValueProvider*>(this->topValue);
		const UnicodeString* topStr = vp->toStringCode();
		this->str->append(topStr);
	}
}

const IValueProvider* SingleColumnIndexAccess::getValue(VirtualMachine* vm) const noexcept {
	if(this->value == nullptr || !this->value->isAvailable(vm)){
		return &IValueProvider::NULLPARAM;
	}

	return this->value;
}

bool SingleColumnIndexAccess::isBottomEq(VirtualMachine* vm) const noexcept {
	if(this->value == nullptr || !this->value->isAvailable(vm)){
		return false;
	}
	return this->bottomEq;
}

const IValueProvider* SingleColumnIndexAccess::getTopValue(VirtualMachine* vm) const noexcept {
	if(this->topValue == nullptr || !this->topValue->isAvailable(vm)){
		return &IValueProvider::INFINITY_PARAM;
	}

	return this->topValue;
}
bool SingleColumnIndexAccess::isTopEq(VirtualMachine* vm) const noexcept {
	if(this->topValue == nullptr || !this->topValue->isAvailable(vm)){
		return false;
	}

	return this->topEq;
}

void SingleColumnIndexAccess::makeString() noexcept {
	this->str = new UnicodeString(L"");

	const UnicodeString* colStr = this->column->getName();
	this->str->append(colStr);

	this->str->append(L" = ");

	IValueProvider* vp = const_cast<IValueProvider*>(this->value);
	const UnicodeString* valStr = vp->toStringCode();
	this->str->append(valStr);
}

AbstractColumnsIndexAccess* SingleColumnIndexAccess::clone() const noexcept {
	return new SingleColumnIndexAccess(*this);
}

void SingleColumnIndexAccess::toRangeAccess() noexcept {
	if(!this->range){
		this->topValue = new InfinityScanParam();
		this->bottomEq = true;
		this->topEq = true;
		this->range = true;
	}
}

bool SingleColumnIndexAccess::hasPlaceHolder() const noexcept {
	bool b1 = this->value != nullptr ? this->value->hasPlaceholder() : false;
	bool b2 = this->topValue != nullptr ? this->topValue->hasPlaceholder() : false;

	return b1 || b2;
}

} /* namespace codablecash */
