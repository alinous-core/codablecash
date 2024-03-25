/*
 * ConnectNodeSelector.h
 *
 *  Created on: 2023/10/19
 *      Author: iizuka
 */

#ifndef BC_P2P_SELECTOR_CONNECTNODESELECTOR_H_
#define BC_P2P_SELECTOR_CONNECTNODESELECTOR_H_

#include "base/ArrayList.h"


namespace alinous {
class BigInteger;
}  // namespace alinous
using namespace alinous;

namespace codablecash {

class P2pNodeRecord;

class ConnectNodeSelector {
public:
	ConnectNodeSelector();
	virtual ~ConnectNodeSelector();

	void importCandidates(const ArrayList<P2pNodeRecord>* list);

	bool hasNext() const noexcept;
	const P2pNodeRecord* fetchNext() const;

private:
	int toIndex(int listsize) const noexcept;

private:
	ArrayList<P2pNodeRecord>* candidates;
	ArrayList<P2pNodeRecord>* usedCandidates;

	BigInteger* randomValue;
};

} /* namespace codablecash */

#endif /* BC_P2P_SELECTOR_CONNECTNODESELECTOR_H_ */
