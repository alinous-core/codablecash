/*
 * ClientBlockHeaderTransferData.h
 *
 *  Created on: May 4, 2025
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_CLIENT_CLIENTBLOCKHEADERTRANSFERDATA_H_
#define BC_P2P_CMD_CLIENT_CLIENTBLOCKHEADERTRANSFERDATA_H_

#include "base/ArrayList.h"

#include "data_history/AbstractTransferedData.h"

#include "bc_block/BlockHeader.h"

#include "bc_block_body/ITransactionVisitor.h"


using namespace alinous;

namespace codablecash {

class BlockHeader;
class VoteTransactionIdCertificate;
class BlockHeaderTransferData;
class BlockTransactionTransferData;
class BloomFilter1024;

class ClientBlockHeaderTransferData : public AbstractTransferedData, public IVoteTransactionIdCertificateBuilder, public ITransactionVisitor {
public:
	ClientBlockHeaderTransferData(const ClientBlockHeaderTransferData& inst);
	ClientBlockHeaderTransferData();
	virtual ~ClientBlockHeaderTransferData();

	static ClientBlockHeaderTransferData* createFromBlock(const Block* block);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void addCert(const VoteTransactionIdCertificate* cert) noexcept;

	virtual TransferedDataId* getTransferedDataId() const;

	virtual void detect(const Block* block, BlockHeaderId* votedHeaderId, TransactionId* voteTrxId);

	void validateVoteTransactionsData() const;
	void validateTransactions() const;


	const VoteTransactionIdCertificate* getVoteTransactionIdCertificate(const TransactionId *voteTrxId) const;
	void setHeader(const BlockHeader* header) noexcept;
	const BlockHeader* getHeader() const noexcept {
		return this->header;
	}

	BlockHeaderTransferData* toBlockHeaderTransferData() const;

	void setFiltersList(const ArrayList<BloomFilter1024> *filtersList);

	virtual void visit(const AbstractBlockchainTransaction* trx, const Block* block);

	const ArrayList<BlockTransactionTransferData>* getTransactionsList() const noexcept {
		return this->trxList;
	}

private:
	BlockHeader* header;
	ArrayList<VoteTransactionIdCertificate>* certList;

	ArrayList<BlockTransactionTransferData>* trxList;

	// temp data
	const ArrayList<BloomFilter1024> *filtersList;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_CLIENT_CLIENTBLOCKHEADERTRANSFERDATA_H_ */
