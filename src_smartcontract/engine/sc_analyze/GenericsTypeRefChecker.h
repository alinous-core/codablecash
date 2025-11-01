/*
 * GenericsTypeRefChecker.h
 *
 *  Created on: Oct 30, 2025
 *      Author: iizuka
 */

#ifndef ENGINE_SC_ANALYZE_GENERICSTYPEREFCHECKER_H_
#define ENGINE_SC_ANALYZE_GENERICSTYPEREFCHECKER_H_

#include "lang/sc_declare_types/ITypeVisitor.h"

#include "base/ArrayList.h"

namespace alinous {

class TypeResolver;

class GenericsTypeRefChecker : public ITypeVisitor {
public:
	explicit GenericsTypeRefChecker(TypeResolver* resolver);
	virtual ~GenericsTypeRefChecker();

	virtual void visit(AbstractType* type);
	virtual void exit(AbstractType* type);

	bool isError() const noexcept {
		return this->hasError;
	}

	const UnicodeString* getErrorTypeName() const noexcept {
		return this->errorTypeName;
	}

private:
	bool hasClass(const UnicodeString* className) const noexcept;

private:
	TypeResolver* resolver;
	ArrayList<const UnicodeString>* stack;
	bool hasError;
	const UnicodeString* errorTypeName;
};

} /* namespace alinous */

#endif /* ENGINE_SC_ANALYZE_GENERICSTYPEREFCHECKER_H_ */
