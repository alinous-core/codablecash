/*
 * BtreeReverseScanner.h
 *
 *  Created on: 2023/02/28
 *      Author: iizuka
 */

#ifndef BTREE_BTREEREVERSESCANNER_H_
#define BTREE_BTREEREVERSESCANNER_H_

namespace alinous {

class NodeCursor;
class IBlockObject;
class AbstractBtreeKey;

class BtreeReverseScanner {
public:
	BtreeReverseScanner(const BtreeReverseScanner& inst) = delete;
	explicit BtreeReverseScanner(NodeCursor* cursor);
	virtual ~BtreeReverseScanner();

	virtual void begin();
	virtual void begin(const AbstractBtreeKey* key);
	virtual bool hasPrevious();

	virtual const IBlockObject* previous();
	virtual const AbstractBtreeKey* previousKey();

private:
	NodeCursor* cursor;
	IBlockObject* nextObj;
	bool initialized;
	AbstractBtreeKey* key;
};

} /* namespace codablecash */

#endif /* BTREE_BTREEREVERSESCANNER_H_ */
