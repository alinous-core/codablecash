/*
 * AbstractLikeMatch.h
 *
 *  Created on: 2022/01/31
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_ABSTRACTLIKEMATCH_H_
#define SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_ABSTRACTLIKEMATCH_H_

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class AbstractLikeMatch {
public:
	AbstractLikeMatch();
	virtual ~AbstractLikeMatch();

	virtual int match(const UnicodeString* str, int start) = 0;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_ABSTRACTLIKEMATCH_H_ */
