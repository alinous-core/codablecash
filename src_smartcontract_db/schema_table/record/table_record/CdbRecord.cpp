/*
 * CdbRecord.cpp
 *
 *  Created on: 2020/05/19
 *      Author: iizuka
 */

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/CdbDataFactory.h"
#include "schema_table/record/table_record/ScanRecordIndentity.h"

#include "schema_table/record/table_record_value/VmInstanceValueConverter.h"
#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "filestore_block/IBlockObject.h"

#include "engine/CdbOid.h"

#include "instance/instance_dom/DomVariableInstance.h"

#include "instance/instance_string/VmStringInstance.h"


namespace codablecash {


CdbRecord::CdbRecord(const CdbRecord& inst) : AbstractCdbValue(AbstractCdbValue::TYPE_RECORD) {
	this->oid = inst.oid != nullptr ? inst.oid->copy() : nullptr;

	int maxLoop = inst.list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractCdbValue* v = inst.list.get(i);

		addValue(v != nullptr ? v->copy() : nullptr);
	}

	this->scanIdentity = nullptr;
	if(inst.scanIdentity != nullptr){
		this->scanIdentity = dynamic_cast<ScanRecordIndentity*>(inst.scanIdentity->copy());
	}
}

CdbRecord::CdbRecord() : AbstractCdbValue(AbstractCdbValue::TYPE_RECORD) {
	this->oid = nullptr;
	this->scanIdentity = nullptr;
}

CdbRecord::~CdbRecord() {
	this->list.deleteElements();
	delete this->oid;

	delete this->scanIdentity;
}

void CdbRecord::initNullColumns(int num) noexcept {
	for(int i = 0; i != num; ++i){
		addValue(nullptr);
	}
}

void CdbRecord::initBlankScanIdentity() noexcept {
	delete this->scanIdentity;
	this->scanIdentity = new ScanRecordIndentity();
}

void CdbRecord::addValue(AbstractCdbValue* value) noexcept {
	this->list.addElement(value);
}

void CdbRecord::setValue(AbstractCdbValue* value, int i) noexcept {
	delete this->list.get(i);
	this->list.setElement(value, i);
}

void CdbRecord::removeColumnValue(int pos) noexcept {
	AbstractCdbValue* value = this->list.get(pos);

	this->list.remove(pos);
	delete value;
}

void CdbRecord::insertValue(AbstractCdbValue *value, int pos) noexcept {
	int lastIndex = this->list.size();
	this->list.addElement(nullptr);
	for(int i = lastIndex; i != pos; --i){
		AbstractCdbValue* v = this->list.get(i - 1);
		this->list.setElement(v, i);
	}

	this->list.setElement(value, pos);
}

int CdbRecord::binarySize() const {
	int total = sizeof(int8_t);

	total += sizeof(uint8_t);
	bool notnull = this->oid != nullptr;
	if(notnull){
		total += this->oid->binarySize();
	}

	int maxLoop = this->list.size();
	total += sizeof(int32_t);

	for(int i = 0; i != maxLoop; ++i){
		AbstractCdbValue* value = this->list.get(i);

		if(value == nullptr){
			total += sizeof(uint8_t);
		}
		else{
			total += value->binarySize();
		}
	}

	total += sizeof(uint8_t);
	notnull = this->scanIdentity != nullptr;
	if(notnull){
		total += this->scanIdentity->binarySize();
	}

	return total;
}

void CdbRecord::toBinary(ByteBuffer* out) const {
	out->put(this->type);

	bool notnull = this->oid != nullptr;
	out->put(notnull ? 1 : 0);

	if(notnull){
		this->oid->toBinary(out);
	}

	int maxLoop = this->list.size();
	out->putInt(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		AbstractCdbValue* value = this->list.get(i);

		if(value == nullptr){
			out->put((char)0);
		}
		else{
			value->toBinary(out);
		}
	}

	notnull = this->scanIdentity != nullptr;
	out->put(notnull ? 1 : 0);

	if(notnull){
		this->scanIdentity->toBinary(out);
	}
}

void CdbRecord::setOid(const CdbOid* oid) noexcept {
	delete this->oid;
	this->oid = oid != nullptr ? oid->copy() : nullptr;
}

void CdbRecord::setOid(const uint64_t oid) noexcept {
	delete this->oid;
	this->oid = new CdbOid(oid);
}

