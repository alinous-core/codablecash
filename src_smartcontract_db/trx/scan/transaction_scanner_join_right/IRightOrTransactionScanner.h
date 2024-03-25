/*
 * IRightOrTransactionScanner.h
 *
 *  Created on: 2022/01/23
 *      Author: iizuka
 */

#ifndef TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_IRIGHTORTRANSACTIONSCANNER_H_
#define TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_IRIGHTORTRANSACTIONSCANNER_H_

namespace codablecash {

class IRightOrTransactionScanner {
public:
	IRightOrTransactionScanner();
	virtual ~IRightOrTransactionScanner();

	virtual void increaseOrCondition() = 0;

};

} /* namespace codablecash */

#endif /* TRX_SCAN_TRANSACTION_SCANNER_JOIN_RIGHT_IRIGHTORTRANSACTIONSCANNER_H_ */
