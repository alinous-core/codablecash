/*
 * InExpressionScanColumnTarget.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_exp/InExpressionScanColumnTarget.h"

#include "scan_select/scan_columns/scan_columns_exp/ExpressionListScanColumnTarget.h"

#include "instance/instance_exception/TypeCastExceptionClassDeclare.h"
#include "instance/instance_exception/ExceptionInterrupt.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "engine/CdbException.h"

#include "scan_select/scan_condition/exp/InExpressionScanCondition.h"
#include "scan_select/scan_condition/exp/ExpressionListScanCondition.h"

#include "schema_table/record/table_record/CdbRecord.h"


namespace codablecash {

InExpressionScanColumnTarget::InExpressionScanColumnTarget() {
	this->left = nullptr;
	this->list = nullptr;
	this->cond = nullptr;
}

InExpressionScanColumnTarget::~InExpressionScanColumnTarget() {
	delete this->left;
	delete this->list;
	delete this->cond;
}

void InExpressionScanColumnTarget::setLeft(AbstractScanColumnsTarget* cond) noexcept {
	this->left = cond;
}

void InExpressionScanColumnTarget::setList(ExpressionListScanColumnTarget* list) noexcept {
	this->list = list;
}

ExpressionListScanColumnTarget* InExpressionScanColumnTarget::castToExpressionListScanColumnTarget(AbstractScanColumnsTarget* col, VirtualMachine* vm, const CodeElement* element) {
	ExpressionListScanColumnTarget* val = dynamic_cast<ExpressionListScanColumnTarget*>(col);

	if(val == nullptr){
		delete col;

		throw new CdbException(__FILE__, __LINE__);
	}

	return val;
}

const UnicodeString* InExpressionScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		this->str->append(this->left->toStringCode());
		this->str->append(L" IN (");

		this->str->append(this->list->toStringCode());
		this->str->append(L")");
	}

	return this->str;
}

void InExpressionScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->resolveColumns(vm, planner);
	this->list->resolveColumns(vm, planner);

	this->cond = new InExpressionScanCondition();
	this->cond->setLeft(this->left->toValueProvider());

	ExpressionListScanCondition* exlist = dynamic_cast<ExpressionListScanCondition*>(this->list->toValueProvider());
	this->cond->setList(exlist);
}

void InExpressionScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	InExpressionScanCondition* cnd = dynamic_cast<InExpressionScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* InExpressionScanColumnTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
