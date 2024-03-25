/*
 * BlochchainP2pManager.h
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#ifndef BC_P2P_BLOCHCHAINP2PMANAGER_H_
#define BC_P2P_BLOCHCHAINP2PMANAGER_H_

#include "pubsub/IPubsubConnectionListener.h"

#include "base/ArrayList.h"
#include "base/HashMap.h"

namespace alinous {
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class P2pZone;
class PubSubId;
class BlockchainNodeHandshake;
class ClientNodeHandshake;
class NodeIdentifier;
class AbstractNodeCommand;
class P2pRequestProcessor;
class AbstractClientNotifyCommand;

class BlochchainP2pManager : public IPubsubConnectionListener {
public:
	BlochchainP2pManager();
	virtual ~BlochchainP2pManager();

	void init(int numZones);

	void resetZones() noexcept;

	virtual void onLoginHandshake(P2pHandshake *handshake, const LoginPubSubCommand *loginCommand);
	virtual void onLoginClinentHandshake(P2pHandshake *handshake, const LoginClientCommand *clientLoginCommand);
	virtual void onHandshakeEnds(P2pHandshake *handshake);

	void registerHandshake(uint16_t zone, P2pHandshake *handshake, const NodeIdentifier* nodeId);

	BlockchainNodeHandshake* getNodeHandshake(const PubSubId* pubsubId) const noexcept;
	ClientNodeHandshake* getClientHandshake(const PubSubId* pubsubId) const noexcept;

	BlockchainNodeHandshake* getNodeHandshakeByNodeId(const NodeIdentifier* nodeId) const noexcept;
	ClientNodeHandshake* getClientHandshakeByNodeId(const NodeIdentifier* nodeId) const noexcept;

	void bloadCastWithinZone(uint16_t zoneSelf, const AbstractNodeCommand* command, P2pRequestProcessor* processor);
	void bloadCastWithinZone(uint16_t zoneSelf, const NodeIdentifier* excludeNodeId, const AbstractNodeCommand* command, P2pRequestProcessor* processor);

	void bloadCastAllZones(const NodeIdentifier* excludeNodeId, const AbstractNodeCommand* command, P2pRequestProcessor* processor);

	void bloadCastToClients(AbstractClientNotifyCommand* commnad, P2pRequestProcessor* processor);

	int getNumNodes(uint16_t zone) const;
	ArrayList<NodeIdentifier>* getNodeIds(uint16_t zone) const;

	void endCommunication() noexcept;

private:
	void __removeHandshake(const PubSubId* pubsubId);
	void __bloadCastWithinZone(uint16_t zone, const NodeIdentifier* excludeNodeId, const AbstractNodeCommand* command, P2pRequestProcessor* processor);

private:
	SysMutex* mutex;
	ArrayList<P2pZone>* zones;

	HashMap<PubSubId, BlockchainNodeHandshake> blockchainHandshakeHash;
	HashMap<PubSubId, ClientNodeHandshake> clientHandshakeHash;

	bool end;
};

} /* namespace codablecash */

#endif /* BC_P2P_BLOCHCHAINP2PMANAGER_H_ */
