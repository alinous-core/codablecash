/*
 * UtxoIdKeyFactory.h
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#ifndef BC_BASE_UTXO_INDEX_UTXOIDKEYFACTORY_H_
#define BC_BASE_UTXO_INDEX_UTXOIDKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class UtxoIdKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t UTXO_ID_KEY{0x14};

	UtxoIdKeyFactory();
	virtual ~UtxoIdKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BASE_UTXO_INDEX_UTXOIDKEYFACTORY_H_ */
