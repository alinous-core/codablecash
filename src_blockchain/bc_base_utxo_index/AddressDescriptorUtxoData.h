/*
 * AddressDescriptorUtxoData.h
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORUTXODATA_H_
#define BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORUTXODATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class AbstractUtxo;
class BalanceUnit;

class AddressDescriptorUtxoData : public IBlockObject {
public:
	AddressDescriptorUtxoData();
	virtual ~AddressDescriptorUtxoData();

	void join(const AddressDescriptorUtxoData* value) noexcept;
	bool contains(const AbstractUtxo* value) const noexcept;
	void remove(const AbstractUtxo* value) noexcept;
	int indexof(const AbstractUtxo* value) const noexcept;
	bool isEmpty() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static AddressDescriptorUtxoData* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void add(const AbstractUtxo* utxo);

	BalanceUnit getTotalAmount() const noexcept;

	const ArrayList<AbstractUtxo>* getList() const noexcept {
		return &list;
	}

private:
	ArrayList<AbstractUtxo> list;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORUTXODATA_H_ */
