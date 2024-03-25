/*
 * TransferedDataIdList.h
 *
 *  Created on: 2023/09/19
 *      Author: iizuka
 */

#ifndef DATA_HISTORY_TRANSFEREDDATAIDLIST_H_
#define DATA_HISTORY_TRANSFEREDDATAIDLIST_H_

#include "filestore_block/IBlockObject.h"

#include "base/ArrayList.h"

using namespace alinous;

namespace codablecash {

class TransferedDataId;

class TransferedDataIdList : public IBlockObject {
public:
	TransferedDataIdList(const TransferedDataIdList& inst);
	TransferedDataIdList();
	virtual ~TransferedDataIdList();

	virtual int binarySize() const;
	virtual void toBinary(ByteBuffer* out) const;
	static TransferedDataIdList* fromBinary(ByteBuffer* in);

	virtual IBlockObject* copyData() const noexcept;

	void add(const TransferedDataId* dataId) noexcept;
	void join(const TransferedDataIdList* list) noexcept;
	bool contains(const TransferedDataId* value) const noexcept;
	void remove(const TransferedDataId* value) noexcept;
	int indexof(const TransferedDataId* value) const noexcept;
	bool isEmpty() const noexcept;

private:
	ArrayList<TransferedDataId>* list;
};

} /* namespace codablecash */

#endif /* DATA_HISTORY_TRANSFEREDDATAIDLIST_H_ */
