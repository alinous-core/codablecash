/*
 * StatementRepairSkipper.h
 *
 *  Created on: Apr 29, 2026
 *      Author: iizuka
 */

#ifndef ENGINE_COMPILER_STATEMENTREPAIRSKIPPER_H_
#define ENGINE_COMPILER_STATEMENTREPAIRSKIPPER_H_

namespace alinouslang {
class AlinousLang;
class Token;
}
using namespace alinouslang;

namespace codablecash {

class StatementRepairSkipper {
public:
	explicit StatementRepairSkipper(AlinousLang* lang);
	virtual ~StatementRepairSkipper();

	void recoverStatement();
	bool isStatementStopToken(Token* token);

	void recoverForStatement();
	bool isForStatementStopToken(Token* token);

	bool isEOF(Token* token);
	bool isSemicolon(Token* token);
	bool isBraceEnd(Token* token);
	bool isNextBraceStarts(Token* token);
	bool isNextStatement(Token* token);

	Token* getToken();

private:
	void countToken(Token* token);

private:
	AlinousLang* lang;
	Token* lastToken;

};

} /* namespace codablecash */

#endif /* ENGINE_COMPILER_STATEMENTREPAIRSKIPPER_H_ */
