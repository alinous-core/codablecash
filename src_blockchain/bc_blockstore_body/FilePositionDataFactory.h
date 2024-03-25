/*
 * FilePositionDataFactory.h
 *
 *  Created on: 2023/03/02
 *      Author: iizuka
 */

#ifndef BC_BLOCKSTORE_BODY_FILEPOSITIONDATAFACTORY_H_
#define BC_BLOCKSTORE_BODY_FILEPOSITIONDATAFACTORY_H_

#include "btree/AbstractBtreeDataFactory.h"

using namespace alinous;

namespace codablecash {

class FilePositionDataFactory : public AbstractBtreeDataFactory {
public:
	FilePositionDataFactory();
	virtual ~FilePositionDataFactory();

	virtual IBlockObject* makeDataFromBinary(ByteBuffer* in);

	AbstractBtreeDataFactory* copy() const noexcept;
};

} /* namespace codablecash */

#endif /* BC_BLOCKSTORE_BODY_FILEPOSITIONDATAFACTORY_H_ */
