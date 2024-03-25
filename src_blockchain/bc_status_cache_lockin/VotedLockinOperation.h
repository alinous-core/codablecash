/*
 * VotedLockonOperation.h
 *
 *  Created on: 2024/02/11
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_VOTEDLOCKINOPERATION_H_
#define BC_STATUS_CACHE_LOCKIN_VOTEDLOCKINOPERATION_H_

#include "bc_status_cache_lockin/AbstractLockinOperation.h"

namespace codablecash {

class VotedLockinOperation : AbstractLockinOperation {
public:
	VotedLockinOperation();
	virtual ~VotedLockinOperation();
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_VOTEDLOCKINOPERATION_H_ */
