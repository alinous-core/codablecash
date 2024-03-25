/*
 * OrScanCondition.cpp
 *
 *  Created on: 2020/07/30
 *      Author: iizuka
 */

#include "scan_select/scan_condition/logical/OrScanCondition.h"

#include "engine/sc/CodeElement.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"
#include "scan_select/scan_planner/scanner/ctx/FilterConditionStackMarker.h"

#include "scan_select/scan_planner/scanner/join/JoinCandidateStackMarker.h"
#include "scan_select/scan_planner/scanner/join/JoinCandidateHolder.h"
#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"
#include "scan_select/scan_planner/scanner/join/JoinOrCandidate.h"

#include "scan_select/scan_planner/scanner/index/TableIndexDetectorStack.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetectorStackMarker.h"
#include "scan_select/scan_planner/scanner/index/TableIndexDetector.h"

#include "scan_select/scan_planner/scanner/index/AbstractIndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/OrIndexCandidate.h"
#include "scan_select/scan_planner/scanner/index/AbstractIndexCandidateCollection.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbValueCaster.h"
#include "schema_table/record/table_record_value/CdbBooleanValue.h"

using namespace alinous;

namespace codablecash {

OrScanCondition::OrScanCondition() : AbstractLogicalCondition(CodeElement::SQL_EXP_OR) {
	this->str = nullptr;
}

OrScanCondition::~OrScanCondition() {
	this->list.deleteElements();
	resetStr();
}

void OrScanCondition::addCondition(AbstractScanConditionElement* cond) {
	this->list.addElement(cond);
	resetStr();
}

const UnicodeString* OrScanCondition::toStringCode() noexcept {
	if(this->str == nullptr){
		resetStr();

		this->str = new UnicodeString(L"");

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractScanConditionElement* cond = this->list.get(i);

			if(i != 0){
				this->str->append(L" OR ");
			}

			this->str->append(cond->toStringCode());
		}
	}

	return this->str;
}

AbstractScanCondition* OrScanCondition::cloneCondition() const noexcept {
	OrScanCondition* cond = new OrScanCondition();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cnd = this->list.get(i);

		cond->addCondition(dynamic_cast<AbstractScanConditionElement*>(cnd->clone()));
	}

	return cond;
}

void OrScanCondition::detectFilterConditions(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const {
	FilterConditionStackMarker marker(detector->getStack());

	bool allavailable = true;
	ArrayList<AbstractScanCondition> scanList;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cond = this->list.get(i);

		AbstractScanCondition* scanCond = dynamic_cast<AbstractScanCondition*>(cond);
		if(scanCond != nullptr){
			scanCond->detectFilterConditions(vm, planner, detector);
			if(detector->isEmpty()){
				allavailable = false;
				break;
			}

			scanList.addElement(detector->pop());
		}
	}

	if(!allavailable){
		scanList.deleteElements();
		return;
	}

	OrScanCondition* newOr = new OrScanCondition();

	maxLoop = scanList.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanCondition* cond = scanList.get(i);

		newOr->addCondition(cond);
	}

	detector->push(newOr);
}

void OrScanCondition::detectIndexCandidate(VirtualMachine* vm,
		SelectScanPlanner* planner, TableIndexDetector* detector) const {
	TableIndexDetectorStack* stack = detector->getStack();
	TableIndexDetectorStackMarker marker(stack);

	ArrayList<AbstractIndexCandidate> list;
	list.setDeleteOnExit();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cond = this->list.get(i);

		AbstractScanCondition* scanCond = dynamic_cast<AbstractScanCondition*>(cond);
		if(scanCond !=  nullptr){
			scanCond->detectIndexCandidate(vm, planner, detector);

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

		return;
	}

	// or
	OrIndexCandidate* candidate = new OrIndexCandidate();

	maxLoop = list.size();
	for(int i = 0; i != maxLoop; ++i){
		const AbstractIndexCandidateCollection* c = dynamic_cast<const AbstractIndexCandidateCollection*>(list.get(i));
		assert(c != nullptr);

		candidate->add(c);
	}

	detector->push(candidate);
}

void OrScanCondition::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void OrScanCondition::analyzeConditions(VirtualMachine* vm,	SelectScanPlanner* planner) {
	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cond = this->list.get(i);

		cond->analyzeConditions(vm, planner);
	}
}

void OrScanCondition::collectJoinCandidate(VirtualMachine* vm,
		SelectScanPlanner* planner, int joinType, JoinCandidateHolder* jholder) {

	JoinCandidateStackMarker marker(jholder->getStack());

	ArrayList<AbstractJoinCandidate> operandList;
	operandList.setDeleteOnExit();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractScanConditionElement* cond = this->list.get(i);

		AbstractScanCondition* scanCond = dynamic_cast<AbstractScanCondition*>(cond);
		if(scanCond !=  nullptr){
			scanCond->collectJoinCandidate(vm, planner, joinType, jholder);

			if(!jholder->isEmpty()){
				AbstractJoinCandidate* c = jholder->pop();

				operandList.addElement(c);
			}
			else{
				return;
			}
		}
	}

	JoinOrCandidate* candidate = new JoinOrCandidate(joinType);

	int listSize = operandList.size();
	for(int i = 0; i != listSize; ++i){
		AbstractJoinCandidate* c = operandList.get(i);

		candidate->add(c);
	}

	jholder->push(candidate);
}

AbstractCdbValue* OrScanCondition::evaluate(VirtualMachine* vm,
		const CdbRecord* record, const ScanResultMetadata* metadata) {
	bool ans = false;

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
		if(b){
			ans = true;
			break;
		}
	}

	return new CdbBooleanValue(ans);
}

bool OrScanCondition::isRecordIndependent() const noexcept {
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

bool OrScanCondition::isAvailable(VirtualMachine *vm) const {
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

bool OrScanCondition::hasPlaceholder() const noexcept {
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
