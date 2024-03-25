/*
 * AbstractHashAlgorithmElement.h
 *
 *  Created on: 2023/03/09
 *      Author: iizuka
 */

#ifndef POW_HASH_ABSTRACTHASHALGORITHMELEMENT_H_
#define POW_HASH_ABSTRACTHASHALGORITHMELEMENT_H_

namespace alinous {
class BigInteger;
}
using namespace alinous;

namespace codablecash {

class AbstractHashAlgorithmElement {
public:
	static const constexpr int OP_AES{0};
	static const constexpr int OP_SALSA{1};
	static const constexpr int OP_SHA256{2};

	explicit AbstractHashAlgorithmElement();
	virtual ~AbstractHashAlgorithmElement();

	virtual void buildInputs(const BigInteger* nonce, int pos) noexcept;
	void execute(char** params);

protected:
	virtual void calulate(const char* in01, const char* in02, const char* in03, char* out) const noexcept = 0;

private:
	int arg01;
	int arg02;
	int arg03;
	int out01;
};

} /* namespace codablecash */

#endif /* POW_HASH_ABSTRACTHASHALGORITHMELEMENT_H_ */
