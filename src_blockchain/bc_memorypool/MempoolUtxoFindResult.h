/*
 * MempoolUtxoFindResult.h
 *
 *  Created on: 2023/05/12
 *      Author: iizuka
 */

#ifndef BC_MEMORYPOOL_MEMPOOLUTXOFINDRESULT_H_
#define BC_MEMORYPOOL_MEMPOOLUTXOFINDRESULT_H_

namespace codablecash {

class TransactionId;
class AbstractUtxo;

class MempoolUtxoFindResult {
public:
	MempoolUtxoFindResult(const TransactionId* trxId, const AbstractUtxo* utxo);
	virtual ~MempoolUtxoFindResult();

	const TransactionId* getTransactionId() const noexcept {
		return this->trxId;
	}
/*
	AbstractUtxo* moveUtxo() noexcept {
		AbstractUtxo* ret = this->utxo;
		this->utxo = nullptr;

		return ret;
	}
*/

	const AbstractUtxo* getUtxo() const noexcept {
		return this->utxo;
	}

private:
	TransactionId* trxId;
	AbstractUtxo* utxo;
};

} /* namespace codablecash */

#endif /* BC_MEMORYPOOL_MEMPOOLUTXOFINDRESULT_H_ */
