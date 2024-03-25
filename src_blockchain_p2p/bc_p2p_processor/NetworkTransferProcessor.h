/*
 * NetworkTransferProcessor.h
 *
 *  Created on: 2023/09/05
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_NETWORKTRANSFERPROCESSOR_H_
#define BC_P2P_PROCESSOR_NETWORKTRANSFERPROCESSOR_H_

#include <cstdint>

namespace codablecash {

class MultipleCommandProcessor;
class P2pRequestProcessor;
class ISystemLogger;
class NodeIdentifier;
class AbstractNodeCommand;
class AbstractClientNotifyCommand;

class NetworkTransferProcessor {
public:
	static const constexpr wchar_t* THREAD_NAME{L"NetNodeTrx"};

	NetworkTransferProcessor(P2pRequestProcessor* processor, int numThreads, ISystemLogger* logger);
	virtual ~NetworkTransferProcessor();

	void start();
	void shutdown();

	void reserveTransfer(const NodeIdentifier* nodeId, const AbstractNodeCommand *command);
	void reserveClientNotifyTransfer(const NodeIdentifier* nodeId, const AbstractClientNotifyCommand *command);

private:
	MultipleCommandProcessor* executor;
	P2pRequestProcessor* processor; // command param
	int numThreads;
	ISystemLogger* logger;

	uint64_t serial;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_NETWORKTRANSFERPROCESSOR_H_ */
