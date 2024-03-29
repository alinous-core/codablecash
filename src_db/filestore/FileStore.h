/*
 * FileStore.h
 *
 *  Created on: 2018/05/18
 *      Author: iizuka
 */

#ifndef FILESTORE_FILESTORE_H_
#define FILESTORE_FILESTORE_H_

#include <stdint.h>

namespace alinous {

class DiskCacheManager;
class RandomAccessFile;
class UnicodeString;
class File;

class FileStore {
public:
	FileStore(const FileStore& base) = delete;
	FileStore(const UnicodeString* dir, const UnicodeString* name, DiskCacheManager* cacheManager) noexcept;
	virtual ~FileStore() noexcept;

	void createStore(bool del, uint64_t defaultSize) noexcept(false);
	bool exists() const noexcept;

	void open(bool sync) noexcept(false);
	bool isOpened() const noexcept;
	void close() noexcept;

	void deleteFiles() const noexcept;

protected:
	void openFile(File& baseDir, bool sync);
	void openHeaderFile(File& baseDir, bool sync);
	void deleteLastFiles(File& baseDir) const noexcept;
private:
	DiskCacheManager* cacheManager;
protected:
	RandomAccessFile* file;
	RandomAccessFile* headerFile;

	UnicodeString* dir;
	UnicodeString* name;
};

} /* namespace alinous */

#endif /* FILESTORE_FILESTORE_H_ */
