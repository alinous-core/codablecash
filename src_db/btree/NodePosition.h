/*
 * NodePosition.h
 *
 *  Created on: 2018/12/29
 *      Author: iizuka
 */

#ifndef BTREE_NODEPOSITION_H_
#define BTREE_NODEPOSITION_H_

#include "base/ArrayList.h"
#include "base/RawArrayPrimitive.h"

#include <cstdint>

namespace alinous {

class NodeHandle;
class AbstractBtreeKey;
class BtreeStorage;

class NodePosition {
public:
	explicit NodePosition(NodeHandle* nodeHandle);
	~NodePosition();

	void clearCache();
	bool isEmpty() const noexcept;
	int getInnerCount() const noexcept{return this->innerCount;};
	uint64_t getChildFpos(int i) const noexcept;

	const AbstractBtreeKey* getKey() const noexcept;
	void setKey(const AbstractBtreeKey* key) noexcept;

	bool isLeaf() const;
	bool isRoot() const;
	//bool isData() const;
	void setRoot(bool isroot);
	NodeHandle* hasKey(const AbstractBtreeKey* key) const;
	bool isFull(int nodeNumber) const noexcept;
	uint64_t getFpos() const noexcept;
	NodeHandle* getNodeHandle() const noexcept;

	ArrayList<NodeHandle>* getInnerNodes() const noexcept;
	void loadInnerNodes(BtreeStorage* store);
	void setLastPos();
	void addNode(const AbstractBtreeKey* key, uint64_t fpos, int nodeNumber);

	uint64_t getNextChildPrevious(const AbstractBtreeKey* key);
	uint64_t getNextChild(const AbstractBtreeKey* key);
	int indexof(const AbstractBtreeKey* key) const;

	void save(BtreeStorage* store);
	void updateInnerNodeFpos(const RawArrayPrimitive<uint64_t>* newlist);

	uint64_t previousData();
	uint64_t previousNode();
	bool hasPrevious();

	uint64_t nextData();
	uint64_t nextNode();
	bool hasNext();

	const NodeHandle* gotoEqLessThanKey(const AbstractBtreeKey* key);
	const NodeHandle* gotoEqMoreThanKey(const AbstractBtreeKey* key);
	const NodeHandle* gotoEqKey(const AbstractBtreeKey* key);

	bool removeChildNode(const AbstractBtreeKey* key, BtreeStorage* store);

	static void checkNoNull(NodeHandle* nodeHandle, const char* srcfile, int srcline) noexcept(false);

#ifdef __DEBUG__
	void assertTopRightNode();
#endif

private:
	void internalAddNode(int index, uint64_t fpos);
	void internalRemoveLeafChildNode(int index, BtreeStorage* store, const AbstractBtreeKey* key);
	void internalRemoveChildNode(int index, BtreeStorage* store);
	void removeInnerNodeFpos(int index) noexcept;


private:
	int pos;
	NodeHandle* node;
	ArrayList<NodeHandle>* innerNodes;
	int innerCount;
};

} /* namespace alinous */

#endif /* BTREE_NODEPOSITION_H_ */
