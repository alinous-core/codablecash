/*
 * ScanResultMetadata.cpp
 *
 *  Created on: 2020/06/13
 *      Author: iizuka
 */

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"
#include "trx/scan/transaction_scan_result/ScanResultFieldMetadata.h"

#include "base/UnicodeString.h"


namespace codablecash {


ScanResultMetadata::ScanResultMetadata(const ScanResultMetadata& inst) {
	this->list = new ArrayList<ScanResultFieldMetadata>();

	int maxLoop = inst.list->size();
	for(int i = 0; i != maxLoop; ++i){
		ScanResultFieldMetadata* fld = inst.list->get(i);

		addField(fld);
	}
}

ScanResultMetadata::ScanResultMetadata() {
	this->list = new ArrayList<ScanResultFieldMetadata>();
}


ScanResultMetadata::~ScanResultMetadata() {
	this->list->deleteElements();
	delete this->list;
}

void ScanResultMetadata::addField(const ScanResultFieldMetadata* fld) noexcept {
	ScanResultFieldMetadata* newFld = new ScanResultFieldMetadata(*fld);
	newFld->setPosition(this->list->size());

	this->list->addElement(newFld);
}

void ScanResultMetadata::join(const ScanResultMetadata* other) noexcept {
	const ArrayList<ScanResultFieldMetadata>* columns = other->getList();

	int maxLoop = columns->size();
	for(int i = 0; i != maxLoop; ++i){
		ScanResultFieldMetadata* fld = columns->get(i);

		addField(fld);
	}
}

const ScanResultFieldMetadata* ScanResultMetadata::findField(const ColumnIdentifierScanParam* scanColumnId) const noexcept {
	const ScanResultFieldMetadata* result = nullptr;

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		ScanResultFieldMetadata* fld = this->list->get(i);
		if(fld->match(scanColumnId)){
			result = fld;
			break;
		}
	}

	return result;
}


} /* namespace codablecash */
