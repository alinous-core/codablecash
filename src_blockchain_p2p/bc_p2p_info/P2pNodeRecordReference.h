/*
 * P2pNodeReference.h
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#ifndef BC_P2P_INFO_P2PNODERECORDREFERENCE_H_
#define BC_P2P_INFO_P2PNODERECORDREFERENCE_H_

#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class NodeIdentifier;

class P2pNodeRecordReference {
public:
	P2pNodeRecordReference(const P2pNodeRecordReference& inst);
	explicit P2pNodeRecordReference(const NodeIdentifier* nodeId);
	virtual ~P2pNodeRecordReference();

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	static P2pNodeRecordReference* fromBinary(ByteBuffer* in);

	const NodeIdentifier* getNodeIdentifier() const noexcept {
		return this->nodeId;
	}

	void updateLastUpdatedTime() noexcept;

private:
	NodeIdentifier* nodeId;

	uint64_t lastUpdatedTime;
	uint64_t createdTime;
};

} /* namespace codablecash */

#endif /* BC_P2P_INFO_P2PNODERECORDREFERENCE_H_ */
