/*
 * CdbLongValue.h
 *
 *  Created on: 2020/05/20
 *      Author: iizuka
 */

#ifndef TABLE_RECORD_VALUE_CDBLONGVALUE_H_
#define TABLE_RECORD_VALUE_CDBLONGVALUE_H_

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include <cstdlib>

namespace codablecash {

class CdbLongValue : public AbstractCdbValue {
public:
	CdbLongValue(const CdbLongValue& inst);
	explicit CdbLongValue(int64_t value);
	CdbLongValue();
	virtual ~CdbLongValue();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual AbstractCdbKey* toKey() const noexcept;
	virtual AbstractCdbValue* copy() const noexcept;

	int64_t getValue() const noexcept {
		return this->value;
	}

	virtual bool strictEqualsValue(const AbstractCdbValue* v) const;

private:
	int64_t value;
};

} /* namespace codablecash */

#endif /* TABLE_RECORD_VALUE_CDBLONGVALUE_H_ */
