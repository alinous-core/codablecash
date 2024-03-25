/*
 * BetweenScanCondition.cpp
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp/BetweenScanCondition.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "scan_select/scan_planner/scanner/index/IndexRangeCandidate.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "schema_table/record/table_record_value/CdbBooleanValue.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"


using namespace alinous;

namespace codablecash {

BetweenScanCondition::BetweenScanCondition() : AbstractExpressionCondition(CodeElement::SQL_EXP_BETWEEN) {
	this->left = nullptr;
	this->start = nullptr;
	this->end = nullptr;

	this->str = nullptr;
}

BetweenScanCondition::~BetweenScanCondition() {
	delete this->left;
	delete this->start;
	delete this->end;

	resetStr();
}

void BetweenScanCondition::setLeft(IValueProvider* left) noexcept {
	this->left = left;
	resetStr();
}

void BetweenScanCondition::setStart(IValueProvider* start) noexcept {
	this->start = start;
	resetStr();
}

void BetweenScanCondition::setEnd(IValueProvider* end) noexcept {
	this->end = end;
	resetStr();
}

const UnicodeString* BetweenScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();
		this->str = new UnicodeString(L"");

		AbstractScanConditionElement* el = dynamic_cast<AbstractScanConditionElement*>(this->left);
		this->str->append(el->toStringCode());

		this->str->append(L" BETWEEN ");

		el = dynamic_cast<AbstractScanConditionElement*>(this->start);
		this->str->append(el->toStringCode());

		this->str->append(L" AND ");

		el = dynamic_cast<AbstractScanConditionElement*>(this->end);
		this->str->append(el->toStringCode());
	}

	return this->str;
}

AbstractScanCondition* BetweenScanCondition::cloneCondition() const noexcept {
	BetweenScanCondition* cond = new BetweenScanCondition();

	cond->setLeft(this->left->clone());
	cond->setStart(this->start->clone());
	cond->setEnd(this->end->clone());

	return cond;
}

void BetweenScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	if(this->left->isFilterable(vm, planner, detector) &&
			this->start->isFilterable(vm, planner, detector) &&
			this->end->isFilterable(vm, planner, detector)){
		detector->push(cloneCondition());
	}
}

void BetweenScanCondition::detectIndexCandidate(VirtualMachine* vm,
		SelectScanPlanner* planner, TableIndexDetector* detector) const {
	if(this->left->isColumn() && this->start->isRecordIndependent() && this->end->isRecordIndependent()){
		ColumnIdentifierScanParam* column = dynamic_cast<ColumnIdentifierScanParam*>(this->left);

		IndexRangeCandidate* candidate = new IndexRangeCandidate(); __STP(candidate);
		candidate->setColumn(column);
		candidate->setBottomEq(true);
		candidate->setValue(this->start);
		candidate->setTopEq(true);
		candidate->setTopValue(this->end);

	}
}

void BetweenScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void BetweenScanCondition::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->analyzeConditions(vm, planner);
	this->start->analyzeConditions(vm, planner);
	this->end->analyzeConditions(vm, planner);

}

AbstractCdbValue* BetweenScanCondition::evaluate(VirtualMachine* vm, const CdbRecord* record,
		const ScanResultMetadata* metadata) {

	AbstractScanConditionElement* leftElement = dynamic_cast<AbstractScanConditionElement*>(this->left);
	AbstractScanConditionElement* startElement = dynamic_cast<AbstractScanConditionElement*>(this->start);
	AbstractScanConditionElement* endElement = dynamic_cast<AbstractScanConditionElement*>(this->end);

	AbstractCdbValue* lv = leftElement->evaluate(vm, record, metadata); __STP(lv);
	AbstractCdbValue* sv = startElement->evaluate(vm, record, metadata); __STP(sv);
	AbstractCdbValue* ev = endElement->evaluate(vm, record, metadata); __STP(ev);

	AbstractCdbKey* lk = lv->toKey(); __STP(lk);
	AbstractCdbKey* sk = sv->toKey(); __STP(sk);
	AbstractCdbKey* ek = ev->toKey(); __STP(ek);

	int cmpL = lk->compareTo(sk);
	int cmpH = ek->compareTo(lk);

	return new CdbBooleanValue(cmpL >= 0 && cmpH >= 0);
}

bool BetweenScanCondition::isRecordIndependent() const noexcept {
	return this->left->isRecordIndependent() && this->start->isRecordIndependent() && this->end->isRecordIndependent();
}

bool BetweenScanCondition::isAvailable(VirtualMachine *vm) const {
	return this->left->isAvailable(vm) && this->start->isAvailable(vm) && this->end->isAvailable(vm);
}

bool BetweenScanCondition::hasPlaceholder() const noexcept {
	return this->left->hasPlaceholder() && this->start->hasPlaceholder() && this->end->hasPlaceholder();
}

} /* namespace codablecash */
