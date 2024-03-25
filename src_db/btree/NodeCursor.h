/*
 * NodeCursor.h
 *
 *  Created on: 2018/12/26
 *      Author: iizuka
 */

#ifndef BTREE_NODECURSOR_H_
#define BTREE_NODECURSOR_H_

#include "base/ArrayList.h"
#include "base/RawArrayPrimitive.h"

#include <cstdint>

namespace alinous {

class NodeHandle;
class BtreeStorage;
class AbstractBtreeKey;
class IBlockObject;
class AbstractTreeNode;
class AbstractBtreeKey;
class NodePosition;
class TreeNode;

class NodeCursor {
public:
	NodeCursor(const NodeCursor& inst) = delete;
	NodeCursor(NodeHandle* rootNode, BtreeStorage* store, int nodeNumber);
	virtual ~NodeCursor();

	NodePosition* pop() noexcept;
	void push(NodePosition* node) noexcept;
	NodePosition* top() noexcept;

	void insert(const AbstractBtreeKey* key, const IBlockObject* data);

	const AbstractBtreeKey* getCurrentKey();

	IBlockObject* gotoFirst();
	IBlockObject* getNext();
	IBlockObject* gotoKey(const AbstractBtreeKey* key);
	NodePosition* gotoLeaf(const AbstractBtreeKey* key);

	IBlockObject* gotoLast();
	IBlockObject* getPrevious();
	IBlockObject* gotoKeyPrevious(const AbstractBtreeKey* key);
	NodePosition* gotoLeafPrevious(const AbstractBtreeKey* key);

	IBlockObject* find(const AbstractBtreeKey* key);
	bool remove(const AbstractBtreeKey* key);

	static void checkIsDataNode(NodeHandle* nodeHandle, const char* srcfile, int srcline);
private:
	void internalRemoveFromBottomToUpper();
	void internalRemoveRoot();

	void setKeyForSelfAndDescendants(NodePosition* current, const AbstractBtreeKey* key);

	// void setKeyForSelfAndDescendants(NodeHandle* nh, const AbstractBtreeKey* key, BtreeStorage* store);

	void splitLeafNode(const AbstractBtreeKey* key, const IBlockObject* data);
	AbstractBtreeKey* setupTwoLists(ArrayList<NodeHandle>* list, AbstractTreeNode* node,
			RawArrayPrimitive<uint64_t>* list1, RawArrayPrimitive<uint64_t>* list2);
	void createNewRoot(TreeNode* newNode);
	void addToParent(TreeNode* newNode);
	void splitTreeNode(TreeNode* newNode);

private:
	ArrayList<NodePosition>* nodestack;
	int nodeNumber; // max inner nodes number in a node
	BtreeStorage* store;

#ifdef __DEBUG__
public:
	void assertTree();
	void assertCurrent();

#endif
};

#ifdef __DEBUG__
#define __ASSERT_TREE assertTree();
#define __ASSERT_TREE_INSERT assertTreeInsert();
#else
#define __ASSERT_TREE ;
#define __ASSERT_TREE_INSERT ;
#endif

} /* namespace alinous */

#endif /* BTREE_NODECURSOR_H_ */
