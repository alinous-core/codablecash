/*
 * RightJoinScannerFactory.cpp
 *
 *  Created on: 2021/12/28
 *      Author: iizuka
 */

#include "scan_select/scan_planner/scanner/factory/RightJoinScannerFactory.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "engine/CodableDatabase.h"

#include "vm/VirtualMachine.h"

#include "trx/scan/transaction_scanner_join/RightJoinExecutor.h"

#include "vm/vm_trx/VmTransactionHandler.h"

#include "trx/transaction/CdbTransaction.h"

#include "trx/scan/transaction_scanner_join_right/RightWrappedLeftTransactionScanner.h"
#include "trx/scan/transaction_scanner_join_right/RightWrappedLeftOrTransactionScanner.h"

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"
#include "scan_select/scan_planner/scanner/join/JoinOrCandidate.h"

namespace codablecash {

RightJoinScannerFactory::RightJoinScannerFactory(const ScanResultMetadata* metadata, const AbstractScanCondition* joinCondition)
		: AbstractJoinScannerFactory(metadata, joinCondition){

}

RightJoinScannerFactory::~RightJoinScannerFactory() {

}

IJoinLeftSource* RightJoinScannerFactory::createScannerAsLeftSource(VirtualMachine* vm, SelectScanPlanner* planner) {
	ScanJoinContext* joinContext = new ScanJoinContext(this->joinCandidate);

	IJoinRightSource* leftSource = this->leftFactory->createScannerAsRightSource(vm, planner, joinContext, true);
	IJoinLeftSource* rightSource = this->rightFactory->createScannerAsLeftSource(vm, planner);

	CodableDatabase* db = vm->getDb();
	LocalOidFactory* localOidFactory = db->getLocalOidFactory();

	RightJoinExecutor* exec = new RightJoinExecutor(rightSource, leftSource, this->metadata, joinContext, this->filterCondition, localOidFactory);

	return exec;
}

IJoinRightSource* RightJoinScannerFactory::createScannerAsRightSource(
		VirtualMachine* vm, SelectScanPlanner* planner,
		const ScanJoinContext* joinContext, bool leftReverse) {
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
