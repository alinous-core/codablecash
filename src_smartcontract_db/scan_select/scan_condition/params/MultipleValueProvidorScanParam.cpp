/*
 * MultipleValueProvidorScanParam.cpp
 *
 *  Created on: 2021/01/12
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/MultipleValueProvidorScanParam.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "base/UnicodeString.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "scan_select/scan_condition/base/AbstractScanConditionElement.h"
namespace codablecash {

MultipleValueProvidorScanParam::MultipleValueProvidorScanParam(const MultipleValueProvidorScanParam& inst) {
	this->str = nullptr;

	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		const IValueProvider* vp = inst.list.get(i);

		add(vp);
	}
}

MultipleValueProvidorScanParam::MultipleValueProvidorScanParam() {
	this->str = nullptr;
}

MultipleValueProvidorScanParam::~MultipleValueProvidorScanParam() {
	this->list.deleteElements();
	delete this->str;
}

const UnicodeString* MultipleValueProvidorScanParam::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			IValueProvider* vp = this->list.get(i);

			if(i > 0){
				this->str->append(L", ");
			}

			const UnicodeString* s = vp->toStringCode();
			this->str->append(s);
		}
	}

	return this->str;
}

IValueProvider* MultipleValueProvidorScanParam::clone() const noexcept {
	return new MultipleValueProvidorScanParam(*this);
}

void MultipleValueProvidorScanParam::add(const IValueProvider* value) noexcept {
	this->list.addElement(value->clone());
}

bool MultipleValueProvidorScanParam::isRecordIndependent() const noexcept {
	bool ret = true;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IValueProvider* vp = this->list.get(i);

		if(!vp->isRecordIndependent()){
			ret = false;
			break;
		}
	}

	return ret;
}

AbstractCdbValue* MultipleValueProvidorScanParam::evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	CdbRecord* r = new CdbRecord();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		IValueProvider* vp = this->list.get(i);
		AbstractScanConditionElement* element = dynamic_cast<AbstractScanConditionElement*>(vp);
		assert(element != nullptr);

		AbstractCdbValue* v = element->evaluate(vm ,record, metadata);
		r->addValue(v);
	}

	return r;
}

} /* namespace codablecash */
