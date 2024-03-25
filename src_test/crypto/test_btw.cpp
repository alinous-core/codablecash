/*
 * test_btw.cpp
 *
 *  Created on: 2022/04/11
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"
#include "astrobwt/BwtTransform.h"
#include "base_io/ByteBuffer.h"

#include "base/UnicodeString.h"

using namespace alinous;
using namespace codablecash;


TEST_GROUP(TestBtwGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestBtwGroup, case01){
	const char* str = "12345";

	ByteBuffer* buff = ByteBuffer::allocateWithEndian(5, false);

	BwtTransform bwt(5);

	bwt.transform(buff, (const uint8_t*)str, 5); __STP(buff);

	const char* ar = (const char*)buff->array();
	int size = buff->limit();

	UnicodeString result(L"");
	for(int i = 0; i != size; ++i){
		wchar_t ch = ar[i];
		result.append(ch);
	}

	/**
	 * 12345
	 * 51234
	 * 45123
	 * 34512
	 * 23451
	 * ->
	 * 12345
	 * 23451
	 * 34512
	 * 45123
	 * 51234
	 */
	CHECK(result.equals(L"51234"));
}

TEST(TestBtwGroup, equals){
	uint8_t* newData = new uint8_t[2]{1, 2};
	uint8_t* newData2 = new uint8_t[2]{1, 2};

	BwtElement* el0 = new BwtElement(newData, 2, 0); __STP(el0);
	BwtElement* el1 = new BwtElement(newData2, 2, 0); __STP(el1);

	BwtElement::ValueCompare cmp;
	int res = cmp(el0, el1);
	CHECK(res == 0);

	delete [] newData;
	delete [] newData2;
}

TEST(TestBtwGroup, equals02){
	uint8_t* newData = new uint8_t[2]{1, 3};
	uint8_t* newData2 = new uint8_t[2]{1, 2};

	BwtElement* el0 = new BwtElement(newData, 2, 0); __STP(el0);
	BwtElement* el1 = new BwtElement(newData2, 2, 0); __STP(el1);

	BwtElement::ValueCompare cmp;
	int res = cmp(el0, el1);
	CHECK(res > 0);


	delete [] newData;
	delete [] newData2;
}

TEST(TestBtwGroup, equals03){
	uint8_t* newData = new uint8_t[3]{1, 2, 3};
	uint8_t* newData2 = new uint8_t[3]{1, 2, 3};

	BwtElement* el0 = new BwtElement(newData, 3, 0); __STP(el0);
	BwtElement* el1 = new BwtElement(newData2, 3, 0); __STP(el1);

	BwtElement::ValueCompare cmp;
	int res = cmp(el0, el1);
	CHECK(res == 0);

	delete [] newData;
	delete [] newData2;

}

