/*
 * EqualityScanColumnTarget.cpp
 *
 *  Created on: 2020/08/20
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_exp/EqualityScanColumnTarget.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/exp/EqualityScanCondition.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

EqualityScanColumnTarget::EqualityScanColumnTarget() {
	this->left = nullptr;
	this->right = nullptr;
	this->cond = nullptr;
}

EqualityScanColumnTarget::~EqualityScanColumnTarget() {
	delete this->left;
	delete this->right;
	delete this->cond;
}

void EqualityScanColumnTarget::setLeft(AbstractScanColumnsTarget* element) noexcept {
	this->left = element;
}

void EqualityScanColumnTarget::setRight(AbstractScanColumnsTarget* element) noexcept {
	this->right = element;
}

const UnicodeString* EqualityScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		this->str->append(this->left->toStringCode());

		this->str->append(L" = ");

		this->str->append(this->right->toStringCode());
	}

	return this->str;
}

void EqualityScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->resolveColumns(vm, planner);
	this->right->resolveColumns(vm, planner);

	this->cond = new EqualityScanCondition();
	this->cond->setLeft(this->left->toValueProvider());
	this->cond->setRight(this->right->toValueProvider());
}

void EqualityScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	EqualityScanCondition* cnd = dynamic_cast<EqualityScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* EqualityScanColumnTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
