/*
 * CdbNullValue.h
 *
 *  Created on: 2021/04/27
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBNULLVALUE_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBNULLVALUE_H_

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include <cstdlib>

namespace codablecash {

class CdbNullValue : public AbstractCdbValue {
public:
	CdbNullValue(const CdbNullValue& inst);
	CdbNullValue();
	virtual ~CdbNullValue();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual AbstractCdbKey* toKey() const noexcept;
	virtual AbstractCdbValue* copy() const noexcept;

	int64_t getValue() const noexcept {
		return 0;
	}

	virtual bool isNull() const noexcept {
		return true;
	}

	virtual bool strictEqualsValue(const AbstractCdbValue* v) const {
		return true;
	}
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBNULLVALUE_H_ */
