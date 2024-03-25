/*
 * AbstractMerkleElement.cpp
 *
 *  Created on: 2022/03/21
 *      Author: iizuka
 */

#include "merkletree/AbstractMerkleElement.h"

#include "base_io/ByteBuffer.h"

namespace codablecash {

AbstractMerkleElement::AbstractMerkleElement() {
	this->hash = nullptr;
	this->parent = nullptr;
	this->children = new ArrayList<AbstractMerkleElement>();
}

AbstractMerkleElement::~AbstractMerkleElement() {
	delete this->hash;

	this->parent = nullptr;
	this->children->deleteElements();
	delete this->children;
}

void AbstractMerkleElement::setHash(const ByteBuffer* b) noexcept {
	this->hash = b->clone();
}

void AbstractMerkleElement::addChild(AbstractMerkleElement* child) noexcept {
	child->setParent(this);
	this->children->addElement(child);
}

int AbstractMerkleElement::hashSize() const noexcept {
	return this->hash != nullptr ? this->hash->limit() : 0;
}

bool AbstractMerkleElement::isLeaf() const noexcept {
	return this->children->isEmpty();
}

bool AbstractMerkleElement::isRoot() const noexcept {
	return this->parent == nullptr;
}

const AbstractMerkleElement* AbstractMerkleElement::getAnotherPair() const noexcept {
	int idx = isLeft() ? 1 : 0;

	AbstractMerkleElement* element = this->parent->children->get(idx);
	return element;
}

bool AbstractMerkleElement::isLeft() const noexcept {
	AbstractMerkleElement* leftElement = this->parent->children->get(0);

	return this == leftElement;
}

} /* namespace codablecash */
