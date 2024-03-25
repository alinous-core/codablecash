/*
 * RightTableTransactionScanner.h
 *
 *  Created on: 2020/12/19
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEBUFFEREDTRANSACTIONSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEBUFFEREDTRANSACTIONSCANNER_H_

#include "trx/scan/transaction_scanner_join_right/AbstractRightTransactionScanner.h"

namespace codablecash {

class AbstractJoinCandidate;
class AbstractJoinCandidateCollection;
class SingleKeyOidCache;
class OidKeyRecordCache;
class CdbOidValueList;

class RightTableBufferedTransactionScanner : public AbstractRightTransactionScanner {
public:
	RightTableBufferedTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
			const CdbTable* table, const AbstractScanCondition* filterCondition, AbstractJoinCandidate* joinCandidate, bool leftReverse);
	virtual ~RightTableBufferedTransactionScanner();

	virtual void start(VirtualMachine* vm);
	virtual void restartScan(VirtualMachine* vm);
	virtual void reset(const AbstractCdbKey* key);

	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

private:
	AbstractJoinCandidateCollection* joinCandidate;
	bool leftReverse;

	const CdbOidValueList* data;
	int current;

	SingleKeyOidCache* keyOidCache;
	OidKeyRecordCache* oidRecordCache;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEBUFFEREDTRANSACTIONSCANNER_H_ */
