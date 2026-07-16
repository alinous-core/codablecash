/*
 * StatementRepairSkipper.cpp
 *
 *  Created on: Apr 29, 2026
 *      Author: iizuka
 */

#include "engine/compiler/StatementRepairSkipper.h"

#include "alinous_lang/AlinousLang.h"
#include "alinous_lang/Token.h"

namespace codablecash {

StatementRepairSkipper::StatementRepairSkipper(AlinousLang* lang) {
	this->lang = lang;
	this->lastToken = nullptr;
}

StatementRepairSkipper::~StatementRepairSkipper() {
	this->lang = nullptr;
	this->lastToken = nullptr;
}

void StatementRepairSkipper::recoverStatement() {
	Token* token = nullptr;
	do{
		token = getToken();
	}while(!isStatementStopToken(token));
}

bool StatementRepairSkipper::isStatementStopToken(Token *token) {
	return isEOF(token) || isSemicolon(token) || isBraceEnd(token) || isNextStatement(token) || isNextBraceStarts(token);
}

/***
 * for statement
 */
void StatementRepairSkipper::recoverForStatement() {
	Token* token = nullptr;
	do{
		token = getToken();
	}while(!isForStatementStopToken(token));
}

bool StatementRepairSkipper::isForStatementStopToken(Token *token) {
	return isEOF(token) || isNextBraceStarts(token);
}

bool StatementRepairSkipper::isEOF(Token *token) {
	return token->kind == alinouslang::_EOF;
}

bool StatementRepairSkipper::isSemicolon(Token *token) {
	return token->kind == alinouslang::SEMI_COLON;
}

bool StatementRepairSkipper::isBraceEnd(Token *token) {
	bool res = token->kind == alinouslang::R_BRACE;
	if(res == true){
		// push back
		this->lang->token = this->lastToken;
	}

	return res;
}

Token* StatementRepairSkipper::getToken() {
	this->lastToken = this->lang->token;
	Token* token = lang->getNextToken();

	countToken(token);

	return token;
}

void StatementRepairSkipper::countToken(Token *token) {
	if(token->kind == alinouslang::L_BRACE){

	}
	else if(token->kind == alinouslang::R_BRACE){

	}
}

bool StatementRepairSkipper::isNextStatement(Token *token) {
	bool bl = false;
	int k = token->kind;
	if(k == alinouslang::IF || k == alinouslang::WHILE || k == alinouslang::FOR || k == alinouslang::DO
			|| k == alinouslang::SELECT || k == alinouslang::INSERT || k == alinouslang::UPDATE || k == alinouslang::DELETE
			|| k == alinouslang::BEGIN || k == alinouslang::COMMIT || k == alinouslang::ROLLBACK){
		bl = true;
		// push back
		this->lang->token = this->lastToken;
	}

	return bl;
}

bool StatementRepairSkipper::isNextBraceStarts(Token *token) {
	bool bl = false;
	int k = token->kind;
	if(k == alinouslang::L_BRACE){
		bl = true;
		// push back
		this->lang->token = this->lastToken;
	}

	return bl;
}

} /* namespace codablecash */
