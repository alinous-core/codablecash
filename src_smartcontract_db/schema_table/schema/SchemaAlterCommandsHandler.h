/*
 * SchemaAlterCommandsHandler.h
 *
 *  Created on: 2020/10/05
 *      Author: iizuka
 */

#ifndef SCHEMA_SCHEMAALTERCOMMANDSHANDLER_H_
#define SCHEMA_SCHEMAALTERCOMMANDSHANDLER_H_

namespace codablecash {

class AlterAddIndexCommandLog;
class AlterAddColumnCommandLog;
class AlterDropIndexCommandLog;
class AlterDropColumnCommandLog;
class AlterAddPrimaryKeyCommandLog;
class AlterDropPrimaryKeyCommandLog;
class AlterModifyCommandLog;
class AlterRenameColumnCommandLog;
class AlterRenameTableCommandLog;
class CdbTable;
class ColumnModifyContext;
class AbstractAlterCommandLog;
class CdbTransactionManager;

class SchemaManager;

class SchemaAlterCommandsHandler {
public:
	explicit SchemaAlterCommandsHandler(SchemaManager* schemaManager);
	virtual ~SchemaAlterCommandsHandler();

	void handleAlterTableAddIndex(const AlterAddIndexCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableAddColumn(const AlterAddColumnCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableDropIndex(const AlterDropIndexCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableDropColumn(const AlterDropColumnCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableAddPrimaryKey(const AlterAddPrimaryKeyCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableDropPrimaryKey(const AlterDropPrimaryKeyCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableModify(const AlterModifyCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableRenameColumn(const AlterRenameColumnCommandLog* cmd, CdbTransactionManager* trxManager);
	void handleAlterTableRenameTable(const AlterRenameTableCommandLog* cmd, CdbTransactionManager* trxManager);

private:
	void handleUniqueIndexOnModify(CdbTable* table, ColumnModifyContext* ctx, CdbTransactionManager* trxManager);
	void handleToNotUnique(CdbTable* table, ColumnModifyContext* ctx, CdbTransactionManager* trxManager);
	void handleToUnique(CdbTable* table, ColumnModifyContext* ctx, CdbTransactionManager* trxManager);

	CdbTable* findTableFromCommand(const AbstractAlterCommandLog* cmdlog);

private:
	SchemaManager* schemaManager;
};

} /* namespace codablecash */

#endif /* SCHEMA_SCHEMAALTERCOMMANDSHANDLER_H_ */
