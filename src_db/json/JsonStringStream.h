/*
 * JsonStringStream.h
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#ifndef JSON_JSONSTRINGSTREAM_H_
#define JSON_JSONSTRINGSTREAM_H_

#include "json/IJsonStream.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class JsonStringStream : public IJsonStream {
public:
	explicit JsonStringStream(const UnicodeString* buffer);
	virtual ~JsonStringStream();

	virtual wchar_t getChar();
	virtual wchar_t lookNextChar();
	virtual bool isEndOfFile() const;

private:
	UnicodeString* buffer;
	int pos;
};

} /* namespace codablecash */

#endif /* JSON_JSONSTRINGSTREAM_H_ */
