/*
 * test_sql_exp.cpp
 *
 *  Created on: 2019/02/16
 *      Author: iizuka
 */


#include "test_utils/t_macros.h"
#include "engine/compiler/SmartContractParser.h"
#include "lang/sc_statement/AbstractStatement.h"

#include "base/StackRelease.h"
#include "alinous_lang/AlinousLang.h"
#include "lang_sql/sql_expression/SQLWildCard.h"

#include "lang_sql/sql_expression/SQLNullLiteral.h"

#include "vm/VirtualMachine.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"
static bool checkBinary(ByteBuffer* buff){
	int lastSize = buff->capacity();

	buff->position(0);
	CodeElement* element = CodeElement::createFromBinary(buff); __STP(element);

	int size = element->binarySize();
	if(lastSize != size){
		return false;
	}

	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(size, true); __STP(buff2);
	element->toBinary(buff2);

	if(buff2->position() != size){
		return false;
	}

	return Mem::memcmp(buff->array(), buff2->array(), size) == 0;
}

TEST_GROUP(TestSQLExpressionGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestSQLExpressionGroup, tableId){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/tableid.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	TableIdentifier* tableId = lang->tableIdentifier(); __STP(tableId);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, tableIdBinary){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/tableid.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	TableIdentifier* tableId = lang->tableIdentifier(); __STP(tableId);

	CHECK(!parser.hasError())

	int size = tableId->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	tableId->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, literal){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/literal.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, literalBinary){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/literal.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, boolliteral){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/boolliteral.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, boolliteralBinary){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/boolliteral.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, colid){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/colid.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, colidBinary){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/colid.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, between){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/between.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, betweenBinary){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/between.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, isnull){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/isnull.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, isnullBinary){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/isnull.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}


TEST(TestSQLExpressionGroup, in01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/in01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, in01Binary){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/in01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, like01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/like01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, like01Binary){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/like01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, holder){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/holder.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, holderBinary){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/holder.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, func01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/func01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, func01bin){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/func01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, add01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/add01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, add01bin){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/add01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, mul01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/mul01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, mul01bin){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/mul01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, not01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/not01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, not01bin){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/not01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, and01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/and01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, and01bin){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/and01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, or01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/or01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, or01bin){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/or01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, rational01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/rational01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())
}

TEST(TestSQLExpressionGroup, rational01bin){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/rational01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, distinct01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/distinct01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlDistinctArgument(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, distinct02){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/distinct01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlDistinctArgument(); __STP(exp);
	CHECK(!parser.hasError())

	VirtualMachine* vm = new VirtualMachine(10*1024); __STP(vm);
	exp->init(vm);

	SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
	vm->setSelectPlanner(planner);

	exp->onSelectTarget(vm);
	vm->popSelectPlanner();
}

TEST(TestSQLExpressionGroup, wildcard01){
	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract/resources/sqlexp/wildcard01.alns"))

	SmartContractParser parser(sourceFile);
	AlinousLang* lang = parser.getDebugAlinousLang();

	AbstractSQLExpression* exp = lang->sqlExpression(); __STP(exp);

	CHECK(!parser.hasError())

	int size = exp->binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

	exp->toBinary(buff);
	CHECK(buff->position() == size)

	bool res = checkBinary(buff);
	CHECK(res)
}

TEST(TestSQLExpressionGroup, wildcard02){
	SQLWildCard exp;

	AnalyzedType atype = exp.getType(nullptr);
	AnalyzedType none;

	CHECK(atype.getType() == none.getType());
}

TEST(TestSQLExpressionGroup, sqlnull01){
	VirtualMachine* vm = new VirtualMachine(10*1024); __STP(vm);
	SQLNullLiteral exp;
	exp.init(vm);

	AnalyzedType atype = exp.getType(nullptr);
	AnalyzedType none(AnalyzedType::TYPE_NULL);

	CHECK(atype.getType() == none.getType());
}

TEST(TestSQLExpressionGroup, sqlnull02){
	VirtualMachine* vm = new VirtualMachine(10*1024); __STP(vm);
	SQLNullLiteral exp;
	exp.init(vm);

	SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
	vm->setSelectPlanner(planner);

	exp.interpret(vm);

	exp.onSelectTarget(vm);
	vm->popSelectPlanner();
}
/*
TEST(TestSQLExpressionGroup, sqlnull03){
	VirtualMachine* vm = new VirtualMachine(10*1024); __STP(vm);
	SQLNullLiteral exp;
	exp.init(vm);

	SelectScanPlanner* planner = new SelectScanPlanner(); __STP(planner);
	vm->setSelectPlanner(planner);

	exp.interpret(vm);

	vm->popSelectPlanner();

	exp.onSelectTarget(vm);
}*/

