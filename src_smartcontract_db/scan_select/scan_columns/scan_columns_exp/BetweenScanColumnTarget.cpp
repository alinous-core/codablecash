/*
 * BetweenScanColumnTarget.cpp
 *
 *  Created on: 2020/08/20
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_exp/BetweenScanColumnTarget.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_condition/exp/BetweenScanCondition.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

BetweenScanColumnTarget::BetweenScanColumnTarget() {
	this->left = nullptr;
	this->start = nullptr;
	this->end = nullptr;
	this->cond = nullptr;
}

BetweenScanColumnTarget::~BetweenScanColumnTarget() {
	delete this->left;
	delete this->start;
	delete this->end;
	delete this->cond;
}

void BetweenScanColumnTarget::setLeft(AbstractScanColumnsTarget* left) noexcept {
	this->left = left;
}

void BetweenScanColumnTarget::setStart(AbstractScanColumnsTarget* start) noexcept {
	this->start = start;
}

void BetweenScanColumnTarget::setEnd(AbstractScanColumnsTarget* end) noexcept {
	this->end = end;
}

const UnicodeString* BetweenScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		this->str->append(this->left->toStringCode());

		this->str->append(L" BETWEEN ");

		this->str->append(this->start->toStringCode());

		this->str->append(L" AND ");

		this->str->append(this->end->toStringCode());
	}

	return this->str;
}

void BetweenScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->resolveColumns(vm, planner);
	this->start->resolveColumns(vm, planner);
	this->end->resolveColumns(vm, planner);

	this->cond = new BetweenScanCondition();
	this->cond->setLeft(this->left->toValueProvider());
	this->cond->setStart(this->start->toValueProvider());
	this->cond->setEnd(this->end->toValueProvider());
}

void BetweenScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	BetweenScanCondition* cnd = dynamic_cast<BetweenScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* BetweenScanColumnTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
