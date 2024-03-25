/*
 * UpdatedColumn.h
 *
 *  Created on: 2023/07/25
 *      Author: iizuka
 */

#ifndef TRX_SESSION_RECORD_UPDATEDCOLUMN_H_
#define TRX_SESSION_RECORD_UPDATEDCOLUMN_H_

#include <cstdlib>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class AbstractCdbValue;
class CdbRecord;

class UpdatedColumn {
public:
	UpdatedColumn(int16_t pos, const AbstractCdbValue *lastValue);
	virtual ~UpdatedColumn();

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	static UpdatedColumn* fromBinary(ByteBuffer* in);

	void update(CdbRecord* record);

private:
	int16_t pos;
	AbstractCdbValue *lastValue;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_RECORD_UPDATEDCOLUMN_H_ */
