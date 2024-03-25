/*
 * ISnapshotHandler.h
 *
 *  Created on: 2023/06/30
 *      Author: iizuka
 */

#ifndef SNAPSHOT_ISNAPSHOTHANDLER_H_
#define SNAPSHOT_ISNAPSHOTHANDLER_H_

#include <cstdint>

namespace codablecash {

class AbstractSnapshotCommand;

class ISnapshotHandler {
public:
	ISnapshotHandler();
	virtual ~ISnapshotHandler();

	virtual void putCommand(const AbstractSnapshotCommand* command) = 0;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_ISNAPSHOTHANDLER_H_ */
