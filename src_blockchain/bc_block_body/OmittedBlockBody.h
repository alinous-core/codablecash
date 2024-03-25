/*
 * OmittedBlockBody.h
 *
 *  Created on: 2023/11/23
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BODY_OMITTEDBLOCKBODY_H_
#define BC_BLOCK_BODY_OMITTEDBLOCKBODY_H_

#include <cstdint>

#include "filestore_block/IBlockObject.h"

#include "base/HashMap.h"

using namespace alinous;

namespace codablecash {

class BlockMerkleRoot;
class BlockRewordBase;
class TransactionId;
class BlockBody;
class IBlockBodyFetcher;
class DownloadTransactionsNodeCommand;
class DownloadTransactionsNodeCommandResponse;
class NodeIdentifierSource;
class AbstractBlockchainTransaction;
class MemPoolTransaction;
class BlockHeaderId;
class ReservedVotes;

class OmittedBlockBody : public alinous::IBlockObject {
public:
	OmittedBlockBody(const OmittedBlockBody& inst);
	OmittedBlockBody();
	virtual ~OmittedBlockBody();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static OmittedBlockBody* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setNonce(uint64_t nonce);
	void addBalanceTransaction(const TransactionId* trxId);
	void addControlTransaction(const TransactionId* trxId);
	void addInterChainCommunicationTransaction(const TransactionId* trxId);
	void addSmartContractTransaction(const TransactionId* trxId);
	void setRewordBase(const BlockRewordBase* rewardBase);

	BlockBody* toBlockBody(uint64_t height, const BlockHeaderId* headerId, IBlockBodyFetcher* fetcher) const;

private:
	void exportBalanceTransactions(uint64_t height, const BlockHeaderId* headerId, BlockBody* body, IBlockBodyFetcher *fetcher) const;
	void exportControlTransactions(uint64_t height, const BlockHeaderId* headerId, BlockBody* body, IBlockBodyFetcher *fetcher) const;
	void exportInterChainCommunicationTransactions(uint64_t height, const BlockHeaderId* headerId, BlockBody* body, IBlockBodyFetcher *fetcher) const;
	void exportSmartcontractTransaction(uint64_t height, const BlockHeaderId* headerId, BlockBody* body, IBlockBodyFetcher *fetcher) const;

private:
	uint64_t nonce;

	ArrayList<TransactionId>* controlTransactions;
	ArrayList<TransactionId>* iccTransactions;
	ArrayList<TransactionId>* balanceTransactions;
	ArrayList<TransactionId>* smartcontractTransactions;

	BlockRewordBase* rewardBase;
};

class IBlockBodyFetcher {
public:
	virtual ~IBlockBodyFetcher(){};

	virtual MemPoolTransaction* begin() = 0;
	virtual DownloadTransactionsNodeCommandResponse* downloadTransactions(const DownloadTransactionsNodeCommand* command) const = 0;
	virtual NodeIdentifierSource* getNetworkKey() const noexcept = 0;

};

} /* namespace codablecash */

#endif /* BC_BLOCK_BODY_OMITTEDBLOCKBODY_H_ */
