/*
 * SmartcontractModuleAddress.cpp
 *
 *  Created on: Oct 2, 2025
 *      Author: iizuka
 */

#include "transaction/SmartcontractModuleAddress.h"

#include "ecda/Secp256k1CompressedPoint.h"
#include "ecda/ScPublicKey.h"

namespace codablecash {

const char* SmartcontractModuleAddress::PREFIX = "cm";

SmartcontractModuleAddress::SmartcontractModuleAddress() : AbstractSmartcontractAddress() {

}

SmartcontractModuleAddress::SmartcontractModuleAddress(uint16_t zone, Secp256k1CompressedPoint *pubkey)
	: AbstractSmartcontractAddress(zone, pubkey) {

}

SmartcontractModuleAddress::~SmartcontractModuleAddress() {

}

const char* SmartcontractModuleAddress::getPrefix() const noexcept {
	return SmartcontractModuleAddress::PREFIX;
}

IBlockObject* SmartcontractModuleAddress::copyData() const noexcept {
	return new SmartcontractModuleAddress(this->zone, dynamic_cast<Secp256k1CompressedPoint*>(this->pubkey->copyData()));
}

SmartcontractModuleAddress* SmartcontractModuleAddress::createAddress(uint16_t zone, const ScPublicKey *publicKey) {
	Secp256k1CompressedPoint pubkey = Secp256k1CompressedPoint::compress(publicKey);
	SmartcontractModuleAddress* address = new SmartcontractModuleAddress(zone, dynamic_cast<Secp256k1CompressedPoint*>(pubkey.copyData()));

	return address;
}

} /* namespace codablecash */
