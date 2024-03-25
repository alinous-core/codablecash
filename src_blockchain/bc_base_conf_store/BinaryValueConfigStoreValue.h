/*
 * BinaryValue.h
 *
 *  Created on: 2023/02/24
 *      Author: iizuka
 */

#ifndef BC_BASE_CONF_STORE_BINARYVALUECONFIGSTOREVALUE_H_
#define BC_BASE_CONF_STORE_BINARYVALUECONFIGSTOREVALUE_H_

#include "bc_base_conf_store/AbstractConfigStoreElement.h"

namespace codablecash {

class BinaryValueConfigStoreValue : public AbstractConfigStoreElement {
public:
	BinaryValueConfigStoreValue();
	virtual ~BinaryValueConfigStoreValue();

	void init(const uint8_t *data, int length);

	virtual int binarySize() const noexcept;
	virtual void toBinary(ByteBuffer* out) const;
	virtual void fromBinary(ByteBuffer* in);

	uint16_t getLength() const noexcept {
		return this->length;
	}
	const char* getData() const noexcept {
		return this->data;
	}

private:
	char* data;
	uint16_t length;
};

} /* namespace codablecash */

#endif /* BC_BASE_CONF_STORE_BINARYVALUECONFIGSTOREVALUE_H_ */
