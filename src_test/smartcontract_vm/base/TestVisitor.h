/*
 * TestVisitor.h
 *
 *  Created on: Oct 30, 2025
 *      Author: iizuka
 */

#ifndef SMARTCONTRACT_VM_BASE_TESTVISITOR_H_
#define SMARTCONTRACT_VM_BASE_TESTVISITOR_H_

#include "lang/sc_declare_types/ITypeVisitor.h"

namespace alinous {

class TestVisitor : public ITypeVisitor {
public:
	TestVisitor();
	virtual ~TestVisitor();

	virtual void visit(AbstractType* type);
	virtual void exit(AbstractType* type);

	int getCount() const noexcept {
		return this->count;
	}

private:
	int count;
};

} /* namespace alinous */

#endif /* SMARTCONTRACT_VM_BASE_TESTVISITOR_H_ */
