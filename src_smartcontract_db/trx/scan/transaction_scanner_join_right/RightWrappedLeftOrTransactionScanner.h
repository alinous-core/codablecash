/*
 * RightWrappedLeftOrTransactionScanner.h
 *
 *  Created on: 2022/01/20
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTWRAPPEDLEFTORTRANSACTIONSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTWRAPPEDLEFTORTRANSACTIONSCANNER_H_

#include "trx/scan/transaction_scanner_join_right/AbstractRightTransactionScanner.h"
#include "trx/scan/transaction_scanner_join_right/IRightOrTransactionScanner.h"

#include "base/ArrayList.h"

namespace codablecash {

class AbstractJoinCandidate;
class JoinOrCandidate;
class AbstractJoinCandidateCollection;
class JoinCandidateCursor;
class SingleKeyOidCache;
class OidKeyRecordCache;
class CdbRecord;
class CdbOidValueList;

class RightWrappedLeftOrTransactionScanner : public AbstractRightTransactionScanner, public IRightOrTransactionScanner {
public:
	RightWrappedLeftOrTransactionScanner(ScanResultMetadata* metadata, CdbTransaction* trx,
			const CdbTable* table, const AbstractScanCondition* filterCondition, IJoinLeftSource* leftSource, JoinOrCandidate* joinCandidate, bool leftReverse);
	virtual ~RightWrappedLeftOrTransactionScanner();

	virtual void start(VirtualMachine* vm);
	virtual void reset(const AbstractCdbKey* key);

	virtual bool hasNext(VirtualMachine* vm);
	virtual const CdbRecord* next(VirtualMachine* vm);

	virtual void shutdown();

	virtual void increaseOrCondition();

private:
	void makeCaches(const CdbRecord* record);

private:
	JoinOrCandidate* joinCandidate;
	bool leftReverse;

	IJoinLeftSource* leftSource;

	JoinCandidateCursor* joinCandidateCursor;
	ArrayList<SingleKeyOidCache> caches;

	OidKeyRecordCache* oidRecordCache;

	const CdbOidValueList* data;
	int current;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_RIGHTWRAPPEDLEFTORTRANSACTIONSCANNER_H_ */
