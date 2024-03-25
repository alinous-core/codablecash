/*
 * StatusStore.h
 *
 *  Created on: 2022/05/14
 *      Author: iizuka
 */

#ifndef BC_BASE_CONF_STORE_STATUSSTORE_H_
#define BC_BASE_CONF_STORE_STATUSSTORE_H_

#include "base/HashMap.h"

namespace alinous {
class RandomAccessFile;
class File;
class DiskCacheManager;
class UnicodeString;
class ByteBuffer;
class ConcurrentGate;
}
using namespace alinous;

namespace codablecash {
class AbstractConfigStoreElement;

class StatusStore {
public:
	StatusStore(const File* baseDir, const UnicodeString* name);
	StatusStore(const File* baseDir, const wchar_t* name);
	virtual ~StatusStore();

	bool exists() const;

	void load();

	void open();
	void close() noexcept;

	void addLongValue(const wchar_t* key, int64_t value);
	void addShortValue(const wchar_t* key, int16_t);
	void addShortValue(const UnicodeString* key, int16_t);

	void addBinaryValue(const wchar_t* key, const uint8_t* data, int length);

	void addValue(const UnicodeString* key, AbstractConfigStoreElement* value);

	const AbstractConfigStoreElement* getValue(const wchar_t* key) const noexcept;
	const AbstractConfigStoreElement* getValue(const UnicodeString* key) const noexcept;

	int16_t getShortValue(const UnicodeString* key) const noexcept;
	int16_t getShortValue(const wchar_t* key) const noexcept;
	int64_t getLongValue(const wchar_t* key) const noexcept;
	ByteBuffer* getBinaryValue(const wchar_t* key) const noexcept;

private:
	void write();

	int binarySize() const;

	int stringSize(const UnicodeString* str) const noexcept;
	void putString(ByteBuffer* out, const UnicodeString* str) const noexcept;
	UnicodeString* getString(ByteBuffer* in) const noexcept;

	void __nlk_addValue(const UnicodeString* key, AbstractConfigStoreElement* value);

private:
	File* file;
	File* baseDir;

	DiskCacheManager* diskCacheManager;
	RandomAccessFile* store;

	HashMap<UnicodeString, AbstractConfigStoreElement> map;

	ConcurrentGate* rwLock;
};

} /* namespace codablecash */

#endif /* BC_BASE_CONF_STORE_STATUSSTORE_H_ */
