/*
 * TestDbSchema02.h
 *
 *  Created on: 2021/11/16
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_DB_TOOLKIT_TESTDBSCHEMA02_H_
#define SMARTCONTRACT_DB_TOOLKIT_TESTDBSCHEMA02_H_

#include "TestDbSchemaBase.h"
#include "base/ArrayList.h"

namespace codablecash {

class CdbRecord;
class CdbTransaction;

class TestDbSchema02 : public TestDbSchemaBase {
public:
	explicit TestDbSchema02(TestEnv* env);
	virtual ~TestDbSchema02();

	virtual void init();
	virtual void init(uint64_t memCapacity);

private:
	void createTable();
	void createTestTable1();
	void createTestTable2();

	void insertData();

	void insertData1();
	void insertRecord1(CdbTransaction* trx, int id, const wchar_t* name, int email_id, ArrayList<CdbRecord>* list);

	void insertData2();
	void insertRecord2(CdbTransaction* trx, int email_id, const wchar_t* email, ArrayList<CdbRecord>* list);

private:
	ArrayList<CdbRecord> list1;
	ArrayList<CdbRecord> list2;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_DB_TOOLKIT_TESTDBSCHEMA02_H_ */
