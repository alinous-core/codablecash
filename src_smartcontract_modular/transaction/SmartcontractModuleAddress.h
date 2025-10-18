/*
 * SmartcontractModuleAddress.h
 *
 *  Created on: Oct 2, 2025
 *      Author: iizuka
 */

#ifndef TRANSACTION_SMARTCONTRACTMODULEADDRESS_H_
#define TRANSACTION_SMARTCONTRACTMODULEADDRESS_H_

#include "transaction/AbstractSmartcontractAddress.h"

namespace codablecash {

class ScPublicKey;

class SmartcontractModuleAddress : public AbstractSmartcontractAddress {
public:
	static const char* PREFIX;

	SmartcontractModuleAddress();
	SmartcontractModuleAddress(uint16_t zone, Secp256k1CompressedPoint* pubkey);
	virtual ~SmartcontractModuleAddress();

	virtual uint8_t getType() const noexcept {
		return AbstractAddress::ADDRESS_TYPE_SMARTCONTRACT_MODULE;
	}

	virtual const char* getPrefix() const noexcept;

	virtual IBlockObject* copyData() const noexcept;

	static SmartcontractModuleAddress* createAddress(uint16_t zone, const ScPublicKey* publicKey);
};

} /* namespace codablecash */

#endif /* TRANSACTION_SMARTCONTRACTMODULEADDRESS_H_ */
