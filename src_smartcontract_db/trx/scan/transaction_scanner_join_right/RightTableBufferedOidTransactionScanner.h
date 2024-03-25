/*
 * RightTableBufferedOidTransactionScanner.h
 *
 *  Created on: 2020/12/24
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEBUFFEREDOIDTRANSACTIONSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEBUFFEREDOIDTRANSACTIONSCANNER_H_

#include "trx/scan/transaction_scanner_join_right/AbstractRightTransactionScanner.h"

namespace alinous {
class IBtreeScanner;
}
using namespace alinous;

namespace codablecash {

class OidKeyRecordCache;

/**
 * scan all for CROSS JOIN
 */
class RightTableBufferedOidTransactionScanner : public AbstractRightTransactionScanner {
public:
	RightTableBufferedOidTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
			const CdbTable* table, const AbstractScanCondition* filterCondition);
	virtual ~RightTableBufferedOidTransactionScanner();

	virtual void start(VirtualMachine* vm);
	virtual void reset(const AbstractCdbKey* key);

	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

private:
	OidKeyRecordCache* oidRecordCache;

	IBtreeScanner* scanner;
	CdbRecord* nextRecord;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTTABLEBUFFEREDOIDTRANSACTIONSCANNER_H_ */
