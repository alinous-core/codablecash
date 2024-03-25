/*
 * AdditiveScanCondition.cpp
 *
 *  Created on: 2020/07/31
 *      Author: iizuka
 */

#include "scan_select/scan_condition/arithmetic/AdditiveScanCondition.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"

#include "lang_sql/sql_expression/SQLAdditiveExpression.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbLongValue.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

namespace codablecash {

AdditiveScanCondition::AdditiveScanCondition() : AbstractArithmeticCondition(CodeElement::SQL_EXP_ADDITIVE) {

}

AdditiveScanCondition::~AdditiveScanCondition() {

}

const UnicodeString* AdditiveScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();
		this->str = new UnicodeString(L"");

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			if(i > 0){
				int pos = i - 1;
				uint8_t op = this->operations.get(pos);

				if(op == SQLAdditiveExpression::ADD){
					this->str->append(L" + ");
				}
				else if(op == SQLAdditiveExpression::SUB){
					this->str->append(L" - ");
				}
			}

			AbstractScanConditionElement* el = this->list.get(i);

			this->str->append(el->toStringCode());
		}

	}

	return this->str;
}

IValueProvider* AdditiveScanCondition::clone() const noexcept {
	AdditiveScanCondition* cond = new AdditiveScanCondition();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* el = this->list.get(i);

		cond->addOperand(dynamic_cast<AbstractScanConditionElement*>(el->clone()));
	}

	maxLoop = this->operations.size();
	for(int i = 0; i != maxLoop; ++i){
		uint8_t op = this->operations.get(i);

		cond->addOperator(op);
	}

	return cond;
}

AbstractCdbValue* AdditiveScanCondition::evaluate(VirtualMachine* vm, const CdbRecord* record,
		const ScanResultMetadata* metadata) {
	AbstractScanConditionElement* vpf = this->list.get(0);

	AbstractCdbValue* fv = vpf->evaluate(vm, record, metadata); __STP(fv);
	AbstractCdbKey* fk = fv->toKey(); __STP(fk);
	int64_t ivalue = fk->toInt64();

	int maxLoop = this->list.size();
	for(int i = 1; i != maxLoop; ++i){
		AbstractScanConditionElement* vp = this->list.get(i);

		AbstractCdbValue* cdbv = vp->evaluate(vm, record, metadata); __STP(cdbv);
		AbstractCdbKey* cdbk = cdbv->toKey(); __STP(cdbk);
		int64_t operandv = cdbk->toInt64();

		uint8_t op = this->operations.get(i - 1);
		if(op == SQLAdditiveExpression::SUB){
			ivalue = ivalue - operandv;
		}
		else {
			ivalue = ivalue + operandv;
		}
	}

	return new CdbLongValue(ivalue);
}

} /* namespace codablecash */
