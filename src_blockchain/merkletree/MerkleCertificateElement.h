/*
 * MerkleCertificateElement.h
 *
 *  Created on: 2022/03/22
 *      Author: iizuka
 */

#ifndef MERKLETREE_MERKLECERTIFICATEELEMENT_H_
#define MERKLETREE_MERKLECERTIFICATEELEMENT_H_

#include "filestore_block/IBlockObject.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class MerkleCertificateElement : public IBlockObject {
public:
	MerkleCertificateElement(const MerkleCertificateElement& inst);
	MerkleCertificateElement(const ByteBuffer* hash, bool left);
	virtual ~MerkleCertificateElement();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static MerkleCertificateElement* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	bool isLeft() const noexcept {
		return left;
	}

	const ByteBuffer* getHash() const noexcept {
		return this->hash;
	}

private:
	ByteBuffer* hash;
	bool left;
};

} /* namespace codablecash */

#endif /* MERKLETREE_MERKLECERTIFICATEELEMENT_H_ */
