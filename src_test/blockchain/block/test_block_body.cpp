/*
 * test_block_body.cpp
 *
 *  Created on: 2023/03/01
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "../wallet_util/WalletDriver.h"

#include "bc_block_body/BlockBody.h"

#include "bc_block/BlockMerkleRoot.h"

using namespace codablecash;

TEST_GROUP(TestBlockBlockBodyGroup) {
	TEST_SETUP(){
		env->setup();
	}
	TEST_TEARDOWN(){
		env->teardown();
	}
};


TEST(TestBlockBlockBodyGroup, case01){
	File projectFolder = this->env->testCaseDir();
	_ST(File, walletBaseDir, projectFolder.get(L"wallet"))

	WalletDriver driver(0, walletBaseDir, L"changeit");
	driver.init(2);
	driver.initGenesis(0, 100000);

	BalanceTransferTransaction* trx = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
	__STP(trx);
	BalanceTransferTransaction* trx2 = driver.createBalanceTransferTransaction(0, 1, 0, BalanceUnit(10000), BalanceUnit(1), false);
	__STP(trx2);

	{
		BlockBody body;
		body.addBalanceTransaction(trx);
		body.addBalanceTransaction(trx2);
		body.build();

		int capacity = body.binarySize();
		ByteBuffer* buff = ByteBuffer::allocateWithEndian(capacity, true); __STP(buff);
		body.toBinary(buff);

		buff->position(0);
		BlockBody* __body = BlockBody::fromBinary(buff); __STP(__body);

		BlockBody* body2 = dynamic_cast<BlockBody*>(__body->copyData()); __STP(body2);
		const BlockMerkleRoot* root2 = body2->getMerkleRoot();

		CHECK(root2->equals(body.getMerkleRoot()));
	}
}
