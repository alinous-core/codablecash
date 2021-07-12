/*
 * CdbNullKey.h
 *
 *  Created on: 2021/04/27
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_KEY_CDBNULLKEY_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_KEY_CDBNULLKEY_H_

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

namespace codablecash {

class CdbNullKey : public AbstractCdbKey {
public:
	CdbNullKey(const CdbNullKey& inst);
	CdbNullKey();
	virtual ~CdbNullKey();

	virtual bool isNull() const;

	virtual AbstractBtreeKey* clone()  const noexcept;
	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual int64_t toInt64() const noexcept {
		return 0;
	}
	virtual AbstractCdbValue* toCdbValue() const;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_KEY_CDBNULLKEY_H_ */
