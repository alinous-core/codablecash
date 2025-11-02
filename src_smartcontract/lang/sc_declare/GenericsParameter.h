/*
 * GenericsParameter.h
 *
 *  Created on: Oct 18, 2025
 *      Author: iizuka
 */

#ifndef LANG_SC_DECLARE_GENERICSPARAMETER_H_
#define LANG_SC_DECLARE_GENERICSPARAMETER_H_

#include "engine/sc/CodeElement.h"

namespace alinous {

class UnicodeString;
class ClassName;
class AnalyzeContext;
class AnalyzedType;

class GenericsParameter : public CodeElement {
public:
	GenericsParameter();
	virtual ~GenericsParameter();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	void setGenericsName(UnicodeString* name) noexcept;
	void setGenericsExtendsName(ClassName* name) noexcept;

	const UnicodeString* getGenericsName() const noexcept {
		return this->genericsName;
	}

	void preAnalyze(AnalyzeContext* actx);
	void analyzeTypeRef(AnalyzeContext* actx);
	void analyze(AnalyzeContext* actx);

private:
	UnicodeString* genericsName;
	ClassName* genericsExtendsName;

	AnalyzedType* aExtendType;

};

} /* namespace alinous */

#endif /* LANG_SC_DECLARE_GENERICSPARAMETER_H_ */
