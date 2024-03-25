/*
 * JoinCandidateCursor.h
 *
 *  Created on: 2021/01/23
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_JOINCANDIDATECURSOR_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_JOINCANDIDATECURSOR_H_

#include "base/ArrayList.h"

namespace alinous {
class VirtualMachine;
}
using namespace alinous;

namespace codablecash {

class AbstractJoinCandidate;
class AbstractJoinCandidateCollection;
class CdbRecord;
class AbstractCdbKey;
class RecordScanIdentityCache;

class JoinCandidateCursor {
public:
	explicit JoinCandidateCursor(AbstractJoinCandidate* joinCandidate);
	virtual ~JoinCandidateCursor();

	void init(VirtualMachine* vm, bool isleft) noexcept;
	bool finished() const noexcept;
	void inc() noexcept;
	void resetPos() noexcept;

	AbstractCdbKey* makeKey(const CdbRecord* leftRecord) noexcept;
	AbstractCdbKey* makeKeyRight(const CdbRecord* rightRecord) noexcept;

	int getPos() const noexcept {
		return this->pos;
	}

	bool hasOrConditions() const noexcept;
	bool hasAlreadyScanned(const CdbRecord* record);

	ArrayList<const AbstractJoinCandidateCollection>* getCandidatesList() noexcept{
		return &this->list;
	}

private:
	AbstractJoinCandidate* joinCandidate;
	ArrayList<const AbstractJoinCandidateCollection> list;

	int pos;

	RecordScanIdentityCache* cache;
};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_JOINCANDIDATECURSOR_H_ */
