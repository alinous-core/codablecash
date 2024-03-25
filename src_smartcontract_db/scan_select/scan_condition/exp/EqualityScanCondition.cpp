/*
 * EqualityScanCondition.cpp
 *
 *  Created on: 2020/07/20
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp/EqualityScanCondition.h"

#include "engine/sc/CodeElement.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "engine/CdbException.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_planner/scanner/join/JoinCandidate.h"
#include "scan_select/scan_planner/scanner/join/JoinCandidateHolder.h"
#include "scan_select/scan_planner/scanner/join/JoinCandidateStackMarker.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "scan_select/scan_table/AbstractScanTableTarget.h"

#include "scan_select/scan_planner/scanner/index/IndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetector.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbByteValue.h"
#include "schema_table/record/table_record_value/CdbBooleanValue.h"

#include "vm/VirtualMachine.h"

#include "scan_select/scan_condition/exp_escape/EscapeTargetCondition.h"

using namespace alinous;

namespace codablecash {

EqualityScanCondition::EqualityScanCondition() : AbstractExpressionCondition(CodeElement::SQL_EXP_EQUALITY) {
	this->left = nullptr;
	this->right = nullptr;
	this->str = nullptr;
}

EqualityScanCondition::~EqualityScanCondition() {
	delete this->left;
	delete this->right;
	resetStr();
}

void EqualityScanCondition::setLeft(IValueProvider* element) {
	this->left = element;
	resetStr();
}

void EqualityScanCondition::setRight(IValueProvider* element) {
	this->right = element;
	resetStr();
}

const UnicodeString* EqualityScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();

		this->str = new UnicodeString(L"");

		AbstractScanConditionElement* cond = dynamic_cast<AbstractScanConditionElement*>(this->left);
		this->str->append(cond->toStringCode());

		this->str->append(L" = ");

		cond = dynamic_cast<AbstractScanConditionElement*>(this->right);
		this->str->append(cond->toStringCode());
	}

	return this->str;
}

AbstractScanCondition* EqualityScanCondition::cloneCondition() const noexcept {
	EqualityScanCondition* cond = new EqualityScanCondition();

	cond->setLeft(this->left->clone());
	cond->setRight(this->right->clone());

	return cond;
}

void EqualityScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	if(this->left->isFilterable(vm, planner, detector) &&
			this->right->isFilterable(vm, planner, detector)){
		detector->push(cloneCondition());
	}
}

void EqualityScanCondition::detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner,
		TableIndexDetector* detector) const {
	ColumnIdentifierScanParam* column = nullptr;
	IValueProvider* value = nullptr;

	if(this->left->isColumn() && this->right->isRecordIndependent()){
		column = dynamic_cast<ColumnIdentifierScanParam*>(this->left);
		value = this->right;
	}
	else if(this->left->isRecordIndependent() && this->right->isColumn()){
		column = dynamic_cast<ColumnIdentifierScanParam*>(this->right);
		value = this->left;
	}
	else {
		return;
	}

	IndexCandidate* candidate = new IndexCandidate(AbstractIndexCandidate::IndexType::EQUALS);
	candidate->setColumn(column);
	candidate->setValue(value);

	detector->push(candidate);
}

void EqualityScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void EqualityScanCondition::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->analyzeConditions(vm, planner);
	this->right->analyzeConditions(vm, planner);
}

void EqualityScanCondition::collectJoinCandidate(VirtualMachine* vm, SelectScanPlanner* planner, int joinType,
							JoinCandidateHolder* jholder) {
	if(!(this->left->isColumn() && this->right->isColumn())){
		return;
	}

	const AbstractScanTableTarget* left = jholder->getLeft();
	const AbstractScanTableTarget* right = jholder->getRight();
	if(!hasLeftAndRightScanTarget(left, right)){
		return;
	}

	JoinCandidateStackMarker marker(jholder->getStack());

	JoinCandidate* candidate = new JoinCandidate(joinType, getTargetCond(left),
			getTargetCond(right));
	StackRelease<JoinCandidate> st_candidate(candidate);

	if(jholder->isJoinCondition(candidate)){
		jholder->push(st_candidate.move());
	}
}

ColumnIdentifierScanParam* EqualityScanCondition::getTargetCond(const AbstractScanTableTarget* target) {
	ColumnIdentifierScanParam* lparam = dynamic_cast<ColumnIdentifierScanParam*>(this->left);
	ColumnIdentifierScanParam* rparam = dynamic_cast<ColumnIdentifierScanParam*>(this->right);

	const AbstractScanTableTarget* lparamTarget = lparam->getSourceTarget();
	const AbstractScanTableTarget* rparamTarget = rparam->getSourceTarget();

	if(lparamTarget == target){
		return lparam;
	}

	return rparam;
}


bool EqualityScanCondition::hasLeftAndRightScanTarget(
		const AbstractScanTableTarget* left,
		const AbstractScanTableTarget* right) const noexcept {
	bool l = false;
	bool r = false;

	ColumnIdentifierScanParam* lparam = dynamic_cast<ColumnIdentifierScanParam*>(this->left);
	ColumnIdentifierScanParam* rparam = dynamic_cast<ColumnIdentifierScanParam*>(this->right);

	const AbstractScanTableTarget* lparamTarget = lparam->getSourceTarget();
	const AbstractScanTableTarget* rparamTarget = rparam->getSourceTarget();

	if(left->hasTarget(lparamTarget)){
		l = true;
	}else if(right->hasTarget(lparamTarget)){
		r = true;
	}

	if(left->hasTarget(rparamTarget)){
		l = true;
	}else if(right->hasTarget(rparamTarget)){
		r = true;
	}

	return l && r;
}

AbstractCdbValue* EqualityScanCondition::evaluate(VirtualMachine* vm, const CdbRecord* record,
		const ScanResultMetadata* metadata) {
	EscapeTargetCondition* escape = vm->getEscapeTargetCondition();
	if(escape != nullptr && (escape->isEscape(this->left) || escape->isEscape(this->right))){
		return new CdbBooleanValue(true);
	}

	AbstractScanConditionElement* leftElement = dynamic_cast<AbstractScanConditionElement*>(this->left);
	AbstractScanConditionElement* rightElement = dynamic_cast<AbstractScanConditionElement*>(this->right);

	AbstractCdbValue* lv = leftElement->evaluate(vm, record, metadata); __STP(lv);
	AbstractCdbValue* rv = rightElement->evaluate(vm, record, metadata); __STP(rv);

	AbstractCdbKey* lk = lv->toKey(); __STP(lk);
	AbstractCdbKey* rk = rv->toKey(); __STP(rk);

	int cmp = lk->compareTo(rk);
	int result = cmp == 0 ? 1 : 0;

	return new CdbBooleanValue(result > 0);
}

bool EqualityScanCondition::isRecordIndependent() const noexcept {
	return this->left->isRecordIndependent() && this->right->isRecordIndependent();
}

bool EqualityScanCondition::isAvailable(VirtualMachine *vm) const {
	return this->left->isAvailable(vm) && this->right->isAvailable(vm);
}

bool EqualityScanCondition::hasPlaceholder() const noexcept {
	return this->left->hasPlaceholder() && this->right->hasPlaceholder();
}

} /* namespace codablecash */
