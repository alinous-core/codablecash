/*
 * LeftJoinScannerFactory.cpp
 *
 *  Created on: 2020/08/29
 *      Author: iizuka
 */

#include "scan_select/scan_planner/scanner/factory/LeftJoinScannerFactory.h"

#include "trx/scan/transaction_scanner_join/OuterJoinExecutor.h"
#include "trx/scan/transaction_scanner_join/IJoinLeftSource.h"
#include "trx/scan/transaction_scanner_join/IJoinRightSource.h"

#include "trx/scan/transaction_scanner_join_right/RightWrappedLeftTransactionScanner.h"

#include "trx/transaction/CdbTransaction.h"

#include "scan_select/scan_planner/scanner/ctx/ScanJoinContext.h"

#include "engine/CodableDatabase.h"

#include "vm/VirtualMachine.h"

#include "vm/vm_trx/VmTransactionHandler.h"

#include "trx/scan/transaction_scanner_join_right/RightWrappedLeftOrTransactionScanner.h"

#include "scan_select/scan_planner/scanner/join/JoinOrCandidate.h"
#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"

namespace codablecash {

LeftJoinScannerFactory::LeftJoinScannerFactory(const ScanResultMetadata* metadata, const AbstractScanCondition* joinCondition)
				: AbstractJoinScannerFactory(metadata, joinCondition){

}

LeftJoinScannerFactory::~LeftJoinScannerFactory() {

}

IJoinLeftSource* LeftJoinScannerFactory::createScannerAsLeftSource(VirtualMachine* vm, SelectScanPlanner* planner) {
	ScanJoinContext* joinContext = new ScanJoinContext(this->joinCandidate);

	IJoinLeftSource* leftSource = this->leftFactory->createScannerAsLeftSource(vm, planner);
	IJoinRightSource* rightSource = this->rightFactory->createScannerAsRightSource(vm, planner, joinContext, false);

	CodableDatabase* db = vm->getDb();
	LocalOidFactory* localOidFactory = db->getLocalOidFactory();

	OuterJoinExecutor* exec = new OuterJoinExecutor(leftSource, rightSource, this->metadata, joinContext, this->filterCondition, localOidFactory);

	return exec;
}

IJoinRightSource* LeftJoinScannerFactory::createScannerAsRightSource(VirtualMachine* vm, SelectScanPlanner* planner, const ScanJoinContext* joinContext, bool leftReverse) {
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
