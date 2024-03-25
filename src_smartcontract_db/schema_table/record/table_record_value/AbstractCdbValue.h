/*
 * AbstractCdbValue.h
 *
 *  Created on: 2020/05/20
 *      Author: iizuka
 */

#ifndef TABLE_RECORD_VALUE_ABSTRACTCDBVALUE_H_
#define TABLE_RECORD_VALUE_ABSTRACTCDBVALUE_H_

#include <cstdint>

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class AbstractCdbKey;

class AbstractCdbValue : public IBlockObject {
public:
	static const constexpr uint8_t TYPE_BYTE{1};
	static const constexpr uint8_t TYPE_SHORT{2};
	static const constexpr uint8_t TYPE_INT{3};
	static const constexpr uint8_t TYPE_LONG{4};
	static const constexpr uint8_t TYPE_STRING{5};
	static const constexpr uint8_t TYPE_BOOLEAN{6};

	static const constexpr uint8_t TYPE_RECORD{17};
	static const constexpr uint8_t TYPE_OID_LIST{18};
	static const constexpr uint8_t TYPE_RECORD_IDENTITY{19};

	static const constexpr uint8_t TYPE_NULL{100};
	static const constexpr uint8_t TYPE_INFINITY{101};

	static const constexpr uint8_t TYPE_VALUE_LIST{200};

	explicit AbstractCdbValue(uint8_t type);
	virtual ~AbstractCdbValue();

	virtual void fromBinary(ByteBuffer* in) = 0;

	virtual AbstractCdbValue* copy() const noexcept = 0;
	virtual AbstractCdbKey* toKey() const noexcept = 0;

	virtual IBlockObject* copyData() const noexcept;

	virtual bool isNull() const noexcept {
		return false;
	}

	uint8_t getType() const noexcept {
		return type;
	}

	bool strictEquals(const AbstractCdbValue* v) const;

protected:
	virtual bool strictEqualsValue(const AbstractCdbValue* v) const = 0;

protected:
	uint8_t type;
};

} /* namespace codablecash */

#endif /* TABLE_RECORD_VALUE_ABSTRACTCDBVALUE_H_ */
