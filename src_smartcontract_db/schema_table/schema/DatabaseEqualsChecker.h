/*
 * DatabaseEqualsChecker.h
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_SCHEMA_DATABASEEQUALSCHECKER_H_
#define SCHEMA_TABLE_SCHEMA_DATABASEEQUALSCHECKER_H_

#include <cstdint>

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {
class CdbOid;
class CdbRecordKey;

class DatabaseEqualsChecker {
public:
	static void checkLongEquals(uint64_t a, uint64_t b);
	static void checkIntEquals(int a, int b);
	static void checkBoolEquals(bool a, bool b);
	static void checkCdbOidEquals(const CdbOid* a, const CdbOid* b);
	static void checkUnicodeStringEquals(const UnicodeString* a, const UnicodeString* b);
	static void checkRecordEquals(const CdbRecordKey* a, const CdbRecordKey* b);
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_SCHEMA_DATABASEEQUALSCHECKER_H_ */
