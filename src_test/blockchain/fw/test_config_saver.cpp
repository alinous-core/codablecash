/*
 * test_config_saver.cpp
 *
 *  Created on: 2022/05/14
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_base_conf_store/StatusStore.h"
#include "bc_base_conf_store/LongValueConfigStoreValue.h"
#include "bc_base_conf_store/AbstractConfigStoreElement.h"
#include "bc_base_conf_store/ShortValueConfigStoreValue.h"

#include "base_io/ByteBuffer.h"


using namespace codablecash;

TEST_GROUP(TestConfigSaverGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestConfigSaverGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	UnicodeString fileName(L"conf.bin");
	{
		StatusStore store(baseDir, L"conf.bin");

		LongValueConfigStoreValue* value = new LongValueConfigStoreValue(10);
		UnicodeString key(L"height");
		store.addValue(&key, value);

		LongValueConfigStoreValue* value2 = new LongValueConfigStoreValue(11);
		store.addValue(&key, value2);
	}

	{
		StatusStore store(baseDir, &fileName);
		bool bl = store.exists();
		CHECK(bl == true);

		store.load();

		UnicodeString key(L"height");
		const AbstractConfigStoreElement* v = store.getValue(&key);

		const LongValueConfigStoreValue* value = dynamic_cast<const LongValueConfigStoreValue*>(v);
		uint64_t lv = value->getValue();

		CHECK(lv == 11);
	}
}

TEST(TestConfigSaverGroup, case02){
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(8, true); __STP(buff);
	buff->put(110);

	AbstractConfigStoreElement* ret = AbstractConfigStoreElement::createFromBinary(buff);
	CHECK(ret == nullptr);
}

TEST(TestConfigSaverGroup, case03){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	UnicodeString fileName(L"conf.bin");
	{
		StatusStore store(baseDir, &fileName);
		store.addLongValue(L"key1", 100);

		const AbstractConfigStoreElement* v = store.getValue(L"key1");
		const LongValueConfigStoreValue* value = dynamic_cast<const LongValueConfigStoreValue*>(v);
		uint64_t lv = value->getValue();

		CHECK(lv == 100);
	}
}

TEST(TestConfigSaverGroup, case04){
	File projectFolder = this->env->testCaseDir();
	_ST(File, baseDir, projectFolder.get(L"store"))

	UnicodeString fileName(L"conf.bin");
	UnicodeString key(L"key1");
	{
		StatusStore store(baseDir, &fileName);
		store.addShortValue(&key, 123);
	}

	{
		StatusStore store(baseDir, &fileName);
		store.load();

		const AbstractConfigStoreElement* v = store.getValue(&key);
		const ShortValueConfigStoreValue* value = dynamic_cast<const ShortValueConfigStoreValue*>(v);
		uint32_t lv = value->getValue();

		CHECK(lv == 123);
	}
}
