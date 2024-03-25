/*
 * BlockchainNodeHandshake.h
 *
 *  Created on: 2023/04/05
 *      Author: iizuka
 */

#ifndef BC_P2P_BLOCKCHAINNODEHANDSHAKE_H_
#define BC_P2P_BLOCKCHAINNODEHANDSHAKE_H_

namespace alinous {
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class P2pHandshake;
class PubSubId;
class NodeIdentifier;
class AbstractNodeCommand;
class AbstractCommandResponse;
class AbstractClientNotifyCommand;

class BlockchainNodeHandshake {
public:
	explicit BlockchainNodeHandshake(P2pHandshake *handshake, int zone, const NodeIdentifier* nodeId);
	virtual ~BlockchainNodeHandshake();

	void dispose(bool force) noexcept;

	const PubSubId* getPubsubId() const noexcept;
	const NodeIdentifier* getNodeId() const noexcept {
		return this->nodeId;
	}
	P2pHandshake* getHandshake() const noexcept {
		return this->handshake;
	}
	SysMutex* getSysMutex() const noexcept {
		return this->mutex;
	}

	void incRef() noexcept;
	void decRef() noexcept;
	bool isDeletable() const noexcept;

	AbstractCommandResponse* sendCommnad(const AbstractNodeCommand *command);
	AbstractCommandResponse* sendCommnad(const AbstractClientNotifyCommand *command);

protected:
	int zone;
	P2pHandshake *handshake;
	NodeIdentifier* nodeId;

	SysMutex* mutex;
	int ref;

};

} /* namespace codablecash */

#endif /* BC_P2P_BLOCKCHAINNODEHANDSHAKE_H_ */
