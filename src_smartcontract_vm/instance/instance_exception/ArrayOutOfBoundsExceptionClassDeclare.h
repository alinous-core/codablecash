/*
 * ArrayOutOfBoundsExceptionClassDeclare.h
 *
 *  Created on: 2020/04/17
 *      Author: iizuka
 */

#ifndef INSTANCE_EXCEPTION_ARRAYOUTOFBOUNDSEXCEPTIONCLASSDECLARE_H_
#define INSTANCE_EXCEPTION_ARRAYOUTOFBOUNDSEXCEPTIONCLASSDECLARE_H_

#include "instance/instance_exception_class/AbstractExceptionClassDeclare.h"

namespace alinous {

class ArrayOutOfBoundsExceptionClassDeclare : public AbstractExceptionClassDeclare {
public:
	static UnicodeString NAME;
	static AnalyzedClass* createAnalyzedClass() noexcept;
	static void throwException(VirtualMachine* vm, const CodeElement* element) noexcept;

	ArrayOutOfBoundsExceptionClassDeclare();
	virtual ~ArrayOutOfBoundsExceptionClassDeclare();

	virtual uint16_t getClassType() const noexcept {
		return TYPE_ARRAY_OUT_OF_BOUNDS_EXCEPTION;
	}

	virtual const UnicodeString* getName() const noexcept;
	virtual const UnicodeString* getFullQualifiedName() noexcept;

	virtual ClassDeclare* getBaseClass() const noexcept;

	virtual IVmInstanceFactory* getFactory() const noexcept;
};

} /* namespace alinous */

#endif /* INSTANCE_EXCEPTION_ARRAYOUTOFBOUNDSEXCEPTIONCLASSDECLARE_H_ */
