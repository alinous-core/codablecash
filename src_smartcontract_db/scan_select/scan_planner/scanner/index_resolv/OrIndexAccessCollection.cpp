/*
 * OrIndexWrapperCollection.cpp
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#include "base/UnicodeString.h"
#include "base/ArrayList.h"
#include "scan_select/scan_planner/scanner/index_resolv/AbstractColumnsIndexAccess.h"
#include "scan_select/scan_planner/scanner/index_resolv/OrIndexAccessCollection.h"


namespace codablecash {

OrIndexAccessCollection::OrIndexAccessCollection(const OrIndexAccessCollection& inst) : AbstractColumnsIndexAccess(inst) {
	int maxLoop = inst.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractColumnsIndexAccess* index = inst.list.get(i);

		add(index->clone());
	}
}

OrIndexAccessCollection::OrIndexAccessCollection() {

}

OrIndexAccessCollection::~OrIndexAccessCollection() {
	this->list.deleteElements();
}

bool OrIndexAccessCollection::hasIndex(SchemaManager* schemaManager, VirtualMachine* vm) {
	int maxLoop = size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractColumnsIndexAccess* index = this->list.get(i);

		if(!index->hasIndex(schemaManager, vm)){
			return false;
		}
	}

	return true;
}

int OrIndexAccessCollection::size() const noexcept {
	return this->list.size();
}

void OrIndexAccessCollection::add(AbstractColumnsIndexAccess* index) noexcept {
	this->list.addElement(index);
}

AbstractColumnsIndexAccess* OrIndexAccessCollection::get(int i) const noexcept {
	return this->list.get(i);
}

const UnicodeString* OrIndexAccessCollection::toCodeString(VirtualMachine* vm) noexcept {
	if(this->str == nullptr){
		makeCodeString(vm);
	}

	return this->str;
}

void OrIndexAccessCollection::makeCodeString(VirtualMachine* vm) noexcept {
	this->str = new UnicodeString(L"");

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractColumnsIndexAccess* index = this->list.get(i);

		if(i != 0){
			this->str->append(L" OR ");
		}

		const UnicodeString* s = index->toCodeString(vm);
		this->str->append(s);
	}
}

AbstractColumnsIndexAccess* OrIndexAccessCollection::clone() const noexcept {
	return new OrIndexAccessCollection(*this);
}

} /* namespace codablecash */
