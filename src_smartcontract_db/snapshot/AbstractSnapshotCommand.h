/*
 * AbstractSnapshotCommand.h
 *
 *  Created on: 2023/07/01
 *      Author: iizuka
 */

#ifndef SNAPSHOT_ABSTRACTSNAPSHOTCOMMAND_H_
#define SNAPSHOT_ABSTRACTSNAPSHOTCOMMAND_H_

#include <cstdint>

#include "engine/CdbBinaryObject.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;


namespace codablecash {

class CodableDatabase;
class ISnapshotRecoverer;
class CdbTransactionManager;

class AbstractSnapshotCommand : public CdbBinaryObject {
public:
	static const constexpr uint8_t IDS_BACKUP{1};
	static const constexpr uint8_t SCHEMA_START{2};
	static const constexpr uint8_t SCHEMA_END{3};
	static const constexpr uint8_t TABLE_START{4};
	static const constexpr uint8_t TABLE_END{5};

	static const constexpr uint8_t INDEX{6};
	static const constexpr uint8_t RECORD{7};


	explicit AbstractSnapshotCommand(uint8_t type);
	virtual ~AbstractSnapshotCommand();

	virtual void recover(ISnapshotRecoverer* recoverer, CodableDatabase* db) = 0;

	virtual bool pushStack(){return false;}
	virtual bool popStack(){return false;}


	static AbstractSnapshotCommand* createFromBinary(ByteBuffer* in);

	virtual int binarySize() const = 0;
	virtual void toBinary(ByteBuffer* out) const = 0;
	virtual void fromBinary(ByteBuffer* in) = 0;

	uint8_t getType() const noexcept {
		return this->type;
	}

	virtual void __testCheckEquals(const AbstractSnapshotCommand* other) const = 0;

protected:
	uint8_t type;
};

} /* namespace codablecash */

#endif /* SNAPSHOT_ABSTRACTSNAPSHOTCOMMAND_H_ */
