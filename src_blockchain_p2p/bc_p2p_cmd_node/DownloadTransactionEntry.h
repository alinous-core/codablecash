/*
 * DownloadTransactionEntry.h
 *
 *  Created on: 2023/12/06
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_DOWNLOADTRANSACTIONENTRY_H_
#define BC_P2P_CMD_NODE_DOWNLOADTRANSACTIONENTRY_H_

#include <cstdint>

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class TransactionId;

class DownloadTransactionEntry : public IBlockObject {
public:
	static const constexpr uint8_t TYPE_CONTROL{1};
	static const constexpr uint8_t TYPE_ICC{2};
	static const constexpr uint8_t TYPE_BALANCE{3};
	static const constexpr uint8_t TYPE_SMARTCONTRACT{4};

	DownloadTransactionEntry(const DownloadTransactionEntry& inst) = delete;
	DownloadTransactionEntry(uint64_t height, const TransactionId* trxId, uint8_t trxType);
	virtual ~DownloadTransactionEntry();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static DownloadTransactionEntry* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	const TransactionId* getTransactionId() const noexcept {
		return this->trxId;
	}

	uint8_t getTrxType() const noexcept {
		return this->trxType;
	}

private:
	uint64_t height;
	TransactionId* trxId;
	uint8_t trxType;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_DOWNLOADTRANSACTIONENTRY_H_ */
