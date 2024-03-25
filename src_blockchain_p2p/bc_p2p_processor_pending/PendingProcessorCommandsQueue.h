/*
 * PendingProcessorCommandsQueue.h
 *
 *  Created on: 2023/11/10
 *      Author: iizuka
 */

#ifndef BC_P2P_PROCESSOR_PENDING_PENDINGPROCESSORCOMMANDSQUEUE_H_
#define BC_P2P_PROCESSOR_PENDING_PENDINGPROCESSORCOMMANDSQUEUE_H_

#include <cstdint>

namespace alinous {
class File;
class Btree;
class DiskCacheManager;
class ULongKey;
}
using namespace alinous;

namespace codablecash {

class PendingCommandData;

class PendingProcessorCommandsQueue {
public:
	static const constexpr wchar_t* DIR_NAME{L"pending_queue"};
	static const constexpr wchar_t* MAIN_DATA{L"main_data"};

	explicit PendingProcessorCommandsQueue(const File* baseDir);
	virtual ~PendingProcessorCommandsQueue();

	void createBlankDatabase();
	void open();
	void close();

	void put(const PendingCommandData* data);

	bool isEmpty() const;
	ULongKey* getFirstKey() const;
	PendingCommandData* fetchFirst() const;

private:
	File* baseDir;
	DiskCacheManager* cacheManager;
	Btree* dataStore;

	uint64_t serial;
};

} /* namespace codablecash */

#endif /* BC_P2P_PROCESSOR_PENDING_PENDINGPROCESSORCOMMANDSQUEUE_H_ */
