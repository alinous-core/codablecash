/*
 * JsonStringStream.cpp
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#include "json/JsonStringStream.h"

#include "base/UnicodeString.h"


namespace codablecash {

JsonStringStream::JsonStringStream(const UnicodeString* buffer) {
	this->buffer = new UnicodeString(buffer);
	this->pos = 0;
}

JsonStringStream::~JsonStringStream() {
	delete this->buffer;
}

wchar_t JsonStringStream::getChar() {
	return this->buffer->charAt(this->pos++);
}

wchar_t JsonStringStream::lookNextChar() {
	return this->buffer->charAt(this->pos);
}

bool JsonStringStream::isEndOfFile() const {
	int length = this->buffer->length();
	bool bl = length <= this->pos;

	return bl;
}

} /* namespace codablecash */
