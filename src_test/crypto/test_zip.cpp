/*
 * test_zip.cpp
 *
 *  Created on: 2023/02/26
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "zip/ZipCompress.h"

#include "base_io/ByteBuffer.h"
using namespace alinous;

TEST_GROUP(TestZipGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};


TEST(TestZipGroup, case01){
	const char* inbuf = "Append some data here...";
	int size = strlen(inbuf);

	ByteBuffer* compessed = ZipCompress::compress(inbuf, size, "a"); __STP(compessed);
	CHECK(compessed != nullptr);

	ByteBuffer* buff = ZipCompress::extract((const char*)compessed->array(), compessed->limit(), "a"); __STP(buff);
	CHECK(buff != nullptr);
	CHECK(buff->limit() == size);

	CHECK(strncmp(inbuf, (const char*)buff->array(), size) == 0);
}




