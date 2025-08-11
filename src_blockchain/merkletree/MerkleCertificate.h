/*
 * MerkleCertificate.h
 *
 *  Created on: 2022/03/22
 *      Author: iizuka
 */

#ifndef MERKLETREE_MERKLECERTIFICATE_H_
#define MERKLETREE_MERKLECERTIFICATE_H_

#include "base/ArrayList.h"

#include "filestore_block/IBlockObject.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class MerkleCertificateElement;

class MerkleCertificate : public IBlockObject {
public:
	MerkleCertificate(const MerkleCertificate& inst);
	MerkleCertificate();
	virtual ~MerkleCertificate();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static MerkleCertificate* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void setMerkleRoot(const ByteBuffer* hash) noexcept;

	void addHash(MerkleCertificateElement* element) noexcept;
	int size() const noexcept {
		return this->list.size();
	}

	bool certificate(ByteBuffer* data) const;

	ByteBuffer* getRootHash() const noexcept;

	ByteBuffer* getOriginalHash() const noexcept;

private:
	ByteBuffer* joinHash(const ByteBuffer* left, const ByteBuffer* right) const noexcept;

private:
	ByteBuffer* rootHash;
	ArrayList<MerkleCertificateElement> list;
};

} /* namespace codablecash */

#endif /* MERKLETREE_MERKLECERTIFICATE_H_ */
