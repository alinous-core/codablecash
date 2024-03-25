/*
 * LikeTokenListCursor.h
 *
 *  Created on: 2022/01/31
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_LIKETOKENLISTCURSOR_H_
#define SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_LIKETOKENLISTCURSOR_H_

#include "base/ArrayList.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class LikeTokenListCursor {
public:
	explicit LikeTokenListCursor(const ArrayList<UnicodeString>* list);
	virtual ~LikeTokenListCursor();

	const UnicodeString* next() noexcept;
	const UnicodeString* lookAhead() const noexcept;
private:
	const ArrayList<UnicodeString>* list;
	int pos;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKE_LIKETOKENLISTCURSOR_H_ */
