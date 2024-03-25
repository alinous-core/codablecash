/*
 * FileSnapshotRecoverer.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SNAPSHOT_FILESNAPSHOTRECOVERER_H_
#define SNAPSHOT_FILESNAPSHOTRECOVERER_H_

#include <cstdint>

#include "base/ArrayList.h"

#include "snapshot/ISnapshotRecoverer.h"

namespace alinous {
class FileInputStream;
class File;
}
using namespace alinous;

namespace codablecash {

class AbstractSnapshotCommand;
class CodableDatabase;

class FileSnapshotRecoverer : public ISnapshotRecoverer {
public:
	FileSnapshotRecoverer();
	virtual ~FileSnapshotRecoverer();

	void open(const File* file);
	void close() noexcept;

	void recover(CodableDatabase* db);

	virtual SchemaStartSnapshotCommand* getSchemaStartSnapshotCommand() const;
	virtual TableStartSnapshotCommand* getTableStartSnapshotCommand() const;

private:
	AbstractSnapshotCommand* findCommand(uint8_t type) const;

private:
	AbstractSnapshotCommand* readCommand(uint16_t size);
	uint16_t readShort();

	void pushStack(AbstractSnapshotCommand* cmd) noexcept;
	AbstractSnapshotCommand* popStack() noexcept;

private:
	FileInputStream* inStream;

	ArrayList<AbstractSnapshotCommand>* stack;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_FILESNAPSHOTRECOVERER_H_ */
