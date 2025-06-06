/*
 * funcs.h
 *
 *  Created on: 2018/04/18
 *      Author: iizuka
 */

#ifndef OSENV_FUNCS_H_
#define OSENV_FUNCS_H_

#include "base/ArrayList.h"
#include "base/RawArrayPrimitive.h"

#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>


namespace alinous {



#ifdef _WIN64
#define PATH_SEPARATOR L"\\"
#elseif _WIN32
#define PATH_SEPARATOR L"\\"
#else
#define PATH_SEPARATOR L"/"
#endif

#define THREAD_ID pthread_t
#define THREAD_KEY pthread_key_t

class UnicodeString;
class File;
class SystemTimestamp;

typedef void *(*SysThreadRoutine) (void *);

class FileDescriptor {
public:
	int fd;
	FileDescriptor() : fd(0) {};
	explicit FileDescriptor(int fd) : fd(fd) {};
	bool isOpened() const noexcept;
};


class Os {
public:
	/**************************************************************************
	 * Thread functions
	 */
	static THREAD_ID getCurrentThreadId() noexcept;
	static THREAD_ID createThread(SysThreadRoutine threadFunc, void* params) noexcept;
	static void setThreadName(THREAD_ID id, const char* name) noexcept;
	static UnicodeString* getThreadName(THREAD_ID id) noexcept;
	static void joinThread(THREAD_ID id) noexcept;
	static int threadKeyCreate(THREAD_KEY* key);
	static int threadKeyDelete(THREAD_KEY key);
	static int threadSetSpecific(THREAD_KEY key, const void* value) noexcept;
	static void* threadGetSpecific(THREAD_KEY key) noexcept;

	static void usleep(uint32_t microsec) noexcept;

	static uint64_t getMicroSec() noexcept;
	static uint64_t getTimestampLong() noexcept;
	static SystemTimestamp now() noexcept;

	/**************************************************************************
	 * File functions
	 */
	static char* realpath(const char *path, char *resolved_path) noexcept;
	static void realpath_free(char* path) noexcept;
	static bool file_exists(UnicodeString* path) noexcept;
	static int mkdirs(UnicodeString *path,
			bool __S_IRUSR , bool __S_IWUSR, bool __S_IXUSR,
			bool __S_IRGRP , bool __S_IWGRP, bool __S_IXGRP,
			bool __S_IROTH , bool __S_IWOTH, bool __S_IXOTH) noexcept;
	static int deleteFile(const UnicodeString *path) noexcept;
	static bool isDirectory(const UnicodeString* path) noexcept;
	static bool isFile(const UnicodeString* path) noexcept;
	static ArrayList<UnicodeString>* listFiles(const UnicodeString* path) noexcept;
	static int64_t fileLength(const File* const file) noexcept;
	static bool rename(const UnicodeString* src, const UnicodeString* dest) noexcept;

	static uint64_t getSystemPageSize() noexcept;

	static FileDescriptor openFile2Write(const File *file, bool append, bool sync) noexcept;
	static FileDescriptor openFile2Read(const File *file) noexcept;
	static FileDescriptor openFile2ReadWrite(const File *file, bool sync) noexcept;
	static void closeFileDescriptor(FileDescriptor *fd) noexcept;

	static int write2File(const FileDescriptor* fd, const char* buff, int length) noexcept;
	static int readFile(const FileDescriptor* fd, char* buffer, int size) noexcept;
	static int syncFile(const FileDescriptor* fd) noexcept;

	enum SeekOrigin {
		FROM_BEGINING = SEEK_SET,
		CURRENT_POS = SEEK_CUR,
		FROM_END = SEEK_END
	};
	static uint64_t seekFile(const FileDescriptor* fd, int64_t offset, SeekOrigin origin) noexcept;

	/**************************************************************************
	 * File functions
	 */
	static int random() noexcept;
};

}

#endif /* OSENV_FUNCS_H_ */
