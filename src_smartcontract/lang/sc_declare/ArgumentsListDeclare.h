/*
 * ArgumentsListDeclare.h
 *
 *  Created on: 2019/01/19
 *      Author: iizuka
 */

#ifndef SC_DECLARE_ARGUMENTSLISTDECLARE_H_
#define SC_DECLARE_ARGUMENTSLISTDECLARE_H_

#include "engine/sc/CodeElement.h"

#include "base/ArrayList.h"
#include "base/HashMap.h"

namespace alinous {

class ArgumentDeclare;
class AnalyzeContext;
class UnicodeString;
class AnalyzedType;
class AbstractType;

class ArgumentsListDeclare : public CodeElement {
public:
	ArgumentsListDeclare();
	virtual ~ArgumentsListDeclare();

	void preAnalyze(AnalyzeContext* actx);
	void analyzeTypeRef(AnalyzeContext* actx);
	void analyze(AnalyzeContext* actx);

	void addArgument(ArgumentDeclare* arg) noexcept;
	int getSize() const noexcept;
	const ArrayList<ArgumentDeclare>* getArguments() const noexcept;
	ArrayList<AnalyzedType>* getArgumentsAnalyzedType() noexcept;
	const UnicodeString* getCallSignature() noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	const UnicodeString* toString() noexcept;

	ArgumentsListDeclare* generateGenericsImplement(HashMap<UnicodeString, AbstractType> *input) const;

private:
	void makeString() noexcept;
private:
	ArrayList<ArgumentDeclare> list;

	UnicodeString* callSig;
	ArrayList<AnalyzedType>* typelist;

	UnicodeString* str;
};

} /* namespace alinous */

#endif /* SC_DECLARE_ARGUMENTSLISTDECLARE_H_ */
