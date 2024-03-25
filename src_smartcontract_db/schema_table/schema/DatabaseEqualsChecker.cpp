/*
 * DatabaseEqualsChecker.cpp
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */

#include "schema_table/schema/DatabaseEqualsChecker.h"
#include "schema_table/schema/DatabaseEqualsCheckException.h"

#include "engine/CdbOid.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"
namespace codablecash {

void DatabaseEqualsChecker::checkLongEquals(uint64_t a, uint64_t b) {
	if(a != b){
		throw new DatabaseEqualsCheckException(L"checkLongEquals()", __FILE__, __LINE__);
	}
}

void DatabaseEqualsChecker::checkIntEquals(int a, int b) {
	if(a != b){
		throw new DatabaseEqualsCheckException(L"checkLongEquals()", __FILE__, __LINE__);
	}
}

void DatabaseEqualsChecker::checkBoolEquals(bool a, bool b) {
	if(a != b){
		throw new DatabaseEqualsCheckException(L"checkLongEquals()", __FILE__, __LINE__);
	}
}

void DatabaseEqualsChecker::checkCdbOidEquals(const CdbOid *a,	const CdbOid *b) {
	if((a == nullptr && b != nullptr) || (a != nullptr && b == nullptr)){
		throw new DatabaseEqualsCheckException(L"checkLongEquals()", __FILE__, __LINE__);
	}

	if(a != nullptr && !a->equals(b)){
		throw new DatabaseEqualsCheckException(L"checkLongEquals()", __FILE__, __LINE__);
	}
}

void DatabaseEqualsChecker::checkUnicodeStringEquals(const UnicodeString *a,	const UnicodeString *b) {
	if((a == nullptr && b != nullptr) || (a != nullptr && b == nullptr)){
		throw new DatabaseEqualsCheckException(L"checkUnicodeStringEquals()", __FILE__, __LINE__);
	}

	if(a != nullptr && !a->equals(b)){
		throw new DatabaseEqualsCheckException(L"checkUnicodeStringEquals()", __FILE__, __LINE__);
	}
}

void DatabaseEqualsChecker::checkRecordEquals(const CdbRecordKey *a, const CdbRecordKey *b) {
	if((a == nullptr && b != nullptr) || (a != nullptr && b == nullptr)){
		throw new DatabaseEqualsCheckException(L"checkRecordEquals()", __FILE__, __LINE__);
	}

	if(a != nullptr && a->compareTo(b) != 0){
		throw new DatabaseEqualsCheckException(L"checkRecordEquals()", __FILE__, __LINE__);
	}
}

} /* namespace codablecash */
