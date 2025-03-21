/*
 * P2pClient.h
 *
 *  Created on: 2023/09/09
 *      Author: iizuka
 */

#ifndef BC_P2P_CLIENT_P2PCLIENT_H_
#define BC_P2P_CLIENT_P2PCLIENT_H_

#include <cstdint>

#include "pubsub/IPubsubCommandListner.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class ISystemLogger;
class PubSubId;
class P2pHandshake;
class NodeIdentifierSource;
class AbstractClientRequestCommand;
class AbstractCommandResponse;
class ClientExecutor;
class IClientNotifyListner;
class AbstractBlockchainPubsubCommand;

class P2pClient : public IPubsubCommandListner {
public:
	explicit P2pClient(uint16_t zone, ISystemLogger* logger);
	virtual ~P2pClient();

	void close() noexcept;
	void close(bool force) noexcept;

	void sendEndConnectionCommand();

	void connect(int protocol, const UnicodeString* hostName, int port);
	void connectIpV4(const UnicodeString* hostName, int port);
	void connectIpV6(const UnicodeString* hostName, int port);

	void sign(AbstractBlockchainPubsubCommand* cmd) const;
	AbstractCommandResponse* sendCommnad(const AbstractClientRequestCommand* cmd);

	bool ping();

	virtual void fireExecuteCommand(const AbstractPubSubCommand* cmd);
	virtual void fireOnExeptionOnSubscriber(const Exception* e);
	virtual void fireOnSubscribeEnds();

	virtual IPubsubCommandExecutor* getExecutor() const noexcept;


	void addListner(IClientNotifyListner* listner) noexcept;
	bool removeListner(IClientNotifyListner* listner) noexcept;

#ifndef __DEBUG__
private:
#endif
	void __connectIpV6(const UnicodeString* hostName, int port);
	void __connectIpV4(const UnicodeString* hostName, int port);
	void __connectLogin(const UnicodeString* hostName, int port);

	NodeIdentifierSource* getNodeIdentifierSource() const noexcept {
		return this->source;
	}

private:
	ISystemLogger* logger;
	UnicodeString* hostName;
	int port;

	NodeIdentifierSource* source;
	uint16_t zone;
	PubSubId* psId;
	P2pHandshake *handshake;

	ClientExecutor* commandExecutor;
};

} /* namespace codablecash */

#endif /* BC_P2P_CLIENT_P2PCLIENT_H_ */
