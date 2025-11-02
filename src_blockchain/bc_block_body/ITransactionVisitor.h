/*
 * ITransactionVisitor.h
 *
 *  Created on: May 6, 2025
 *      Author: iizuka
 */

#ifndef BC_BLOCK_BODY_ITRANSACTIONVISITOR_H_
#define BC_BLOCK_BODY_ITRANSACTIONVISITOR_H_

namespace codablecash {

class AbstractBlockchainTransaction;
class Block;

class ITransactionVisitor {
public:
	ITransactionVisitor();
	virtual ~ITransactionVisitor();

	virtual void visit(const AbstractBlockchainTransaction* trx, const Block* block) = 0;
};

} /* namespace codablecash */

#endif /* BC_BLOCK_BODY_ITRANSACTIONVISITOR_H_ */
