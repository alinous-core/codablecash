/*
 * RegisterSmartcontractModuleTransaction.h
 *
 *  Created on: Oct 2, 2025
 *      Author: iizuka
 */

#ifndef TRANSACTION_REGISTERSMARTCONTRACTMODULETRANSACTION_H_
#define TRANSACTION_REGISTERSMARTCONTRACTMODULETRANSACTION_H_

#include "transaction/AbstractSmartcontractTransaction.h"

namespace codablecash {

class RegisterSmartcontractModuleTransaction : public AbstractSmartcontractTransaction {
public:
	RegisterSmartcontractModuleTransaction();
	virtual ~RegisterSmartcontractModuleTransaction();
};

} /* namespace codablecash */

#endif /* TRANSACTION_REGISTERSMARTCONTRACTMODULETRANSACTION_H_ */
