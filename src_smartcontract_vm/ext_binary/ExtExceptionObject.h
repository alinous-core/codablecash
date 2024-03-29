/*
 * ExtExceptionObject.h
 *
 *  Created on: 2020/04/16
 *      Author: iizuka
 */

#ifndef EXT_BINARY_EXTEXCEPTIONOBJECT_H_
#define EXT_BINARY_EXTEXCEPTIONOBJECT_H_

#include "ext_binary/AbstractExtObject.h"

namespace alinous {

class CodeElement;

class ExtExceptionObject : public AbstractExtObject {
public:
	explicit ExtExceptionObject(const UnicodeString* name);
	virtual ~ExtExceptionObject();

	void setCodeElement(const CodeElement* element) noexcept;
	const CodeElement* getCodeElement() const noexcept;

	void setClassName(const UnicodeString* className) noexcept;
	const UnicodeString* getClassName() const noexcept;

	virtual AbstractExtObject* copy() const noexcept;

	virtual const UnicodeString* toString() const noexcept;

private:
	const CodeElement* element;
	UnicodeString* className;
};

} /* namespace alinous */

#endif /* EXT_BINARY_EXTEXCEPTIONOBJECT_H_ */
