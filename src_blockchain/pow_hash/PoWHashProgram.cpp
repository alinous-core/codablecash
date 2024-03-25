/*
 * PoWHashProgram.cpp
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#include "pow_hash/PoWHashProgram.h"

namespace codablecash {

PoWHashProgram::PoWHashProgram()
		: AbstractContainerHashAlgorithmElement() {

}

PoWHashProgram::~PoWHashProgram() {

}

void PoWHashProgram::calulate(const char *in01, const char *in02,	const char *in03, char *out) const noexcept {
	char* arrays[4];
	arrays[0] = const_cast<char*>(in01);
	arrays[1] = const_cast<char*>(in02);
	arrays[2] = const_cast<char*>(in03);
	arrays[3] = const_cast<char*>(out);

	int maxLoop = this->list->size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractHashAlgorithmElement* algo = this->list->get(i);
		algo->execute(arrays);
	}
}

} /* namespace codablecash */