CdbRecord* CdbRecord::createFromBinary(ByteBuffer* in) {
	CdbDataFactory factory;
	CdbRecord* record = new CdbRecord();

	in->get(); // type

	uint8_t bl = in->get();

	if(bl != 0){
		record->oid = CdbOid::fromBinary(in);
	}

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		IBlockObject* blockObj = factory.makeDataFromBinary(in);
		AbstractCdbValue* value = dynamic_cast<AbstractCdbValue*>(blockObj);

		record->addValue(value);
	}

	bl = in->get();
	if(bl != 0){
		IBlockObject* blockObj = factory.makeDataFromBinary(in);
		record->scanIdentity = dynamic_cast<ScanRecordIndentity*>(blockObj);
	}

	return record;
}

void CdbRecord::fromBinary(ByteBuffer* in) {
	uint8_t bl = in->get();
	if(bl != 0){
		this->oid = CdbOid::fromBinary(in);
	}

	CdbDataFactory factory;
	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		IBlockObject* blockObj = factory.makeDataFromBinary(in);
		AbstractCdbValue* value = dynamic_cast<AbstractCdbValue*>(blockObj);

		addValue(value);
	}

	bl = in->get();
	if(bl != 0){
		IBlockObject* blockObj = factory.makeDataFromBinary(in);
		this->scanIdentity = dynamic_cast<ScanRecordIndentity*>(blockObj);
	}
}

const ArrayList<AbstractCdbValue>* CdbRecord::getValues() const noexcept {
	return &this->list;
}

AbstractCdbKey* CdbRecord::toKey() const noexcept {
	CdbRecordKey* key = new CdbRecordKey();

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractCdbValue* value = this->list.get(i);

		AbstractCdbKey* k = (value == nullptr) ? nullptr : value->toKey();
		key->addKey(k);
	}

	return key;
}

AbstractCdbValue* CdbRecord::copy() const noexcept {
	return new CdbRecord(*this);
}

void CdbRecord::initScanIdentity() noexcept {
	clearScanIdentity();
	importScanIdentity();
}

void CdbRecord::importScanIdentity() noexcept {
	if(this->scanIdentity == nullptr){
		this->scanIdentity = new ScanRecordIndentity();
	}

	assert(this->oid != nullptr);
	this->scanIdentity->addOid(this->oid);
}

CdbRecord* CdbRecord::joinRecord(const CdbRecord* record) const noexcept {
	CdbRecord* ret = new CdbRecord(*this);

	int maxLoop = record->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractCdbValue* value = record->list.get(i);

		ret->addValue(value != nullptr ? value->copy() : nullptr);
	}

	ret->scanIdentity->addOids(record->getScanIdentity());

	return ret;
}

CdbRecord* CdbRecord::joinEmptyRecord(int numNull) const noexcept {
	CdbRecord* ret = new CdbRecord(*this);

	for(int i = 0; i != numNull; ++i){
		ret->addValue(nullptr);
	}

	return ret;
}

const AbstractCdbValue* CdbRecord::get(int pos) const noexcept {
	return this->list.get(pos);
}

DomVariableInstance* CdbRecord::toDomInstance(VirtualMachine* vm, ArrayList<UnicodeString>* nameList) const {
	DomVariableInstance* dom = new(vm) DomVariableInstance(vm);

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractCdbValue* value = this->list.get(i);
		const UnicodeString* name = nameList->get(i);
		VmStringInstance* vmString = new(vm) VmStringInstance(vm, name); __STP(vmString);

		if(dom->hasProperty(vmString)){
			continue;
		}

		IAbstractVmInstanceSubstance* inst = VmInstanceValueConverter::toVmInstance(vm, value);
		dom->putProperty(vm, vmString, inst);
	}

	return dom;
}

void CdbRecord::clearScanIdentity() noexcept {
	delete this->scanIdentity;
	this->scanIdentity = nullptr;
}

bool CdbRecord::strictEqualsValue(const AbstractCdbValue *v) const {
	const CdbRecord* other = dynamic_cast<const CdbRecord*>(v);

	if(!this->oid->equals(other->oid)){
		return false;
	}

	int maxLoop = other->list.size();
	if(maxLoop != this->list.size()){
		return false;
	}

	bool ret = true;
	for(int i = 0; i != maxLoop; ++i){
		AbstractCdbValue* otherValue = other->list.get(i);
		AbstractCdbValue* value = this->list.get(i);

		if(!otherValue->strictEquals(value)){
			ret = false;
			break;
		}
	}

	return ret;
}

} /* namespace codablecash */
