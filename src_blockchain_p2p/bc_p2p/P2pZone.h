/*
 * P2pZone.h
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#ifndef BC_P2P_P2PZONE_H_
#define BC_P2P_P2PZONE_H_

#include <cstdint>

#include "base/ArrayList.h"

namespace alinous {
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class BlockchainNodeHandshake;
class P2pHandshake;
class PubSubId;
class LoginClientCommand;
class ClientNodeHandshake;
class NodeIdentifier;
class P2pRequestProcessor;
class AbstractNodeCommand;
class AbstractClientNotifyCommand;

class P2pZone {
public:
	explicit P2pZone(uint16_t zone);
	virtual ~P2pZone();

	BlockchainNodeHandshake* add(P2pHandshake *handshake, const NodeIdentifier* nodeId);
	ClientNodeHandshake* addClient(P2pHandshake *handshake, const NodeIdentifier* nodeId);
	void remove(const PubSubId* pubsubId) noexcept;

	static BlockchainNodeHandshake* __getHandshake(const PubSubId* pubsubid, ArrayList<BlockchainNodeHandshake>* list) noexcept;
	static ClientNodeHandshake* __getClientHandshake(const PubSubId* pubsubid, ArrayList<ClientNodeHandshake>* list) noexcept;

	void generateTransferRequest(const NodeIdentifier *excludeNodeId, const AbstractNodeCommand *command, P2pRequestProcessor *processor);

	void generateClientNotifyRequest(AbstractClientNotifyCommand *commnad, P2pRequestProcessor *processor);

	int nodeSize() const noexcept;

	void exportNodesIdList(ArrayList<NodeIdentifier>* list) noexcept;

	bool checkRemovalStatus();

private:
	void checkAlreadyRegistered(const PubSubId* pubsubId);

	void __removeRemovable() noexcept;

private:
	uint16_t zone;
	ArrayList<BlockchainNodeHandshake> nodeslist;
	ArrayList<ClientNodeHandshake> clientslist;

	ArrayList<BlockchainNodeHandshake> removeList;

	SysMutex* mutex;
};

} /* namespace codablecash */

#endif /* BC_P2P_P2PZONE_H_ */
