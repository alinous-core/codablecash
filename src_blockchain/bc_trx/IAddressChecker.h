/*
 * IAddressChecker.h
 *
 *  Created on: Jul 24, 2025
 *      Author: iizuka
 */

#ifndef BC_TRX_IADDRESSCHECKER_H_
#define BC_TRX_IADDRESSCHECKER_H_

namespace codablecash {

class AddressDescriptor;


class IAddressChecker {
public:
	IAddressChecker();
	virtual ~IAddressChecker();

	virtual bool checkAddress(const AddressDescriptor* desc) const = 0;
};

} /* namespace codablecash */

#endif /* BC_TRX_IADDRESSCHECKER_H_ */
