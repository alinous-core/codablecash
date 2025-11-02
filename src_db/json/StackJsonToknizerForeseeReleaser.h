/*
 * StackJsonToknizerForeseeReleaser.h
 *
 *  Created on: Sep 27, 2025
 *      Author: iizuka
 */

#ifndef JSON_STACKJSONTOKNIZERFORESEERELEASER_H_
#define JSON_STACKJSONTOKNIZERFORESEERELEASER_H_

namespace codablecash {

class JsonTokenizer;

class StackJsonToknizerForeseeReleaser {
public:
	StackJsonToknizerForeseeReleaser(JsonTokenizer* tokenizer, bool consume);
	virtual ~StackJsonToknizerForeseeReleaser();

private:
	JsonTokenizer* tokenizer;
	bool consume;
};

} /* namespace codablecash */

#endif /* JSON_STACKJSONTOKNIZERFORESEERELEASER_H_ */
