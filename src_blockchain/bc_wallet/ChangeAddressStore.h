/*
 * ChangeAddressStore.h
 *
 *  Created on: 2023/01/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_CHANGEADDRESSSTORE_H_
#define BC_WALLET_CHANGEADDRESSSTORE_H_

#include "bc_wallet/AbstractAddressStore.h"

namespace codablecash {

class ChangeAddressStore : public AbstractAddressStore {
public:
	static const constexpr wchar_t* STORE_NAME{L"ChangeAddressStore"};
	static const constexpr wchar_t* KEY_CURRENT_INDEX{L"currentIndex"};
	static const constexpr wchar_t* KEY_NUM_GROUP{L"numgroup"};


	explicit ChangeAddressStore(uint16_t zone, int numAddressInThisGroup, const File* baseDir);
	virtual ~ChangeAddressStore();

	void init(const IWalletDataEncoder *encoder);

	AddressDescriptor* getNextChangeAddress(const IWalletDataEncoder *encoder) noexcept;
	void loadAllChangeAddresses(const IWalletDataEncoder *encoder);

	virtual void save();
	virtual void load(const IWalletDataEncoder *encoder);

private:
	int currentIndex;
	int numAddressInThisGroup;
};

} /* namespace codablecash */

#endif /* BC_WALLET_CHANGEADDRESSSTORE_H_ */
