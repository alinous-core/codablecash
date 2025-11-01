/*
 * GeneratedGenericsTypeNaming.h
 *
 *  Created on: Oct 27, 2025
 *      Author: iizuka
 */

#ifndef ENGINE_SC_ANALYZE_GENERATEDGENERICSTYPENAMING_H_
#define ENGINE_SC_ANALYZE_GENERATEDGENERICSTYPENAMING_H_

#include "base/HashMap.h"

namespace alinous {

class UnicodeString;
class GenericsObjectType;
class AbstractType;
class GenericsClassDeclare;

class GeneratedGenericsTypeNaming {
public:
	static constexpr const wchar_t* PREFIX = L"__Generics_";
	static constexpr const wchar_t* SPLITTER = L"_";

	static UnicodeString* fromGenericsObjectType(const GenericsObjectType* objectRefType);
	static UnicodeString* fromGenericsClassDeclare(const GenericsClassDeclare* gclazz, HashMap<UnicodeString, AbstractType>* input);

private:
	AbstractType* findType(HashMap<UnicodeString, AbstractType>* input, const UnicodeString* paramName);
};

} /* namespace alinous */

#endif /* ENGINE_SC_ANALYZE_GENERATEDGENERICSTYPENAMING_H_ */
