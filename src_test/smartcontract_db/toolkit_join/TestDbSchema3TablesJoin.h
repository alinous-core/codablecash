/*
 * TestDbSchema3TableJoin.h
 *
 *  Created on: 2022/01/04
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_DB_TOOLKIT_JOIN_TESTDBSCHEMA3TABLESJOIN_H_
#define SMARTCONTRACT_DB_TOOLKIT_JOIN_TESTDBSCHEMA3TABLESJOIN_H_

#include "smartcontract_db/toolkit/TestDbSchemaBase.h"
#include "base/ArrayList.h"

namespace codablecash {

class CdbRecord;
class CdbTransaction;

class TestDbSchema3TablesJoin: public TestDbSchemaBase {
public:
	explicit TestDbSchema3TablesJoin(TestEnv* env);
	virtual ~TestDbSchema3TablesJoin();

	virtual void init();
	virtual void init(uint64_t memCapacity);

	void initSchema();

	void createTable();
	void createTestTable1();
	void createTestTable2();
	void createTestTable3();

	void insertData();

	void insertData1();
	void insertRecord1(CdbTransaction* trx, int id, const wchar_t* name, int email_id, ArrayList<CdbRecord>* list);

	void insertData2();
	void insertRecord2(CdbTransaction* trx, int email_id, const wchar_t* email, int server_id, ArrayList<CdbRecord>* list);

	void insertData3();
	void insertRecord3(CdbTransaction* trx, int server_id, const wchar_t* server, ArrayList<CdbRecord>* list);

private:
	ArrayList<CdbRecord> list1;
	ArrayList<CdbRecord> list2;
	ArrayList<CdbRecord> list3;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_DB_TOOLKIT_JOIN_TESTDBSCHEMA3TABLESJOIN_H_ */
