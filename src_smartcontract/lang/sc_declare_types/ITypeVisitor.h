/*
 * ITypeVisitor.h
 *
 *  Created on: Oct 30, 2025
 *      Author: iizuka
 */

#ifndef LANG_SC_DECLARE_TYPES_ITYPEVISITOR_H_
#define LANG_SC_DECLARE_TYPES_ITYPEVISITOR_H_

#include "base/ArrayList.h"

namespace alinous {

class AbstractType;

class ITypeVisitor {
public:
	ITypeVisitor();
	virtual ~ITypeVisitor();

	virtual void visit(AbstractType* type) = 0;
	virtual void exit(AbstractType* type) = 0;
};

} /* namespace alinous */

#endif /* LANG_SC_DECLARE_TYPES_ITYPEVISITOR_H_ */
