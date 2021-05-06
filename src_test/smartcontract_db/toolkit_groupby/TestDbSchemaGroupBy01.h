/*
 * TestDbSchemaGroupBy01.h
 *
 *  Created on: 2021/05/06
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_DB_TOOLKIT_GROUPBY_TESTDBSCHEMAGROUPBY01_H_
#define SMARTCONTRACT_DB_TOOLKIT_GROUPBY_TESTDBSCHEMAGROUPBY01_H_

#include "../toolkit/TestDbSchemaBase.h"

namespace codablecash {

class TestDbSchemaGroupBy01 : public TestDbSchemaBase {
public:
	explicit TestDbSchemaGroupBy01(TestEnv* env);
	virtual ~TestDbSchemaGroupBy01();

	virtual void init();
	virtual void init(uint64_t memCapacity);

private:
	void createTable();
	void insertData();
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_DB_TOOLKIT_GROUPBY_TESTDBSCHEMAGROUPBY01_H_ */
