/*
 * MultipleColumnIndex.cpp
 *
 *  Created on: 2020/11/26
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/MultipleValueProvidorScanParam.h"

#include "scan_select/scan_table/TableScanTarget.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableColumn.h"

#include "base/ArrayList.h"
#include "base/UnicodeString.h"

#include "engine/CdbOid.h"
#include "scan_select/scan_planner/scanner/index_resolv/MultipleColumnIndexAccess.h"
#include "scan_select/scan_planner/scanner/index_resolv/SingleColumnIndexAccess.h"

#include "schema_table/table/CdbTableIndex.h"

#include "scan_select/scan_condition/params/NullScanParam.h"

#include "scan_select/scan_condition/params/InfinityScanParam.h"

#include "scan_select/scan_planner/scanner/index_resolv/IndexScorer.h"
namespace codablecash {

MultipleColumnIndexAccess::MultipleColumnIndexAccess(const MultipleColumnIndexAccess& inst) : AbstractColumnsIndexAccess(inst) {
	this->value = nullptr;
	this->topValue = nullptr;

	int maxLoop = inst.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractColumnsIndexAccess* index = inst.list.get(i);
		SingleColumnIndexAccess* idx = dynamic_cast<SingleColumnIndexAccess*>(index->clone());

		assert(idx != nullptr);

		add(idx);
	}
}

MultipleColumnIndexAccess::MultipleColumnIndexAccess() {
	this->value = nullptr;
	this->topValue = nullptr;
}

MultipleColumnIndexAccess::~MultipleColumnIndexAccess() {
	this->list.deleteElements();

	delete this->value;
	delete this->topValue;
}

bool MultipleColumnIndexAccess::hasIndex(SchemaManager* schemaManager, VirtualMachine* vm) {
	const TableScanTarget* tableTarget = dynamic_cast<const TableScanTarget*>(this->target);
	const CdbTable* cdbTable = tableTarget->getTable();

	IndexScorer scorer;

	int maxLoop = size();
	for(int i = 0; i != maxLoop; ++i){
		SingleColumnIndexAccess* idx = this->list.get(i);

		scorer.add(idx);
	}

	this->index = cdbTable->findMostAvailableIndex(&scorer);
	if(this->index == nullptr){
		return false;
	}

	int indexLength = this->index->getColumnLength();
	if(indexLength > maxLoop){
		// index size > data list size
		makeLongerIndexAccess(vm);
		makeRange(vm);

		return this->index != nullptr;
	}

	// index size <= data list size
	makeIndexAccess(vm);
	makeRange(vm);

	return this->index != nullptr;
}

void MultipleColumnIndexAccess::makeRange(VirtualMachine* vm) {
	if(isRange(vm)){
		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			SingleColumnIndexAccess* access = this->list.get(i);
			access->toRangeAccess();
		}
	}
}

/**
 * index size <= data list size
 */
void MultipleColumnIndexAccess::makeIndexAccess(VirtualMachine* vm){
	ArrayList<SingleColumnIndexAccess> newList;
	ArrayList<SingleColumnIndexAccess> removeList;

	// remove unused
	{
		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			SingleColumnIndexAccess* idx = this->list.get(i);
			const CdbTableColumn* col = idx->getColumn();
			const CdbOid* oid = col->getOid();

			if(!this->index->hasColumnOid(oid)){
				removeList.addElement(idx);
			}
		}
	}

	// copy from index
	{
		const ArrayList<CdbOid>* collist = this->index->getColumns();

		int maxLoop = collist->size();
		for(int i = 0; i != maxLoop; ++i){
			const CdbOid* colId = collist->get(i);

			SingleColumnIndexAccess* access = getIndexAccess(colId);
			assert(access != nullptr);

			newList.addElement(access);
		}
	}

	// copy result
	{
		this->list.reset();
		int maxLoop = newList.size();
		for(int i = 0; i != maxLoop; ++i){
			SingleColumnIndexAccess* access = newList.get(i);

			this->list.addElement(access);
		}
	}

	// remove unused
	{
		int maxLoop = removeList.size();
		for(int i = 0; i != maxLoop; ++i){
			SingleColumnIndexAccess* access = removeList.get(i);
			delete access;
		}
	}
}

/**
 * index size > data list size
 */
