/*
 * ColumnIdentifierColumn.cpp
 *
 *  Created on: 2020/07/21
 *      Author: iizuka
 */

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "schema_table/table/CdbTable.h"
#include "schema_table/table/CdbTableColumn.h"

#include "lang_sql/sql_expression/SQLColumnIdentifier.h"

#include "vm/VirtualMachine.h"

#include "scan_select/scan_planner/base/SelectScanPlanner.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbException.h"

#include "schema_table/schema/SchemaManager.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "schema_table/record/table_record_value/CdbNullValue.h"

#include "scan_select/scan_planner/analyze/AnalyzedScanPlan.h"
#include "scan_select/scan_planner/analyze/ScanTargetNameResolver.h"

#include "scan_select/scan_table/AbstractScanTableTarget.h"

#include "scan_select/scan_planner/scanner/ctx/FilterConditionDitector.h"

#include "scan_select/scan_planner/base/TablesHolder.h"

#include "trx/scan/transaction_scan_result/ScanResultMetadata.h"
#include "trx/scan/transaction_scan_result/ScanResultFieldMetadata.h"


namespace codablecash {

ColumnIdentifierScanParam::ColumnIdentifierScanParam(const ColumnIdentifierScanParam& inst) : sqlColId(inst.sqlColId) {
	this->cdbColumn = inst.cdbColumn;
	this->str = nullptr;
	this->sourceTarget = inst.sourceTarget;
	this->fieldMetadata = nullptr;
	this->distinct = inst.distinct;
}

ColumnIdentifierScanParam::ColumnIdentifierScanParam(const SQLColumnIdentifier* sqlColId) : sqlColId(sqlColId){
	this->cdbColumn = nullptr;
	this->str = nullptr;
	this->sourceTarget = nullptr;
	this->fieldMetadata = nullptr;
	this->distinct = false;
}

ColumnIdentifierScanParam::~ColumnIdentifierScanParam() {
	delete this->str;
	this->cdbColumn = nullptr;
	this->fieldMetadata = nullptr;
}

const UnicodeString* ColumnIdentifierScanParam::toStringCode() noexcept {
	if(this->str == nullptr){
		delete this->str;
		this->str = new UnicodeString(L"");

		const UnicodeString* s = nullptr;
		s = this->sqlColId->getSchema();
		if(s != nullptr){
			this->str->append(s);
			this->str->append(L".");
		}
		s = this->sqlColId->getTableName();
		if(s != nullptr){
			this->str->append(s);
			this->str->append(L".");
		}

		s = this->sqlColId->getColumnName();
		this->str->append(s);
	}

	return this->str;
}


void ColumnIdentifierScanParam::analyzeConditions(VirtualMachine* vm, SelectScanPlanner* planner) {
	CodableDatabase* db = vm->getDb();
	SchemaManager* scMgr = db->getSchemaManager();

	AnalyzedScanPlan* plan = planner->getPlan();
	ScanTargetNameResolver* aliasResolver = plan->getScanTargetNameResolver();

	UnicodeString tablefqn(L"");
	const UnicodeString* schemaName = this->sqlColId->getSchema();

	const UnicodeString* tableName = this->sqlColId->getTableName();
	const UnicodeString* colName = this->sqlColId->getColumnName();

	if(tableName == nullptr){
		TablesHolder* tablesHolder = planner->getTablesHolder();
		this->sourceTarget = tablesHolder->findTable(colName);
		if(this->sourceTarget == nullptr){
			throw new CdbException(L"Can not resolve column name.", __FILE__, __LINE__);
		}

		ScanTableColumnParam* param = this->sourceTarget->findTableColumns(colName); __STP(param);
		this->cdbColumn = param->column;

		return;
	}

	if(schemaName == nullptr && resolveAlias(tableName, aliasResolver)){
		ScanTableColumnParam* param = this->sourceTarget->findTableColumns(colName); __STP(param);
		if(param == nullptr){
			throw new CdbException(L"Can not resolve column name.", __FILE__, __LINE__);
		}
		this->cdbColumn = param->column;
		this->sourceTarget = param->target;
		return;
	}

	if(schemaName == nullptr){
		schemaName = vm->getCurrentSchema();
	}

	CdbTable* table = scMgr->getTable(schemaName, tableName);
	this->cdbColumn = table->getColumn(colName);
	if(this->cdbColumn == nullptr){
		throw new CdbException(L"Can not resolve column name.", __FILE__, __LINE__);
	}

	this->sourceTarget = aliasResolver->get(table->getTableFqn());
}

IValueProvider* ColumnIdentifierScanParam::clone() const noexcept {
	return new ColumnIdentifierScanParam(*this);
}

bool ColumnIdentifierScanParam::isColumn() const noexcept {
	return true;
}

bool ColumnIdentifierScanParam::hasColumnId() const noexcept {
	return true;
}

bool ColumnIdentifierScanParam::isFilterable(VirtualMachine* vm,
		SelectScanPlanner* planner, FilterConditionDitector* detector) const noexcept {
	return detector->hasTarget(this->sourceTarget);
}

bool ColumnIdentifierScanParam::resolveAlias(const UnicodeString* tableAlias, ScanTargetNameResolver* aliasResolver) {
	this->sourceTarget = aliasResolver->get(tableAlias);
	if(this->sourceTarget != nullptr){
		return true;
	}

	return false;
}

bool ColumnIdentifierScanParam::hasIndex() const noexcept {
	ScanTableColumnParam* p = this->sourceTarget->findTableColumns(sqlColId->getColumnName()); __STP(p);

	if(p->table == nullptr){
		return false;
	}

	const CdbTable* table = p->table;
	CdbTableIndex* index = table->getIndexByColumnOid(p->column->getOid());

	return index != nullptr;
}

AbstractCdbValue* ColumnIdentifierScanParam::evaluate(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata) {
	if(this->fieldMetadata == nullptr){
		this->fieldMetadata = metadata->findField(this);
	}

	int pos = this->fieldMetadata->getPosition();
	const AbstractCdbValue* v = record->get(pos);

	return v != nullptr ? v->copy() : new CdbNullValue();
}

bool ColumnIdentifierScanParam::isRecordIndependent() const noexcept {
	return false;
}

} /* namespace codablecash */
