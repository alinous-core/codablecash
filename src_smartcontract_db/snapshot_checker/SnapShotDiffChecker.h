/*
 * SnapShotDiffChecker.h
 *
 *  Created on: 2023/07/29
 *      Author: iizuka
 */

#ifndef SNAPSHOT_SNAPSHOTDIFFCHECKER_H_
#define SNAPSHOT_SNAPSHOTDIFFCHECKER_H_

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class SnapshotFileReader;

class SnapShotDiffChecker {
public:
	SnapShotDiffChecker(const File* a, const File* b);
	virtual ~SnapShotDiffChecker();

	void test();

private:
	SnapshotFileReader* readerA;
	SnapshotFileReader* readerB;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_SNAPSHOTDIFFCHECKER_H_ */
