/*
 * MultiplicativeExpressionScanColumnTarget.cpp
 *
 *  Created on: 2020/08/21
 *      Author: iizuka
 */

#include "scan_select/scan_columns/scan_columns_arithmetic/MultiplicativeExpressionScanColumnTarget.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "lang_sql/sql_expression/SqlMultiplicativeExpression.h"

#include "scan_select/scan_condition/arithmetic/MultiplicativeScanCondition.h"

#include "schema_table/record/table_record/CdbRecord.h"


namespace codablecash {

MultiplicativeExpressionScanColumnTarget::MultiplicativeExpressionScanColumnTarget() : operations(2) {
	this->cond = nullptr;
}

MultiplicativeExpressionScanColumnTarget::~MultiplicativeExpressionScanColumnTarget() {
	this->list.deleteElements();
	delete this->cond;
}

void MultiplicativeExpressionScanColumnTarget::addOperand(AbstractScanColumnsTarget* vp) noexcept {
	this->list.addElement(vp);
}

void MultiplicativeExpressionScanColumnTarget::addOperator(uint8_t op) noexcept {
	this->operations.addElement(op);
}

const UnicodeString* MultiplicativeExpressionScanColumnTarget::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			if(i > 0){
				int pos = i - 1;
				uint8_t op = this->operations.get(pos);

				if(op == SqlMultiplicativeExpression::MUL){
					this->str->append(L" * ");
				}
				else if(op == SqlMultiplicativeExpression::DIV){
					this->str->append(L" / ");
				}
				else if(op == SqlMultiplicativeExpression::MOD){
					this->str->append(L" % ");
				}
			}

			AbstractScanColumnsTarget* vp = this->list.get(i);

			this->str->append(vp->toStringCode());
		}
	}

	return this->str;
}

void MultiplicativeExpressionScanColumnTarget::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* vp = this->list.get(i);
		vp->resolveColumns(vm, planner);
	}

	delete this->cond;
	this->cond = new MultiplicativeScanCondition();

	for(int i = 0; i != maxLoop; ++i){
		AbstractScanColumnsTarget* col = this->list.get(i);

		IValueProvider* vp = col->toValueProvider();
		this->cond->addOperand(dynamic_cast<AbstractScanConditionElement*>(vp));

		if(this->operations.size() > i){
			uint8_t op = this->operations.get(i);
			this->cond->addOperator(op);
		}
	}
}

void MultiplicativeExpressionScanColumnTarget::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata,	CdbRecord* newRecord) {
	MultiplicativeScanCondition* cnd = dynamic_cast<MultiplicativeScanCondition*>(toValueProvider()); __STP(cnd);

	AbstractCdbValue* value = cnd->evaluate(vm, record, metadata);
	newRecord->addValue(value);
}

IValueProvider* MultiplicativeExpressionScanColumnTarget::toValueProvider() {
	return this->cond->clone();
}

} /* namespace codablecash */
