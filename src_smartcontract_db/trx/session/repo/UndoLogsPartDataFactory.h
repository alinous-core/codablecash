/*
 * UndoLogsPartDataFactory.h
 *
 *  Created on: 2023/07/10
 *      Author: iizuka
 */

#ifndef TRX_SESSION_REPO_UNDOLOGSPARTDATAFACTORY_H_
#define TRX_SESSION_REPO_UNDOLOGSPARTDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class UndoLogsPartDataFactory : public AbstractBtreeDataFactory {
public:
	UndoLogsPartDataFactory();
	virtual ~UndoLogsPartDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* TRX_SESSION_REPO_UNDOLOGSPARTDATAFACTORY_H_ */
