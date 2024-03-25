/*
 * CdbRecord.h
 *
 *  Created on: 2020/05/19
 *      Author: iizuka
 */

#ifndef TABLE_RECORD_CDBRECORD_H_
#define TABLE_RECORD_CDBRECORD_H_

#include "base/ArrayList.h"
#include <cstdint>

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

namespace alinous {
class ByteBuffer;
class VirtualMachine;
class DomVariableInstance;
class IAbstractVmInstanceSubstance;
}
using namespace alinous;

namespace codablecash {

class AbstractCdbValue;
class CdbOid;
class ScanRecordIndentity;

class CdbRecord : public AbstractCdbValue {
public:
	CdbRecord(const CdbRecord& inst);
	CdbRecord();
	virtual ~CdbRecord();

	void setOid(const uint64_t oid) noexcept;
	void setOid(const CdbOid* oid) noexcept;
	const CdbOid* getOid() const noexcept {
		return oid;
	}

	virtual bool isGrouped() const noexcept {
		return false;
	}

	void initNullColumns(int num) noexcept;
	void addValue(AbstractCdbValue* value) noexcept;
	void setValue(AbstractCdbValue* value, int i) noexcept;
	void removeColumnValue(int pos) noexcept;
	void insertValue(AbstractCdbValue* value, int pos) noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual AbstractCdbValue* copy() const noexcept;
	virtual AbstractCdbKey* toKey() const noexcept;

	void initScanIdentity() noexcept;
	void importScanIdentity() noexcept;
	CdbRecord* joinRecord(const CdbRecord* record) const noexcept;
	CdbRecord* joinEmptyRecord(int numNull) const noexcept;

	static CdbRecord* createFromBinary(ByteBuffer* in);

	const ArrayList<AbstractCdbValue>* getValues() const noexcept;

	const AbstractCdbValue* get(int pos) const noexcept;

	DomVariableInstance* toDomInstance(VirtualMachine* vm, ArrayList<UnicodeString>* nameList) const;

	void initBlankScanIdentity() noexcept;
	ScanRecordIndentity* getScanIdentity() const noexcept {
		return this->scanIdentity;
	}
	void clearScanIdentity() noexcept;

	virtual bool strictEqualsValue(const AbstractCdbValue* v) const;

protected:
	CdbOid* oid;
	ArrayList<AbstractCdbValue> list;

	ScanRecordIndentity* scanIdentity;
};

} /* namespace codablecash */

#endif /* TABLE_RECORD_CDBRECORD_H_ */
