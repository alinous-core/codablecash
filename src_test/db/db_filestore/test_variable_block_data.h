/*
 * test_variable_block_data.h
 *
 *  Created on: 2023/05/13
 *      Author: iizuka
 */

#ifndef DB_DB_FILESTORE_TEST_VARIABLE_BLOCK_DATA_H_
#define DB_DB_FILESTORE_TEST_VARIABLE_BLOCK_DATA_H_

static const char* makeTestData(int start, int length) {
	char* ptr = new char[length];

	for(int i = 0; i != length; ++i){
		char ch = start % 128;
		start++;

		ptr[i] = ch;
	}
	return ptr;
}

static bool checkTestData(int start, const char* data, int length) {
	for(int i = 0; i != length; ++i){
		char ch = start % 128;
		start++;

		if(data[i] != ch){
			return false;
		}
	}
	return true;
}




#endif /* DB_DB_FILESTORE_TEST_VARIABLE_BLOCK_DATA_H_ */
