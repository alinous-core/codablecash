/*
 * AddressAndPrivateKey.h
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_ADDRESSANDPRIVATEKEY_H_
#define BC_WALLET_ADDRESSANDPRIVATEKEY_H_

namespace codablecash {

class ScPrivateKey;
class BalanceAddress;

class AddressAndPrivateKey {
public:
	AddressAndPrivateKey();
	virtual ~AddressAndPrivateKey();

	void setPrivateKey(ScPrivateKey* privateKey) noexcept;
	void setBalanceAddress(BalanceAddress* address) noexcept;

	const BalanceAddress* getAddress() const noexcept {
		return this->address;
	}
	const ScPrivateKey* getPrivateKey() const noexcept {
		return this->privateKey;
	}

private:
	ScPrivateKey* privateKey;
	BalanceAddress* address;
};

} /* namespace codablecash */

#endif /* BC_WALLET_ADDRESSANDPRIVATEKEY_H_ */
