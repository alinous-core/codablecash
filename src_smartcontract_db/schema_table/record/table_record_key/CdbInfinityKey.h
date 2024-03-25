/*
 * CdbInfinityKey.h
 *
 *  Created on: 2022/01/01
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_KEY_CDBINFINITYKEY_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_KEY_CDBINFINITYKEY_H_

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

namespace codablecash {

class CdbInfinityKey : public AbstractCdbKey {
public:
	CdbInfinityKey(const CdbInfinityKey& inst);
	CdbInfinityKey();
	virtual ~CdbInfinityKey();

	virtual AbstractBtreeKey* clone()  const noexcept;
	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual bool isInfinity() const;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual int64_t toInt64() const noexcept {
		return 0;
	}
	virtual AbstractCdbValue* toCdbValue() const;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_KEY_CDBINFINITYKEY_H_ */
