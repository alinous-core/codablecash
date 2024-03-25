/*
 * NodeIdentifierListDataFactory.h
 *
 *  Created on: 2023/09/07
 *      Author: iizuka
 */

#ifndef BC_NETWORK_NODEIDENTIFIERLISTDATAFACTORY_H_
#define BC_NETWORK_NODEIDENTIFIERLISTDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class NodeIdentifierDataFactory : public AbstractBtreeDataFactory {
public:
	NodeIdentifierDataFactory();
	virtual ~NodeIdentifierDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_NETWORK_NODEIDENTIFIERLISTDATAFACTORY_H_ */
