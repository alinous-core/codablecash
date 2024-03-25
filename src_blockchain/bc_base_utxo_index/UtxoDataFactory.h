/*
 * UtxoDataFactory.h
 *
 *  Created on: 2023/05/09
 *      Author: iizuka
 */

#ifndef BC_BASE_UTXO_INDEX_UTXODATAFACTORY_H_
#define BC_BASE_UTXO_INDEX_UTXODATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class UtxoDataFactory : public AbstractBtreeDataFactory {
public:
	UtxoDataFactory();
	virtual ~UtxoDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	virtual void registerData(const AbstractBtreeKey* key, const IBlockObject* data, DataNode* dataNode, BtreeStorage* store) const;
	virtual bool beforeRemove(DataNode* dataNode, BtreeStorage* store, const AbstractBtreeKey* key) const;

	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BASE_UTXO_INDEX_UTXODATAFACTORY_H_ */
