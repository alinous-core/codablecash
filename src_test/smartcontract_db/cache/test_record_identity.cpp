/*
 * test_record_identity.cpp
 *
 *  Created on: 2022/01/26
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "schema_table/record/table_record_local/LocalOidFactory.h"
#include "schema_table/record/table_record_local/LocalCdbOid.h"

#include "schema_table/record/table_record/CdbRecord.h"
#include "schema_table/record/table_record/ScanRecordIndentity.h"
#include "schema_table/record/table_record/CdbDataFactory.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include "schema_table/record/table_record_value/CdbIntValue.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record/CdbKeyFactory.h"

#include "schema_table/record/table_record/ScanRecordIndentityKey.h"

#include "btreekey/InfinityKey.h"

#include "schema_table/record/table_record_key/CdbNullKey.h"
using namespace codablecash;


static bool checkBinary(ByteBuffer* buff){
	CdbDataFactory factory;

	int lastSize = buff->capacity();

	buff->position(0);
	IBlockObject* element = factory.makeDataFromBinary(buff); __STP(element);

	int size = element->binarySize();
	if(lastSize != size){
		return false;
	}

	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(size, true); __STP(buff2);
	element->toBinary(buff2);

	if(buff2->position() != size){
		return false;
	}

	return Mem::memcmp(buff->array(), buff2->array(), size) == 0;
}

static bool checkBinaryRecord(ByteBuffer* buff){
	CdbDataFactory factory;

	int lastSize = buff->capacity();

	buff->position(0);
	IBlockObject* element = CdbRecord::createFromBinary(buff); __STP(element);

	int size = element->binarySize();
	if(lastSize != size){
		return false;
	}

	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(size, true); __STP(buff2);
	element->toBinary(buff2);

	if(buff2->position() != size){
		return false;
	}

	return Mem::memcmp(buff->array(), buff2->array(), size) == 0;
}

static bool checkBinaryKey(ByteBuffer* buff){
	CdbKeyFactory factory;

	int lastSize = buff->capacity();

	buff->position(0);
	uint32_t keyType = buff->getInt();
	IBlockObject* element = factory.fromBinary(keyType, buff); __STP(element);

	int size = element->binarySize();
	if(lastSize != size){
		return false;
	}

	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(size, true); __STP(buff2);
	element->toBinary(buff2);

	if(buff2->position() != size){
		return false;
	}

	return Mem::memcmp(buff->array(), buff2->array(), size) == 0;
}


TEST_GROUP(TestRecordIdentityGroup) {
	TEST_SETUP() {
	}
	TEST_TEARDOWN() {
	}
};

TEST(TestRecordIdentityGroup, case01){
	LocalOidFactory factory;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(1));

	LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

	rec.setOid(loid);
	rec.importScanIdentity();

	CdbRecord* rec2 = new CdbRecord(rec); __STP(rec2);

	ScanRecordIndentity* id = rec.getScanIdentity();
	ScanRecordIndentity* id2 = rec2->getScanIdentity();

	AbstractCdbKey* key = id->toKey(); __STP(key);
	AbstractCdbKey* key2 = id2->toKey(); __STP(key2);

	int result = key->compareTo(key2);
	CHECK(result == 0);
}

TEST(TestRecordIdentityGroup, case02){
	LocalOidFactory factory;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(1));

	LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

	rec.setOid(loid);
	rec.importScanIdentity();

	CdbRecord* rec2 = new CdbRecord(); __STP(rec2);

	LocalCdbOid* loid2 = factory.createLocalOid(); __STP(loid2);
	rec2->setOid(loid2);
	rec2->importScanIdentity();

	ScanRecordIndentity* id = rec.getScanIdentity();
	ScanRecordIndentity* id2 = rec2->getScanIdentity();

	AbstractCdbKey* key = id->toKey(); __STP(key);
	AbstractCdbKey* key2 = id2->toKey(); __STP(key2);

	int result = key->compareTo(key2);
	CHECK(result < 0);
}

TEST(TestRecordIdentityGroup, binary01){
	LocalOidFactory factory;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(1));

	LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

	rec.setOid(loid);
	rec.importScanIdentity();

	int size = rec.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	rec.toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res);
}

TEST(TestRecordIdentityGroup, binary02){
	LocalOidFactory factory;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(1));

	LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

	rec.setOid(loid);
	rec.importScanIdentity();

	int size = rec.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	rec.toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinaryRecord(buff);
	CHECK(res);
}

TEST(TestRecordIdentityGroup, binaryKey01){
	LocalOidFactory factory;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(1));

	LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

	rec.setOid(loid);
	rec.importScanIdentity();

	AbstractCdbKey* key = rec.toKey(); __STP(key);

	int size = key->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	key->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinaryKey(buff);
	CHECK(res);
}

TEST(TestRecordIdentityGroup, binaryKey02){
	LocalOidFactory factory;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(1));

	LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

	rec.setOid(loid);
	rec.importScanIdentity();

	ScanRecordIndentity* id = rec.getScanIdentity();

	AbstractCdbKey* key = id->toKey(); __STP(key);

	int size = key->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	key->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinaryKey(buff);
	CHECK(res);
}

TEST(TestRecordIdentityGroup, key01){
	LocalOidFactory factory;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(1));

	LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

	rec.setOid(loid);
	rec.importScanIdentity();

	ScanRecordIndentity* id = rec.getScanIdentity();

	AbstractCdbKey* key = id->toKey(); __STP(key);

	CHECK(key->toInt64() == 0);

	ScanRecordIndentityKey* key2 = dynamic_cast<ScanRecordIndentityKey*>(key->clone()); __STP(key2);
	CHECK(key2->compareTo(key) == 0);

	AbstractCdbValue* v = key2->toCdbValue(); __STP(v);
}

TEST(TestRecordIdentityGroup, key02){
	LocalOidFactory factory;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(1));

	LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

	rec.setOid(loid);
	rec.importScanIdentity();

	ScanRecordIndentity* id = rec.getScanIdentity();

	AbstractCdbKey* key = id->toKey(); __STP(key);

	InfinityKey inf;
	CdbNullKey nullkey;

	CHECK(key->compareTo(&inf) < 0);
	CHECK(key->compareTo(&nullkey) > 0);
}

TEST(TestRecordIdentityGroup, key03){
	LocalOidFactory factory;

	CdbRecord rec;
	rec.addValue(new CdbIntValue(1));

	LocalCdbOid* loid = factory.createLocalOid(); __STP(loid);

	rec.setOid(loid);
	rec.importScanIdentity();

	ScanRecordIndentity* id = rec.getScanIdentity();

	AbstractCdbKey* key = id->toKey(); __STP(key);


	ScanRecordIndentity* id2 = new ScanRecordIndentity(); __STP(id2);
	AbstractCdbKey* key2 = id2->toKey(); __STP(key2);

	CHECK(key->compareTo(key2) != 0);
}
