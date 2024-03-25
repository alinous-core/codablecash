/*
 * ScanRecordIndentityKey.h
 *
 *  Created on: 2022/01/26
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_SCANRECORDINDENTITYKEY_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_SCANRECORDINDENTITYKEY_H_

#include "schema_table/record/table_record_key/AbstractCdbKey.h"

namespace codablecash {

class ScanRecordIndentity;

class ScanRecordIndentityKey : public AbstractCdbKey {
public:
	ScanRecordIndentityKey(const ScanRecordIndentityKey& inst);
	ScanRecordIndentityKey();
	virtual ~ScanRecordIndentityKey();

	virtual AbstractBtreeKey* clone()  const noexcept;
	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);
	virtual int64_t toInt64() const noexcept;

	virtual AbstractCdbValue* toCdbValue() const;

	void setRecordScanId(ScanRecordIndentity* scanId) noexcept;
private:
	ScanRecordIndentity* scanId;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_SCANRECORDINDENTITYKEY_H_ */
