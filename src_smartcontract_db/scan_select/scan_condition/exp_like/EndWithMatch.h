/*
 * EndWithMatch.h
 *
 *  Created on: 2022/01/31
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_ENDWITHMATCH_H_
#define SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_ENDWITHMATCH_H_

#include "scan_select/scan_condition/exp_like/AbstractLikeMatch.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class EndWithMatch : public AbstractLikeMatch {
public:
	explicit EndWithMatch(const UnicodeString* pattern);
	virtual ~EndWithMatch();

	virtual int match(const UnicodeString* str, int start);

private:
	UnicodeString* pattern;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_ENDWITHMATCH_H_ */
