/*
 * RelationalExpressionScanTarget.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_exp/RelationalExpressionScanTarget.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "lang_sql/sql_expression/SQLRelationalExpression.h"

#include "scan_select/scan_condition/exp/RelationalScanCondition.h"

#include "schema_table/record/table_record/CdbRecord.h"

namespace codablecash {

RelationalExpressionScanTarget::RelationalExpressionScanTarget() {
	this->left = nullptr;
	this->right = nullptr;
	this->op = 0;
	this->cond = nullptr;
}

RelationalExpressionScanTarget::~RelationalExpressionScanTarget() {
	delete this->left;
	delete this->right;
	delete this->cond;
}

void RelationalExpressionScanTarget::setLeft(AbstractScanColumnsTarget* element) noexcept {
	this->left = element;
}

void RelationalExpressionScanTarget::setRight(AbstractScanColumnsTarget* element) noexcept {
	this->right = element;
}

void RelationalExpressionScanTarget::setOp(uint8_t op) noexcept {
	this->op = op;
}

const UnicodeString* RelationalExpressionScanTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		this->str->append(this->left->toStringCode());

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

		this->str->append(this->right->toStringCode());
	}

	return this->str;
}

void RelationalExpressionScanTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->resolveColumns(vm, planner);
	this->right->resolveColumns(vm, planner);

	this->cond = new RelationalScanCondition();
	this->cond->setLeft(this->left->toValueProvider());
	this->cond->setOp(this->op);
	this->cond->setRight(this->right->toValueProvider());
}

void RelationalExpressionScanTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	RelationalScanCondition* cnd = dynamic_cast<RelationalScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* RelationalExpressionScanTarget::toValueProvider() {
	return this->cond->cloneCondition();
}

} /* namespace codablecash */
