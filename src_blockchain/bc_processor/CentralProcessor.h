/*
 * CentralProcessor.h
 *
 *  Created on: 2023/04/01
 *      Author: iizuka
 */

#ifndef BC_PROCESSOR_CENTRALPROCESSOR_H_
#define BC_PROCESSOR_CENTRALPROCESSOR_H_

#include "procesor/ICommandParameter.h"

#include <cstdint>

namespace alinous {
class UnicodeString;
class SysMutex;
}
using namespace alinous;

namespace codablecash {

class MessageProcessor;
class CodablecashSystemParam;
class BlockchainController;
class MemoryPool;
class BlockchainController;
class Block;
class BlockHeaderId;
class ISystemLogger;
class ICommandMessage;
class P2pRequestProcessor;
class BlochchainP2pManager;

class CentralProcessor : public ICommandParameter {
public:
	static const UnicodeString THREAD_NAME;

	CentralProcessor(CodablecashSystemParam* config, MemoryPool* memPool, BlockchainController* ctrl, ISystemLogger* logger);
	virtual ~CentralProcessor();

	void start(const UnicodeString* nodeName);
	void shutdown();

	void setBlochchainP2pManager(BlochchainP2pManager* p2pManager) noexcept;
	void setP2pRequestProcessor(P2pRequestProcessor* p2pRequestProcessor) noexcept;

	void onMinedBlock(const Block* block);
	void requestFinalizing(uint16_t zone, uint64_t finalizingHeight, const BlockHeaderId* headerId) noexcept;
	void requestFinalizingHeader(uint16_t zone, uint64_t finalizingHeight, const BlockHeaderId* headerId) noexcept;

	void addCommandMessage(ICommandMessage* cmd);

	BlockchainController* getCtrl() const noexcept {
		return this->ctrl;
	}
	MemoryPool* getMemoryPool() const noexcept {
		return this->memPool;
	}
	P2pRequestProcessor* getP2pRequestProcessor() const noexcept {
		return this->p2pRequestProcessor;
	}
	BlochchainP2pManager* getBlochchainP2pManager() const noexcept {
		return this->p2pManager;
	}
	ISystemLogger* getLogger() const noexcept {
		return this->logger;
	}
	CodablecashSystemParam* getCodablecashSystemParam() const noexcept {
		return this->config;
	}

private:
	ISystemLogger* logger;
	CodablecashSystemParam* config;

	MessageProcessor* processor;

	MemoryPool* memPool;
	BlockchainController* ctrl;

	SysMutex* p2pRequestProcessorMutex;
	P2pRequestProcessor* p2pRequestProcessor;

	BlochchainP2pManager* p2pManager;

	bool end;
};

} /* namespace codablecash */

#endif /* BC_PROCESSOR_CENTRALPROCESSOR_H_ */
