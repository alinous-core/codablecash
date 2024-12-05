/*
 * ILockinManager.h
 *
 *  Created on: Oct 13, 2024
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_LOCKIN_ILOCKINMANAGER_H_
#define BC_STATUS_CACHE_LOCKIN_ILOCKINMANAGER_H_

#include <cstdint>

namespace codablecash {

class AbstractLockinOperation;
class LockInOperationData;

class ILockinManager {
public:
	ILockinManager();
	virtual ~ILockinManager();

	virtual uint64_t getFinalizingHeight() const noexcept = 0;
	virtual void addOperation(uint64_t height, const AbstractLockinOperation* op) = 0;
	virtual LockInOperationData* getOperantions(uint64_t height) = 0;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_LOCKIN_ILOCKINMANAGER_H_ */
