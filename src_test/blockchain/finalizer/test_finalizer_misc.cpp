/*
 * test_finalizer_misc.cpp
 *
 *  Created on: 2023/05/26
 *      Author: iizuka
 */

#include "test_utils/t_macros.h"

#include "bc_status_cache_context_finalizer/VoteCandidate.h"
#include "bc_status_cache_context_finalizer/VotingBlockStatus.h"
#include "bc_status_cache_context_finalizer/AlreadyFinalizedException.h"

#include "bc_trx/UtxoId.h"

#include "bc_network/NodeIdentifierSource.h"
#include "bc_network/NodeIdentifier.h"

#include "base_io/ByteBuffer.h"

#include "bc_base/BinaryUtils.h"
#include "bc_base/BalanceAddress.h"
#include "bc_base/AddressDescriptor.h"

#include "bc_base_utxo_index/UtxoData.h"

#include "ecda/ScPrivateKey.h"
#include "ecda/ScPublicKey.h"

#include "bc_block/BlockHeaderId.h"

#include "bc_status_cache/BlockchainController.h"

#include "bc_status_cache_context/RemovedDummyUtxo.h"


#include "osenv/memory.h"

#include "bc_trx/TransactionId.h"

#include <cstring>


using namespace codablecash;

TEST_GROUP(TestFinalizerMiscGroup) {
	TEST_SETUP(){
	}
	TEST_TEARDOWN(){
	}
};

TEST(TestFinalizerMiscGroup, case01){
	char tmp[32];
	Mem::memset(tmp, 1, 32);

	UtxoId utxoId(tmp, 32);
	NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
	NodeIdentifier nodeId = source->toNodeIdentifier();
	const BigInteger* pub = nodeId.getPublicKey();

	ScPrivateKey* privateKey = new ScPrivateKey(pub, 1); __STP(privateKey);
	ScPublicKey pubkey = privateKey->generatePublicKey();

	BalanceAddress* addr = BalanceAddress::createAddress(0, &pubkey); __STP(addr);

	VoteCandidate candidate(&utxoId, &nodeId);
	AddressDescriptor* desc = addr->toAddressDescriptor(); __STP(desc);
	candidate.setAddressDescriptor(desc);
	candidate.setVoterAddressDescriptor(desc);

	VoteCandidate* other = dynamic_cast<VoteCandidate*>(candidate.copyData()); __STP(other);

	int cap = candidate.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
	candidate.toBinary(buff);
	__ASSERT_POS(buff);
	buff->position(0);

	cap = other->binarySize();
	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(cap, true); __STP(buff2);
	other->toBinary(buff2);
	__ASSERT_POS(buff2);
	buff2->position(0);

	CHECK(buff->binaryCmp(buff2) == 0);
}

TEST(TestFinalizerMiscGroup, VotingBlockStatus01){
	char tmpid[32];
	Mem::memset(tmpid, 1, 32);
	BlockHeaderId headerId(tmpid, 32);

	VotingBlockStatus status(1, &headerId);
	{
		char tmp[32];
		Mem::memset(tmp, 1, 32);

		UtxoId utxoId(tmp, 32);
		NodeIdentifierSource* source = NodeIdentifierSource::create(); __STP(source);
		NodeIdentifier nodeId = source->toNodeIdentifier();
		const BigInteger* pub = nodeId.getPublicKey();

		ScPrivateKey* privateKey = new ScPrivateKey(pub, 1); __STP(privateKey);
		ScPublicKey pubkey = privateKey->generatePublicKey();

		BalanceAddress* addr = BalanceAddress::createAddress(0, &pubkey); __STP(addr);

		VoteCandidate candidate(&utxoId, &nodeId);
		AddressDescriptor* desc = addr->toAddressDescriptor(); __STP(desc);
		candidate.setAddressDescriptor(desc);
		candidate.setVoterAddressDescriptor(desc);

		status.addVoteCandidate(dynamic_cast<VoteCandidate*>(candidate.copyData()));
	}

	VotingBlockStatus* other = dynamic_cast<VotingBlockStatus*>(status.copyData()); __STP(other);

	int cap = status.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
	status.toBinary(buff);
	__ASSERT_POS(buff);
	buff->position(0);

	cap = other->binarySize();
	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(cap, true); __STP(buff2);
	other->toBinary(buff2);
	__ASSERT_POS(buff2);
	buff2->position(0);

	CHECK(buff->binaryCmp(buff2) == 0);
}

TEST(TestFinalizerMiscGroup, UtxoData01){
	char tmpid[32];
	Mem::memset(tmpid, 1, 32);
	BlockHeaderId headerId(tmpid, 32);

	TransactionId trxId(tmpid, 32);

	RemovedDummyUtxo utxo;
	UtxoId utxoId(tmpid, 32);
	utxo.setUtxoId(&utxoId);

	UtxoData data(&utxo, &trxId, 1, &headerId);

	int cap = data.binarySize();
	ByteBuffer* buff = ByteBuffer::allocateWithEndian(cap, true); __STP(buff);
	data.toBinary(buff);
	__ASSERT_POS(buff);
	buff->position(0);

	UtxoData* other = dynamic_cast<UtxoData*>(data.copyData()); __STP(other);

	cap = other->binarySize();
	ByteBuffer* buff2 = ByteBuffer::allocateWithEndian(cap, true); __STP(buff2);
	other->toBinary(buff2);
	__ASSERT_POS(buff2);
	buff2->position(0);

	CHECK(buff->binaryCmp(buff2) == 0);
}

TEST(TestFinalizerMiscGroup, AlreadyFinalizedException01){
	testException<AlreadyFinalizedException>();
}

TEST(TestFinalizerMiscGroup, AlreadyFinalizedException02){
	uint64_t finalizedHeight = 10;
	uint64_t finalizingHeight = 10;

	AlreadyFinalizedException* ex = nullptr;
	try{
		BlockchainController::checkFinalizedHeight(finalizedHeight, finalizingHeight);
	}
	catch(AlreadyFinalizedException* e){
		ex = e;
	}

	CHECK(ex != nullptr);
	delete ex;
}

