/*
 * NodeConnectionSimulator.h
 *
 *  Created on: 2023/12/04
 *      Author: iizuka
 */

#ifndef BLOCKCHAIN_UTILS_NODECONNECTIONSIMULATOR_H_
#define BLOCKCHAIN_UTILS_NODECONNECTIONSIMULATOR_H_

#include <cstdint>

#include "pubsub/IPubsubCommandExecutor.h"
#include "pubsub/IPubsubCommandListner.h"

#include "bc_block_body/OmittedBlockBody.h"

namespace alinous {
class UnicodeString;
class File;
}
using namespace alinous;

namespace codablecash {

class P2pHandshake;
class ISystemLogger;
class PubSubId;
class P2pServerConnectionManager;
class NodeIdentifierSource;
class AbstractNodeCommand;
class AbstractCommandResponse;
class MemoryPool;

class NodeConnectionSimulator : public IPubsubCommandExecutor, public IPubsubCommandListner, public IBlockBodyFetcher {
public:
	explicit NodeConnectionSimulator(ISystemLogger* logger, const File* baseDir);
	virtual ~NodeConnectionSimulator();

	void connectIpv6Node(uint16_t zone, const UnicodeString* host, int port);
	void close();

	AbstractCommandResponse* signAndAbstractNodeCommand(const AbstractNodeCommand* commnad);

	virtual void fireExecuteCommand(const PubSubId* pubsubId, const AbstractPubSubCommand* cmd);
	virtual void fireOnExeptionOnSubscriber(const PubSubId* pubsubId, const Exception* e);
	virtual void fireOnSubscribeEnds(const PubSubId* pubsubId);

	virtual IPubsubCommandExecutor* getExecutor() const noexcept;

	virtual MemPoolTransaction* begin();
	virtual DownloadTransactionsNodeCommandResponse* downloadTransactions(const DownloadTransactionsNodeCommand* command) const;
	virtual NodeIdentifierSource* getNetworkKey() const noexcept;

private:
	void loginNode(uint16_t zone, P2pHandshake *handshake);

private:
	ISystemLogger* logger;
	PubSubId* psId;
	P2pHandshake *handshake;
	NodeIdentifierSource* source;

	P2pServerConnectionManager* connectionManager;

	MemoryPool* memPool;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_NODECONNECTIONSIMULATOR_H_ */
