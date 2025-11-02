/*
 * AbstractReservedClassDeclare.h
 *
 *  Created on: 2020/03/17
 *      Author: iizuka
 */

#ifndef RESERVED_CLASSES_ABSTRACTRESERVEDCLASSDECLARE_H_
#define RESERVED_CLASSES_ABSTRACTRESERVEDCLASSDECLARE_H_

#include "lang/sc_declare/ClassDeclare.h"


namespace alinous {

class AbstractReservedClassDeclare : public ClassDeclare {
public:
	static constexpr const uint16_t TYPE_STRING_CLASS = 1;
	static constexpr const uint16_t TYPE_ARRAY_OUT_OF_BOUNDS_EXCEPTION = 2;
	static constexpr const uint16_t TYPE_DATABASE_EXCEPTION = 3;
	static constexpr const uint16_t TYPE_EXCEPTION = 4;
	static constexpr const uint16_t TYPE_NULL_POINTER_EXCEPTION = 5;
	static constexpr const uint16_t TYPE_TYPE_CAST_EXCEPTION = 6;
	static constexpr const uint16_t TYPE_ZERO_DIVISION_EXCEPTION = 7;


	AbstractReservedClassDeclare();
	virtual ~AbstractReservedClassDeclare();

	static AbstractReservedClassDeclare* createFromBinary(ByteBuffer* in);

	virtual void preAnalyze(AnalyzeContext* actx);
	virtual void analyzeTypeRef(AnalyzeContext* actx);
	virtual void analyze(AnalyzeContext* actx);

	virtual ArrayList<MethodDeclare>* getMethods() noexcept;
	virtual ArrayList<MemberVariableDeclare>* getMemberVariables() noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out);
	virtual void fromBinary(ByteBuffer* in);

	virtual uint16_t getClassType() const noexcept = 0;

protected:
	void addDefaultConstructor(const UnicodeString* className) noexcept;

protected:
	ArrayList<MethodDeclare>* methods;
	ArrayList<MemberVariableDeclare>* members;
};

} /* namespace alinous */

#endif /* RESERVED_CLASSES_ABSTRACTRESERVEDCLASSDECLARE_H_ */
