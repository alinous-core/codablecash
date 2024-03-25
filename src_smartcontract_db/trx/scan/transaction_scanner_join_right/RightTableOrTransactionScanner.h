/*
 * RightTableOrTransactionScanner.h
 *
 *  Created on: 2020/12/20
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEORTRANSACTIONSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEORTRANSACTIONSCANNER_H_

#include "trx/scan/transaction_scanner_join_right/AbstractRightTransactionScanner.h"
#include "trx/scan/transaction_scanner_join_right/IRightOrTransactionScanner.h"

#include "base/ArrayList.h"

namespace codablecash {

class JoinOrCandidate;
class IJoinRightSource;
class JoinCandidateCursor;

class RightTableOrTransactionScanner : public AbstractRightTransactionScanner, public IRightOrTransactionScanner {
public:
	RightTableOrTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
			const CdbTable* table, const AbstractScanCondition* filterCondition, const JoinOrCandidate* orCandidate);
	virtual ~RightTableOrTransactionScanner();

	virtual void start(VirtualMachine* vm);
	virtual void restartScan(VirtualMachine* vm);
	virtual void reset(const AbstractCdbKey* key);

	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

	virtual void increaseOrCondition();

	void addRightSource(IJoinRightSource* joinSrc) noexcept;

private:
	JoinOrCandidate* orCandidate;

	ArrayList<IJoinRightSource> srclist;
	JoinCandidateCursor* joinCandidateCursor;

	IJoinRightSource* currentSource;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEORTRANSACTIONSCANNER_H_ */
