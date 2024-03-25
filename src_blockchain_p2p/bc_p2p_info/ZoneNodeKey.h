/*
 * ZoneNodeKey.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_P2P_INFO_ZONENODEKEY_H_
#define BC_P2P_INFO_ZONENODEKEY_H_

#include <cstdint>

#include "btree/AbstractBtreeKey.h"

using namespace alinous;

namespace codablecash {

class NodeIdentifierKey;

class ZoneNodeKey: public alinous::AbstractBtreeKey {
public:
	ZoneNodeKey(const ZoneNodeKey& inst);
	ZoneNodeKey();
	virtual ~ZoneNodeKey();

	void init(uint16_t zone, const NodeIdentifierKey* key) noexcept;

	virtual bool isInfinity() const { return false; }
	virtual bool isNull() const { return false; }

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static ZoneNodeKey* fromBinary(ByteBuffer* in);

	virtual int compareTo(const AbstractBtreeKey* key) const noexcept;
	virtual AbstractBtreeKey* clone() const noexcept;

	uint16_t getZone() const noexcept {
		return this->zone;
	}

private:
	uint16_t zone;
	NodeIdentifierKey* key;
};

} /* namespace codablecash */

#endif /* BC_P2P_INFO_ZONENODEKEY_H_ */
