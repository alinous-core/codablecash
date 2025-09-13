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
#include "bc_block_body/IOmittedBlockBodyFixer.h"


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
class DownloadTransactionEntry;

class NodeConnectionSimulator : public IPubsubCommandExecutor, public IPubsubCommandListner, public IOmittedBlockBodyFixer {
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


	//
	virtual MemPoolTransaction* begin();
	virtual NodeIdentifierSource* getNetworkKey() const noexcept;

	virtual void addDownloadTransactionEntry(const DownloadTransactionEntry* entry);
	virtual void downloadTransactions(ISystemLogger *logger);
	virtual const AbstractBlockchainTransaction* get(const TransactionId* trxId) const noexcept;

	void setCommandData(uint64_t height, const BlockHeaderId* headerId);


	MemoryPool* getMemoryPool() const noexcept {
		return this->memPool;
	}

private:
	void loginNode(uint16_t zone, P2pHandshake *handshake);

private:
	ISystemLogger* logger;
	PubSubId* psId;
	P2pHandshake *handshake;
	NodeIdentifierSource* source;

	P2pServerConnectionManager* connectionManager;

	MemoryPool* memPool;

	//
	ArrayList<DownloadTransactionEntry>* entrylist;
	uint64_t height;
	const BlockHeaderId* headerId;

	HashMap<TransactionId, AbstractBlockchainTransaction>* map;
};

} /* namespace codablecash */

#endif /* BLOCKCHAIN_UTILS_NODECONNECTIONSIMULATOR_H_ */
