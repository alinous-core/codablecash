/*
 * ClientListStakingNodeIdsCommandResponse.h
 *
 *  Created on: Aug 6, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTLISTSTAKINGNODEIDSCOMMANDRESPONSE_H_
#define BC_P2P_CMD_CLIENT_CLIENTLISTSTAKINGNODEIDSCOMMANDRESPONSE_H_

#include "pubsub_cmd/AbstractCommandResponse.h"

#include "base/ArrayList.h"


namespace codablecash {

class NodeIdentifier;
class MerkleCertificate;

class ClientListStakingNodeIdsCommandResponse : public AbstractCommandResponse {
public:
	ClientListStakingNodeIdsCommandResponse();
	virtual ~ClientListStakingNodeIdsCommandResponse();

	virtual void fromBinary(ByteBuffer* buff);
	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* buff) const;

	virtual UnicodeString* toString() const noexcept;

	void addNodeIdentifier(const NodeIdentifier* nodeId) noexcept;
	void addMerkleCertificate(const MerkleCertificate* cert) noexcept;

	void certifyNodeIds();

	ArrayList<NodeIdentifier>* getNodeIdentifierList() const noexcept {
		return this->list;
	}

private:
	ArrayList<NodeIdentifier>* list;
	ArrayList<MerkleCertificate>* certlist;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTLISTSTAKINGNODEIDSCOMMANDRESPONSE_H_ */
