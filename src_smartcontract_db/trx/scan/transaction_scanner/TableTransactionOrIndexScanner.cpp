/*
 * TableTransactionOrIndexScanner.cpp
 *
 *  Created on: 2020/12/11
 *      Author: iizuka
 */

#include "trx/scan/transaction_scanner/TableTransactionOrIndexScanner.h"

#include "schema_table/table_store/TableStore.h"

namespace codablecash {

TableTransactionOrIndexScanner::TableTransactionOrIndexScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
		TableStore* tableStore, const AbstractScanCondition* filterCondition, const OrIndexWrapperCollection* index)
		: AbstractTransactionScanner(metadata, trx, tableStore->getTable(), filterCondition){
	this->index = index;
}

TableTransactionOrIndexScanner::~TableTransactionOrIndexScanner() {
	this->index = nullptr;
}

void TableTransactionOrIndexScanner::start() {
}

bool TableTransactionOrIndexScanner::hasNext() {
	return false;
}

const CdbRecord* TableTransactionOrIndexScanner::next() {
	return nullptr;
}

void TableTransactionOrIndexScanner::shutdown() {
}


} /* namespace codablecash */
