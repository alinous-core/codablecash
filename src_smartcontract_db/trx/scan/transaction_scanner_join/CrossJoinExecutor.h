/*
 * CrossJoinExecutor.h
 *
 *  Created on: 2022/01/02
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_CROSSJOINEXECUTOR_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_CROSSJOINEXECUTOR_H_

#include "trx/scan/transaction_scanner_join/AbstractJoinExecutor.h"

namespace codablecash {

class JoinCandidateCursor;

class CrossJoinExecutor : public AbstractJoinExecutor {
public:
	CrossJoinExecutor(IJoinLeftSource* left, IJoinRightSource* right, ScanResultMetadata* metadata, ScanJoinContext* context
			, AbstractScanCondition* filterCondition, LocalOidFactory* localOidFactory);
	virtual ~CrossJoinExecutor();

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
	CdbRecord* nextRecord;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_CROSSJOINEXECUTOR_H_ */
