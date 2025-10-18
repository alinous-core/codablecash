/*
 * DependencyConfig.cpp
 *
 *  Created on: Oct 8, 2025
 *      Author: iizuka
 */

#include "modular_project/DependencyConfig.h"
#include "modular_project/AbstractDependencyConfig.h"


namespace codablecash {

DependencyConfig::DependencyConfig() {
	this->list = new ArrayList<AbstractDependencyConfig>();

}

DependencyConfig::~DependencyConfig() {
	this->list->deleteElements();
	delete this->list;
}

void DependencyConfig::load(const JsonArrayObject *dependencies) {


}

} /* namespace codablecash */
