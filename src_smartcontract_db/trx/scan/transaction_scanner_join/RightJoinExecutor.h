/*
 * RightJoinExecutor.h
 *
 *  Created on: 2021/12/28
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHTJOINEXECUTOR_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHTJOINEXECUTOR_H_

#include "trx/scan/transaction_scanner_join/AbstractJoinExecutor.h"

namespace codablecash {

class JoinCandidateCursor;

class RightJoinExecutor : public AbstractJoinExecutor {
public:
	RightJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata, ScanJoinContext* context
			, AbstractScanCondition* filterCondition, LocalOidFactory* localOidFactory);
	virtual ~RightJoinExecutor();

	virtual void start(VirtualMachine* vm);
	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

private:
	void resetRightRecord();
	bool hasNextRightRecord(VirtualMachine* vm);
	void onChangeRight();

private:
	CdbRecord* rightRecord;
	JoinCandidateCursor* joinCandidateCursor;
	CdbRecord* nextRecord;

	bool joined;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHTJOINEXECUTOR_H_ */
