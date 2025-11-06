/*
 * AbstractReservedGenericsClassDeclare.h
 *
 *  Created on: Nov 6, 2025
 *      Author: iizuka
 */

#ifndef INSTANCE_RESERVED_GENERICS_ABSTRACTRESERVEDGENERICSCLASSDECLARE_H_
#define INSTANCE_RESERVED_GENERICS_ABSTRACTRESERVEDGENERICSCLASSDECLARE_H_

#include "lang/sc_declare/GenericsClassDeclare.h"

namespace alinous {

class AbstractReservedGenericsClassDeclare : public GenericsClassDeclare {
public:
	static constexpr const uint16_t TYPE_GENERICS_LIST_CLASS = 1;

	AbstractReservedGenericsClassDeclare();
	virtual ~AbstractReservedGenericsClassDeclare();

	static AbstractReservedGenericsClassDeclare* createFromBinary(ByteBuffer* in);


};

} /* namespace alinous */

#endif /* INSTANCE_RESERVED_GENERICS_ABSTRACTRESERVEDGENERICSCLASSDECLARE_H_ */
