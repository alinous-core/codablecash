/*
 * VmObjectClassInstance.cpp
 *
 *  Created on: Nov 2, 2025
 *      Author: iizuka
 */

#include "instance/reserved_classes/object/VmObjectClassInstance.h"

namespace alinous {

VmObjectClassInstance::VmObjectClassInstance(AnalyzedClass* clazz, VirtualMachine* v)
		: AbstractVmReservedInstance(clazz, v) {
	// TODO Auto-generated constructor stub

}

VmObjectClassInstance::~VmObjectClassInstance() {

}

} /* namespace alinous */
