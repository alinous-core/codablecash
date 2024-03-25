/*
 * MultiplicativeScanCondition.cpp
 *
 *  Created on: 2020/08/04
 *      Author: iizuka
 */

#include "scan_select/scan_condition/arithmetic/MultiplicativeScanCondition.h"

#include "scan_select/scan_condition/base/AbstractScanConditionElement.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "lang_sql/sql_expression/SqlMultiplicativeExpression.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "schema_table/record/table_record_value/CdbLongValue.h"

#include "engine/CdbException.h"
namespace codablecash {

MultiplicativeScanCondition::MultiplicativeScanCondition() : AbstractArithmeticCondition(CodeElement::SQL_EXP_MULTIPLICATIVE) {

}

MultiplicativeScanCondition::~MultiplicativeScanCondition() {

}

const UnicodeString* MultiplicativeScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();
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

			IValueProvider* vp = this->list.get(i);
			AbstractScanConditionElement* el = dynamic_cast<AbstractScanConditionElement*>(vp);

			this->str->append(el->toStringCode());
		}

	}

	return this->str;
}

IValueProvider* MultiplicativeScanCondition::clone() const noexcept {
	MultiplicativeScanCondition* cond = new MultiplicativeScanCondition();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* vp = this->list.get(i);

		cond->addOperand(dynamic_cast<AbstractScanConditionElement*>(vp->clone()));
	}

	maxLoop = this->operations.size();
	for(int i = 0; i != maxLoop; ++i){
		uint8_t op = this->operations.get(i);

		cond->addOperator(op);
	}

	return cond;
}

AbstractCdbValue* MultiplicativeScanCondition::evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	AbstractScanConditionElement* vpf = dynamic_cast<AbstractScanConditionElement*>(this->list.get(0));

	AbstractCdbValue* fv = vpf->evaluate(vm, record, metadata); __STP(fv);
	AbstractCdbKey* fk = fv->toKey(); __STP(fk);
	int64_t ivalue = fk->toInt64();

	int maxLoop = this->list.size();
	for(int i = 1; i != maxLoop; ++i){
		AbstractScanConditionElement* vp = dynamic_cast<AbstractScanConditionElement*>(this->list.get(i));

		AbstractCdbValue* cdbv = vp->evaluate(vm, record, metadata); __STP(cdbv);
		AbstractCdbKey* cdbk = cdbv->toKey(); __STP(cdbk);
		int64_t operandv = cdbk->toInt64();

		uint8_t op = this->operations.get(i - 1);

		// error check
		if(op != SqlMultiplicativeExpression::MUL && operandv == 0){
			throw new CdbException(L"", __FILE__, __LINE__);
		}


		if(op == SqlMultiplicativeExpression::DIV){
			ivalue = ivalue / operandv;
		}
		else if(op == SqlMultiplicativeExpression::MOD){
			ivalue = ivalue % operandv;
		}
		else {
			ivalue = ivalue * operandv;
		}
	}

	return new CdbLongValue(ivalue);
}

} /* namespace codablecash */
