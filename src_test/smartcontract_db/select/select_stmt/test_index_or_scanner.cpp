/*
 * test_index_or_scanner.cpp
 *
 *  Created on: 2022/01/30
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbException.h"

#include "trx/transaction/CdbTransaction.h"
#include "trx/transaction_log/CreateTableLog.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "base/ArrayList.h"

#include "trx/transaction_log/InsertLog.h"

#include "schema_table/record/table_record/CdbTableIdentifier.h"
#include "schema_table/record/table_record_value/CdbIntValue.h"
#include "schema_table/record/table_record_value/CdbStringValue.h"

#include "scan_select/scan/IndexScanner.h"

#include "schema_table/record/table_record_key/CdbIntKey.h"

#include "schema_table/record/table_record_key/CdbRecordKey.h"

#include "schema_table/record/table_record_value/VmInstanceValueConverter.h"

#include "schema_table/record/table_record_local/LocalCdbOid.h"

#include "../../toolkit_alter/TestDbSchemaAlter03.h"
#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"
#include "ext_binary/ExtDomArrayObject.h"

#include "ext_binary/ExtDomObject.h"

#include "../../toolkit/TestDbSchema03.h"
using namespace codablecash;

TEST_GROUP(TestIndexOrScannerGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestIndexOrScannerGroup, case01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_stmt/resources/indexor/case01/select01.alns"))

	TestDbSchemaAlter03 schem(this->env);
	schem.init(1024*512);
	schem.insert01();

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);

		exobj = array->get(0);
		CHECK(exobj != nullptr);
		const ExtDomObject* domObj = dynamic_cast<const ExtDomObject*>(exobj);
		const UnicodeString* str = domObj->toString();

		UnicodeString ans0(L"{name: 'tanaka', id: 1, email_id: 11}");
		CHECK(str->equals(ans0));
	}
}

TEST(TestIndexOrScannerGroup, case02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/select/select_stmt/resources/indexor/case02/select01.alns"))

	TestDbSchema03 schem(this->env);
	schem.init(1024*512);

	VirtualMachine* vm = schem.getVm();
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		SelectStatement* stmt = lang->selectStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		schem.execSelectStmt(stmt);

		const AbstractExtObject* exobj = schem.getExtResultObj();
		const ExtDomArrayObject* array = dynamic_cast<const ExtDomArrayObject*>(exobj);

		int size = array->size();
		CHECK(size == 1);
	}
}

