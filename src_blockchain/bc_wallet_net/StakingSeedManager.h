/*
 * StakingSeedManager.h
 *
 *  Created on: Mar 14, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_NET_STAKINGSEEDMANAGER_H_
#define BC_WALLET_NET_STAKINGSEEDMANAGER_H_


namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class StatusStore;
class IWalletDataEncoder;
class StakingEncriptedSeed;
class NodeIdentifierSource;

class StakingSeedManager {
public:
	static const constexpr wchar_t* STAKING_SEGMENT{L"staking_seed"};
	static const constexpr wchar_t* STORE_NAME{L"staking_seed_store"};
	static const constexpr wchar_t* KEY_ENCRYPTED_SOURCE{L"encryptedSource"};

	explicit StakingSeedManager(const File* baseDir);
	virtual ~StakingSeedManager();

	void setSource(const NodeIdentifierSource *source, const IWalletDataEncoder* encoder);
	NodeIdentifierSource* getSource(const IWalletDataEncoder *encoder) const noexcept;

	void save();
	void load();

private:
	File* storeDir;
	StatusStore* store;

	StakingEncriptedSeed* encodedSource;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_STAKINGSEEDMANAGER_H_ */
