/*
 * ModularProxyClassDeclare.cpp
 *
 *  Created on: Dec 3, 2025
 *      Author: iizuka
 */

#include "inter_modular_access/ModularProxyClassDeclare.h"

#include "engine/sc_analyze/IVmInstanceFactory.h"


namespace codablecash {

ModularProxyClassDeclare::ModularProxyClassDeclare() {
	this->factory = nullptr;
	this->dctx = nullptr;
}

ModularProxyClassDeclare::~ModularProxyClassDeclare() {
	delete this->factory;
	this->dctx = nullptr;
}

IVmInstanceFactory* ModularProxyClassDeclare::getFactory() const noexcept {
	return this->factory;
}

void ModularProxyClassDeclare::setFactory(IVmInstanceFactory *factory) {
	this->factory = factory;
}

void ModularProxyClassDeclare::setDependencyContext(InstanceDependencyContext *dctx) noexcept {
	this->dctx;
}

} /* namespace codablecash */
