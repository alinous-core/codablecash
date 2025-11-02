/*
 * ExceptionClassDeclare.h
 *
 *  Created on: 2020/04/12
 *      Author: iizuka
 */

#ifndef INSTANCE_EXCEPTION_CLASS_EXCEPTIONCLASSDECLARE_H_
#define INSTANCE_EXCEPTION_CLASS_EXCEPTIONCLASSDECLARE_H_

#include "instance/instance_exception_class/AbstractExceptionClassDeclare.h"

namespace alinous {

class AnalyzedClass;

class ExceptionClassDeclare : public AbstractExceptionClassDeclare {
public:
	static UnicodeString NAME;
	static AnalyzedClass* createAnalyzedClass() noexcept;

	ExceptionClassDeclare();
	virtual ~ExceptionClassDeclare();

	virtual uint16_t getClassType() const noexcept {
		return TYPE_EXCEPTION;
	}

	virtual const UnicodeString* getName() const noexcept;
	virtual const UnicodeString* getFullQualifiedName() noexcept;

	virtual ClassDeclare* getBaseClass() const noexcept;

	virtual IVmInstanceFactory* getFactory() const noexcept;
};

} /* namespace alinous */

#endif /* INSTANCE_EXCEPTION_CLASS_EXCEPTIONCLASSDECLARE_H_ */
