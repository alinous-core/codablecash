/*
 * SmartcontractProjectData.cpp
 *
 *  Created on: Nov 27, 2025
 *      Author: iizuka
 */

#include "modular_project_registory/SmartcontractProjectData.h"
#include "modular_project_registory/ProjectIdKey.h"

#include "smartcontract_instance/ModularSmartcontractInstance.h"

#include "base_io/ByteBuffer.h"

#include "base/StackRelease.h"


namespace codablecash {

SmartcontractProjectData::SmartcontractProjectData(const SmartcontractProjectData &inst) {
	this->projectData = inst.projectData != nullptr ? inst.projectData->clone() : nullptr;
	this->key = inst.key != nullptr ? dynamic_cast<ProjectIdKey*>(inst.key->clone()) : nullptr;
}

SmartcontractProjectData::SmartcontractProjectData() {
	this->projectData = nullptr;
	this->key = nullptr;
}

SmartcontractProjectData::~SmartcontractProjectData() {
	delete this->projectData;
	delete this->key;
}

int SmartcontractProjectData::binarySize() const {
	int total = sizeof(uint64_t);
	total += this->projectData->capacity();

	return total;
}

void SmartcontractProjectData::toBinary(ByteBuffer *out) const {
	uint64_t cap = this->projectData->capacity();
	out->putLong(cap);

	out->put(this->projectData);
}

SmartcontractProjectData* SmartcontractProjectData::createFromBinary(ByteBuffer *in) {
	SmartcontractProjectData* inst = new SmartcontractProjectData(); __STP(inst);

	uint64_t length = in->getLong();

	uint8_t* b = new uint8_t[length];
	StackArrayRelease<uint8_t> __stp(b);

	in->get(b, length);

	inst->projectData = ByteBuffer::wrapWithEndian(b, length, true);

	return __STP_MV(inst);
}

void SmartcontractProjectData::setData(const ByteBuffer *data) {
	delete this->projectData;
	this->projectData = data->clone();
}

void SmartcontractProjectData::setKey(const ProjectIdKey *key) {
	delete this->key;
	this->key = dynamic_cast<ProjectIdKey*>(key->clone());
}

IBlockObject* SmartcontractProjectData::copyData() const noexcept {
	return new SmartcontractProjectData(*this);
}

ModularSmartcontractInstance* SmartcontractProjectData::toSmartcontractInstance() const {
	this->projectData->position(0);
	ModularSmartcontractInstance* instance = ModularSmartcontractInstance::createFromBinary(this->projectData);

	return instance;
}

} /* namespace codablecash */
