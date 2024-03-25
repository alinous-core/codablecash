/*
 * AbstractTransactionLog.h
 *
 *  Created on: 2020/05/11
 *      Author: iizuka
 */

#ifndef TRANSACTION_LOG_ABSTRACTTRANSACTIONLOG_H_
#define TRANSACTION_LOG_ABSTRACTTRANSACTIONLOG_H_
#include <cstdint>

#include "engine/CdbBinaryObject.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class CdbTransactionManager;
class CdbTransaction;

class AbstractTransactionLog : public CdbBinaryObject {
public:
	static const constexpr uint8_t TRX_CREATE_TABLE{1};
	static const constexpr uint8_t TRX_DROP_TABLE{2};

	static const constexpr uint8_t TRX_ALTER_ADD_INDEX{31};
	static const constexpr uint8_t TRX_ALTER_ADD_COLUMN{32};
	static const constexpr uint8_t TRX_ALTER_DROP_INDEX{33};
	static const constexpr uint8_t TRX_ALTER_DROP_COLUMN{34};
	static const constexpr uint8_t TRX_ALTER_ADD_PRIMARY_KEY{35};
	static const constexpr uint8_t TRX_ALTER_DROP_PRIMARY_KEY{36};
	static const constexpr uint8_t TRX_ALTER_MODIFY{37};
	static const constexpr uint8_t TRX_ALTER_RENAME_COLUMN{38};
	static const constexpr uint8_t TRX_ALTER_RENAME_TABLE{39};

	static const constexpr uint8_t TRX_INSERT{60};
	static const constexpr uint8_t TRX_UPDATE{61};
	static const constexpr uint8_t TRX_DELETE{62};

	AbstractTransactionLog(uint8_t type, uint64_t oid);
	virtual ~AbstractTransactionLog();

	virtual int binarySize() const = 0;
	virtual void toBinary(ByteBuffer* out) const = 0;
	virtual void fromBinary(ByteBuffer* in) = 0;

	virtual void commit(CdbTransactionManager* trxManager, CdbTransaction* trx) = 0;

	uint8_t getType() const noexcept {
		return type;
	}

	AbstractTransactionLog* copy() const;

protected:
	uint8_t type;
	uint64_t oid;

};

} /* namespace codablecash */

#endif /* TRANSACTION_LOG_ABSTRACTTRANSACTIONLOG_H_ */
