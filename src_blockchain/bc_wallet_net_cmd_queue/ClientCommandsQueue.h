/*
 * ClientCommandsQueue.h
 *
 *  Created on: Mar 26, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUE_H_
#define BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUE_H_

#include <cstdint>

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
class ULongKey;
}
using namespace alinous;

namespace codablecash {

class AbstractClientQueueCommand;

class ClientCommandsQueue {
public:
	static const constexpr wchar_t* DIR_NAME{L"client_command_queue"};
	static const constexpr wchar_t* MAIN_DATA{L"main_data"};

	explicit ClientCommandsQueue(const File* baseDir);
	virtual ~ClientCommandsQueue();

	void createBlankDatabase();
	void open();
	void close();

	void addCommand(const AbstractClientQueueCommand* cmd);

	AbstractClientQueueCommand* getFirst() const;
	ULongKey* getFirstKey() const;
	AbstractClientQueueCommand* getLast() const;

	bool isEmpty() const;
	AbstractClientQueueCommand* fetchFirst() const;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;
	Btree* dataStore;

	uint64_t serial;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_CMD_QUEUE_CLIENTCOMMANDSQUEUE_H_ */
