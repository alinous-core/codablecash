/*
 * LongValueConfigStoreValue.h
 *
 *  Created on: 2022/05/14
 *      Author: iizuka
 */

#ifndef BC_BASE_CONF_STORE_LONGVALUECONFIGSTOREVALUE_H_
#define BC_BASE_CONF_STORE_LONGVALUECONFIGSTOREVALUE_H_

#include "bc_base_conf_store/AbstractConfigStoreElement.h"

namespace codablecash {

class LongValueConfigStoreValue : public AbstractConfigStoreElement {
public:
	LongValueConfigStoreValue();
	explicit LongValueConfigStoreValue(uint64_t value);
	virtual ~LongValueConfigStoreValue();

	virtual int binarySize() const noexcept;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	uint64_t getValue() const noexcept {
		return value;
	}

private:
	uint64_t value;
};

} /* namespace codablecash */

#endif /* BC_BASE_CONF_STORE_LONGVALUECONFIGSTOREVALUE_H_ */
