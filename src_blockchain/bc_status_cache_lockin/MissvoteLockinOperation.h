/*
 * MissvoteLockonOperation.h
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_MISSVOTELOCKINOPERATION_H_
#define BC_STATUS_CACHE_LOCKIN_MISSVOTELOCKINOPERATION_H_

#include "bc_status_cache_lockin/AbstractLockinOperation.h"

namespace codablecash {

class MissvoteLockinOperation : public AbstractLockinOperation {
public:
	MissvoteLockinOperation();
	virtual ~MissvoteLockinOperation();
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_MISSVOTELOCKINOPERATION_H_ */
