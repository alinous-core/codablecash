/*
 * AndScanCondition.cpp
 *
 *  Created on: 2020/07/30
 *      Author: iizuka
 */

#include "scan_select/scan_condition/logical/AndScanCondition.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

#include "scan_select/scan_planner/scanner/join/JoinCandidateStackMarker.h"
#include "scan_select/scan_planner/scanner/join/JoinCandidateHolder.h"
#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"
#include "scan_select/scan_planner/scanner/join/JoinMultipleCandidate.h"

#include "scan_select/scan_planner/scanner/index/TableIndexDetector.h"
#include "scan_select/scan_planner/scanner/index/AbstractIndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/MultipleIndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetectorStackMarker.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetectorStack.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbBooleanValue.h"

#include "schema_table/record/table_record_value/CdbValueCaster.h"
using namespace alinous;

namespace codablecash {

AndScanCondition::AndScanCondition() : AbstractLogicalCondition(CodeElement::SQL_EXP_AND) {
	this->str = nullptr;
}

AndScanCondition::~AndScanCondition() {
	this->list.deleteElements();
	resetStr();
}

void AndScanCondition::addCondition(AbstractScanConditionElement* cond) {
	if(cond == nullptr){
		return;
	}
	this->list.addElement(cond);
	resetStr();
}

const UnicodeString* AndScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();

		this->str = new UnicodeString(L"");

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractScanConditionElement* cond = this->list.get(i);

			if(i != 0){
				this->str->append(L" AND ");
			}

			this->str->append(cond->toStringCode());
		}
	}

	return this->str;
}

AbstractScanCondition* AndScanCondition::cloneCondition() const noexcept {
	AndScanCondition* cond = new AndScanCondition();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cnd = this->list.get(i);

		cond->addCondition(dynamic_cast<AbstractScanConditionElement*>(cnd->clone()));
	}

	return cond;
}

void AndScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	ArrayList<AbstractScanCondition> scanList;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cnd = this->list.get(i);

		AbstractScanCondition* scanCondition = dynamic_cast<AbstractScanCondition*>(cnd);
		if(scanCondition != nullptr){
			scanCondition->detectFilterConditions(vm, planner, detector);
			if(!detector->isEmpty()){
				scanList.addElement(detector->pop());
			}
		}
	}

	maxLoop = scanList.size();
	if(maxLoop > 1){
		AndScanCondition* newAnd = new AndScanCondition();
		for(int i = 0; i != maxLoop; ++i){
			AbstractScanCondition* cnd = scanList.get(i);

			newAnd->addCondition(cnd);
		}

		detector->push(newAnd);
	}
	else if(maxLoop == 1){
		detector->push(scanList.get(0));
	}
}

void AndScanCondition::detectIndexCandidate(VirtualMachine* vm, SelectScanPlanner* planner, TableIndexDetector* detector) const {
	TableIndexDetectorStack* stack = detector->getStack();
	TableIndexDetectorStackMarker marker(stack);

	ArrayList<AbstractIndexCandidate> list;
	list.setDeleteOnExit();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cond = this->list.get(i);

		AbstractScanCondition* cnd = dynamic_cast<AbstractScanCondition*>(cond);
		if(cnd != nullptr){
			cnd->detectIndexCandidate(vm, planner, detector);

			if(!detector->isEmpty()){
				AbstractIndexCandidate* c = detector->pop();
				list.addElement(c);
			}
		}
	}

	if(list.isEmpty()){
		return;
	}
	else if(list.size() == 1){
		AbstractIndexCandidate* c = list.get(0);
		detector->push(c);

		list.reset();

		return;
	}

	// and
	AbstractIndexCandidate* candidate = list.get(0)->copy();

	maxLoop = list.size();
	for(int i = 1; i != maxLoop; ++i){
		const AbstractIndexCandidate* c = list.get(i);

		__STP(candidate);
		candidate = candidate->multiply(c);
	}

	detector->push(candidate);
}

void AndScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void AndScanCondition::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cond = this->list.get(i);

		cond->analyzeConditions(vm, planner);
	}
}

void AndScanCondition::collectJoinCandidate(VirtualMachine* vm,
		SelectScanPlanner* planner, int joinType,
		JoinCandidateHolder* jholder) {

	JoinCandidateStackMarker marker(jholder->getStack());

	ArrayList<AbstractJoinCandidate> operandList;
	operandList.setDeleteOnExit();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cond = this->list.get(i);

		AbstractScanCondition* scanCondition = dynamic_cast<AbstractScanCondition*>(cond);

		if(scanCondition != nullptr){
			scanCondition->collectJoinCandidate(vm, planner, joinType, jholder);
			if(!jholder->isEmpty()){
				AbstractJoinCandidate* c = jholder->pop();

				operandList.addElement(c);
			}
		}
	}

	int listSize = operandList.size();
	if(listSize > 0){
		if(listSize == 1){
			const AbstractJoinCandidate* c = operandList.get(0);
			jholder->push(c->copy());
		}
		else{
			AbstractJoinCandidate* multi = new JoinMultipleCandidate(joinType);

			for(int i = 0; i != listSize; ++i){
				AbstractJoinCandidate* c = operandList.get(i);

				AbstractJoinCandidate* newmulti = multi->multiply(c);

				delete multi;
				multi = newmulti;
			}

			jholder->push(multi);
		}

	}
}

AbstractCdbValue* AndScanCondition::evaluate(VirtualMachine* vm,
		const CdbRecord* record, const ScanResultMetadata* metadata) {
	bool ans = true;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cond = this->list.get(i);

		if(cond->isAvailable(vm) == false){
			continue;
		}

		AbstractCdbValue* value = cond->evaluate(vm, record, metadata); __STP(value);
		AbstractCdbValue* bl = CdbValueCaster::cast(value, AbstractCdbValue::TYPE_BOOLEAN); __STP(bl);

		CdbBooleanValue* blValue = dynamic_cast<CdbBooleanValue*>(bl);
		bool b =blValue->getValue();
		if(!b){
			ans = false;
			break;
		}
	}

	return new CdbBooleanValue(ans);
}

bool AndScanCondition::isRecordIndependent() const noexcept {
	int ret = true;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* vp = this->list.get(i);

		if(!vp->isRecordIndependent()){
			ret = false;
			break;
		}
	}

	return ret;
}

bool AndScanCondition::isAvailable(VirtualMachine *vm) const {
	int ret = false;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* vp = this->list.get(i);

		if(vp->isAvailable(vm)){
			ret = true;
			break;
		}
	}

	return ret;
}

bool AndScanCondition::hasPlaceholder() const noexcept {
	int ret = false;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* vp = this->list.get(i);

		if(vp->hasPlaceholder()){
			ret = true;
			break;
		}
	}

	return ret;
}

} /* namespace codablecash */
