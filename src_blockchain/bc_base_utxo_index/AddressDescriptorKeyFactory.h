/*
 * AddressDescriptorKeyFactory.h
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORKEYFACTORY_H_
#define BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class AddressDescriptorKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t ADDRESS_DESC_KEY{0x14};

	AddressDescriptorKeyFactory();
	virtual ~AddressDescriptorKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORKEYFACTORY_H_ */
