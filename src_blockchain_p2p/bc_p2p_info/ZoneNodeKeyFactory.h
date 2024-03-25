/*
 * ZoneNodeKeyFactory.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_INFO_ZONENODEKEYFACTORY_H_
#define BC_P2P_INFO_ZONENODEKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class ZoneNodeKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t ZONE_NODE_KEY{0x14};

	ZoneNodeKeyFactory();
	virtual ~ZoneNodeKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_P2P_INFO_ZONENODEKEYFACTORY_H_ */
