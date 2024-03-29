/*
 * test_alter_log_binary.cpp
 *
 *  Created on: 2020/09/10
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "base/StackRelease.h"

#include "vm/VirtualMachine.h"

#include "engine/compiler/SmartContractParser.h"

#include "alinous_lang/AlinousLang.h"

#include "trx/transaction_log/TransactionLogFactory.h"
#include "trx/transaction_log/AbstractTransactionLog.h"

#include "trx/transaction_log_alter/AlterAddColumnCommandLog.h"
#include "trx/transaction_log_alter/AlterAddIndexCommandLog.h"
#include "trx/transaction_log_alter/AlterDropColumnCommandLog.h"
#include "trx/transaction_log_alter/AlterDropIndexCommandLog.h"

#include "lang_sql/sql_ddl_alter/AlterAddColumnCommand.h"
#include "lang_sql/sql_ddl_alter/AlterAddIndexCommand.h"
#include "lang_sql/sql_ddl_alter/AlterDropColumnCommand.h"
#include "lang_sql/sql_ddl_alter/AlterDropIndexCommand.h"

#include "lang_sql/sql_join_parts/TableIdentifier.h"

TEST_GROUP(TestAlterLogBinaryGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};
static bool checkBinary(ByteBuffer* buff);

bool checkBinary(ByteBuffer* buff){
	int lastSize = buff->capacity();

	buff->position(0);
	AbstractTransactionLog* element = TransactionLogFactory::createFromBinary(buff); __STP(element);

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


TEST(TestAlterLogBinaryGroup, AlterAddColumnCommand01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter/resources/parse/alterAddColumn01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();
		TableIdentifier tableId;
		tableId.setTableName(new UnicodeString(L"table1"));

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AbstractAlterDdlCommand* cmd = stmt->getCmd();
		stmt->setCommand(nullptr);

		AlterAddColumnCommandLog log(1L);
		log.setCommand(dynamic_cast<AlterAddColumnCommand*>(cmd));
		log.setTableIdentifier(&tableId);

		int size = log.binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

		log.toBinary(buff);
		CHECK(buff->position() == size)

		bool res = checkBinary(buff);
		CHECK(res)
	}
}

TEST(TestAlterLogBinaryGroup, AlterAddIndexCommandLog01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter/resources/parse/alterAddIndexCommand01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		TableIdentifier tableId;
		tableId.setTableName(new UnicodeString(L"table1"));

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AbstractAlterDdlCommand* cmd = stmt->getCmd();
		stmt->setCommand(nullptr);

		AlterAddIndexCommandLog log(1L);
		log.setCommand(dynamic_cast<AlterAddIndexCommand*>(cmd));
		log.setTableIdentifier(&tableId);

		int size = log.binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

		log.toBinary(buff);
		CHECK(buff->position() == size)

		bool res = checkBinary(buff);
		CHECK(res)
	}
}

TEST(TestAlterLogBinaryGroup, AlterDropColumnCommand01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter/resources/parse/alterDropColumnCommand01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		TableIdentifier tableId;
		tableId.setTableName(new UnicodeString(L"table1"));

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AbstractAlterDdlCommand* cmd = stmt->getCmd();
		stmt->setCommand(nullptr);

		AlterDropColumnCommandLog log(1L);
		log.setCommand(dynamic_cast<AlterDropColumnCommand*>(cmd));
		log.setTableIdentifier(&tableId);

		int size = log.binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

		log.toBinary(buff);
		CHECK(buff->position() == size)

		bool res = checkBinary(buff);
		CHECK(res)
	}
}

TEST(TestAlterLogBinaryGroup, AlterDropIndexCommand01){
	VirtualMachine* vm = new VirtualMachine(1024 * 10); __STP(vm);

	const File* projectFolder = this->env->getProjectRoot();
	_ST(File, sourceFile, projectFolder->get(L"src_test/smartcontract_db/ddl/table_alter/resources/parse/alterDropIndexCommand01.alns"))
	{
		SmartContractParser parser(sourceFile);
		AlinousLang* lang = parser.getDebugAlinousLang();

		TableIdentifier tableId;
		tableId.setTableName(new UnicodeString(L"table1"));

		AlterTableStatement* stmt = lang->alterTableStatement(); __STP(stmt);
		CHECK(!parser.hasError())

		AbstractAlterDdlCommand* cmd = stmt->getCmd();
		stmt->setCommand(nullptr);

		AlterDropIndexCommandLog log(1L);
		log.setCommand(dynamic_cast<AlterDropIndexCommand*>(cmd));
		log.setTableIdentifier(&tableId);

		int size = log.binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(size, true); __STP(buff);

		log.toBinary(buff);
		CHECK(buff->position() == size)

		bool res = checkBinary(buff);
		CHECK(res)
	}
}
