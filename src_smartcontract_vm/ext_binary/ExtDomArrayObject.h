/*
 * ExtDomArrayObject.h
 *
 *  Created on: 2020/07/02
 *      Author: iizuka
 */

#ifndef EXT_BINARY_EXTDOMARRAYOBJECT_H_
#define EXT_BINARY_EXTDOMARRAYOBJECT_H_

#include "ext_binary/AbstractExtObject.h"

#include "base/ArrayList.h"

namespace alinous {

class ExtDomArrayObject : public AbstractExtObject {
public:
	explicit ExtDomArrayObject(const UnicodeString* name);
	virtual ~ExtDomArrayObject();

	void add(AbstractExtObject* obj) noexcept;
	AbstractExtObject* get(int pos) const noexcept;
	int size() const noexcept;

	virtual AbstractExtObject* copy() const noexcept;

	virtual const UnicodeString* toString() const noexcept;

private:
	ArrayList<AbstractExtObject>* list;
};

} /* namespace alinous */

#endif /* EXT_BINARY_EXTDOMARRAYOBJECT_H_ */
