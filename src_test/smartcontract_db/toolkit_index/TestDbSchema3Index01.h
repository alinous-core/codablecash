/*
 * TestDbSchema3Index01.h
 *
 *  Created on: 2023/06/20
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_DB_TOOLKIT_INDEX_TESTDBSCHEMA3INDEX01_H_
#define SMARTCONTRACT_DB_TOOLKIT_INDEX_TESTDBSCHEMA3INDEX01_H_

#include "../toolkit/TestDbSchemaBase.h"

namespace codablecash {

class CdbTransaction;

class TestDbSchema3Index01 : public TestDbSchemaBase {
public:
	explicit TestDbSchema3Index01(TestEnv* env);
	virtual ~TestDbSchema3Index01();

	virtual void init(uint64_t memCapacity);
	void createTable();
	void createIndex();

	void insert01();

private:
	void insertRecord(CdbTransaction* trx, int id, const wchar_t* name, int email_id, const wchar_t* comment);
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_DB_TOOLKIT_INDEX_TESTDBSCHEMA3INDEX01_H_ */
