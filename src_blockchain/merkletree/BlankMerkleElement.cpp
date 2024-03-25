/*
 * BlankMerkleElement.cpp
 *
 *  Created on: 2022/03/21
 *      Author: iizuka
 */

#include "merkletree/BlankMerkleElement.h"

#include "crypto/Sha256.h"
#include "bc_base/Base58.h"

#include "base/StackRelease.h"
#include "base/UnicodeString.h"

#include "base_io/ByteBuffer.h"


namespace codablecash {

const UnicodeString BlankMerkleElement::BLANK_HASH(L"GKot5hBsd81kMupNCXHaqbhv3huEbxAFMLnpcX2hniwn");

BlankMerkleElement::BlankMerkleElement() : AbstractMerkleElement () {
	this->hash = Base58::decode(&BLANK_HASH);
	//this->hash = Sha256::sha256("", 0, true);

	//UnicodeString* str = Base58::encode((const char*)this->hash->array(), this->hash->limit()); __STP(str);
	//Base58::decode(str);
}

BlankMerkleElement::~BlankMerkleElement() {

}

const AbstractMerkleElement* BlankMerkleElement::find(const ByteBuffer* hash) const noexcept {
	return nullptr;
}

} /* namespace codablecash */
