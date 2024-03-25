/*
 * test_undo_misc.cpp
 *
 *  Created on: 2023/07/30
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "trx/session/record/UndoModifyRecordColumnsPart.h"
#include "trx/session/record/UndoDeletedRecordColumnPart.h"
#include "trx/session/record/UndoCreateSchemaPart.h"
#include "trx/session/record/UndoInsertRecordPart.h"
#include "trx/session/record/UndoUpdateRecordPart.h"

#include "trx/session/index/UndoAddIndexPart.h"
#include "trx/session/index/UndoIndexUniquePart.h"
#include "trx/session/index/UndoIndexPrimaryPart.h"


using namespace codablecash;


TEST_GROUP(TestUndoMiscGroup) {
	TEST_SETUP() {
		env->setup();
	}
	TEST_TEARDOWN() {
		env->teardown();
	}
};

TEST(TestUndoMiscGroup, UndoModifyRecordColumnsPart01){
	UndoModifyRecordColumnsPart part;
	UndoModifyRecordColumnsPart* part2 = dynamic_cast<UndoModifyRecordColumnsPart*>(part.copyData()); __STP(part2);

	CHECK(part2 != nullptr);
}

TEST(TestUndoMiscGroup, UndoAddIndexPart01){
	UndoAddIndexPart part;
	UndoAddIndexPart* part2 = dynamic_cast<UndoAddIndexPart*>(part.copyData()); __STP(part2);

	CHECK(part2 != nullptr);
}

TEST(TestUndoMiscGroup, UndoIndexUniquePart01){
	UndoIndexUniquePart part;
	UndoIndexUniquePart* part2 = dynamic_cast<UndoIndexUniquePart*>(part.copyData()); __STP(part2);

	CHECK(part2 != nullptr);
}

TEST(TestUndoMiscGroup, UndoDeletedRecordColumnPart01){
	UndoDeletedRecordColumnPart part;
	UndoDeletedRecordColumnPart* part2 = dynamic_cast<UndoDeletedRecordColumnPart*>(part.copyData()); __STP(part2);

	CHECK(part2 != nullptr);
}

TEST(TestUndoMiscGroup, UndoCreateSchemaPart01){
	UndoCreateSchemaPart part;
	UndoCreateSchemaPart* part2 = dynamic_cast<UndoCreateSchemaPart*>(part.copyData()); __STP(part2);

	CHECK(part2 != nullptr);
}

TEST(TestUndoMiscGroup, UndoIndexPrimaryPart01){
	UndoIndexPrimaryPart part;
	UndoIndexPrimaryPart* part2 = dynamic_cast<UndoIndexPrimaryPart*>(part.copyData()); __STP(part2);

	CHECK(part2 != nullptr);
}

TEST(TestUndoMiscGroup, UndoInsertRecordPart01){
	UndoInsertRecordPart part;
	UndoInsertRecordPart* part2 = dynamic_cast<UndoInsertRecordPart*>(part.copyData()); __STP(part2);

	CHECK(part2 != nullptr);
}

TEST(TestUndoMiscGroup, UndoUpdateRecordPart01){
	UndoUpdateRecordPart part;
	UndoUpdateRecordPart* part2 = dynamic_cast<UndoUpdateRecordPart*>(part.copyData()); __STP(part2);

	CHECK(part2 != nullptr);
}


