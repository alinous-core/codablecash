/*
 * CdbBooleanKey.h
 *
 *  Created on: 2021/12/06
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_KEY_CDBBOOLEANKEY_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_KEY_CDBBOOLEANKEY_H_

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

namespace codablecash {

class CdbBooleanKey: public AbstractCdbKey {
public:
	CdbBooleanKey(const CdbBooleanKey& inst);
	explicit CdbBooleanKey(bool value);
	CdbBooleanKey();
	virtual ~CdbBooleanKey();

	virtual AbstractBtreeKey* clone()  const noexcept;
	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;

	virtual int64_t toInt64() const noexcept {
		return this->value ? 1 : 0;
	}
	virtual AbstractCdbValue* toCdbValue() const;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

private:
	bool value;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_KEY_CDBBOOLEANKEY_H_ */
