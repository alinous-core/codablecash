/*
 * BloomFilter512.h
 *
 *  Created on: Mar 30, 2025
 *      Author: iizuka
 */

#ifndef BC_WALLET_FILTER_BLOOMFILTER512_H_
#define BC_WALLET_FILTER_BLOOMFILTER512_H_

#include "bc_wallet_filter/BloomFilter.h"

#include "filestore_block/IBlockObject.h"


namespace codablecash {

class BalanceAddress;
class AddressDescriptor;

class BloomFilter512 : public BloomFilter, public IBlockObject {
public:
	BloomFilter512(const BloomFilter512& inst);
	BloomFilter512();
	virtual ~BloomFilter512();

	void add(const BalanceAddress* address);
	void add(const AddressDescriptor* addressDesc);

	bool checkBytes(const BalanceAddress* address);
	bool checkBytes(const AddressDescriptor* addressDesc);

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static BloomFilter512* createFromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_WALLET_FILTER_BLOOMFILTER512_H_ */
