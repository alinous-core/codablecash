/*
 * TestDbSchema1Index01.h
 *
 *  Created on: 2023/06/27
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_DB_TOOLKIT_INDEX_TESTDBSCHEMA1INDEX01_H_
#define SMARTCONTRACT_DB_TOOLKIT_INDEX_TESTDBSCHEMA1INDEX01_H_

#include "../toolkit/TestDbSchemaBase.h"

namespace codablecash {

class CdbTransaction;

class TestDbSchema1Index01 : public TestDbSchemaBase {
public:
	explicit TestDbSchema1Index01(TestEnv* env);
	virtual ~TestDbSchema1Index01();

	virtual void init(uint64_t memCapacity);
	void createTable();
	void createIndex();

	void insert01();

private:
	void insertRecord(CdbTransaction* trx, int id, const wchar_t* name, int email_id, const wchar_t* comment);
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_DB_TOOLKIT_INDEX_TESTDBSCHEMA1INDEX01_H_ */
