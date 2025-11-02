/*
 * AbstractType.h
 *
 *  Created on: 2019/01/19
 *      Author: iizuka
 */

#ifndef SC_DECLARE_TYPES_ABSTRACTTYPE_H_
#define SC_DECLARE_TYPES_ABSTRACTTYPE_H_

#include "engine/sc/CodeElement.h"
#include <cstdint>

#include "base/HashMap.h"

namespace alinous {

class ByteBuffer;
class TypeResolver;
class AnalyzeContext;
class ITypeVisitor;

class AbstractType : public CodeElement {
public:
	explicit AbstractType(short kind);
	virtual ~AbstractType();

	virtual const UnicodeString* toString() noexcept = 0;

	void addDimension() noexcept;
	int getDimension() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual bool isGenericsType() const noexcept {
		return false;
	}

	virtual void preAnalyze(AnalyzeContext *actx);
	virtual void analyzeTypeRef(AnalyzeContext* actx);
	virtual void analyze(AnalyzeContext* actx);

	virtual AbstractType* generateGenericsImplement(HashMap<UnicodeString, AbstractType> *input) const = 0;

	virtual void visit(ITypeVisitor* visitor) = 0;

private:
	uint8_t dimension;
};

} /* namespace alinous */

#endif /* SC_DECLARE_TYPES_ABSTRACTTYPE_H_ */
