/*
 * TestDbSchemaOrJoin01.h
 *
 *  Created on: 2022/01/18
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_DB_TOOLKIT_JOIN_TESTDBSCHEMAORJOIN01_H_
#define SMARTCONTRACT_DB_TOOLKIT_JOIN_TESTDBSCHEMAORJOIN01_H_

#include "smartcontract_db/toolkit/TestDbSchemaBase.h"
#include "base/ArrayList.h"

namespace codablecash {

class CdbRecord;
class CdbTransaction;

class TestDbSchemaOrJoin01 : public TestDbSchemaBase {
public:
	TestDbSchemaOrJoin01(TestEnv* env);
	virtual ~TestDbSchemaOrJoin01();

	virtual void init();
	virtual void init(uint64_t memCapacity);

private:
	void createTable();
	void createTestTable1();
	void createTestTable2();

	void insertData();

	void insertData1();
	void insertRecord1(CdbTransaction* trx, int id, const wchar_t* name, int email_id, int email_id2, ArrayList<CdbRecord>* list);

	void insertData2();
	void insertRecord2(CdbTransaction* trx, int email_id, const wchar_t* email, int email_id2, ArrayList<CdbRecord>* list);

private:
	ArrayList<CdbRecord> list1;
	ArrayList<CdbRecord> list2;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_DB_TOOLKIT_JOIN_TESTDBSCHEMAORJOIN01_H_ */
