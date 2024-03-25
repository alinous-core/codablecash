/*
 * UndoIndexPart.cpp
 *
 *  Created on: 2023/07/16
 *      Author: iizuka
 */

#include "trx/session/ddl/UndoIndexPart.h"

#include "schema_table/table/CdbTableColumn.h"
#include "schema_table/table/TableObjectFactory.h"

#include "engine/CdbOid.h"

#include "base/UnicodeString.h"

#include "bc_base/BinaryUtils.h"


namespace codablecash {

UndoIndexPart::UndoIndexPart(const UndoIndexPart &inst) {
	this->oid = inst.oid != nullptr ? inst.oid->copy() : nullptr;
	this->name = inst.name != nullptr ? new UnicodeString(inst.name) : nullptr;
	this->primary = inst.primary;
	this->unique = inst.unique;

	this->columns = new ArrayList<CdbOid>();
	int maxLoop = inst.columns->size();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = inst.columns->get(i);

		this->columns->addElement(col->copy());
	}
}

UndoIndexPart::UndoIndexPart() {
	this->oid = nullptr;
	this->name = nullptr;
	this->primary = false;
	this->unique = false;

	this->columns = new ArrayList<CdbOid>();
}

UndoIndexPart::~UndoIndexPart() {
	delete this->oid;
	delete this->name;

	this->columns->deleteElements();
	delete this->columns;
}

int UndoIndexPart::binarySize() const {
	BinaryUtils::checkNotNull(this->oid);
	BinaryUtils::checkNotNull(this->name);

	int total = this->oid->binarySize();
	total += BinaryUtils::stringSize(this->name);

	total += sizeof(uint8_t);
	total += sizeof(uint8_t);

	int maxLoop = this->columns->size();
	total += sizeof(uint16_t);

	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = this->columns->get(i);

		total += col->binarySize();
	}

	return total;
}

void UndoIndexPart::toBinary(ByteBuffer *out) const {
	BinaryUtils::checkNotNull(this->oid);
	BinaryUtils::checkNotNull(this->name);

	this->oid->toBinary(out);
	BinaryUtils::putString(out, this->name);

	out->put(this->primary ? 1 : 0);
	out->put(this->unique ? 1 : 0);

	int maxLoop = this->columns->size();
	out->putShort(maxLoop);

	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = this->columns->get(i);

		col->toBinary(out);
	}
}

UndoIndexPart* UndoIndexPart::fromBinary(ByteBuffer *in) {
	UndoIndexPart* part = new UndoIndexPart();

	part->oid = CdbOid::fromBinary(in);
	BinaryUtils::checkNotNull(part->oid);

	part->name = BinaryUtils::getString(in);
	BinaryUtils::checkNotNull(part->name);

	uint8_t bl = in->get();
	part->primary = bl > 0;

	bl = in->get();
	part->unique = bl > 0;

	int maxLoop = in->getShort();
	for(int i = 0; i != maxLoop; ++i){
		CdbOid* col = CdbOid::fromBinary(in);
		BinaryUtils::checkNotNull(col);

		part->columns->addElement(col);
	}

	return part;
}

void UndoIndexPart::setOid(const CdbOid *oid) noexcept {
	delete this->oid;
	this->oid = oid->copy();
}

void UndoIndexPart::setName(const UnicodeString *name) noexcept {
	delete this->name;
	this->name = new UnicodeString(name);
}

void UndoIndexPart::setPrimary(bool bl) noexcept {
	this->primary = bl;
}

void UndoIndexPart::setUnique(bool bl) noexcept {
	this->unique = bl;
}

void UndoIndexPart::addColumn(const CdbOid *colOid) noexcept {
	this->columns->addElement(colOid->copy());
}

} /* namespace codablecash */
