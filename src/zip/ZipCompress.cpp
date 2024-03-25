/*
 * Zip.cpp
 *
 *  Created on: 2023/02/26
 *      Author: iizuka
 */

#include "zip/ZipCompress.h"

#include <cstdlib>
#include <cstdint>

#include "base_io/ByteBuffer.h"
#include "zip/zip.h"

namespace alinous {

ByteBuffer* ZipCompress::compress(const char *inbuf, int length, const char* entryName) {
	ByteBuffer* buff = nullptr;

	struct zip_t *zip = zip_stream_open(NULL, 0, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
	{
		zip_entry_open(zip, entryName);
		{
			zip_entry_write(zip, inbuf, length);
		}
		zip_entry_close(zip);

		{
			char *outbuf = nullptr;
			size_t outbufsize = 0;
			zip_stream_copy(zip, (void **)&outbuf, &outbufsize);

			buff = ByteBuffer::allocateWithEndian(outbufsize, true);
			buff->put((const uint8_t*)outbuf, outbufsize);

			::free(outbuf);
		}
	}
	zip_stream_close(zip);

	return buff;
}

ByteBuffer* ZipCompress::extract(const char* zipstream, int length, const char* entryName) {
	ByteBuffer* buff = nullptr;

	struct zip_t *zip = zip_stream_open(zipstream, length, 0, 'r');
	{
		zip_entry_open(zip, entryName);
		{
			char *buf = nullptr;
			size_t bufsize = 0;

			zip_entry_read(zip, (void **)&buf, &bufsize);

			buff = ByteBuffer::allocateWithEndian(bufsize, true);
			buff->put((const uint8_t*)buf, bufsize);

			::free(buf);
		}
		zip_entry_close(zip);
	}
	zip_stream_close(zip);

	return buff;
}

} /* namespace codablecash */
