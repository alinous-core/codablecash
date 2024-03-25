/*
 * AbstractBlockRewordTransaction.h
 *
 *  Created on: 2023/05/15
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BODY_ABSTRACTBLOCKREWORDTRANSACTION_H_
#define BC_BLOCK_BODY_ABSTRACTBLOCKREWORDTRANSACTION_H_

#include "bc_trx/AbstractBalanceTransaction.h"

namespace codablecash {

class InputUtxoCollection;

class AbstractBlockRewordTransaction : public AbstractBalanceTransaction {
public:
	AbstractBlockRewordTransaction(const AbstractBlockRewordTransaction& inst);
	AbstractBlockRewordTransaction();
	virtual ~AbstractBlockRewordTransaction();

	void addUtxo(const AbstractUtxo* utxo) noexcept;
	void addInputUtxoRef(const AbstractUtxoReference* ref) noexcept;

	virtual int getUtxoSize() const noexcept;
	virtual AbstractUtxo* getUtxo(int i) const noexcept;

	virtual int getUtxoReferenceSize() const noexcept;
	virtual AbstractUtxoReference* getUtxoReference(int i) const noexcept;

	virtual BalanceUnit getFee() const noexcept;

	void setHeight(uint64_t height) noexcept {
		this->height = height;
	}
	uint64_t getHeight() const noexcept {
		return this->height;
	}

protected:
	void setUtxoNonce() noexcept;

	int __binarySize() const;
	void __toBinary(ByteBuffer* out) const;
	void __fromBinary(ByteBuffer* in);

private:
	uint64_t height;
	ArrayList<AbstractUtxoReference> inputlist;
	ArrayList<AbstractUtxo> list;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BODY_ABSTRACTBLOCKREWORDTRANSACTION_H_ */
