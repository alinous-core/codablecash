/*
 * InstanceSpace.h
 *
 *  Created on: Nov 28, 2025
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_CACHE_INSTANCESPACE_H_
#define SMARTCONTRACT_CACHE_INSTANCESPACE_H_

namespace codablecash {

class ModularSmartcontractInstance;

class InstanceSpace {
public:
	InstanceSpace();
	virtual ~InstanceSpace();

private:
	ModularSmartcontractInstance* instance;
};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_CACHE_INSTANCESPACE_H_ */
