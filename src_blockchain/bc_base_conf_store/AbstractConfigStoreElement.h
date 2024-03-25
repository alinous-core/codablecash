/*
 * AbstractConfigStoreElement.h
 *
 *  Created on: 2022/05/14
 *      Author: iizuka
 */

#ifndef BC_BASE_CONF_STORE_ABSTRACTCONFIGSTOREELEMENT_H_
#define BC_BASE_CONF_STORE_ABSTRACTCONFIGSTOREELEMENT_H_

#include <cstdint>

namespace alinous {
class ByteBuffer;
}
using namespace alinous;


namespace codablecash {

class AbstractConfigStoreElement {
public:
	static const constexpr uint8_t TYPE_LONG_VALUE{1};
	static const constexpr uint8_t TYPE_SHORT_VALUE{2};
	static const constexpr uint8_t TYPE_BINARY_VALUE{10};

	explicit AbstractConfigStoreElement(uint8_t type);
	virtual ~AbstractConfigStoreElement();

	virtual int binarySize() const noexcept = 0;
	virtual void toBinary(ByteBuffer* out) const = 0;
	virtual void fromBinary(ByteBuffer* in) = 0;

	static AbstractConfigStoreElement* createFromBinary(ByteBuffer* in);

protected:
	uint8_t type;
};

} /* namespace codablecash */

#endif /* BC_BASE_CONF_STORE_ABSTRACTCONFIGSTOREELEMENT_H_ */
