/*
 * AbstractScanColumns.cpp
 *
 *  Created on: 2020/08/19
 *      Author: iizuka
 */

#include "scan_select/scan_columns/AbstractScanColumnsTarget.h"

#include "base/UnicodeString.h"

namespace codablecash {

AbstractScanColumnsTarget::AbstractScanColumnsTarget() {
	this->asName = nullptr;
	this->str = nullptr;
}

AbstractScanColumnsTarget::~AbstractScanColumnsTarget() {
	delete this->asName;
	resetStr();
}

void AbstractScanColumnsTarget::setAsName(const UnicodeString* asName) noexcept {
	delete this->asName;
	this->asName = nullptr;

	if(asName != nullptr){
		this->asName = new UnicodeString(asName);
	}

}

void AbstractScanColumnsTarget::resetStr() noexcept {
	if(this->str != nullptr){
		delete this->str;
		this->str = nullptr;
	}
}

void AbstractScanColumnsTarget::buildNameList(ArrayList<UnicodeString>* list, const ScanResultMetadata* metadata) const noexcept {
	if(this->asName != nullptr){
		list->addElement(new UnicodeString(*this->asName));
		return;
	}

	list->addElement(new UnicodeString(*this->str));
}


} /* namespace codablecash */
