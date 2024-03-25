/*
 * test_merkletree.cpp
 *
 *  Created on: 2022/03/22
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"
#include "base/StackRelease.h"

#include "base_io/ByteBuffer.h"

#include "merkletree/MerkleTree.h"
#include "merkletree/MerkleCertificate.h"

#include "crypto/Sha256.h"


TEST_GROUP(TestMerkleTreeGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

using namespace alinous;
using namespace codablecash;

TEST(TestMerkleTreeGroup, case01){
	MerkleTree tree;

	int maxLoop = 7;
	for(int i = 0; i != maxLoop; ++i){
		uint64_t data = i;

		ByteBuffer* b = Sha256::sha256((const char*)&data, sizeof(data), true); __STP(b);
		tree.addElement(b);
	}

	tree.pack();

	uint64_t data = 3;
	ByteBuffer* b = Sha256::sha256((const char*)&data, sizeof(data), true); __STP(b);

	MerkleCertificate* cert = tree.makeCertificate((const char*)b->array(), b->limit()); __STP(cert);

	int len = cert->size();
	CHECK(len == 4);

	bool bl = cert->certificate();
	CHECK(bl == true);
}

TEST(TestMerkleTreeGroup, case02){
	MerkleTree tree;

	int maxLoop = 1;
	for(int i = 0; i != maxLoop; ++i){
		uint64_t data = i;

		ByteBuffer* b = Sha256::sha256((const char*)&data, sizeof(data), true); __STP(b);
		tree.addElement(b);
	}

	tree.pack();

	uint64_t data = 0;
	ByteBuffer* b = Sha256::sha256((const char*)&data, sizeof(data), true); __STP(b);

	MerkleCertificate* cert = tree.makeCertificate((const char*)b->array(), b->limit()); __STP(cert);

	int len = cert->size();
	CHECK(len == 1);

	bool bl = cert->certificate();
	CHECK(bl == true);
}

TEST(TestMerkleTreeGroup, case03){
	MerkleTree tree;

	int maxLoop = 7;
	for(int i = 0; i != maxLoop; ++i){
		uint64_t data = i;

		ByteBuffer* b = Sha256::sha256((const char*)&data, sizeof(data), true); __STP(b);
		tree.addElement(b);
	}

	tree.pack();

	uint64_t data = 300;
	ByteBuffer* b = Sha256::sha256((const char*)&data, sizeof(data), true); __STP(b);

	MerkleCertificate* cert = tree.makeCertificate((const char*)b->array(), b->limit()); __STP(cert);
	CHECK(cert == nullptr);
}
