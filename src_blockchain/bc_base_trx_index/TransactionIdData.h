/*
 * TransactionIdData.h
 *
 *  Created on: 2023/02/27
 *      Author: iizuka
 */

#ifndef BC_BASE_TRX_INDEX_TRANSACTIONIDDATA_H_
#define BC_BASE_TRX_INDEX_TRANSACTIONIDDATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;


namespace codablecash {

class TransactionId;

class TransactionIdData : public IBlockObject {
public:
	explicit TransactionIdData(const TransactionId* trxId);
	virtual ~TransactionIdData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static TransactionIdData* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	const TransactionId* getTrxId() const noexcept {
		return this->trxId;
	}

private:
	TransactionId* trxId;
};

} /* namespace codablecash */

#endif /* BC_BASE_TRX_INDEX_TRANSACTIONIDDATA_H_ */
