/*
 * PendingClientCommandsQueue.h
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_PROCESSOR_PENDINGCLIENTCOMMANDSQUEUE_H_
#define BC_WALLET_NET_PROCESSOR_PENDINGCLIENTCOMMANDSQUEUE_H_

#include <cstdint>

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
class ULongKey;
}
using namespace alinous;

namespace codablecash {

class ClientCommandsQueueData;

class PendingClientCommandsQueue {
public:
	static const constexpr wchar_t* DIR_NAME{L"client_command_queue"};
	static const constexpr wchar_t* MAIN_DATA{L"pending_data"};

	explicit PendingClientCommandsQueue(const File* baseDir);
	virtual ~PendingClientCommandsQueue();

	void open();
	void close();

	void createBlankDatabase();

	bool isEmpty() const noexcept;

	ULongKey* getFirstKey() const;
	ClientCommandsQueueData* fetchFirst() const;

	void addCommnadData(const ClientCommandsQueueData* queueData);

private:
	File* baseDir;
	DiskCacheManager* cacheManager;
	Btree* dataStore;

	uint64_t serial;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_PROCESSOR_PENDINGCLIENTCOMMANDSQUEUE_H_ */
