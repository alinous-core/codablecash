/*
 * SnapshotFileReader.h
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */

#ifndef SNAPSHOT_CHECKER_SNAPSHOTFILEREADER_H_
#define SNAPSHOT_CHECKER_SNAPSHOTFILEREADER_H_

#include <cstdint>

namespace alinous {
class File;
class FileInputStream;
}
using namespace alinous;

namespace codablecash {

class AbstractSnapshotCommand;

class SnapshotFileReader {
public:
	explicit SnapshotFileReader(const File* file);
	virtual ~SnapshotFileReader();

	void init();
	void close() noexcept;

	uint16_t readShort();
	AbstractSnapshotCommand* readCommand(uint16_t size);

private:
	File* file;
	FileInputStream* inStream;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_CHECKER_SNAPSHOTFILEREADER_H_ */
