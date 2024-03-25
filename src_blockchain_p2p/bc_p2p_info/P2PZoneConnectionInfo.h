/*
 * P2PZoneConnectionInfo.h
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#ifndef BC_P2P_INFO_P2PZONECONNECTIONINFO_H_
#define BC_P2P_INFO_P2PZONECONNECTIONINFO_H_

#include "base/ArrayList.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class P2pNodeRecordReference;
class NodeIdentifier;

class P2PZoneConnectionInfo {
public:
	P2PZoneConnectionInfo(const P2PZoneConnectionInfo& inst);
	P2PZoneConnectionInfo();
	virtual ~P2PZoneConnectionInfo();

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	static P2PZoneConnectionInfo* fromBinary(ByteBuffer* in);

	void addConnectedNode(const NodeIdentifier* nodeId);
	void removeConnectedNode(const NodeIdentifier* nodeId);

	ArrayList<P2pNodeRecordReference>* getReferenceList() const noexcept {
		return this->list;
	}

private:
	P2pNodeRecordReference* getConnectedNode(const NodeIdentifier* nodeId) const noexcept;

private:
	ArrayList<P2pNodeRecordReference>* list;
};

} /* namespace codablecash */

#endif /* BC_P2P_INFO_P2PZONECONNECTIONINFO_H_ */
