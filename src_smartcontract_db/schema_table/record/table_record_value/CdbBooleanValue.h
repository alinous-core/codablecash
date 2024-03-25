/*
 * CdbBooleanValue.h
 *
 *  Created on: 2021/12/06
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBBOOLEANVALUE_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBBOOLEANVALUE_H_

#include "schema_table/record/table_record_value/AbstractCdbValue.h"
#include <cstdlib>

namespace codablecash {

class CdbBooleanValue : public AbstractCdbValue {
public:
	CdbBooleanValue(const CdbBooleanValue& inst);
	explicit CdbBooleanValue(bool bl);
	CdbBooleanValue();
	virtual ~CdbBooleanValue();

	bool getValue() const noexcept {
		return this->value;
	}

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual AbstractCdbValue* copy() const noexcept;
	virtual AbstractCdbKey* toKey() const noexcept;

	virtual bool strictEqualsValue(const AbstractCdbValue* v) const;

private:
	bool value;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBBOOLEANVALUE_H_ */
