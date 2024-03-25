/*
 * AddressDescriptorUtxoDataFactory.h
 *
 *  Created on: 2023/02/23
 *      Author: iizuka
 */

#ifndef BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORUTXODATAFACTORY_H_
#define BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORUTXODATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class AddressDescriptorUtxoDataFactory : public AbstractBtreeDataFactory {
public:
	AddressDescriptorUtxoDataFactory();
	virtual ~AddressDescriptorUtxoDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;
	virtual bool beforeRemove(DataNode* dataNode, BtreeStorage* store, const AbstractBtreeKey* key) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_WALLET_TRX_REPO_ADDRESSDESCRIPTORUTXODATAFACTORY_H_ */
