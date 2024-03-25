/*
 * InputUtxoCollection.h
 *
 *  Created on: 2023/01/25
 *      Author: iizuka
 */

#ifndef BC_TRX_BALANCE_INPUTUTXOCOLLECTION_H_
#define BC_TRX_BALANCE_INPUTUTXOCOLLECTION_H_

#include "base/ArrayList.h"

namespace alinous {
class ByteBuffer;
}
using namespace alinous;

namespace codablecash {

class BalanceUtxoReference;
class IMuSigSignerProvidor;
class MuSig;
class IUtxoFinder;

class InputUtxoCollection {
public:
	InputUtxoCollection(const InputUtxoCollection& inst);
	InputUtxoCollection();
	virtual ~InputUtxoCollection();

	void addReference(const BalanceUtxoReference* ref) noexcept;

	MuSig sign(IMuSigSignerProvidor* providor, IUtxoFinder* finder, const char *data, int length);

	int binarySize() const;
	void toBinary(ByteBuffer* out) const;
	static InputUtxoCollection* fromBinary(ByteBuffer* in);

	const ArrayList<BalanceUtxoReference>* getList() const noexcept {
		return &this->list;
	}

private:
	ArrayList<BalanceUtxoReference> list;
};

} /* namespace codablecash */

#endif /* BC_TRX_BALANCE_INPUTUTXOCOLLECTION_H_ */
