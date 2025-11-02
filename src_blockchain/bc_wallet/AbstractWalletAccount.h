/*
 * AbstractWalletAccount.h
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_ABSTRACTWALLETACCOUNT_H_
#define BC_WALLET_ABSTRACTWALLETACCOUNT_H_

#include <cstdint>

namespace codablecash {

class AbstractWalletAccount {
public:
	AbstractWalletAccount(uint16_t zone, int accountIndex);
	virtual ~AbstractWalletAccount();

	uint16_t getZone() const noexcept {
		return this->zone;
	}

protected:
	int accountIndex;
	uint16_t zone;

};

} /* namespace codablecash */

#endif /* BC_WALLET_ABSTRACTWALLETACCOUNT_H_ */
