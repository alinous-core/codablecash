/*
 * BalanceAddress.h
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#ifndef BC_BASE_BALANCEADDRESS_H_
#define BC_BASE_BALANCEADDRESS_H_

#include "bc_base/AbstractAddress.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class ScPublicKey;
class Secp256k1CompressedPoint;

class BalanceAddress : public AbstractAddress {
public:
	static const char* PREFIX;

	BalanceAddress();
	BalanceAddress(uint16_t zone, Secp256k1CompressedPoint* pubkey);
	virtual ~BalanceAddress();

	virtual uint8_t getType() const noexcept {
		return AbstractAddress::ADDRESS_TYPE_BALANCE;
	}

	static BalanceAddress* createAddress(uint16_t zone, const ScPublicKey* publicKey);

	virtual const char* getPrefix() const noexcept;
	virtual ByteBuffer* getBodyPart() const noexcept;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

private:
	Secp256k1CompressedPoint* pubkey;
};

} /* namespace codablecash */

#endif /* BC_BASE_BALANCEADDRESS_H_ */
