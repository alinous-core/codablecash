/*
 * AbstractSnapshotCommand.cpp
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#include "snapshot/AbstractSnapshotCommand.h"
#include "snapshot/IdsBackupCommand.h"
#include "snapshot/SchemaStartSnapshotCommand.h"
#include "snapshot/SchemaEndSnapshotCommand.h"
#include "snapshot/TableStartSnapshotCommand.h"
#include "snapshot/TableEndSnapshotCommand.h"
#include "snapshot/SnapshotIndexCommand.h"
#include "snapshot/SnapshotRecordCommand.h"

#include "base_io/ByteBuffer.h"

#include "bc_base/BinaryUtils.h"

namespace codablecash {

AbstractSnapshotCommand::AbstractSnapshotCommand(uint8_t type) {
	this->type = type;
}

AbstractSnapshotCommand::~AbstractSnapshotCommand() {

}

AbstractSnapshotCommand* AbstractSnapshotCommand::createFromBinary(ByteBuffer *in) {
	AbstractSnapshotCommand* ret = nullptr;

	uint8_t type = in->get();

	switch(type){
	case IDS_BACKUP:
		ret = new IdsBackupCommand();
		break;
	case SCHEMA_START:
		ret = new SchemaStartSnapshotCommand();
		break;
	case SCHEMA_END:
		ret = new SchemaEndSnapshotCommand();
		break;
	case TABLE_START:
		ret = new TableStartSnapshotCommand();
		break;
	case TABLE_END:
		ret = new TableEndSnapshotCommand();
		break;
	case INDEX:
		ret = new SnapshotIndexCommand();
		break;
	case RECORD:
		ret = new SnapshotRecordCommand();
		break;
	default:
		return nullptr;
	}

	ret->fromBinary(in);
	__ASSERT_POS(in);

	return ret;
}

} /* namespace codablecash */
