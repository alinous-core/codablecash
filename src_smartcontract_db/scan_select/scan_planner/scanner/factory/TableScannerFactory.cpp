/*
 * TableScannerFactory.cpp
 *
 *  Created on: 2020/08/29
 *      Author: iizuka
 */

#include "scan_select/scan_planner/scanner/factory/TableScannerFactory.h"

#include "scan_select/scan_planner/scanner/index/AbstractIndexCandidate.h"

#include "vm/VirtualMachine.h"
#include "vm/vm_trx/VmTransactionHandler.h"

#include "engine/CodableDatabase.h"
#include "engine/CdbOid.h"

#include "schema_table/table/CdbTable.h"

#include "schema_table/table_store/CdbStorageManager.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/scan/transaction_scanner/TableTransactionScanner.h"
#include "trx/scan/transaction_scanner/TableTransactionOrIndexScanner.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"
#include "scan_select/scan_planner/scanner/join/JoinOrCandidate.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "scan_select/scan_condition/base/AbstractScanCondition.h"
#include "scan_select/scan_planner/scanner/index_resolv/AbstractColumnsIndexAccess.h"
#include "scan_select/scan_planner/scanner/index_resolv/OrIndexAccessCollection.h"

#include "trx/scan/transaction_scanner/TableTransactionIndexScanner.h"
#include "trx/scan/transaction_scanner/TableTransactionIndexRangeScanner.h"

#include "trx/scan/transaction_scanner_join_right/RightTableIndexTransactionScanner.h"
#include "trx/scan/transaction_scanner_join_right/RightTableOrTransactionScanner.h"
#include "trx/scan/transaction_scanner_join_right/RightTableBufferedTransactionScanner.h"
#include "trx/scan/transaction_scanner_join_right/RightTableBufferedOidTransactionScanner.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidateCollection.h"

namespace codablecash {

TableScannerFactory::TableScannerFactory(AbstractScanTableTarget* target, const CdbTable* table,
		const ScanResultMetadata* metadata, AbstractColumnsIndexAccess* indexCandidate)
				: AbstractScannerFactory(metadata){
	this->target = target;
	this->indexCandidate = indexCandidate;
	this->table = table;
}

TableScannerFactory::~TableScannerFactory() {
	delete this->indexCandidate;

	this->target = nullptr;
	this->table = nullptr;
}

IJoinLeftSource* TableScannerFactory::createScannerAsLeftSource(
		VirtualMachine* vm, SelectScanPlanner* planner) {
	CodableDatabase* db = vm->getDb();
	CdbStorageManager* storageManager = db->getStorageManager();

	VmTransactionHandler* trxHandler = vm->getTransactionHandler();
	CdbTransaction* trx = trxHandler->getTransaction();

	const CdbOid* tableoid = this->table->getOid();
	TableStore* tableStore = storageManager->getTableStore(tableoid);

	IJoinLeftSource* scanner = nullptr;

	if(this->indexCandidate == nullptr){
		scanner = new TableTransactionScanner(this->metadata, trx, tableStore, this->filterCondition);
	}
	else{
		scanner = createIndexScannerAsLeftSource(vm, planner, tableStore, trx, this->indexCandidate);
	}

	return scanner;
}

IJoinLeftSource* TableScannerFactory::createIndexScannerAsLeftSource(VirtualMachine* vm, SelectScanPlanner* planner,
		TableStore* tableStore, CdbTransaction* trx, AbstractColumnsIndexAccess* indexCandidate) {
	IJoinLeftSource* scanner = nullptr;

	if(indexCandidate->isOr()){
		OrIndexAccessCollection* orIndex = dynamic_cast<OrIndexAccessCollection*>(indexCandidate);
		TableTransactionOrIndexScanner* orscanner = new TableTransactionOrIndexScanner(this->metadata, trx, tableStore, this->filterCondition, orIndex);
		scanner = orscanner;

		int maxLoop = orIndex->size();
		for(int i = 0; i != maxLoop; ++i){
			AbstractColumnsIndexAccess* index = orIndex->get(i);

			IJoinLeftSource* leftSource = createIndexScannerAsLeftSource(vm, planner, tableStore, trx, index);
			orscanner->addLeftSource(leftSource);
		}
	}
	else if(indexCandidate->isRange(vm)){
		scanner = new TableTransactionIndexRangeScanner(this->metadata, trx, tableStore, this->filterCondition, indexCandidate);
	}
	else{
		scanner = new TableTransactionIndexScanner(this->metadata, trx, tableStore, this->filterCondition, indexCandidate);
	}

	return scanner;
}

IJoinRightSource* TableScannerFactory::createScannerAsRightSource(
		VirtualMachine* vm, SelectScanPlanner* planner, const ScanJoinContext* joinContext, bool leftReverse) {
	IJoinRightSource* rightSource = nullptr;

	AbstractJoinCandidate* joinCandidate = joinContext->getJoinCandidate();
	if(joinCandidate != nullptr){
		rightSource = createIndexScannerAsRightSource(vm, planner, joinCandidate, leftReverse);
	}
	else{
		VmTransactionHandler* trxHandler = vm->getTransactionHandler();
		CdbTransaction* trx = trxHandler->getTransaction();

		// cross join
		rightSource = new RightTableBufferedOidTransactionScanner(this->metadata, trx, this->table, this->filterCondition);
	}

	return rightSource;
}

IJoinRightSource* TableScannerFactory::createIndexScannerAsRightSource(
		VirtualMachine* vm, SelectScanPlanner* planner,
		AbstractJoinCandidate* joinCandidate, bool leftReverse) {
	IJoinRightSource* rightSource = nullptr;

	VmTransactionHandler* trxHandler = vm->getTransactionHandler();
	CdbTransaction* trx = trxHandler->getTransaction();

	AbstractJoinCandidate::CandidateType type = joinCandidate->getCandidateType();
	if(type == AbstractJoinCandidate::CandidateType::OR){
		JoinOrCandidate* orCandidate = dynamic_cast<JoinOrCandidate*>(joinCandidate);
		RightTableOrTransactionScanner* orRightSource = new RightTableOrTransactionScanner(this->metadata, trx, this->table, this->filterCondition, orCandidate);
		rightSource = orRightSource;

		int maxLoop = orCandidate->size();
		for(int i = 0; i != maxLoop; ++i){
			const AbstractJoinCandidateCollection* candidate = orCandidate->get(i);
			ScanJoinContext jcontext(candidate);

			IJoinRightSource* src = createScannerAsRightSource(vm, planner, &jcontext, leftReverse);
			orRightSource->addRightSource(src);
		}
	}else{
		CdbTableIndex* index = joinCandidate->getIndex(this->target);

		if(index != nullptr){
			rightSource = new RightTableIndexTransactionScanner(this->metadata, trx, this->table, this->filterCondition, index);
		}
		else{
			rightSource = new RightTableBufferedTransactionScanner(this->metadata, trx, this->table, this->filterCondition, joinCandidate, leftReverse);
		}
	}

	return rightSource;
}

} /* namespace codablecash */
