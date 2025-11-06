/*
 * ListClassDeclare.h
 *
 *  Created on: Nov 6, 2025
 *      Author: iizuka
 */

#ifndef INSTANCE_RESERVED_CLASSES_LIST_LISTCLASSDECLARE_H_
#define INSTANCE_RESERVED_CLASSES_LIST_LISTCLASSDECLARE_H_

#include "instance/reserved_generics/AbstractReservedGenericsClassDeclare.h"

namespace alinous {

class ListClassDeclare : public AbstractReservedGenericsClassDeclare {
public:
	ListClassDeclare();
	virtual ~ListClassDeclare();
};

} /* namespace alinous */

#endif /* INSTANCE_RESERVED_CLASSES_LIST_LISTCLASSDECLARE_H_ */
