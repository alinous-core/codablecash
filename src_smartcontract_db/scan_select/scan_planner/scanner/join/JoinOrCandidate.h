/*
 * JoinBranchCandidate.h
 *
 *  Created on: 2020/09/01
 *      Author: iizuka
 */

#ifndef SCAN_PLANNER_SCANNER_CTX_JOINBRANCHCANDIDATE_H_
#define SCAN_PLANNER_SCANNER_CTX_JOINBRANCHCANDIDATE_H_

#include "scan_select/scan_planner/scanner/join/AbstractJoinCandidate.h"

#include "base/ArrayList.h"

using alinous::ArrayList;

namespace codablecash {

class AbstractJoinCandidateCollection;

class JoinOrCandidate : public AbstractJoinCandidate {
public:
	JoinOrCandidate(const JoinOrCandidate& inst);
	explicit JoinOrCandidate(int joinType);
	virtual ~JoinOrCandidate();

	virtual const AbstractScanTableTarget* getLeftTarget() const noexcept;
	virtual const AbstractScanTableTarget* getRightTarget() const noexcept;

	virtual AbstractJoinCandidate::CandidateType getCandidateType() const noexcept;
	virtual AbstractJoinCandidate* multiply(const AbstractJoinCandidate* other) const noexcept;
	virtual AbstractJoinCandidate* copy() const noexcept;

	virtual int getOverHeadScore(AbstractScanTableTarget* left, AbstractScanTableTarget* right) const noexcept;
	virtual CdbTableIndex* getIndex(const AbstractScanTableTarget* right) const noexcept;

	void add(const AbstractJoinCandidate* candidate) noexcept;

	int size() const noexcept;
	const AbstractJoinCandidateCollection* get(int i) const noexcept;

private:
	AbstractJoinCandidate* multiplyOr(const JoinOrCandidate* other) const noexcept;
	void multiply(const JoinOrCandidate* other, const AbstractJoinCandidateCollection* col, JoinOrCandidate* newCond) const noexcept;

	void addOr(const JoinOrCandidate* candidate) noexcept;

private:
	ArrayList<AbstractJoinCandidateCollection> list;
};

} /* namespace codablecash */

#endif /* SCAN_PLANNER_SCANNER_CTX_JOINBRANCHCANDIDATE_H_ */
