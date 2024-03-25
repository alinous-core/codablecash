/*
 * CdbValueList.h
 *
 *  Created on: 2022/01/15
 *      Author: iizuka
 */

#ifndef SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBVALUELIST_H_
#define SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBVALUELIST_H_

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "base/ArrayList.h"

namespace codablecash {

class CdbValueList : public AbstractCdbValue {
public:
	CdbValueList(const CdbValueList& inst);
	CdbValueList();
	virtual ~CdbValueList();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual AbstractCdbValue* copy() const noexcept;
	virtual AbstractCdbKey* toKey() const noexcept;

	void addValue(AbstractCdbValue* value) noexcept;
	int size() const noexcept;
	const AbstractCdbValue* get(int i) const noexcept;

	virtual bool strictEqualsValue(const AbstractCdbValue* v) const;

private:
	ArrayList<AbstractCdbValue> list;
};

} /* namespace codablecash */

#endif /* SCHEMA_TABLE_RECORD_TABLE_RECORD_VALUE_CDBVALUELIST_H_ */
