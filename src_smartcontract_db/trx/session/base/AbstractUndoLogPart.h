/*
 * AbstractUndoLogPart.h
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#ifndef TRX_SESSION_BASE_ABSTRACTUNDOLOGPART_H_
#define TRX_SESSION_BASE_ABSTRACTUNDOLOGPART_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class TableStore;
class CdbTransactionManager;

class AbstractUndoLogPart : public IBlockObject {
public:
	static const constexpr uint8_t PART_UNDO_DELETE_RECORD{1};
	static const constexpr uint8_t PART_UNDO_INSERT_RECORD{2};
	static const constexpr uint8_t PART_UNDO_UPDATE_RECORD{3};
	static const constexpr uint8_t PART_UNDO_UPDATE_RECORD_COLUMNS{4};
	static const constexpr uint8_t PART_UNDO_DELETED_RECORD_COLUMN{5};

	static const constexpr uint8_t PART_UNDO_INDEX_UNIQUE{10};
	static const constexpr uint8_t PART_UNDO_ADD_INDEX{11};
	static const constexpr uint8_t PART_UNDO_REMOVE_INDEX{12};
	static const constexpr uint8_t PART_UNDO_INDEX_PRIMARY{13};

	static const constexpr uint8_t PART_UNDO_CREATE_SCHEMA{20};


	explicit AbstractUndoLogPart(uint8_t type);
	virtual ~AbstractUndoLogPart();

	static AbstractUndoLogPart* createFromBinary(ByteBuffer* in);

	virtual void fromBinary(ByteBuffer* in) = 0;

	virtual void execute(CdbTransactionManager *trxManager, TableStore* store) const = 0;

	uint8_t getType() const noexcept {
		return this->type;
	}

protected:
	uint8_t type;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_BASE_ABSTRACTUNDOLOGPART_H_ */
