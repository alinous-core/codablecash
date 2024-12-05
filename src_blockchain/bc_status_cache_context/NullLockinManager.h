/*
 * NullLockinManager.h
 *
 *  Created on: Oct 13, 2024
 *      Author: iizuka
 */

#ifndef BC_STATUS_CACHE_CONTEXT_NULLLOCKINMANAGER_H_
#define BC_STATUS_CACHE_CONTEXT_NULLLOCKINMANAGER_H_

#include "bc_status_cache_lockin/ILockinManager.h"

namespace codablecash {

class LockinManager;

class NullLockinManager: public ILockinManager {
public:
	explicit NullLockinManager(LockinManager* lockinManager);
	virtual ~NullLockinManager();

	virtual uint64_t getFinalizingHeight() const noexcept;
	virtual void addOperation(uint64_t height, const AbstractLockinOperation* op);
	virtual LockInOperationData* getOperantions(uint64_t height);

private:
	LockinManager* lockinManage;
};

} /* namespace codablecash */

#endif /* BC_STATUS_CACHE_CONTEXT_NULLLOCKINMANAGER_H_ */
