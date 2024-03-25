/*
 * UpdatedColumn.cpp
 *
 *  Created on: 2023/07/25
 *      Author: iizuka
 */

#include "trx/session/record/UpdatedColumn.h"

#include "schema_table/record/table_record_value/AbstractCdbValue.h"

#include "schema_table/record/table_record/CdbDataFactory.h"

#include "base/StackRelease.h"

#include "filestore_block/IBlockObject.h"

#include "bc_base/BinaryUtils.h"

#include "schema_table/record/table_record/CdbRecord.h"
namespace codablecash {

UpdatedColumn::UpdatedColumn(int16_t pos, const AbstractCdbValue *lastValue) {
	this->pos = pos;
	this->lastValue = lastValue != nullptr ? lastValue->copy() : nullptr;
}

UpdatedColumn::~UpdatedColumn() {
	delete this->lastValue;
}

int UpdatedColumn::binarySize() const {
	int total = sizeof(this->pos);

	total += sizeof(uint8_t);
	if(this->lastValue != nullptr){
		total += this->lastValue->binarySize();
	}

	return total;
}

void UpdatedColumn::toBinary(ByteBuffer *out) const {
	out->putShort(this->pos);

	bool bl = this->lastValue != nullptr;
	out->put(bl ? 1 : 0);

	if(bl){
		this->lastValue->toBinary(out);
	}
}

UpdatedColumn* UpdatedColumn::fromBinary(ByteBuffer *in) {
	int16_t pos = in->getShort();

	AbstractCdbValue* lastValue = nullptr;
	uint8_t b = in->get();
	bool bl = b > 0;
	if(bl){
		static CdbDataFactory factory;
		IBlockObject* obj = factory.makeDataFromBinary(in); __STP(obj);
		lastValue = dynamic_cast<AbstractCdbValue*>(obj);

		BinaryUtils::checkNotNull(lastValue);
		__STP_MV(obj);
	}
	__STP(lastValue);

	return new UpdatedColumn(pos, lastValue);
}

void UpdatedColumn::update(CdbRecord *record) {
	AbstractCdbValue* value = this->lastValue != nullptr ? this->lastValue->copy() : nullptr;
	record->setValue(value, this->pos);
}

} /* namespace codablecash */
