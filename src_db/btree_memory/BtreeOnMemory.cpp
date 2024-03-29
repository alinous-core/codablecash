/*
 * BTreeOnMemory.cpp
 *
 *  Created on: 2020/08/11
 *      Author: iizuka
 */

#include "btree_memory/BtreeOnMemory.h"

#include "btreekey/BtreeKeyFactory.h"
#include "btreekey/InfinityKey.h"

#include "btree/AbstractBtreeDataFactory.h"
#include "btree/BtreeConfig.h"

#include "btree_memory/MemoryTreeNode.h"
#include "btree_memory/MemoryNodeHandle.h"
#include "btree_memory/MemoryNodeCursor.h"
#include "btree_memory/MemoryBtreeScanner.h"

#include "base/StackRelease.h"

namespace alinous {

BtreeOnMemory::BtreeOnMemory(BtreeConfig* config, const BtreeKeyFactory* factory) {
	this->config = config;
	this->factory = factory->copy();

	InfinityKey* infinityKey = new InfinityKey();
	this->rootNode = new MemoryTreeNode(true, config->nodeNumber, infinityKey, true);
}

BtreeOnMemory::~BtreeOnMemory() {
	delete this->rootNode;

	delete this->config;
	delete this->factory;
}

MemoryBtreeScanner* BtreeOnMemory::getScanner() {
	MemoryNodeHandle* rootNode = new MemoryNodeHandle(this->rootNode);
	MemoryNodeCursor* cursor = new MemoryNodeCursor(rootNode, this->config->nodeNumber, this);

	return new MemoryBtreeScanner(cursor);
}

void BtreeOnMemory::putData(const AbstractBtreeKey* key, const IBlockObject* data) {
	MemoryNodeHandle* rootNode = new MemoryNodeHandle(this->rootNode);

	MemoryNodeCursor cursor(rootNode, this->config->nodeNumber, this);
	cursor.insert(key, data->copyData());

}

const IBlockObject* BtreeOnMemory::findByKey(const AbstractBtreeKey* key) {
	MemoryNodeHandle* rootNode = new MemoryNodeHandle(this->rootNode);
	MemoryNodeCursor* cursor = new MemoryNodeCursor(rootNode, this->config->nodeNumber, this); __STP(cursor);

	return cursor->find(key);
}

void BtreeOnMemory::remove(const AbstractBtreeKey* key) {
	MemoryNodeHandle* rootNode = new MemoryNodeHandle(this->rootNode);
	MemoryNodeCursor* cursor = new MemoryNodeCursor(rootNode, this->config->nodeNumber, this); __STP(cursor);

	cursor->remove(key);
}

void BtreeOnMemory::setRoot(MemoryTreeNode* rootNode) noexcept {
	this->rootNode = rootNode;
}

} /* namespace alinous */
