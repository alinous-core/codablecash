/*
 * test_undo_misc.cpp
 *
 *  Created on: 2023/07/17
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "base_io/ByteBuffer.h"

#include "trx/session/base/AbstractUndoLogPart.h"

#include "trx/session/base/AbstractUndoLog.h"

#include "btreekey/NullKey.h"

#include "btreekey/InfinityKey.h"

#include "trx/session/repo/UndoLogsIdkey.h"

#include "trx/session/base/CdbDatabaseSessionId.h"

#include "trx/session/repo/UndoLogsPartIdKey.h"

#include "trx/session/record/UndoDeleteRecordPart.h"
using namespace codablecash;
using namespace alinous;


TEST_GROUP(TestUnodoMiscGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};

TEST(TestUnodoMiscGroup, partcreate01){
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(10, true); __STP(buff);
	buff->put(255);

	buff->position(0);
	AbstractUndoLogPart* part = AbstractUndoLogPart::createFromBinary(buff);
	CHECK(part == nullptr);
}

TEST(TestUnodoMiscGroup, undocreate01){
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(10, true); __STP(buff);
	buff->put(255);

	buff->position(0);
	AbstractUndoLog* part = AbstractUndoLog::createFromBinary(buff);
	CHECK(part == nullptr);
}

TEST(TestUnodoMiscGroup, keycmp01){
	NullKey nullkey;
	InfinityKey infkey;

	CdbDatabaseSessionId* sessionId = CdbDatabaseSessionId::createRandomId(); __STP(sessionId);
	CdbDatabaseSessionId* sessionId2 = CdbDatabaseSessionId::createRandomId(); __STP(sessionId2);

	UndoLogsIdkey key;
	key.setSessionId(sessionId);
	key.setSerial(1);
	UndoLogsIdkey key2;
	key2.setSessionId(sessionId2);
	key2.setSerial(2);

	int diff = key.compareTo(&nullkey);
	CHECK(diff > 0);
	diff = key.compareTo(&infkey);
	CHECK(diff < 0);

	diff = key.compareTo(&key2);
	CHECK(diff != 0);

	UndoLogsPartIdKey partKey;
	partKey.setLogIdKey(&key);
	partKey.setPartSerial(1);
	UndoLogsPartIdKey partKey2;
	partKey2.setLogIdKey(&key2);
	partKey2.setPartSerial(1);

	diff = partKey.compareTo(&nullkey);
	CHECK(diff > 0);
	diff = partKey.compareTo(&infkey);
	CHECK(diff < 0);

	diff = partKey.compareTo(&partKey2);
	CHECK(diff != 0);
}

TEST(TestUnodoMiscGroup, UndoDeleteRecordPartCopy01){
	UndoDeleteRecordPart part;

	UndoDeleteRecordPart* part2 = dynamic_cast<UndoDeleteRecordPart*>(part.copyData()); __STP(part2);
	CHECK(part2 != nullptr);
}
