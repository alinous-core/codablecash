/*
 * SnapShotSha256.h
 *
 *  Created on: 2023/07/03
 *      Author: iizuka
 */

#ifndef SNAPSHOT_SNAPSHOTSHA256_H_
#define SNAPSHOT_SNAPSHOTSHA256_H_

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class Sha256Context;

class SnapShotSha256 {
public:
	explicit SnapShotSha256(const File* file);
	virtual ~SnapShotSha256();

	void process();

	bool equals(const SnapShotSha256* other) const  noexcept;

private:
	File* file;
	Sha256Context* context;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_SNAPSHOTSHA256_H_ */
