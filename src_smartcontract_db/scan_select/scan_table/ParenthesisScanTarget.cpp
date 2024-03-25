/*
 * ParenthesisScanTarget.cpp
 *
 *  Created on: 2020/08/09
 *      Author: iizuka
 */

#include "scan_select/scan_table/ParenthesisScanTarget.h"

#include "base/UnicodeString.h"

#include "scan_select/scan_planner/analyze/AnalyzedScanPlan.h"
#include "scan_select/scan_planner/analyze/ScanTargetNameResolver.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"

namespace codablecash {

ParenthesisScanTarget::ParenthesisScanTarget() {
	this->inner = nullptr;
}

ParenthesisScanTarget::~ParenthesisScanTarget() {
	delete this->inner;
}

const UnicodeString* ParenthesisScanTarget::toString() noexcept {
	if(this->str == nullptr){
		resetStr();
		this->str = new UnicodeString(L"(");

		this->str->append(this->inner->toString());

		this->str->append(L")");
	}

	return this->str;
}

void ParenthesisScanTarget::setInner(AbstractScanTableTarget* inner) noexcept {
	this->inner = inner;
}

void ParenthesisScanTarget::resolveTable(VirtualMachine* vm, SelectScanPlanner* planner) {
	this->inner->resolveTable(vm, planner);

	const ScanResultMetadata* inMeta = this->inner->getMetadata();
	this->metadata = new ScanResultMetadata(*inMeta);
}

void ParenthesisScanTarget::collectScanTargets(VirtualMachine* vm,
		SelectScanPlanner* planner, ArrayList<AbstractScanTableTarget>* list) {
	this->inner->collectScanTargets(vm, planner, list);
}

bool ParenthesisScanTarget::hasTarget(const AbstractScanTableTarget* target) const noexcept {
	return this->inner->hasTarget(target);
}

ScanTableColumnParam* ParenthesisScanTarget::findTableColumns(const UnicodeString* colName) const {
	return this->inner->findTableColumns(colName);
}

AbstractScannerFactory* ParenthesisScanTarget::getScanFactory(VirtualMachine* vm, SelectScanPlanner* planner) {
	return this->inner->getScanFactory(vm, planner);
}


} /* namespace codablecash */
