/*
 * TransferedDataIdKeyFactory.h
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_TRANSFEREDDATAIDKEYFACTORY_H_
#define DATA_HISTORY_TRANSFEREDDATAIDKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class TransferedDataIdKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t TRANSFERED_DATA_ID_KEY{0x14};

	TransferedDataIdKeyFactory();
	virtual ~TransferedDataIdKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_TRANSFEREDDATAIDKEYFACTORY_H_ */
