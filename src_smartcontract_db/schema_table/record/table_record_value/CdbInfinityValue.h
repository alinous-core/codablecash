/*
 * CdbInfinityValue.h
 *
 *  Created on: 2022/01/01
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBINFINITYVALUE_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBINFINITYVALUE_H_

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

namespace codablecash {

class CdbInfinityValue : public AbstractCdbValue {
public:
	CdbInfinityValue(const CdbInfinityValue& inst);
	CdbInfinityValue();
	virtual ~CdbInfinityValue();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual AbstractCdbKey* toKey() const noexcept;
	virtual AbstractCdbValue* copy() const noexcept;

	int64_t getValue() const noexcept {
		return 0;
	}

	virtual bool strictEqualsValue(const AbstractCdbValue* v) const {
		return true;
	}
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBINFINITYVALUE_H_ */
