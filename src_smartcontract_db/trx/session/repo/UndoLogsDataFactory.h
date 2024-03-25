/*
 * UndoLogsDataFactory.h
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGSDATAFACTORY_H_
#define TRX_SESSION_REPO_UNDOLOGSDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class UndoLogsDataFactory : public AbstractBtreeDataFactory {
public:
	UndoLogsDataFactory();
	virtual ~UndoLogsDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGSDATAFACTORY_H_ */
