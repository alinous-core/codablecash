/*
 * LockinManager.h
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_LOCKINMANAGER_H_
#define BC_STATUS_CACHE_LOCKIN_LOCKINMANAGER_H_

#include <cstdint>

#include "bc_status_cache_lockin/ILockinManager.h"


namespace alinous {
class File;
class DiskCacheManager;
class Btree;
}
using namespace alinous;

namespace codablecash {

class AbstractLockinOperation;
class LockInOperationData;

class LockinManager : public ILockinManager {
public:
	static const constexpr wchar_t* NAME_LOCKIN_DATA{L"lockin_data"};

	explicit LockinManager(const File* baseDir);
	virtual ~LockinManager();

	void initBlank();

	void open();
	void close() noexcept;

	virtual void addOperation(uint64_t height, const AbstractLockinOperation* op);
	virtual LockInOperationData* getOperantions(uint64_t height);

	virtual uint64_t getFinalizingHeight() const noexcept {
		return finalizingHeight;
	}
	void setFinalizingHeight(uint64_t finalizingHeight) {
		this->finalizingHeight = finalizingHeight;
	}

private:
	File* baseDir;

	DiskCacheManager* cacheManager;
	Btree* lockinActionsIndex;

	uint64_t finalizingHeight;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_LOCKINMANAGER_H_ */
