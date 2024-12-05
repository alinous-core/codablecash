/*
 * VoterData.h
 *
 *  Created on: 2024/05/01
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_VOTE_VOTERDATA_H_
#define BC_STATUS_CACHE_VOTE_VOTERDATA_H_

#include <cstdint>

#include "base/ArrayList.h"

namespace alinous {
class ByteBuffer;
class SystemTimestamp;
}
using namespace alinous;

namespace codablecash {

class NodeIdentifier;
class VoteBlockTransaction;

class VoterData {
public:
	VoterData(const VoterData& inst);
	VoterData();
	virtual ~VoterData();

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	static VoterData* createFromBinary(ByteBuffer* in);

	void validateByTimeLimit(const SystemTimestamp* timestamp);

private:
	NodeIdentifier* voterId;
	ArrayList<VoteBlockTransaction>* list;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_VOTE_VOTERDATA_H_ */
