/*
 * HdWallet.h
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_HDWALLET_H_
#define BC_WALLET_HDWALLET_H_
#include <cstdint>

#include "base/ArrayList.h"

namespace alinous {
class File;
}
using namespace alinous;

namespace codablecash {

class HdWalletSeed;
class WalletAccount;
class IWalletDataEncoder;
class StatusStore;

class HdWallet {
public:
	static const constexpr wchar_t* STORE_NAME{L"HdWallet"};
	static const constexpr wchar_t* KEY_ENCODED_SEED{L"encodedSeed"};
	static const constexpr wchar_t* KEY_NUM_ACCOUNTS{L"numAccounts"};

	HdWallet(const File* baseDir);
	virtual ~HdWallet();

	static HdWallet* create(const File* dir, const HdWalletSeed* seed, uint16_t defaultZone, const IWalletDataEncoder* encoder);
	static HdWallet* loadWallet(const File* dir, const IWalletDataEncoder* encoder);

	void setSeed(const HdWalletSeed *seed, const IWalletDataEncoder* encoder);
	void newAccount(const HdWalletSeed *rootSeed, uint16_t zone, const IWalletDataEncoder* encoder);

	WalletAccount* getAccount(int i) const noexcept {
		return this->accounts->get(i);
	}

	void save();
	void load(const IWalletDataEncoder* encoder);

private:
	File* baseDir;
	ArrayList<WalletAccount> *accounts;

	HdWalletSeed* encodedSeed;

	StatusStore* store;
};

} /* namespace codablecash */

#endif /* BC_WALLET_HDWALLET_H_ */
