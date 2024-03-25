/*
 * ScanRecordIndentity.h
 *
 *  Created on: 2022/01/24
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_SCANRECORDINDENTITY_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_SCANRECORDINDENTITY_H_

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "base/ArrayList.h"

namespace codablecash {

class CdbOid;

class ScanRecordIndentity : public AbstractCdbValue {
public:
	ScanRecordIndentity(const ScanRecordIndentity& inst);
	ScanRecordIndentity();
	virtual ~ScanRecordIndentity();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual AbstractCdbValue* copy() const noexcept;
	virtual AbstractCdbKey* toKey() const noexcept;

	void addOid(const CdbOid* oid) noexcept;
	void addOids(const ScanRecordIndentity* inst) noexcept;

	int compareTo(const ScanRecordIndentity* right) const noexcept;

	virtual bool strictEqualsValue(const AbstractCdbValue* v) const;

private:
	ArrayList<CdbOid> list;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_SCANRECORDINDENTITY_H_ */
