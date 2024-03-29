/*
 * AbstractTreeNode.cpp
 *
 *  Created on: 2018/12/18
 *      Author: iizuka
 */

#include "btreekey/BtreeKeyFactory.h"
#include "btree/AbstractTreeNode.h"
#include "btree/AbstractBtreeKey.h"
#include "btree/exceptions.h"

#include "btree/TreeNode.h"
#include "btree/DataNode.h"

#include "base_io/ReverseByteBuffer.h"

namespace alinous {

AbstractTreeNode::AbstractTreeNode(AbstractBtreeKey* key) : IBlockObject() {
	this->key = key;
	this->fpos = 0;
}

AbstractTreeNode::~AbstractTreeNode() {
	delete this->key;
}

int AbstractTreeNode::binarySize() const {
	int size = this->key->binarySize();

	size += sizeof(this->fpos);

	return size;
}

void AbstractTreeNode::toBinary(ByteBuffer* out) const {
	this->key->toBinary(out);
	out->putLong(this->fpos);
}

void AbstractTreeNode::fromBinaryAbstract(ByteBuffer* in, BtreeKeyFactory* factory) {
	int32_t keytype = in->getInt();
	this->key = factory->fromBinary(keytype, in);

	this->fpos = in->getLong();
}

DataNode* AbstractTreeNode::toDataNode(AbstractTreeNode* node) {
	if(!node->isData()){
		throw new NodeStructureException(__FILE__, __LINE__);
	}
	return dynamic_cast<DataNode*>(node);
}

TreeNode* AbstractTreeNode::toTreeNode(AbstractTreeNode* node) {
	if(node->isData()){
		throw new NodeStructureException(__FILE__, __LINE__);
	}
	return dynamic_cast<TreeNode*>(node);
}

void AbstractTreeNode::setFpos(uint64_t fpos) noexcept  {
	assert(fpos > 0);
	this->fpos = fpos;
}

void AbstractTreeNode::setKey(const AbstractBtreeKey *key) noexcept {
	delete this->key;
	this->key = key->clone();
}

} /* namespace alinous */

