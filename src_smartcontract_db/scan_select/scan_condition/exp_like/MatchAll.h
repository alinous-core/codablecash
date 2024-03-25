/*
 * MatchAll.h
 *
 *  Created on: 2022/01/31
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_MATCHALL_H_
#define SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_MATCHALL_H_

#include "scan_select/scan_condition/exp_like/AbstractLikeMatch.h"

namespace codablecash {

class MatchAll : public AbstractLikeMatch {
public:
	MatchAll();
	virtual ~MatchAll();

	virtual int match(const UnicodeString* str, int start);
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_MATCHALL_H_ */
