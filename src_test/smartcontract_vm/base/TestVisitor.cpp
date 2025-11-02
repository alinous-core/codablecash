/*
 * TestVisitor.cpp
 *
 *  Created on: Oct 30, 2025
 *      Author: iizuka
 */

#include "TestVisitor.h"

namespace alinous {

TestVisitor::TestVisitor() {
	this->count = 0;
}

TestVisitor::~TestVisitor() {

}

void TestVisitor::visit(AbstractType *type) {
	this->count++;
}

void TestVisitor::exit(AbstractType *type) {
	this->count--;
}

} /* namespace alinous */
