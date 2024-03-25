/*
 * BlockBodyStore.h
 *
 *  Created on: 2023/03/01
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BODY_BLOCKBODYSTORE_H_
#define BC_BLOCKSTORE_BODY_BLOCKBODYSTORE_H_

#include <cstdint>

namespace alinous {
class File;
class DiskCacheManager;
class VariableBlockFileStore;
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class BlockBody;

class BlockBodyStore {
public:
	static const constexpr wchar_t* FILE_NAME{L"bodystore"};

	BlockBodyStore(int index, const File* baseDir, DiskCacheManager* cacheManager);
	virtual ~BlockBodyStore();

	bool exists() const noexcept;
	void create();

	void open();
	void close() noexcept;

	uint64_t storeBody(const BlockBody* body);
	BlockBody* loadBody(uint64_t fpos);
	void removeBody(uint64_t fpos);

private:
	File getStoreFile() const noexcept;
	File getStoreHeaderFile() const noexcept;
	void addIdex2String(UnicodeString* str) const noexcept;

private:
	int index;
	File* baseDir;
	DiskCacheManager* cacheManager;
	VariableBlockFileStore* store;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BODY_BLOCKBODYSTORE_H_ */