void MultipleColumnIndexAccess::makeLongerIndexAccess(VirtualMachine* vm) {
	ArrayList<SingleColumnIndexAccess> newlist;
	const ArrayList<CdbOid>* collist = this->index->getColumns();

	// copy from index
	{
		int maxLoop = size();
		for(int i = 0; i != maxLoop; ++i){
			const CdbOid* colId = collist->get(i);

			SingleColumnIndexAccess* access = getIndexAccess(colId);
			assert(access != nullptr);

			newlist.addElement(access);
		}
	}
	{
		int dataSize = size();
		bool range = isRange(vm);
		int maxLoop = collist->size() - dataSize;
		for(int i = 0; i != maxLoop; ++i){
			NullScanParam nullparam;
			InfinityScanParam infinityParam;

			SingleColumnIndexAccess* access = new SingleColumnIndexAccess();
			access->setRange(true, vm);

			access->setValue(&nullparam);
			access->setTopValue(&infinityParam);

			newlist.addElement(access);
		}
	}

	// copy result
	{
		this->list.reset();
		int maxLoop = newlist.size();
		for(int i = 0; i != maxLoop; ++i){
			SingleColumnIndexAccess* access = newlist.get(i);

			this->list.addElement(access);
		}
	}
}

SingleColumnIndexAccess* MultipleColumnIndexAccess::getIndexAccess(const CdbOid *colId) const noexcept {
	SingleColumnIndexAccess* access = nullptr;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		SingleColumnIndexAccess* ac = this->list.get(i);

		const CdbOid* id = ac->getColumn()->getOid();
		if(id->equals(colId)){
			access = ac;
			break;
		}
	}

	return access;
}

int MultipleColumnIndexAccess::size() const noexcept {
	return this->list.size();
}

void MultipleColumnIndexAccess::add(SingleColumnIndexAccess* idx) noexcept {
	this->list.addElement(idx);
}

const UnicodeString* MultipleColumnIndexAccess::toCodeString(VirtualMachine* vm) noexcept {
	if(this->str == nullptr){
		makeCodeString(vm);
	}

	return this->str;
}

bool MultipleColumnIndexAccess::isBottomEq(VirtualMachine* vm) const noexcept {
	bool ret = true;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		SingleColumnIndexAccess* index = this->list.get(i);

		if(!index->isBottomEq(vm)){
			ret = false;
			break;
		}
	}

	return ret;
}

bool MultipleColumnIndexAccess::isTopEq(VirtualMachine* vm) const noexcept {
	bool ret = true;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		SingleColumnIndexAccess* index = this->list.get(i);

		if(!index->isTopEq(vm)){
			ret = false;
			break;
		}
	}

	return ret;
}

const IValueProvider* MultipleColumnIndexAccess::getTopValue(VirtualMachine* vm) const noexcept {
	if(this->topValue == nullptr){
		this->topValue = new MultipleValueProvidorScanParam();

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			SingleColumnIndexAccess* index = this->list.get(i);
			const IValueProvider* vp = index->getTopValue(vm);

			this->topValue->add(vp);
		}
	}

	return this->topValue;
}

const IValueProvider* MultipleColumnIndexAccess::getValue(VirtualMachine* vm) const noexcept {
	if(this->value == nullptr){
		this->value = new MultipleValueProvidorScanParam();

		int maxLoop = this->list.size();
		for(int i = 0; i != maxLoop; ++i){
			SingleColumnIndexAccess* index = this->list.get(i);
			const IValueProvider* vp = index->getValue(vm);

			this->value->add(vp);
		}
	}

	return this->value;
}

void MultipleColumnIndexAccess::makeCodeString(VirtualMachine* vm) noexcept {
	this->str = new UnicodeString(L"");

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		SingleColumnIndexAccess* index = this->list.get(i);

		if(i != 0){
			this->str->append(L" AND ");
		}

		const UnicodeString* s = index->toCodeString(vm);
		this->str->append(s);
	}
}

AbstractColumnsIndexAccess* MultipleColumnIndexAccess::clone() const noexcept {
	return new MultipleColumnIndexAccess(*this);
}

bool MultipleColumnIndexAccess::isRange(VirtualMachine* vm) const noexcept {
	bool ret = false;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		SingleColumnIndexAccess* index = this->list.get(i);

		if(index->isRange(vm)){
			ret = true;
			break;
		}
	}

	return ret;
}

} /* namespace codablecash */
