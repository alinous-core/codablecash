/*
 * NetworkInfoProcessor.h
 *
 *  Created on: 2023/09/05
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_NETWORKINFOPROCESSOR_H_
#define BC_P2P_PROCESSOR_NETWORKINFOPROCESSOR_H_

#include <cstdint>

namespace codablecash {

class MultipleCommandProcessor;
class P2pRequestProcessor;
class ISystemLogger;
class NodeNetworkInfo;

/********************
 * handle DNS info
 */
class NetworkInfoProcessor {
public:
	static const constexpr wchar_t* THREAD_NAME{L"NInf"};

	NetworkInfoProcessor(P2pRequestProcessor* processor, int numThreads, ISystemLogger* logger);
	virtual ~NetworkInfoProcessor();

	void start();
	void shutdown();

	void requestDnsCheck(const NodeNetworkInfo* nodeNetInfo);

private:
	MultipleCommandProcessor* executor;
	P2pRequestProcessor* processor; // command param
	int numThreads;
	ISystemLogger* logger;

	uint64_t serial;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_NETWORKINFOPROCESSOR_H_ */
