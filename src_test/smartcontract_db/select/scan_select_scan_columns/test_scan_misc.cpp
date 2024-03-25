/*
 * test_scan_misc.cpp
 *
 *  Created on: 2021/12/11
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "../../toolkit/TestDbSchema02.h"
#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"

#include "ext_binary/AbstractExtObject.h"
#include "ext_binary/ExtDomArrayObject.h"
#include "ext_binary/ExtDomObject.h"
#include "ext_binary/ExtPrimitiveObject.h"
#include "ext_binary/ExtStringClass.h"

#include "scan_select/scan_columns/AllScanColumns.h"
#include "scan_select/scan_columns/ScanColumn.h"

#include "scan_select/scan_condition/base/IValueProvider.h"

#include "lang_sql/sql_expression/SQLColumnIdentifier.h"

using namespace codablecash;
using namespace alinous;

TEST_GROUP(TestScanMiscGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestScanMiscGroup, case01){
	AllScanColumns all;

	CHECK(all.toValueProvider() == nullptr);
}
/*
TEST(TestScanMiscGroup, case02){
	VirtualMachine* vm = new VirtualMachine(10*1024); __STP(vm);

	SQLColumnIdentifier* colid = new SQLColumnIdentifier(); __STP(colid);
	ScanColumn* col = new ScanColumn(colid); __STP(col);

	SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
	vm->setSelectPlanner(planner);

	col->buildNameList()

	IValueProvider* vp = col->toValueProvider();
	ScanColumn* p = dynamic_cast<ScanColumn*>(vp);

	CHECK(p != nullptr);
}*/
