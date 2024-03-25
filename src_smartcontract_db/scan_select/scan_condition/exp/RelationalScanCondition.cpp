/*
 * RelationalScanCondition.cpp
 *
 *  Created on: 2020/07/29
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp/RelationalScanCondition.h"

#include "engine/sc/CodeElement.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "engine/CdbException.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "lang_sql/sql_expression/SQLRelationalExpression.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "scan_select/scan_planner/scanner/index/IndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetector.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbByteValue.h"
#include "schema_table/record/table_record_value/CdbBooleanValue.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"


using namespace alinous;

namespace codablecash {

RelationalScanCondition::RelationalScanCondition() : AbstractExpressionCondition(CodeElement::SQL_EXP_RELATIONAL) {
	this->left = nullptr;
	this->right = nullptr;
	this->op = 0;
	this->str = nullptr;
}

RelationalScanCondition::~RelationalScanCondition() {
	delete this->left;
	delete this->right;
	resetStr();
}

void RelationalScanCondition::setLeft(IValueProvider* element) {
	this->left = element;
	resetStr();
}

void RelationalScanCondition::setRight(IValueProvider* element) {
	this->right = element;
	resetStr();
}

void RelationalScanCondition::setOp(uint8_t op) noexcept {
	this->op = op;
}

const UnicodeString* RelationalScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();
		this->str = new UnicodeString(L"");

		AbstractScanConditionElement* cond = dynamic_cast<AbstractScanConditionElement*>(this->left);
		this->str->append(cond->toStringCode());

		if(this->op == SQLRelationalExpression::GT){
			this->str->append(L" > ");
		}
		else if(this->op == SQLRelationalExpression::GT_EQ){
			this->str->append(L" >= ");
		}
		else if(this->op == SQLRelationalExpression::LT){
			this->str->append(L" < ");
		}
		else if(this->op == SQLRelationalExpression::LT_EQ){
			this->str->append(L" <= ");
		}

		cond = dynamic_cast<AbstractScanConditionElement*>(this->right);
		this->str->append(cond->toStringCode());
	}

	return this->str;
}

AbstractScanCondition* RelationalScanCondition::cloneCondition() const noexcept {
	RelationalScanCondition* cond = new RelationalScanCondition();

	cond->setLeft(this->left->clone());
	cond->setRight(this->right->clone());
	cond->setOp(this->op);

	return cond;
}

void RelationalScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	if(this->left->isFilterable(vm, planner, detector) &&
			this->right->isFilterable(vm, planner, detector)){
		detector->push(cloneCondition());
	}
}

void RelationalScanCondition::detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner,
		TableIndexDetector* detector) const {
	ColumnIdentifierScanParam* column = nullptr;
	IValueProvider* value = nullptr;

	bool reverse = false;
	if(this->left->isColumn() && !this->right->isColumn()){
		column = dynamic_cast<ColumnIdentifierScanParam*>(this->left);
		value = this->right;
	}
	else if(!this->left->isColumn() && this->right->isColumn()){
		column = dynamic_cast<ColumnIdentifierScanParam*>(this->right);
		value = this->left;
		reverse = true;
	}
	else {
		return;
	}

	AbstractIndexCandidate::IndexType indexType = toIndexType(this->op, reverse);
	IndexCandidate* candidate = new IndexCandidate(indexType);
	candidate->setColumn(column);
	candidate->setValue(value);

	detector->push(candidate);
}

void RelationalScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void RelationalScanCondition::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->analyzeConditions(vm, planner);
	this->right->analyzeConditions(vm, planner);
}

AbstractIndexCandidate::IndexType RelationalScanCondition::toIndexType(uint8_t op, bool reverse) {
	AbstractIndexCandidate::IndexType idxType;

	switch(op){
	case SQLRelationalExpression::GT:
		idxType = !reverse ? AbstractIndexCandidate::IndexType::RANGE_GT : AbstractIndexCandidate::IndexType::RANGE_LT;
		break;
	case SQLRelationalExpression::GT_EQ:
		idxType = !reverse ? AbstractIndexCandidate::IndexType::RANGE_GT_EQ : AbstractIndexCandidate::IndexType::RANGE_LT_EQ;
		break;
	case SQLRelationalExpression::LT:
		idxType = !reverse ? AbstractIndexCandidate::IndexType::RANGE_LT : AbstractIndexCandidate::IndexType::RANGE_GT;
		break;
	case SQLRelationalExpression::LT_EQ:
		idxType = !reverse ? AbstractIndexCandidate::IndexType::RANGE_LT_EQ : AbstractIndexCandidate::IndexType::RANGE_GT_EQ;
		break;
	default:
		throw new CdbException(L"wrong operation code", __FILE__, __LINE__);
	}

	return idxType;
}

AbstractCdbValue* RelationalScanCondition::evaluate(VirtualMachine* vm,
		const CdbRecord* record, const ScanResultMetadata* metadata) {
	AbstractScanConditionElement* leftElement = dynamic_cast<AbstractScanConditionElement*>(this->left);
	AbstractScanConditionElement* rightElement = dynamic_cast<AbstractScanConditionElement*>(this->right);

	AbstractCdbValue* lv = leftElement->evaluate(vm, record, metadata); __STP(lv);
	AbstractCdbValue* rv = rightElement->evaluate(vm, record, metadata); __STP(rv);

	AbstractCdbKey* lk = lv->toKey(); __STP(lk);
	AbstractCdbKey* rk = rv->toKey(); __STP(rk);

	int cmp = lk->compareTo(rk);
	int result = 0;

	switch(op){
	case SQLRelationalExpression::GT:
		result = cmp > 0 ? 1 : 0;
		break;
	case SQLRelationalExpression::GT_EQ:
		result = cmp >= 0 ? 1 : 0;
		break;
	case SQLRelationalExpression::LT:
		result = cmp < 0 ? 1 : 0;
		break;
	case SQLRelationalExpression::LT_EQ:
	default:
		result = cmp <= 0 ? 1 : 0;
		break;
	}

	return new CdbBooleanValue(result > 0);
}

bool RelationalScanCondition::isRecordIndependent() const noexcept {
	return this->left->isRecordIndependent() && this->right->isRecordIndependent();
}

bool RelationalScanCondition::isAvailable(VirtualMachine *vm) const {
	return this->left->isAvailable(vm) && this->right->isAvailable(vm);
}

bool RelationalScanCondition::hasPlaceholder() const noexcept {
	return this->left->hasPlaceholder() && this->right->hasPlaceholder();
}


} /* namespace codablecash */
