/*
 * IBlockchainStoreProvider.h
 *
 *  Created on: Mar 28, 2025
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_IBLOCKCHAINSTOREPROVIDER_H_
#define BC_STATUS_CACHE_IBLOCKCHAINSTOREPROVIDER_H_

#include <cstdint>

namespace codablecash {

class BlockHeaderStoreManager;
class BlockBodyStoreManager;

class IBlockchainStoreProvider {
public:
	IBlockchainStoreProvider();
	virtual ~IBlockchainStoreProvider();

	virtual BlockHeaderStoreManager* getHeaderManager(uint16_t zone) const noexcept = 0;
	virtual BlockBodyStoreManager* getBlockBodyStoreManager(uint16_t zone) const noexcept = 0;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_IBLOCKCHAINSTOREPROVIDER_H_ */
