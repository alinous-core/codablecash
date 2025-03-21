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

class StakingSeedManager {
public:
	static const constexpr wchar_t* STAKING_SEGMENT{L"staking_seed"};

	explicit StakingSeedManager(const File* baseDir);
	virtual ~StakingSeedManager();

private:
	File* storeDir;
};

} /* namespace codablecash */

#endif /* BC_WALLET_NET_STAKINGSEEDMANAGER_H_ */
