/*
 * BloomFilter512.h
 *
 *  Created on: Mar 30, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_FILTER_BLOOMFILTER1024_H_
#define BC_WALLET_FILTER_BLOOMFILTER1024_H_

#include "bc_wallet_filter/BloomFilter.h"

#include "filestore_block/IBlockObject.h"


namespace codablecash {

class BalanceAddress;
class AddressDescriptor;
class BloomHash1024;

class BloomFilter1024 : public BloomFilter, public IBlockObject {
public:
	BloomFilter1024(const BloomFilter1024& inst);
	BloomFilter1024();
	virtual ~BloomFilter1024();

	void add(const BalanceAddress* address);
	void add(const AddressDescriptor* addressDesc);

	bool checkBytes(const BalanceAddress* address);
	bool checkBytes(const AddressDescriptor* addressDesc);
	bool checkBytes(const BloomHash1024* hash);

	BloomHash1024* getHash(const AddressDescriptor* addressDesc) const;

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BloomFilter1024* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

};

} /* namespace codablecash */

#endif /* BC_WALLET_FILTER_BLOOMFILTER1024_H_ */
