/*
 * SystemTimestampKeyFactory.h
 *
 *  Created on: Jun 7, 2026
 *      Author: iizuka
 */

#ifndef BASE_TIMESTAMP_SYSTEMTIMESTAMPKEYFACTORY_H_
#define BASE_TIMESTAMP_SYSTEMTIMESTAMPKEYFACTORY_H_

#include "btreekey/BtreeKeyFactory.h"

using namespace alinous;

namespace codablecash {

class SystemTimestampKeyFactory : public BtreeKeyFactory {
public:
	static const constexpr uint32_t SYSTEM_TIMESTAMP_KEY{0x14};

	SystemTimestampKeyFactory();
	virtual ~SystemTimestampKeyFactory();

	virtual AbstractBtreeKey* fromBinary(uint32_t keyType, ByteBuffer* in) const;
	virtual BtreeKeyFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BASE_TIMESTAMP_SYSTEMTIMESTAMPKEYFACTORY_H_ */
