/*
 * LikeExpression.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_exp/LikeExpressionScanColumnTarget.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/exp/LikeScanCondition.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

LikeExpressionScanColumnTarget::LikeExpressionScanColumnTarget() {
	this->left = nullptr;
	this->right = nullptr;
	this->escape = nullptr;
	this->cond = nullptr;
}

LikeExpressionScanColumnTarget::~LikeExpressionScanColumnTarget() {
	delete this->left;
	delete this->right;
	delete this->escape;
	delete this->cond;
}

void LikeExpressionScanColumnTarget::setLeft(AbstractScanColumnsTarget* left) noexcept {
	this->left = left;
}

void LikeExpressionScanColumnTarget::setRight(AbstractScanColumnsTarget* right) noexcept {
	this->right = right;
}

void LikeExpressionScanColumnTarget::setEscape(AbstractScanColumnsTarget* escape) noexcept {
	this->escape = escape;
}

const UnicodeString* LikeExpressionScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		this->str->append(this->left->toStringCode());

		this->str->append(L" LIKE ");

		this->str->append(this->right->toStringCode());

		if(this->escape != nullptr){
			this->str->append(L" ESCAPE ");
			this->str->append(this->escape->toStringCode());
		}
	}

	return this->str;
}

void LikeExpressionScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->resolveColumns(vm, planner);
	this->right->resolveColumns(vm, planner);

	if(this->escape != nullptr){
		this->escape->resolveColumns(vm, planner);
	}

	this->cond = new LikeScanCondition();
	this->cond->setLeft(this->left->toValueProvider());
	this->cond->setRight(this->right->toValueProvider());

	if(this->escape != nullptr){
		this->cond->setEscape(this->escape->toValueProvider());
	}
}

void LikeExpressionScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	LikeScanCondition* cnd = dynamic_cast<LikeScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* LikeExpressionScanColumnTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
