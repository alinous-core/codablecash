/*
 * FinalizerPool.h
 *
 *  Created on: 2023/05/02
 *      Author: iizuka
 */

#ifndef BC_FINALIZER_POOL_FINALIZERPOOL_H_
#define BC_FINALIZER_POOL_FINALIZERPOOL_H_

#include "procesor/ICommandParameter.h"

#include <cstdint>

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class MessageProcessor;
class BlockHeaderId;
class CodablecashSystemParam;
class MemoryPool;
class BlockchainController;
class NodeIdentifier;
class NodeIdentifierSource;
class ISystemLogger;
class P2pRequestProcessor;
class BlochchainP2pManager;

class FinalizerPool : public ICommandParameter {
public:
	static const UnicodeString THREAD_NAME;

	FinalizerPool(const CodablecashSystemParam* config, MemoryPool* memoryPool, BlockchainController* ctrl
			, P2pRequestProcessor* p2pRequestProcessor, BlochchainP2pManager* p2pManager, ISystemLogger* logger);
	virtual ~FinalizerPool();

	void start();
	void shutdown() noexcept;

	virtual void requestCheckVoting(uint16_t zone, const BlockHeaderId *headerId, uint64_t height);

	BlockchainController* getBlockchainController() const noexcept {
		return this->ctrl;
	}

	const CodablecashSystemParam* getConfig() const noexcept {
		return this->config;
	}

	void setVoterIdentifier(const NodeIdentifierSource* nodeSource);
	const NodeIdentifier getNodeIdentifier() const noexcept;

	const NodeIdentifierSource* getVoterIdentifierSource() const noexcept {
		return this->voterSource;
	}

	MemoryPool* getMemoryPool() const noexcept {
		return this->memoryPool;
	}

	P2pRequestProcessor* getP2pRequestProcessor() const noexcept {
		return this->p2pRequestProcessor;
	}
	BlochchainP2pManager* getBlochchainP2pManager() const noexcept {
		return this->p2pManager;
	}

protected:
	ISystemLogger* logger;

	MessageProcessor* processor;
	const CodablecashSystemParam* config;
	MemoryPool* memoryPool;
	BlockchainController* ctrl;

	P2pRequestProcessor* p2pRequestProcessor;
	BlochchainP2pManager* p2pManager;

	const NodeIdentifierSource *voterSource;
};

} /* namespace codablecash */

#endif /* BC_FINALIZER_POOL_FINALIZERPOOL_H_ */
