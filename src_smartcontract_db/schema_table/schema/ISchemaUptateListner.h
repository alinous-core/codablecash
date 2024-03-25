/*
 * ISchemaUptateListner.h
 *
 *  Created on: 2020/05/14
 *      Author: iizuka
 */

#ifndef SCHEMA_ISCHEMAUPTATELISTNER_H_
#define SCHEMA_ISCHEMAUPTATELISTNER_H_

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class SchemaManager;
class CdbTable;
class ColumnModifyContext;
class TableRenameContext;
class CdbTableIndex;
class CdbTableColumn;
class CdbTransactionManager;

class ISchemaUptateListner {
public:
	ISchemaUptateListner();
	virtual ~ISchemaUptateListner();

	virtual void schemaLoaded(SchemaManager* sc) = 0;
	virtual void onCreateTable(SchemaManager* mgr, const CdbTable* table) = 0;
	virtual void onDropTable(SchemaManager* mgr, const CdbTable* table) = 0;

	virtual void onAddColumn(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableColumn* newColumn, const CdbTableIndex* newUniqueIndex) = 0;
	virtual void onDropColumn(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableColumn* removalColumn, const ArrayList<CdbTableIndex>* removalIndexes) = 0;
	virtual void onAddIndex(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* newIndex) = 0;
	virtual void onDropIndex(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* removalIndex) = 0;
	virtual void onAlterModify(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const ColumnModifyContext* ctx) = 0;
	virtual void onDropPrimaryKey(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* primaryKey) = 0;
	virtual void onAddPrimaryKey(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, const CdbTableIndex* primaryKey) = 0;
	virtual void onAlterTableRenameTable(CdbTransactionManager* trxManager, SchemaManager* mgr, const CdbTable* table, TableRenameContext* ctx) = 0;
};

} /* namespace codablecash */

#endif /* SCHEMA_ISCHEMAUPTATELISTNER_H_ */
