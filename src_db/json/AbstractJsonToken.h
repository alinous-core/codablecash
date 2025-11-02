/*
 * AbstractJsonToken.h
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#ifndef JSON_ABSTRACTJSONTOKEN_H_
#define JSON_ABSTRACTJSONTOKEN_H_

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

enum TokenType {
	NONE,
	STRING,
	BRACE_BEGIN,
	BRACE_END,
	BRACKET_BEGIN,
	BRACKET_END,
	COLON,
	COMMA,
	BOOLEAN,
	NUMERIC
};


class AbstractJsonToken {
public:
	AbstractJsonToken(int line, int pos);
	virtual ~AbstractJsonToken();

	void addChar(wchar_t ch) noexcept;

	TokenType getType() const noexcept {
		return this->type;
	}
	void setType(TokenType type);

	const UnicodeString* getBodyString() const noexcept {
		return this->body;
	}

	int getLine() const noexcept {
		return this->line;
	}

protected:
	int line;
	int pos;
	UnicodeString* body;

	TokenType type;
};

} /* namespace codablecash */

#endif /* JSON_ABSTRACTJSONTOKEN_H_ */
