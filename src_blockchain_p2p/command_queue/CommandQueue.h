/*
 * CommandQueue.h
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#ifndef COMMAND_QUEUE_COMMANDQUEUE_H_
#define COMMAND_QUEUE_COMMANDQUEUE_H_

#include <cstdint>

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
class ULongKey;
}
using namespace alinous;

namespace codablecash {

class CommandQueueData;

class CommandQueue {
public:
	static const constexpr wchar_t* DIR_NAME{L"command_queue"};
	static const constexpr wchar_t* MAIN_DATA{L"main_data"};

	explicit CommandQueue(const File* baseDir);
	virtual ~CommandQueue();

	void createBlankDatabase();
	void open();
	void close();

	void addCommand(const CommandQueueData* cmd);

	CommandQueueData* getFirst() const;
	ULongKey* getFirstKey() const;
	CommandQueueData* getLast() const;

	bool isEmpty() const;
	CommandQueueData* fetchFirst() const;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;
	Btree* dataStore;

	uint64_t serial;
};

} /* namespace codablecash */

#endif /* COMMAND_QUEUE_COMMANDQUEUE_H_ */
