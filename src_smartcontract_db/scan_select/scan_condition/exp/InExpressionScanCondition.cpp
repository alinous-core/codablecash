/*
 * InExpressionScanCondition.cpp
 *
 *  Created on: 2020/08/05
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp/InExpressionScanCondition.h"
#include "scan_select/scan_condition/exp/ExpressionListScanCondition.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "schema_table/record/table_record_value/CdbValueList.h"
#include "schema_table/record/table_record_value/CdbBooleanValue.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "scan_select/scan_planner/scanner/index/OrIndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetector.h"
#include "scan_select/scan_planner/scanner/index/IndexCandidate.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

namespace codablecash {

InExpressionScanCondition::InExpressionScanCondition() : AbstractExpressionCondition(CodeElement::SQL_EXP_IN) {
	this->left = nullptr;
	this->list = nullptr;

	this->str = nullptr;
}

InExpressionScanCondition::~InExpressionScanCondition() {
	delete this->left;
	delete this->list;
	resetStr();
}

void InExpressionScanCondition::setLeft(IValueProvider* cond) noexcept {
	this->left = cond;
	resetStr();
}

void InExpressionScanCondition::setList(ExpressionListScanCondition* list) noexcept {
	this->list = list;
	resetStr();
}

const UnicodeString* InExpressionScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();
		this->str = new UnicodeString(L"");

		AbstractScanConditionElement* el = dynamic_cast<AbstractScanConditionElement*>(left);
		this->str->append(el->toStringCode());
		this->str->append(L" IN (");

		this->str->append(this->list->toStringCode());
		this->str->append(L")");
	}

	return this->str;
}

AbstractScanCondition* InExpressionScanCondition::cloneCondition() const noexcept {
	InExpressionScanCondition* cond = new InExpressionScanCondition();

	cond->setLeft(this->left->clone());
	cond->setList(dynamic_cast<ExpressionListScanCondition*>(this->list->cloneCondition()));

	return cond;
}

void InExpressionScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	bool leftFilterable = this->left->isFilterable(vm, planner, detector);

	if(leftFilterable){
		this->list->detectFilterConditions(vm, planner, detector);
		if(!detector->isEmpty()){
			delete detector->pop();

			detector->push(cloneCondition());
		}
	}
}

void InExpressionScanCondition::detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner,
		TableIndexDetector* detector) const {
	if(this->left->isColumn() && this->list->isRecordIndependent()){
		OrIndexCandidate* orcandidate = new OrIndexCandidate();

		ColumnIdentifierScanParam* column = dynamic_cast<ColumnIdentifierScanParam*>(this->left);

		const ArrayList<IValueProvider>* list = this->list->getList();
		int maxLoop = list->size();
		for(int i = 0; i != maxLoop; ++i){
			IValueProvider* vp = list->get(i);

			IndexCandidate* candidate = new IndexCandidate(AbstractIndexCandidate::IndexType::EQUALS); __STP(candidate);
			candidate->setColumn(column);
			candidate->setValue(vp);

			orcandidate->add(candidate);
		}

		detector->push(orcandidate);
	}
}

void InExpressionScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void InExpressionScanCondition::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->left->analyzeConditions(vm, planner);
	this->list->analyzeConditions(vm, planner);
}

AbstractCdbValue* InExpressionScanCondition::evaluate(VirtualMachine* vm, const CdbRecord* record,
		const ScanResultMetadata* metadata) {
	bool result = false;

	AbstractCdbValue* value = dynamic_cast<AbstractScanConditionElement*>(this->left)->evaluate(vm ,record, metadata); __STP(value);
	AbstractCdbKey* key = value->toKey(); __STP(key);

	AbstractCdbValue* listvalue = this->list->evaluate(vm, record, metadata); __STP(listvalue);
	CdbValueList* list = dynamic_cast<CdbValueList*>(listvalue);

	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractCdbValue* v = list->get(i);
		AbstractCdbKey* k = v->toKey(); __STP(k);

		if(key->compareTo(k)){
			result = true;
			break;
		}
	}

	return new CdbBooleanValue(result);
}

bool InExpressionScanCondition::isRecordIndependent() const noexcept {
	return this->left->isRecordIndependent() && this->list->isRecordIndependent();
}

bool InExpressionScanCondition::isAvailable(VirtualMachine *vm) const {
	return this->left->isAvailable(vm) && this->list->isAvailable(vm);
}

bool InExpressionScanCondition::hasPlaceholder() const noexcept {
	return this->left->hasPlaceholder() && this->list->hasPlaceholder();
}

} /* namespace codablecash */
