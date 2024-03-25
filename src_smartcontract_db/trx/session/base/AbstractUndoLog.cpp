/*
 * AbstractUndoLog.cpp
 *
 *  Created on: 2023/07/09
 *      Author: iizuka
 */

#include "trx/session/base/AbstractUndoLog.h"
#include "trx/session/base/UndoIds.h"

#include "trx/session/ddl/UndoCreateTableLog.h"
#include "trx/session/ddl/UndoDropTableLog.h"

#include "trx/session/ddl_alter/UndoAlterAddIndexLog.h"
#include "trx/session/ddl_alter/UndoAlterDropIndexLog.h"
#include "trx/session/ddl_alter/UndoAlterModifyLog.h"
#include "trx/session/ddl_alter/UndoAlterAddColumnLog.h"
#include "trx/session/ddl_alter/UndoAlterDropColumnLog.h"
#include "trx/session/ddl_alter/UndoAlterAddPrimaryKeyLog.h"
#include "trx/session/ddl_alter/UndoAlterDropPrimaryKeyLog.h"
#include "trx/session/ddl_alter/UndoAlterRenameColumnLog.h"
#include "trx/session/ddl_alter/UndoAlterRenameTableLog.h"

#include "trx/session/dml/UndoInsertLog.h"
#include "trx/session/dml/UndoUpdateLog.h"
#include "trx/session/dml/UndoDeleteLog.h"


namespace codablecash {

AbstractUndoLog::AbstractUndoLog(uint8_t type) {
	this->type = type;
}

AbstractUndoLog::~AbstractUndoLog() {

}

AbstractUndoLog* AbstractUndoLog::createFromBinary(ByteBuffer *in) {
	AbstractUndoLog* log = nullptr;

	uint8_t type = in->get();

	switch(type){
	case UNDO_TYPE_UNDO_IDS:
		log = new UndoIds();
		break;
	case UNDO_TYPE_CREATE_TABLE:
		log = new UndoCreateTableLog();
		break;
	case UNDO_TYPE_DROP_TABLE:
		log = new UndoDropTableLog();
		break;
	case UNDO_TYPE_ALTER_ADD_INDEX:
		log = new UndoAlterAddIndexLog();
		break;
	case UNDO_TYPE_ALTER_DROP_INDEX:
		log = new UndoAlterDropIndexLog();
		break;
	case UNDO_TYPE_ALTER_MODIFY:
		log = new UndoAlterModifyLog();
		break;
	case UNDO_TYPE_ALTER_ADD_COLUMN:
		log = new UndoAlterAddColumnLog();
		break;
	case UNDO_TYPE_ALTER_DROP_COLUMN:
		log = new UndoAlterDropColumnLog();
		break;
	case UNDO_TYPE_ALTER_ADD_PRIMARY_KEY:
		log = new UndoAlterAddPrimaryKeyLog();
		break;
	case UNDO_TYPE_ALTER_DROP_PRIMARY_KEY:
		log = new UndoAlterDropPrimaryKeyLog();
		break;
	case UNDO_TYPE_ALTER_RENAME_TABLE:
		log = new UndoAlterRenameTableLog();
		break;
	case UNDO_TYPE_ALTER_RENAME_COLUMN:
		log = new UndoAlterRenameColumnLog();
		break;
	case UNDO_TYPE_INSERT:
		log = new UndoInsertLog();
		break;
	case UNDO_TYPE_UPDATE:
		log = new UndoUpdateLog();
		break;
	case UNDO_TYPE_DELETE:
		log = new UndoDeleteLog();
		break;
	default:
		return nullptr;
	}

	log->fromBinary(in);

	return log;
}

} /* namespace codablecash */
