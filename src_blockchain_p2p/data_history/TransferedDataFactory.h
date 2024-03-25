/*
 * TransferedDataFactory.h
 *
 *  Created on: 2023/09/18
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_TRANSFEREDDATAFACTORY_H_
#define DATA_HISTORY_TRANSFEREDDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class TransferedDataFactory : public AbstractBtreeDataFactory {
public:
	TransferedDataFactory();
	virtual ~TransferedDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_TRANSFEREDDATAFACTORY_H_ */
