/*
 * StakingSeedManager.cpp
 *
 *  Created on: Mar 14, 2025
 *      Author: iizuka
 */

#include "bc_wallet_net/StakingSeedManager.h"
#include "bc_wallet_net/StakingEncriptedSeed.h"

#include "base_io/File.h"

#include "bc_base_conf_store/StatusStore.h"

#include "bc_wallet_encoder/IWalletDataEncoder.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"
namespace codablecash {

StakingSeedManager::StakingSeedManager(const File* baseDir) {
	this->storeDir = baseDir->get(StakingSeedManager::STAKING_SEGMENT);
	this->encodedSource = nullptr;

	this->store = new StatusStore(baseDir, STORE_NAME);
}

StakingSeedManager::~StakingSeedManager() {
	delete this->store;
	delete this->encodedSource;

	delete this->storeDir;
}

void StakingSeedManager::setSource(const NodeIdentifierSource *source, const IWalletDataEncoder *encoder) {
	delete this->encodedSource, this->encodedSource = nullptr;

	this->encodedSource = encoder->encodeStakingSource(source);
	save();
}

NodeIdentifierSource* StakingSeedManager::getSource(const IWalletDataEncoder *encoder) const noexcept {
	NodeIdentifierSource* source = encoder->decodeStakingSource(this->encodedSource);
	return source;
}

void StakingSeedManager::save() {
	ByteBuffer* buff = this->encodedSource->getByteBuffer();
	this->store->addBinaryValue(KEY_ENCRYPTED_SOURCE, buff->array(), buff->limit());
}

void StakingSeedManager::load() {
	this->store->load();
	{
		ByteBuffer* buff = this->store->getBinaryValue(KEY_ENCRYPTED_SOURCE); __STP(buff);
		delete this->encodedSource;
		this->encodedSource = new StakingEncriptedSeed(buff->array(), buff->limit());
	}
}

} /* namespace codablecash */
