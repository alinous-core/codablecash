/*
 * ArgumentDeclare.h
 *
 *  Created on: 2019/01/16
 *      Author: iizuka
 */

#ifndef SC_DECLARE_ARGUMENTDECLARE_H_
#define SC_DECLARE_ARGUMENTDECLARE_H_

#include "engine/sc/CodeElement.h"

#include "base/HashMap.h"

namespace alinous {

class AbstractType;
class UnicodeString;
class AnalyzeContext;
class AnalyzedType;
class AbstractType;

class ArgumentDeclare : public CodeElement {
public:
	ArgumentDeclare();
	virtual ~ArgumentDeclare();

	void preAnalyze(AnalyzeContext* actx);
	void analyzeTypeRef(AnalyzeContext* actx);
	void analyze(AnalyzeContext* actx);

	void setType(AbstractType* type) noexcept;
	void setName(UnicodeString* name) noexcept;

	AnalyzedType* getAnalyzedType() const noexcept;
	const UnicodeString* getName() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	ArgumentDeclare* generateGenericsImplement(HashMap<UnicodeString, AbstractType> *input) const;

private:
	AbstractType* type;
	UnicodeString* name;

	AnalyzedType* atype;
};

} /* namespace alinous */

#endif /* SC_DECLARE_ARGUMENTDECLARE_H_ */
