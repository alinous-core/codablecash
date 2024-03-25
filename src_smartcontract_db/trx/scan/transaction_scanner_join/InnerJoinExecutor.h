/*
 * InnerJoinExecutor.h
 *
 *  Created on: 2020/08/23
 *      Author: iizuka
 */

#ifndef TRANSACTION_SCANNER_JOIN_INNERJOINEXECUTOR_H_
#define TRANSACTION_SCANNER_JOIN_INNERJOINEXECUTOR_H_

#include "trx/scan/transaction_scanner_join/AbstractJoinExecutor.h"

namespace codablecash {

class JoinCandidateCursor;

class InnerJoinExecutor : public AbstractJoinExecutor {
public:
	InnerJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata, ScanJoinContext* context
			, AbstractScanCondition* filterCondition, LocalOidFactory* localOidFactory);
	virtual ~InnerJoinExecutor();

	virtual void start(VirtualMachine* vm);
	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

private:
	void resetLeftRecord();
	bool hasNextLeftRecord(VirtualMachine* vm);
	void onChangeLeft();

private:
	CdbRecord* leftRecord;
	JoinCandidateCursor* joinCandidateCursor;
	CdbRecord* nextRecord;
};

} /* namespace codablecash */

#endif /* TRANSACTION_SCANNER_JOIN_INNERJOINEXECUTOR_H_ */
