/*
 * IUtxoRefChecker.h
 *
 *  Created on: Jul 24, 2025
 *      Author: iizuka
 */

#ifndef BC_TRX_IUTXOREFCHECKER_H_
#define BC_TRX_IUTXOREFCHECKER_H_

namespace codablecash {

class AbstractUtxoReference;

class IUtxoRefChecker {
public:
	IUtxoRefChecker();
	virtual ~IUtxoRefChecker();

	virtual bool checkUtxo(const AbstractUtxoReference* ref) const = 0;
};

} /* namespace codablecash */

#endif /* BC_TRX_IUTXOREFCHECKER_H_ */
