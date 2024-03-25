/*
 * IMuSigSignerProvidor.h
 *
 *  Created on: 2023/02/22
 *      Author: iizuka
 */

#ifndef BC_TRX_BALANCE_IMUSIGSIGNERPROVIDOR_H_
#define BC_TRX_BALANCE_IMUSIGSIGNERPROVIDOR_H_

namespace codablecash {

class IMuSigSigner;
class AddressDescriptor;

class IMuSigSignerProvidor {
public:
	IMuSigSignerProvidor();
	virtual ~IMuSigSignerProvidor();

	virtual IMuSigSigner* getSigner(const AddressDescriptor* desc) = 0;
};

} /* namespace codablecash */

#endif /* BC_TRX_BALANCE_IMUSIGSIGNERPROVIDOR_H_ */
