/*
 * IRetryableResponceValidator.h
 *
 *  Created on: 2023/11/19
 *      Author: iizuka
 */

#ifndef BC_P2P_CMD_NODE_IRETRYABLERESPONCEVALIDATOR_H_
#define BC_P2P_CMD_NODE_IRETRYABLERESPONCEVALIDATOR_H_

namespace codablecash {

class AbstractCommandResponse;

class IRetryableResponceValidator {
public:
	IRetryableResponceValidator();
	virtual ~IRetryableResponceValidator();

	virtual bool validate(AbstractCommandResponse* response) = 0;
};

} /* namespace codablecash */

#endif /* BC_P2P_CMD_NODE_IRETRYABLERESPONCEVALIDATOR_H_ */
