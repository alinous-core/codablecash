/*
 * P2pNodeRecordDataFactory.h
 *
 *  Created on: 2023/08/11
 *      Author: iizuka
 */

#ifndef BC_P2P_INFO_P2PNODERECORDDATAFACTORY_H_
#define BC_P2P_INFO_P2PNODERECORDDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class P2pNodeRecordDataFactory : public AbstractBtreeDataFactory {
public:
	P2pNodeRecordDataFactory();
	virtual ~P2pNodeRecordDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);
	virtual AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_P2P_INFO_P2PNODERECORDDATAFACTORY_H_ */
