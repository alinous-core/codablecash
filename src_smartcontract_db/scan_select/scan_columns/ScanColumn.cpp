/*
 * ScanColumn.cpp
 *
 *  Created on: 2020/08/19
 *      Author: iizuka
 */

#include "scan_select/scan_columns/ScanColumn.h"

#include "base/UnicodeString.h"

#include "lang_sql/sql_expression/SQLColumnIdentifier.h"

#include "vm/VirtualMachine.h"

#include "scan_select/scan_condition/params/ColumnIdentifierScanParam.h"

#include "schema_table/record/table_record/CdbRecord.h"

#include "base/StackRelease.h"
namespace codablecash {

ScanColumn::ScanColumn(const SQLColumnIdentifier* sqlColumnId) {
	this->sqlColumnId = sqlColumnId;
	this->scanParam = nullptr;
	this->distinct = false;
}

ScanColumn::~ScanColumn() {
	this->sqlColumnId = nullptr;
	delete this->scanParam;
}

const UnicodeString* ScanColumn::toStringCode() noexcept {
	if(this->str == nullptr){
		this->str = new UnicodeString(L"");

		const UnicodeString* s = nullptr;
		s = this->sqlColumnId->getSchema();
		if(s != nullptr){
			this->str->append(s);
			this->str->append(L".");
		}
		s = this->sqlColumnId->getTableName();
		if(s != nullptr){
			this->str->append(s);
			this->str->append(L".");
		}

		s = this->sqlColumnId->getColumnName();
		this->str->append(s);
	}

	return this->str;
}

void ScanColumn::resolveColumns(VirtualMachine* vm, SelectScanPlanner* planner) {
	CodableDatabase* db = vm->getDb();

	this->scanParam = new ColumnIdentifierScanParam(this->sqlColumnId);
	this->scanParam->setDistinct(this->distinct);
	this->scanParam->analyzeConditions(vm, planner);
}

void ScanColumn::scanColumns(VirtualMachine* vm, const CdbRecord* record, const ScanResultMetadata* metadata, CdbRecord* newRecord) {
	ColumnIdentifierScanParam* param = dynamic_cast<ColumnIdentifierScanParam*>(toValueProvider()); __STP(param);

	AbstractCdbValue* v = param->evaluate(vm, record, metadata);

	newRecord->addValue(v);
}

IValueProvider* ScanColumn::toValueProvider() {
	return this->scanParam->clone();
}

} /* namespace codablecash */
