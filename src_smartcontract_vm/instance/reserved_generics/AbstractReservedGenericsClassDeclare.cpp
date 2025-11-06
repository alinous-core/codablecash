/*
 * AbstractReservedGenericsClassDeclare.cpp
 *
 *  Created on: Nov 6, 2025
 *      Author: iizuka
 */

#include "instance/reserved_generics/AbstractReservedGenericsClassDeclare.h"

#include "instance/reserved_classes/list/ListClassDeclare.h"

#include "filestore_block/exceptions.h"


namespace alinous {


AbstractReservedGenericsClassDeclare::AbstractReservedGenericsClassDeclare() : GenericsClassDeclare(RESERVED_GENERICS_CLASS_DECLARE) {

}

AbstractReservedGenericsClassDeclare::~AbstractReservedGenericsClassDeclare() {

}

AbstractReservedGenericsClassDeclare* AbstractReservedGenericsClassDeclare::createFromBinary(ByteBuffer *in) {
	AbstractReservedGenericsClassDeclare* ret = nullptr;
	uint16_t classType = in->getShort();

	switch(classType){
	case TYPE_GENERICS_LIST_CLASS:
		ret = new ListClassDeclare();
		break;
	default:
		throw new BinaryFormatException(__FILE__, __LINE__);
	}

	ret->fromBinary(in);

	return ret;
}

} /* namespace alinous */
