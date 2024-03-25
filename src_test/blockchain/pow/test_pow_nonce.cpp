/*
 * test_pow_nonce.cpp
 *
 *  Created on: 2023/03/21
 *      Author: iizuka
 */
#include "test_utils/t_macros.h"

#include "numeric/BigInteger.h"

#include "pow/PoWNonce.h"
#include "pow/PoWNonceResult.h"

#include "base_io/ByteBuffer.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_block/BlockMerkleRoot.h"

#include "osenv/funcs.h"

#include "base_timestamp/SystemTimestamp.h"

using namespace codablecash;

TEST_GROUP(TestPoWNonceGroup) {
	TEST_SETUP(){}
	TEST_TEARDOWN(){}
};

TEST(TestPoWNonceGroup, case01){
	PoWNonce* n = PoWNonce::createRandomNonce(); __STP(n);
	PoWNonce* n2 = PoWNonce::createRandomNonce(); __STP(n2);

	ByteBuffer* buff = n2->tobyte32Buffer(); __STP(buff);
	BlockHeaderId id((const char*)buff->array(), buff->limit());

	SystemTimestamp tm = Os::now();

	uint8_t b[32]{};
	BlockMerkleRoot root((const char*)b, 32);

	PoWNonceResult* result = n->calcResult(&id, &root, &tm); __STP(result);
	BigInteger diff = result->getDifficulty();
	uint64_t ldiff = diff.longValue();
}

TEST(TestPoWNonceGroup, case02){
	BigInteger hashRate(20L);
	uint64_t milliseconds = 290;

	BigInteger diff = PoWNonce::calcNecessaryDifficulty(hashRate, milliseconds);
	UnicodeString diffstr = diff.toString(16);

	BigInteger target = PoWNonce::getMaxBigInt()->subtract(diff);
	UnicodeString targetstr = target.toString(16);
}

TEST(TestPoWNonceGroup, case03){
	BigInteger hashRate(100L);
	uint64_t milliseconds = 1000;

	BigInteger diff = PoWNonce::calcNecessaryDifficulty(hashRate, milliseconds);
	UnicodeString diffstr = diff.toString(16);
}

TEST(TestPoWNonceGroup, copyData01){
	PoWNonce* n = PoWNonce::createRandomNonce(); __STP(n);
	PoWNonce* n2 = dynamic_cast<PoWNonce*>(n->copyData()); __STP(n2);

	int diff = n->compareTo(n2);
	CHECK(diff == 0);
}

