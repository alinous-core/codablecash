/*
 * CdbTableIndex.cpp
 *
 *  Created on: 2020/05/13
 *      Author: iizuka
 */

#include "schema_table/table/CdbTableIndex.h"
#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/CdbTable.h"

#include "engine/CdbOid.h"

#include "trx/transaction/SchemaObjectIdPublisher.h"

#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

#include "schema_table/schema/DatabaseEqualsChecker.h"
namespace codablecash {

CdbTableIndex::CdbTableIndex(const CdbTableIndex& inst) {
	this->oid = new CdbOid(*inst.oid);

	this->columns = new ArrayList<CdbOid>();

	{
		int maxLoop = inst.columns->size();
		for(int i = 0; i != maxLoop; ++i){
			CdbOid* col = inst.columns->get(i)->copy();

			this->columns->addElement(col);
		}
	}

	this->primary = inst.primary;
	this->unique = inst.unique;
	this->name = new UnicodeString(inst.name);
}

CdbTableIndex::CdbTableIndex(uint64_t oid) {
	this->oid = new CdbOid(oid);
	this->columns = new ArrayList<CdbOid>();
	this->primary = false;
	this->unique = false;
	this->name = nullptr;
}

CdbTableIndex::~CdbTableIndex() {
	delete this->oid;

	this->columns->deleteElements();
	delete this->columns;

	delete this->name;
}

UnicodeString* CdbTableIndex::createPrimaryKeyIndexName(CdbTableIndex* index, CdbTable* table) noexcept {
	UnicodeString* newName = new UnicodeString(L"idx_");

	const UnicodeString* tableName = table->getName();
	newName->append(tableName);

	const ArrayList<CdbOid>* list = index->getColumns();
	int maxLoop = list->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* colOid = list->get(i);
		CdbTableColumn* col = table->getColumn(colOid);
		const UnicodeString*  colName = col->getName();

		newName->append(L"_");
		newName->append(colName);
	}

	return newName;
}

UnicodeString* CdbTableIndex::createUniqueKeyIndexName(CdbTable* table, const UnicodeString* colName) noexcept {
	UnicodeString* newName = new UnicodeString(L"idx_unique_");

	const UnicodeString* tableName = table->getName();
	newName->append(tableName);

	newName->append(L"_");
	newName->append(colName);

	return newName;
}

void CdbTableIndex::setName(UnicodeString* name) noexcept {
	delete this->name;
	this->name = name;
}

void CdbTableIndex::assignNewOid(SchemaObjectIdPublisher* publisher) {
	uint64_t oid = publisher->newOid();
	setOid(oid);
}

// sync local to public
void CdbTableIndex::syncColumnOid(const CdbTable* table, const CdbOid *lastOid, const CdbOid *newOid) {
	int maxLoop = this->columns->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* oid = this->columns->get(i);

		if(oid->equals(lastOid)){
			delete oid;
			this->columns->setElement(newOid->copy(), i);
		}
	}
}

void CdbTableIndex::setOid(uint64_t oid) noexcept {
	delete this->oid;
	this->oid = new CdbOid(oid);
}

void CdbTableIndex::addColumn(const CdbOid* colId) noexcept {
	CdbOid* newColumn = colId->copy();
	this->columns->addElement(newColumn);
}


bool CdbTableIndex::hasColumnOid(const CdbOid* colOid) const noexcept {
	bool ret = false;

	int maxLoop = this->columns->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* oid = this->columns->get(i);

		if(oid->equals(colOid)){
			ret = true;
			break;
		}
	}

	return ret;
}

int CdbTableIndex::getColumnCoverCount(const ArrayList<const CdbOid>* oidlist) const noexcept {
	int count = 0;

	int maxLoop = this->columns->size();
	for(int i = 0; i != maxLoop; ++i){
		const CdbOid* colOid = this->columns->get(i);

		if(!hasOid(oidlist, colOid)){
			break;
		}
		count++;
	}

	return count;
}

bool CdbTableIndex::hasOid(const ArrayList<const CdbOid> *oidlist, const CdbOid *oid) const noexcept {
	bool ret = false;

	int maxLoop = oidlist->size();
	for(int i = 0; i != maxLoop; ++i){
		const CdbOid* id = oidlist->get(i);

		if(oid->equals(id)){
			ret = true;
			break;
		}
	}

	return ret;
}

int CdbTableIndex::getColumnLength() const noexcept {
	return this->columns->size();
}

void CdbTableIndex::setPrimaryKey(bool bl) {
	this->primary = bl;
}

bool CdbTableIndex::isPrimaryKey() const noexcept {
	return this->primary;
}

void CdbTableIndex::setUnique(bool unique) noexcept {
	this->unique = unique;
}

bool CdbTableIndex::isUnique() const noexcept {
	return this->unique;
}


int CdbTableIndex::binarySize() const {
	int total = sizeof(uint8_t);
	total += sizeof(uint64_t); // oid

	total += stringSize(this->name); // name

	total += sizeof(uint8_t) * 2; // primary unique

	int maxLoop = this->columns->size();
	total += sizeof(int32_t);

	total += maxLoop * sizeof(uint64_t);

	return total;
}

void CdbTableIndex::toBinary(ByteBuffer* out) const {
	out->put(CdbTableIndex::CDB_OBJ_TYPE);
	out->putLong(this->oid->getOidValue());

	putString(out, this->name); //name

	out->put(this->primary ? 1 : 0);// primary
	out->put(this->unique ? 1 : 0);// unique

	int maxLoop = this->columns->size();
	out->putInt(maxLoop);
	for(int i = 0; i != maxLoop; ++i){
		const CdbOid* oid = this->columns->get(i);
		uint64_t id = oid->getOidValue();

		out->putLong(id);
	}
}

void CdbTableIndex::fromBinary(ByteBuffer* in, CdbTable* table) {
	this->name = getString(in);

	this->primary = in->get() > 0;
	this->unique = in->get() > 0;

	int maxLoop = in->getInt();
	for(int i = 0; i != maxLoop; ++i){
		uint64_t id = in->getLong();
		CdbOid oid(id);

		addColumn(&oid);
	}
}

const CdbOid* CdbTableIndex::getColumnAt(int idxColumnPos) const noexcept {
	return this->columns->get(idxColumnPos);
}

void CdbTableIndex::__testCheckEquals(CdbStorageManager *storeManager, CdbTableIndex *other, CdbStorageManager *otherStoreManager) {
	DatabaseEqualsChecker::checkCdbOidEquals(this->oid, other->oid);
	DatabaseEqualsChecker::checkUnicodeStringEquals(this->name, other->name);
	DatabaseEqualsChecker::checkBoolEquals(this->primary, other->primary);
	DatabaseEqualsChecker::checkBoolEquals(this->unique, other->unique);

	DatabaseEqualsChecker::checkIntEquals(this->columns->size(), other->columns->size());
	{
		int maxLoop = this->columns->size();
		for(int i = 0; i != maxLoop; ++i){
			CdbOid* col = this->columns->get(i);
			CdbOid* otherCol = other->columns->get(i);

			DatabaseEqualsChecker::checkCdbOidEquals(col, otherCol);
		}
	}
}

} /* namespace codablecash */
