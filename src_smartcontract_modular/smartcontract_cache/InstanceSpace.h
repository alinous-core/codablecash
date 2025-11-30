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
class SmartcontractInstanceAddress;

class InstanceSpace {
public:
	InstanceSpace(const SmartcontractInstanceAddress *instAddress, ModularSmartcontractInstance* instance);
	virtual ~InstanceSpace();

	bool isDeletable() const noexcept;
	void beforeCacheOut();

	const SmartcontractInstanceAddress* getSmartContractInstanceAddress() const noexcept;



private:
	ModularSmartcontractInstance* instance;

	int ref;

};

} /* namespace codablecash */

#endif /* SMARTCONTRACT_CACHE_INSTANCESPACE_H_ */
