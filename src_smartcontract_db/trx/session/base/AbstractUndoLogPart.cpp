/*
 * AbstractUndoLogPart.cpp
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#include "trx/session/base/AbstractUndoLogPart.h"

#include "trx/session/record/UndoDeleteRecordPart.h"
#include "trx/session/record/UndoInsertRecordPart.h"
#include "trx/session/record/UndoUpdateRecordPart.h"
#include "trx/session/record/UndoModifyRecordColumnsPart.h"
#include "trx/session/record/UndoDeletedRecordColumnPart.h"
#include "trx/session/record/UndoCreateSchemaPart.h"

#include "trx/session/index/UndoIndexUniquePart.h"
#include "trx/session/index/UndoAddIndexPart.h"
#include "trx/session/index/UndoRemoveIndexPart.h"
#include "trx/session/index/UndoIndexPrimaryPart.h"


namespace codablecash {

AbstractUndoLogPart::AbstractUndoLogPart(uint8_t type) {
	this->type = type;
}

AbstractUndoLogPart::~AbstractUndoLogPart() {

}

AbstractUndoLogPart* AbstractUndoLogPart::createFromBinary(ByteBuffer *in) {
	AbstractUndoLogPart* log = nullptr;

	uint8_t type = in->get();

	switch(type){
	case PART_UNDO_DELETE_RECORD:
		log = new UndoDeleteRecordPart();
		break;
	case PART_UNDO_INSERT_RECORD:
		log = new UndoInsertRecordPart();
		break;
	case PART_UNDO_UPDATE_RECORD:
		log = new UndoUpdateRecordPart();
		break;
	case PART_UNDO_UPDATE_RECORD_COLUMNS:
		log = new UndoModifyRecordColumnsPart();
		break;
	case PART_UNDO_DELETED_RECORD_COLUMN:
		log = new UndoDeletedRecordColumnPart();
		break;
	case PART_UNDO_INDEX_UNIQUE:
		log = new UndoIndexUniquePart();
		break;
	case PART_UNDO_ADD_INDEX:
		log = new UndoAddIndexPart();
		break;
	case PART_UNDO_REMOVE_INDEX:
		log = new UndoRemoveIndexPart();
		break;
	case PART_UNDO_INDEX_PRIMARY:
		log = new UndoIndexPrimaryPart();
		break;
	case PART_UNDO_CREATE_SCHEMA:
		log = new UndoCreateSchemaPart();
		break;
	default:
		return nullptr;
	}

	log->fromBinary(in);

	return log;
}

} /* namespace codablecash */
