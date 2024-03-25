/*
 * AddressDescriptorData.h
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#ifndef BC_BASE_UTXO_INDEX_ADDRESSDESCRIPTORDATA_H_
#define BC_BASE_UTXO_INDEX_ADDRESSDESCRIPTORDATA_H_

#include "filestore_block/IBlockObject.h"

using namespace alinous;

namespace codablecash {

class AddressDescriptor;

class AddressDescriptorData : public IBlockObject {
public:
	explicit AddressDescriptorData(const AddressDescriptor* desc);
	virtual ~AddressDescriptorData();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static AddressDescriptorData* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	const AddressDescriptor* getDescriptor() const noexcept {
		return this->desc;
	}

private:
	AddressDescriptor* desc;
};

} /* namespace codablecash */

#endif /* BC_BASE_UTXO_INDEX_ADDRESSDESCRIPTORDATA_H_ */
