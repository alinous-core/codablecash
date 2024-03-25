/*
 * AbstractUndoLog.h
 *
 *  Created on: 2023/07/09
 *      Author: iizuka
 */

#ifndef TRX_SESSION_BASE_ABSTRACTUNDOLOG_H_
#define TRX_SESSION_BASE_ABSTRACTUNDOLOG_H_

#include "filestore_block/IBlockObject.h"

#include <cstdint>

using namespace alinous;

namespace codablecash {

class CdbTransactionManager;
class UndoLogPartsRepo;
class UndoLogsIdkey;

class AbstractUndoLog : public IBlockObject {
public:
	static const constexpr uint8_t UNDO_TYPE_UNDO_IDS{1};

	static const constexpr uint8_t UNDO_TYPE_CREATE_TABLE{2};
	static const constexpr uint8_t UNDO_TYPE_DROP_TABLE{3};

	static const constexpr uint8_t UNDO_TYPE_ALTER_ADD_INDEX{4};
	static const constexpr uint8_t UNDO_TYPE_ALTER_DROP_INDEX{5};

	static const constexpr uint8_t UNDO_TYPE_ALTER_MODIFY{6};

	static const constexpr uint8_t UNDO_TYPE_ALTER_ADD_COLUMN{7};
	static const constexpr uint8_t UNDO_TYPE_ALTER_DROP_COLUMN{8};

	static const constexpr uint8_t UNDO_TYPE_ALTER_ADD_PRIMARY_KEY{9};
	static const constexpr uint8_t UNDO_TYPE_ALTER_DROP_PRIMARY_KEY{10};

	static const constexpr uint8_t UNDO_TYPE_ALTER_RENAME_TABLE{11};
	static const constexpr uint8_t UNDO_TYPE_ALTER_RENAME_COLUMN{12};

	static const constexpr uint8_t UNDO_TYPE_INSERT{13};
	static const constexpr uint8_t UNDO_TYPE_UPDATE{14};
	static const constexpr uint8_t UNDO_TYPE_DELETE{15};

	explicit AbstractUndoLog(uint8_t type);
	virtual ~AbstractUndoLog();

	static AbstractUndoLog* createFromBinary(ByteBuffer* in);

	virtual void fromBinary(ByteBuffer* in) = 0;
	virtual void execute(CdbTransactionManager* trxManager, UndoLogPartsRepo* partsRepo, UndoLogsIdkey* key) = 0;

protected:
	uint8_t type;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_BASE_ABSTRACTUNDOLOG_H_ */
