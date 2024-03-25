/*
 * AddressDescriptorKey.h
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORKEY_H_
#define BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORKEY_H_

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class AddressDescriptor;
class AbstractUtxo;

class AddressDescriptorKey : public AbstractBtreeKey {
public:
	AddressDescriptorKey();
	explicit AddressDescriptorKey(const AddressDescriptor* desc);
	virtual ~AddressDescriptorKey();

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static AddressDescriptorKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	const AbstractUtxo* getUtxo() const noexcept {
		return this->utxo;
	}

	void setUtxo(const AbstractUtxo* utxo) noexcept;

private:
	AddressDescriptor* desc;
	AbstractUtxo* utxo; // remove param
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORKEY_H_ */
