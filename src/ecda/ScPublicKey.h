/*
 * ScPublicKey.h
 *
 *  Created on: 2022/03/23
 *      Author: iizuka
 */

#ifndef ECDA_SCPUBLICKEY_H_
#define ECDA_SCPUBLICKEY_H_

#include "Secp256k1Point.h"
#include "filestore_block/IBlockObject.h"

namespace codablecash {

class ScPublicKey : public Secp256k1Point, public IBlockObject {
public:
	explicit ScPublicKey(const Secp256k1Point& pt);

	//ScPublicKey();
	virtual ~ScPublicKey();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static ScPublicKey* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	bool equals(const ScPublicKey* other) const noexcept;
};

} /* namespace codablecash */

#endif /* ECDA_SCPUBLICKEY_H_ */
