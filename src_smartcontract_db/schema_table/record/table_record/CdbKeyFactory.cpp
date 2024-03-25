/*
 * CdbKeyFactory.cpp
 *
 *  Created on: 2020/05/20
 *      Author: iizuka
 */

#include "schema_table/record/table_record/CdbKeyFactory.h"

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

#include "base_io/ByteBuffer.h"

#include "schema_table/record/table_record_key/CdbByteKey.h"
#include "schema_table/record/table_record_key/CdbShortKey.h"
#include "schema_table/record/table_record_key/CdbIntKey.h"
#include "schema_table/record/table_record_key/CdbLongKey.h"
#include "schema_table/record/table_record_key/CdbStringKey.h"
#include "schema_table/record/table_record_key/CdbBooleanKey.h"
#include "schema_table/record/table_record_key/CdbRecordKey.h"
#include "schema_table/record/table_record_key/CdbOidKey.h"
#include "schema_table/record/table_record_key/CdbNullKey.h"
#include "schema_table/record/table_record_key/CdbInfinityKey.h"

#include "schema_table/record/table_record/ScanRecordIndentityKey.h"

namespace codablecash {

CdbKeyFactory::CdbKeyFactory() {

}

CdbKeyFactory::~CdbKeyFactory() {

}

AbstractBtreeKey* CdbKeyFactory::fromBinary(uint32_t keyType, ByteBuffer* in) const {
	AbstractCdbKey* retKey = nullptr;

	switch(keyType){
	case AbstractCdbKey::TYPE_NULL:
		retKey = new CdbNullKey();
		break;
	case AbstractCdbKey::TYPE_INFINITY:
		retKey = new CdbInfinityKey();
		break;
	case AbstractCdbKey::TYPE_BYTE:
		retKey = new CdbByteKey();
		break;
	case AbstractCdbKey::TYPE_SHORT:
		retKey = new CdbShortKey();
		break;
	case AbstractCdbKey::TYPE_INT:
		retKey = new CdbIntKey();
		break;
	case AbstractCdbKey::TYPE_LONG:
		retKey = new CdbLongKey();
		break;
	case AbstractCdbKey::TYPE_STRING:
		retKey = new CdbStringKey();
		break;
	case AbstractCdbKey::TYPE_BOOLEAN:
		retKey = new CdbBooleanKey();
		break;

	case AbstractCdbKey::TYPE_RECORD_KEY:
		retKey = new CdbRecordKey();
		break;
	case AbstractCdbKey::TYPE_OID_KEY:
		retKey = new CdbOidKey();
		break;
	case AbstractCdbKey::TYPE_RECORD_IDENTITY_KEY:
		retKey = new ScanRecordIndentityKey();
		break;
	case 0:
		return nullptr;
	default:
		break;
	}

	if(retKey != nullptr){
		retKey->fromBinary(in);
		return retKey;
	}

	return BtreeKeyFactory::fromBinary(keyType, in);
}

BtreeKeyFactory* CdbKeyFactory::copy() const noexcept {
	return new CdbKeyFactory();
}

} /* namespace codablecash */
