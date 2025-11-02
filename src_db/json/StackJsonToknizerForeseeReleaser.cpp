/*
 * StackJsonToknizerForeseeReleaser.cpp
 *
 *  Created on: Sep 27, 2025
 *      Author: iizuka
 */

#include "json/StackJsonToknizerForeseeReleaser.h"
#include "json/JsonTokenizer.h"

namespace codablecash {

StackJsonToknizerForeseeReleaser::StackJsonToknizerForeseeReleaser(JsonTokenizer* tokenizer, bool consume) {
	this->tokenizer = tokenizer;
	this->consume = consume;

	if(!consume){
		tokenizer->beginForesee();
	}
}

StackJsonToknizerForeseeReleaser::~StackJsonToknizerForeseeReleaser() {
	if(!this->consume){
		this->tokenizer->resetForesee();
	}
}

} /* namespace codablecash */
