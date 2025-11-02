/*
 * JsonTokenizer.h
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#ifndef JSON_JSONTOKENIZER_H_
#define JSON_JSONTOKENIZER_H_

#include "base/ArrayList.h"
#include "base/RawArrayPrimitive.h"


namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class IJsonStream;
class AbstractJsonToken;
class SkippedToken;
class JsonToken;
class JsonStringToken;

class JsonTokenizer {
public:
	static const constexpr wchar_t SKIP_CHARS[] = {L' ', L'\t', L'\n', 0};
	static const constexpr wchar_t TOKEN_CHARS[] = {L'{', L'}', L'[', L']', L':', L',', 0};

	static const constexpr wchar_t ESCAPED_CHAR = L'\\';
	static const constexpr wchar_t NEWLINE_CHAR = L'\n';

	static const constexpr wchar_t STRING_CHAR = L'"';
	static const constexpr wchar_t BRACE_BEGIN_CHAR = L'{';
	static const constexpr wchar_t BRACE_END_CHAR = L'}';
	static const constexpr wchar_t BRACKET_BEGIN_CHAR = L'[';
	static const constexpr wchar_t BRACKET_END_CHAR = L']';
	static const constexpr wchar_t COLON_CHAR = L':';
	static const constexpr wchar_t COMMA_CHAR = L',';

	static const constexpr wchar_t* TRUE = L"true";
	static const constexpr wchar_t* FALSE = L"false";

	explicit JsonTokenizer(IJsonStream* stream);
	virtual ~JsonTokenizer();

	AbstractJsonToken* getToken();
	AbstractJsonToken* getToken(bool foresee);
	const AbstractJsonToken* foreseeToken();
	void beginForesee();
	void resetForesee();

	bool isEndOfFile() const noexcept;

	void checkNotEof();

	/*
	int getLine() const noexcept {
		return this->line;
	}
	int getColumn() const noexcept {
		return this->pos;
	}*/

private:
	void handleSkippedToken(SkippedToken* token);
	void handleStringToken(JsonStringToken* token);
	void handleToken(JsonToken* token);

	bool isSkippedToken(wchar_t ch) noexcept;
	bool isTokenChar(wchar_t ch) noexcept;

	wchar_t getChar();

	bool isBoolean(const UnicodeString* bodyStr);

private:
	IJsonStream* stream;

	int line;
	int pos;

	ArrayList<AbstractJsonToken>* foreseeList;
	int foreseeCount;

	RawArrayPrimitive<int>* stack;

};

} /* namespace codablecash */

#endif /* JSON_JSONTOKENIZER_H_ */
