/*
 * LikeScanCondition.cpp
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp/LikeScanCondition.h"

#include "scan_select/scan_condition/exp_like/LikeExpressionProcessor.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "schema_table/record/table_record_value/CdbValueCaster.h"
#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"
#include "schema_table/record/table_record_value/CdbValueCastException.h"
#include "schema_table/record/table_record_value/CdbBooleanValue.h"

using namespace alinous;

namespace codablecash {

LikeScanCondition::LikeScanCondition() : AbstractExpressionCondition(CodeElement::SQL_EXP_LIKE) {
	this->left = nullptr;
	this->right = nullptr;
	this->escape = nullptr;

	this->str = nullptr;
	this->processor = nullptr;
}

LikeScanCondition::~LikeScanCondition() {
	delete this->left;
	delete this->right;
	delete this->escape;

	delete this->processor;

	resetStr();
}

void LikeScanCondition::setLeft(IValueProvider* left) noexcept {
	this->left = left;
}

void LikeScanCondition::setRight(IValueProvider* right) noexcept {
	this->right = right;
}

void LikeScanCondition::setEscape(IValueProvider* escape) noexcept {
	this->escape = escape;
}


const UnicodeString* LikeScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();

		this->str = new UnicodeString(L"");

		AbstractScanConditionElement* cond = dynamic_cast<AbstractScanConditionElement*>(this->left);
		this->str->append(cond->toStringCode());

		this->str->append(L" LIKE ");

		cond = dynamic_cast<AbstractScanConditionElement*>(this->right);
		this->str->append(cond->toStringCode());

		if(this->escape != nullptr){
			this->str->append(L" ESCAPE ");
			cond = dynamic_cast<AbstractScanConditionElement*>(this->escape);
			this->str->append(cond->toStringCode());
		}
	}

	return this->str;
}

AbstractScanCondition* LikeScanCondition::cloneCondition() const noexcept {
	LikeScanCondition* cond = new LikeScanCondition();

	cond->setLeft(this->left->clone());
	cond->setRight(this->right->clone());

	if(this->escape != nullptr){
		cond->setEscape(this->escape->clone());
	}

	return cond;
}

void LikeScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	if(this->left->isFilterable(vm, planner, detector) &&
			this->right->isFilterable(vm, planner, detector) &&
			(this->escape == nullptr || this->escape->isFilterable(vm, planner, detector) )){
		detector->push(cloneCondition());
	}
}

void LikeScanCondition::detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const {

}

void LikeScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void LikeScanCondition::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->analyzeConditions(vm, planner);
	this->right->analyzeConditions(vm, planner);

	if(this->escape != nullptr){
		this->escape->analyzeConditions(vm, planner);
	}
}

AbstractCdbValue* LikeScanCondition::evaluate(VirtualMachine* vm,
		const CdbRecord* record, const ScanResultMetadata* metadata) {
	if(this->processor == nullptr){
		AbstractScanConditionElement* cond = dynamic_cast<AbstractScanConditionElement*>(this->right);
		AbstractCdbValue* cdbv = cond->evaluate(vm, record, metadata); __STP(cdbv);

		AbstractCdbValue* cdbstrv = CdbValueCaster::cast(cdbv, AbstractCdbValue::TYPE_STRING); __STP(cdbstrv);
		CdbStringValue* str = dynamic_cast<CdbStringValue*>(cdbstrv);

		assert(str != nullptr);

		this->processor = new LikeExpressionProcessor(str->getValue());
		if(this->escape != nullptr){
			cond = dynamic_cast<AbstractScanConditionElement*>(this->escape);
			AbstractCdbValue* esccdbv = cond->evaluate(vm, record, metadata); __STP(esccdbv);

			AbstractCdbValue* esccdbstrv = CdbValueCaster::cast(cdbv, AbstractCdbValue::TYPE_STRING); __STP(esccdbstrv);
			CdbStringValue* escstr = dynamic_cast<CdbStringValue*>(esccdbstrv);

			assert(escstr != nullptr);

			this->processor->setEscape(escstr->getValue());
		}

		this->processor->compile();
	}

	AbstractScanConditionElement* cond = dynamic_cast<AbstractScanConditionElement*>(this->left);
	AbstractCdbValue* v = cond->evaluate(vm, record, metadata); __STP(v);

	AbstractCdbValue* strv = CdbValueCaster::cast(v, AbstractCdbValue::TYPE_STRING); __STP(strv);
	CdbStringValue* str = dynamic_cast<CdbStringValue*>(strv);

	static UnicodeString NULLBLANK(L"");
	bool res = this->processor->match(strv != nullptr ? str->getValue() : &NULLBLANK);

	return new CdbBooleanValue(res);
}

bool LikeScanCondition::isRecordIndependent() const noexcept {
	return this->left->isRecordIndependent() && this->right->isRecordIndependent() && (this->escape == nullptr || this->escape->isRecordIndependent());
}

bool LikeScanCondition::isAvailable(VirtualMachine *vm) const {
	return this->left->isAvailable(vm) && this->right->isAvailable(vm) && (this->escape == nullptr || this->escape->isAvailable(vm));
}

bool LikeScanCondition::hasPlaceholder() const noexcept {
	return this->left->hasPlaceholder() && this->right->hasPlaceholder() && (this->escape == nullptr || this->escape->hasPlaceholder());
}

} /* namespace codablecash */
