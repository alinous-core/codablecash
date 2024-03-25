/*
 * LikeExpressionProcessor.cpp
 *
 *  Created on: 2022/01/30
 *      Author: iizuka
 */

#include "scan_select/scan_condition/exp_like/LikeExpressionProcessor.h"

#include "base/UnicodeString.h"
#include "base/StackRelease.h"

#include "base/ArrayList.h"

#include "scan_select/scan_condition/exp_like/LikeTokenListCursor.h"

#include "scan_select/scan_condition/exp_like/BeginWithMatch.h"

#include "scan_select/scan_condition/exp_like/MatchAll.h"

#include "scan_select/scan_condition/exp_like/CompleteMatch.h"

#include "scan_select/scan_condition/exp_like/EndWithMatch.h"


using namespace alinous;

namespace codablecash {

LikeExpressionProcessor::LikeExpressionProcessor(const UnicodeString* target) {
	this->target = new UnicodeString(*target);
	this->escape = new UnicodeString(L"%");
	this->esch = this->escape->get(0);
}

LikeExpressionProcessor::~LikeExpressionProcessor() {
	delete this->target;
	delete this->escape;
	this->list.deleteElements();
}

void LikeExpressionProcessor::setEscape(const UnicodeString* escape) noexcept {
	delete this->escape;

	this->esch = escape->get(0);
	this->escape = new UnicodeString(L"");
	this->escape->append(this->esch);
}

bool LikeExpressionProcessor::match(const wchar_t* str) {
	UnicodeString uni(str);
	return match(&uni);
}

bool LikeExpressionProcessor::match(const UnicodeString* str) {
	int mpos = 0;

	int maxLoop = this->list.size();
	for(int i = 0; i != maxLoop; ++i){
		AbstractLikeMatch* m = this->list.get(i);

		mpos = m->match(str, mpos);
		if(mpos < 0){
			return false;
		}
	}

	int len = str->length();
	return mpos == len;
}

void LikeExpressionProcessor::compile() {
	ArrayList<UnicodeString> list;
	list.setDeleteOnExit();

	UnicodeString* t = getToken();
	while(t != nullptr){
		list.addElement(t);

		t = getToken();
	}

	parseList(&list);
}

void LikeExpressionProcessor::parseList(ArrayList<UnicodeString>* list) {
	LikeTokenListCursor cursor(list);

	const UnicodeString* t = nullptr;
	do{
		t = cursor.next();

		if(t->equals(this->escape)){
			t = cursor.lookAhead();

			if(t == nullptr){
				MatchAll* match = new MatchAll();
				this->list.addElement(match);
				break;
			}

			// always non-escape comes here
			if(!t->equals(this->escape)){
				t = cursor.next();
				EndWithMatch* match = new EndWithMatch(t);
				this->list.addElement(match);
			}

			continue;
		}

		const UnicodeString* lastT = t; // not escape
		t = cursor.lookAhead();
		if(t == nullptr){
			CompleteMatch* match = new CompleteMatch(lastT);
			this->list.addElement(match);
			break;
		}

		// always escape comes here
		if(t->equals(this->escape)){
			BeginWithMatch* match = new BeginWithMatch(lastT);
			this->list.addElement(match);
		}

	}while(cursor.lookAhead() != nullptr);
}

UnicodeString* LikeExpressionProcessor::getToken() {
	UnicodeString* ret = new UnicodeString(L""); __STP(ret);

	while(!isEof()){
		wchar_t ch = getCh();
		if(ch == this->esch){
			if(ret->isEmpty()){
				ret->append(ch);
				break;
			}
			else{
				pushBack();
				break;
			}
		}

		ret->append(ch);
	}

	if(ret->isEmpty()){
		return nullptr;
	}

	return __STP_MV(ret);
}

wchar_t LikeExpressionProcessor::getCh() {
	wchar_t ch = this->target->get(this->pos++);

	return ch;
}

bool LikeExpressionProcessor::isEof() const noexcept {
	return this->pos >= this->target->length();
}

void LikeExpressionProcessor::pushBack() noexcept {
	this->pos--;
}

} /* namespace codablecash */
