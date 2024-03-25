/*
 * FileStoreSnapshot.h
 *
 *  Created on: 2023/06/30
 *      Author: iizuka
 */

#ifndef SNAPSHOT_FILESTORESNAPSHOT_H_
#define SNAPSHOT_FILESTORESNAPSHOT_H_

#include "snapshot/ISnapshotHandler.h"

namespace alinous {
class FileOutputStream;
class File;
}
using namespace alinous;

namespace codablecash {

class FileStoreSnapshot : public ISnapshotHandler {
public:
	FileStoreSnapshot();
	virtual ~FileStoreSnapshot();

	virtual void putCommand(const AbstractSnapshotCommand* command);

	void openOutFile(const File* file);
	void close() noexcept;

private:
	FileOutputStream* outStream;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_FILESTORESNAPSHOT_H_ */
