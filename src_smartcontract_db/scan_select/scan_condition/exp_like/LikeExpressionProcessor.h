/*
 * LikeExpressionProcessor.h
 *
 *  Created on: 2022/01/30
 *      Author: iizuka
 */

#ifndef SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKEEXPRESSIONPROCESSOR_H_
#define SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKEEXPRESSIONPROCESSOR_H_
#include "base/ArrayList.h"

namespace alinous {
class UnicodeString;
}
using namespace alinous;

namespace codablecash {

class AbstractLikeMatch;

class LikeExpressionProcessor {
public:
	explicit LikeExpressionProcessor(const UnicodeString* target);
	virtual ~LikeExpressionProcessor();

	void compile();
	bool match(const wchar_t* str);
	bool match(const UnicodeString* str);

	void setEscape(const UnicodeString* escape) noexcept;

private:
	UnicodeString* getToken();
	wchar_t getCh();
	bool isEof() const noexcept;
	void pushBack() noexcept;

	void parseList(ArrayList<UnicodeString>* list);

private:
	UnicodeString* target;
	UnicodeString* escape;
	wchar_t esch;

	ArrayList<AbstractLikeMatch> list;

	int pos = 0;
};

} /* namespace codablecash */

#endif /* SCAN_SELECT_SCAN_COLUMNS_SCAN_COLUMNS_EXP_LIKEEXPRESSIONPROCESSOR_H_ */
