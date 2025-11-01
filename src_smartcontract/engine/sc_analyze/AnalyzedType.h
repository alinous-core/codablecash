/*
 * AnalyzedType.h
 *
 *  Created on: 2019/06/11
 *      Author: iizuka
 */

#ifndef SC_ANALYZE_ANALYZEDTYPE_H_
#define SC_ANALYZE_ANALYZEDTYPE_H_
#include <cstdint>

namespace alinous {

class AnalyzedClass;
class UnicodeString;

class AnalyzedType {
public:
	static const constexpr uint8_t TYPE_NONE{100};
	static const constexpr uint8_t TYPE_NULL{0};

	static const constexpr uint8_t TYPE_BOOL{20};
	static const constexpr uint8_t TYPE_BYTE{21};
	static const constexpr uint8_t TYPE_CHAR{22};
	static const constexpr uint8_t TYPE_SHORT{23};
	static const constexpr uint8_t TYPE_INT{24};
	static const constexpr uint8_t TYPE_LONG{25}; // <- used in AnalyzedType::isPrimitiveInteger()
	static const constexpr uint8_t TYPE_STRING{26};
	static const constexpr uint8_t TYPE_VOID{27};
	static const constexpr uint8_t TYPE_OBJECT{28};
	static const constexpr uint8_t TYPE_DOM{29};
	static const constexpr uint8_t TYPE_DOM_ARRAY{30};
	static const constexpr uint8_t TYPE_DOM_VALUE_PAIR{31};
	static const constexpr uint8_t TYPE_DOM_VALUE{32};

	static const constexpr uint8_t TYPE_GENERICS_TYPE{101};


	static const UnicodeString SIG_BOOL;
	static const UnicodeString SIG_BYTE;
	static const UnicodeString SIG_CHAR;
	static const UnicodeString SIG_SHORT;
	static const UnicodeString SIG_INT;
	static const UnicodeString SIG_LONG;
	static const UnicodeString SIG_STRING;
	static const UnicodeString SIG_VOID;
	static const UnicodeString SIG_VAR;

	AnalyzedType(const AnalyzedType& obj); // copy constructor

	explicit AnalyzedType(uint8_t type);
	explicit AnalyzedType(AnalyzedClass* clazz);
	AnalyzedType();

	virtual ~AnalyzedType();

	void setAnalyzedClass(AnalyzedClass* aclass) noexcept;
	AnalyzedClass* getAnalyzedClass() const noexcept;

	void setGenericsType(const UnicodeString* genericsType);
	const UnicodeString* getGenericsType() const noexcept {
		return this->genericsType;
	}

	uint8_t getType() const noexcept;
	const UnicodeString* stringName() noexcept;
	const UnicodeString* getSignatureName() noexcept;

	bool isGenericsType() const noexcept;

	bool isVoid() const noexcept;
	bool isPrimitiveInteger() const noexcept;
	bool isNull() const noexcept;
	bool isBool() const noexcept;

	bool isInterface() const noexcept;

	void setDim(int dim) noexcept;
	int getDim() const noexcept;
	bool isArray() const noexcept;

	bool equals(AnalyzedType* other) const noexcept;
private:
	void makeStringName() noexcept;
	void makeObjectString() noexcept;

private:
	uint8_t type;
	AnalyzedClass* aclazz; // reference
	int dim;
	UnicodeString* genericsType;

	UnicodeString* str;
};

} /* namespace alinous */

#endif /* SC_ANALYZE_ANALYZEDTYPE_H_ */
