/*
 * AbstractJoinScanTarget.cpp
 *
 *  Created on: 2020/08/09
 *      Author: iizuka
 */

#include "scan_select/scan_table/AbstractJoinScanTarget.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_condition/base/AbstractScanCondition.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

#include "engine/CdbException.h"

#include "base/StackRelease.h"
namespace codablecash {

AbstractJoinScanTarget::AbstractJoinScanTarget() {
	this->left = nullptr;
	this->right = nullptr;
	this->cond = nullptr;

	this->str = nullptr;
}

AbstractJoinScanTarget::~AbstractJoinScanTarget() {
	delete this->left;
	delete this->right;
	delete this->cond;

	resetStr();
}

void AbstractJoinScanTarget::setLeft(AbstractScanTableTarget* left) noexcept {
	this->left = left;
	resetStr();
}

void AbstractJoinScanTarget::setRight(AbstractScanTableTarget* right) noexcept {
	this->right = right;
	resetStr();
}

void AbstractJoinScanTarget::setCondition(AbstractScanCondition* cond) noexcept {
	this->cond = cond;
	resetStr();
}

void AbstractJoinScanTarget::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void AbstractJoinScanTarget::addConditionString() noexcept {
	if(this->cond != nullptr){
		this->str->append(L" ON ");
		this->str->append(this->cond->toStringCode());
	}
}

void AbstractJoinScanTarget::resolveTable(VirtualMachine* vm, SelectScanPlanner* planner) {
	if(this->metadata == nullptr){
		this->left->resolveTable(vm, planner);
		this->right->resolveTable(vm, planner);

		if(this->cond != nullptr){
			this->cond->analyzeConditions(vm, planner);
		}

		const ScanResultMetadata* lmetadata = this->left->getMetadata();
		this->metadata = new ScanResultMetadata(*lmetadata);

		lmetadata = this->right->getMetadata();
		this->metadata->join(lmetadata);
	}
}

bool AbstractJoinScanTarget::hasTarget(const AbstractScanTableTarget* target) const noexcept {
	return this == target || this->left->hasTarget(target) || this->right->hasTarget(target);
}

void AbstractJoinScanTarget::collectScanTargets(VirtualMachine* vm, SelectScanPlanner* planner, ArrayList<AbstractScanTableTarget>* list) {
	list->addElement(this);

	this->left->collectScanTargets(vm, planner, list);
	this->right->collectScanTargets(vm, planner, list);
}

ScanTableColumnParam* AbstractJoinScanTarget::findTableColumns(const UnicodeString* colName) const {
	ScanTableColumnParam* pleft = this->left->findTableColumns(colName);
	ScanTableColumnParam* pright = this->right->findTableColumns(colName);

	StackRelease<ScanTableColumnParam> stpleft(pleft);
	StackRelease<ScanTableColumnParam> stpright(pright);

	if(pleft != nullptr && pright != nullptr){
		throw new CdbException(L"The column is ambiguous.", __FILE__, __LINE__);
	}

	return pleft != nullptr ? stpleft.move() : (pright != nullptr ? stpright.move() : nullptr);
}

} /* namespace codablecash */
