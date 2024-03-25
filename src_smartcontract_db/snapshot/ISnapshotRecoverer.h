/*
 * ISnapshotRecoverer.h
 *
 *  Created on: 2023/07/03
 *      Author: iizuka
 */

#ifndef SNAPSHOT_ISNAPSHOTRECOVERER_H_
#define SNAPSHOT_ISNAPSHOTRECOVERER_H_

namespace codablecash {

class SchemaStartSnapshotCommand;
class TableStartSnapshotCommand;

class ISnapshotRecoverer {
public:
	ISnapshotRecoverer();
	virtual ~ISnapshotRecoverer();

	virtual SchemaStartSnapshotCommand* getSchemaStartSnapshotCommand() const = 0;
	virtual TableStartSnapshotCommand* getTableStartSnapshotCommand() const = 0;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_ISNAPSHOTRECOVERER_H_ */
