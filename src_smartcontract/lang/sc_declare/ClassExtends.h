/*
 * ClassExtends.h
 *
 *  Created on: 2019/06/19
 *      Author: iizuka
 */

#ifndef SC_DECLARE_CLASSEXTENDS_H_
#define SC_DECLARE_CLASSEXTENDS_H_

#include "engine/sc/CodeElement.h"

#include "base/HashMap.h"

namespace alinous {

class ClassName;
class AnalyzeContext;
class AnalyzedType;
class UnicodeString;
class AbstractType;

class ClassExtends : public CodeElement {
public:
	ClassExtends();
	virtual ~ClassExtends();

	void preAnalyze(AnalyzeContext* actx);
	void analyzeTypeRef(AnalyzeContext* actx);

	void setClassName(ClassName* className) noexcept;
	void setClassName(const UnicodeString* className) noexcept;

	AnalyzedType* getAnalyzedType() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	ClassExtends* generateGenericsImplement(HashMap<UnicodeString, AbstractType>* input) const;

private:
	ClassName* className;
	AnalyzedType* atype;

};

} /* namespace alinous */

#endif /* SC_DECLARE_CLASSEXTENDS_H_ */
