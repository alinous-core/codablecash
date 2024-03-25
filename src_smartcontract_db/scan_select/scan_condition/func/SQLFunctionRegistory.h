/*
 * SQLFunctionRegistory.h
 *
 *  Created on: 2021/10/28
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_CONDITION_FUNC_SQLFUNCTIONREGISTORY_H_
#define SCAN_SELECT_SCAN_CONDITION_FUNC_SQLFUNCTIONREGISTORY_H_

#include "base/HashMap.h"


namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class AbstractSQLFunctionBody;

class SQLFunctionRegistory {
public:
	SQLFunctionRegistory();
	virtual ~SQLFunctionRegistory();

	AbstractSQLFunctionBody* getFunction(const UnicodeString* name) const noexcept;
private:
	void init();
	void addFunc(AbstractSQLFunctionBody* funcBody);

	HashMap<UnicodeString, AbstractSQLFunctionBody> map;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_CONDITION_FUNC_SQLFUNCTIONREGISTORY_H_ */
