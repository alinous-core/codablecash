/*
 * Zip.h
 *
 *  Created on: 2023/02/26
 *      Author: iizuka
 */

#ifndef ZIP_ZIPCOMPRESS_H_
#define ZIP_ZIPCOMPRESS_H_

#include <cstdint>

namespace alinous {

class ByteBuffer;

class ZipCompress {
public:
	static ByteBuffer* compress(const char *inbuf, int length, const char* entryName);
	static ByteBuffer* extract(const char *zipstream, int length, const char* entryName);
};

} /* namespace codablecash */

#endif /* ZIP_ZIPCOMPRESS_H_ */
