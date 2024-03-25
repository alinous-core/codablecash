/*
 * CdbOidValueList.h
 *
 *  Created on: 2020/06/01
 *      Author: iizuka
 */

#ifndef TABLE_RECORD_VALUE_CDBOIDVALUELIST_H_
#define TABLE_RECORD_VALUE_CDBOIDVALUELIST_H_

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "base/ArrayList.h"

#include "engine/CdbOid.h"
namespace codablecash {

class CdbOid;

class CdbOidValueList : public AbstractCdbValue {
public:
	CdbOidValueList(const CdbOidValueList& inst);
	CdbOidValueList();
	virtual ~CdbOidValueList();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	void addOid(CdbOid* oid) noexcept;
	int size() const noexcept;
	bool isEmpty() const noexcept {
		return this->size() == 0;
	}
	CdbOid* get(int i) const noexcept;

	void removeOid(const CdbOid* oid) noexcept;

	//bool hasOid(const CdbOid* oid) const noexcept;

	virtual AbstractCdbValue* copy() const noexcept;
	virtual AbstractCdbKey* toKey() const noexcept;

	virtual bool strictEqualsValue(const AbstractCdbValue* v) const;

private:
	ArrayList<CdbOid, CdbOid::ValueCompare> list;
};

} /* namespace codablecash */

#endif /* TABLE_RECORD_VALUE_CDBOIDVALUELIST_H_ */
