/*
 * RightWrappedLeftTransactionScanner.h
 *
 *  Created on: 2022/01/05
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTWRAPPEDLEFTTRANSACTIONSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTWRAPPEDLEFTTRANSACTIONSCANNER_H_

#include "trx/scan/transaction_scanner_join_right/AbstractRightTransactionScanner.h"

namespace codablecash {

class SingleKeyOidCache;
class OidKeyRecordCache;
class AbstractJoinCandidate;
class AbstractJoinCandidateCollection;

class CdbOidValueList;

class RightWrappedLeftTransactionScanner : public AbstractRightTransactionScanner {
public:
	RightWrappedLeftTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
			const CdbTable* table, const AbstractScanCondition* filterCondition, IJoinLeftSource* leftSource, AbstractJoinCandidate* joinCandidate, bool leftReverse);
	virtual ~RightWrappedLeftTransactionScanner();

	virtual void start(VirtualMachine* vm);
	virtual void reset(const AbstractCdbKey* key);

	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

private:
	AbstractJoinCandidateCollection* joinCandidate;
	bool leftReverse;

	IJoinLeftSource* leftSource;

	SingleKeyOidCache* keyOidCache;
	OidKeyRecordCache* oidRecordCache;

	const CdbOidValueList* data;
	int current;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTWRAPPEDLEFTTRANSACTIONSCANNER_H_ */
