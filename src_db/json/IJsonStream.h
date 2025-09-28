/*
 * IJsonStream.h
 *
 *  Created on: Sep 25, 2025
 *      Author: iizuka
 */

#ifndef JSON_IJSONSTREAM_H_
#define JSON_IJSONSTREAM_H_

namespace codablecash {

class IJsonStream {
public:
	IJsonStream();
	virtual ~IJsonStream();

	virtual wchar_t getChar() = 0;
	virtual wchar_t lookNextChar() = 0;
	virtual bool isEndOfFile() const = 0;
};

} /* namespace codablecash */

#endif /* JSON_IJSONSTREAM_H_ */
