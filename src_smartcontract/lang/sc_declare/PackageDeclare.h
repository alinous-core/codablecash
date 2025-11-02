/*
 * PackageDeclare.h
 *
 *  Created on: 2019/01/16
 *      Author: iizuka
 */

#ifndef SC_DECLARE_PACKAGEDECLARE_H_
#define SC_DECLARE_PACKAGEDECLARE_H_

#include "engine/sc/CodeElement.h"

#include "base/HashMap.h"


namespace alinous {

class PackageNameDeclare;
class AbstractType;

class PackageDeclare : public CodeElement {
public:
	PackageDeclare();
	virtual ~PackageDeclare();

	void setName(PackageNameDeclare* name);
	const UnicodeString* getPackageName() noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	PackageDeclare* generateGenericsImplement(HashMap<UnicodeString, AbstractType>* input);

private:
	PackageNameDeclare* name;
};

} /* namespace alinous */

#endif /* SC_DECLARE_PACKAGEDECLARE_H_ */
