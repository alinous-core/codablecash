/*
 * ShortValueConfigStoreValue.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_BASE_CONF_STORE_SHORTVALUECONFIGSTOREVALUE_H_
#define BC_BASE_CONF_STORE_SHORTVALUECONFIGSTOREVALUE_H_

#include "bc_base_conf_store/AbstractConfigStoreElement.h"

namespace codablecash {

class ShortValueConfigStoreValue : public AbstractConfigStoreElement {
public:
	ShortValueConfigStoreValue();
	explicit ShortValueConfigStoreValue(int16_t value);
	virtual ~ShortValueConfigStoreValue();

	virtual int binarySize() const noexcept;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	int16_t getValue() const noexcept {
		return value;
	}

private:
	int16_t value;
};

} /* namespace codablecash */

#endif /* BC_BASE_CONF_STORE_SHORTVALUECONFIGSTOREVALUE_H_ */
