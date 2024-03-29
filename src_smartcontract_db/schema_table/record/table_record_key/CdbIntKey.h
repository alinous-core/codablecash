/*
 * CdbIntKey.h
 *
 *  Created on: 2020/05/21
 *      Author: iizuka
 */

#ifndef TABLE_RECORD_KEY_CDBINTKEY_H_
#define TABLE_RECORD_KEY_CDBINTKEY_H_

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

namespace codablecash {

class CdbIntKey : public AbstractCdbKey {
public:
	CdbIntKey(const CdbIntKey& inst);
	explicit CdbIntKey(int32_t value);
	CdbIntKey();
	virtual ~CdbIntKey();

	virtual AbstractBtreeKey* clone()  const noexcept;
	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual int64_t toInt64() const noexcept {
		return this->value;
	}
	virtual AbstractCdbValue* toCdbValue() const;

private:
	int32_t value;
};

} /* namespace codablecash */

#endif /* TABLE_RECORD_KEY_CDBINTKEY_H_ */
