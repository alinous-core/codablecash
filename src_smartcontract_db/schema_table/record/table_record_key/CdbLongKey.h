/*
 * CdbLongKey.h
 *
 *  Created on: 2020/05/21
 *      Author: iizuka
 */

#ifndef TABLE_RECORD_KEY_CDBLONGKEY_H_
#define TABLE_RECORD_KEY_CDBLONGKEY_H_

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

namespace codablecash {

class CdbLongKey : public AbstractCdbKey {
public:
	CdbLongKey(const CdbLongKey& inst);
	CdbLongKey();
	explicit CdbLongKey(int64_t value);
	virtual ~CdbLongKey();

	virtual AbstractBtreeKey* clone()  const noexcept;
	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	int64_t getValue() const noexcept {
		return value;
	}

	virtual int64_t toInt64() const noexcept {
		return this->value;
	}
	virtual AbstractCdbValue* toCdbValue() const;

private:
	int64_t value;
};

} /* namespace codablecash */

#endif /* TABLE_RECORD_KEY_CDBLONGKEY_H_ */
