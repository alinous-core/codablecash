/*
 * CrossJoinScanerTarget.cpp
 *
 *  Created on: 2020/08/29
 *      Author: iizuka
 */

#include "scan_select/scan_planner/scanner/factory/CrossJoinScanerFactory.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "engine/CodableDatabase.h"

#include "schema_table/record/table_record_local/LocalOidFactory.h"

#include "vm/VirtualMachine.h"

#include "trx/scan/transaction_scanner_join/CrossJoinExecutor.h"

#include "trx/scan/transaction_scanner_join_right/RightWrappedLeftTransactionScanner.h"
#include "trx/scan/transaction_scanner_join_right/RightWrappedLeftOrTransactionScanner.h"

#include "vm/vm_trx/VmTransactionHandler.h"

#include "trx/transaction/CdbTransaction.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"
#include "scan_select/scan_planner/scanner/join/JoinOrCandidate.h"


namespace codablecash {

CrossJoinScanerFactory::CrossJoinScanerFactory(const ScanResultMetadata* metadata, const AbstractScanCondition* joinCondition)
			: AbstractJoinScannerFactory(metadata, joinCondition) {

}

CrossJoinScanerFactory::~CrossJoinScanerFactory() {

}

IJoinLeftSource* CrossJoinScanerFactory::createScannerAsLeftSource(VirtualMachine* vm, SelectScanPlanner* planner) {
	ScanJoinContext* joinContext = new ScanJoinContext(this->joinCandidate);

	IJoinLeftSource* leftSource = this->leftFactory->createScannerAsLeftSource(vm, planner);
	IJoinRightSource* rightSource = this->rightFactory->createScannerAsRightSource(vm, planner, joinContext, false);

	CodableDatabase* db = vm->getDb();
	LocalOidFactory* localOidFactory = db->getLocalOidFactory();

	CrossJoinExecutor* exec = new CrossJoinExecutor(leftSource, rightSource, this->metadata, joinContext, this->filterCondition, localOidFactory);

	return exec;
}

IJoinRightSource* CrossJoinScanerFactory::createScannerAsRightSource(VirtualMachine* vm, SelectScanPlanner* planner, const ScanJoinContext* joinContext, bool leftReverse) {
	VmTransactionHandler* trxHandler = vm->getTransactionHandler();
	CdbTransaction* trx = trxHandler->getTransaction();

	IJoinLeftSource* leftSource = createScannerAsLeftSource(vm, planner);
	AbstractJoinCandidate* joinCandidate = joinContext->getJoinCandidate();

	IJoinRightSource* source = nullptr;

	AbstractJoinCandidate::CandidateType type = joinCandidate->getCandidateType();
	if(type == AbstractJoinCandidate::CandidateType::OR){
		JoinOrCandidate* orCandidate = dynamic_cast<JoinOrCandidate*>(joinCandidate);
		source
			= new RightWrappedLeftOrTransactionScanner(this->metadata, trx, nullptr, this->filterCondition, leftSource, orCandidate, leftReverse);
	}
	else{
		source = new RightWrappedLeftTransactionScanner(this->metadata, trx, nullptr, this->filterCondition, leftSource, joinCandidate, leftReverse);
	}

	return source;
}

} /* namespace codablecash */
