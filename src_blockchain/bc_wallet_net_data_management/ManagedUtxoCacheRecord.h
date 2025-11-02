/*
 * ManagedUtxoCacheRecord.h
 *
 *  Created on: Jul 22, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_DATA_MANAGEMENT_MANAGEDUTXOCACHERECORD_H_
#define BC_WALLET_NET_DATA_MANAGEMENT_MANAGEDUTXOCACHERECORD_H_

#include <cstdint>

namespace codablecash {

class AbstractUtxo;
class UtxoId;
class TransactionId;

class ManagedUtxoCacheRecord {
public:
	static constexpr const uint8_t NONE{0};
	static constexpr const uint8_t FINALIZED{1};
	static constexpr const uint8_t UNFINALIZED{2};
	static constexpr const uint8_t MEMPOOL{3};

	ManagedUtxoCacheRecord();
	virtual ~ManagedUtxoCacheRecord();

	void setUtxo(const AbstractUtxo* utxo);
	void setType(uint8_t type);
	void setTransactionId(const TransactionId* trxId);

	const UtxoId* getUtxoId() const noexcept;

	const AbstractUtxo* getUtxo() const noexcept {
		return this->utxo;
	}
	uint8_t getType() const noexcept {
		return this->type;
	}
	const TransactionId* getTransactionId() const noexcept {
		return this->transactionId;
	}

private:
	AbstractUtxo* utxo;
	uint8_t type;
	TransactionId* transactionId;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_DATA_MANAGEMENT_MANAGEDUTXOCACHERECORD_H_ */
