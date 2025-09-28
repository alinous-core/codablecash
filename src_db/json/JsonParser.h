/*
 * JsonParser.h
 *
 *  Created on: Sep 26, 2025
 *      Author: iizuka
 */

#ifndef JSON_JSONPARSER_H_
#define JSON_JSONPARSER_H_

#include "json/AbstractJsonToken.h"

namespace codablecash {


class AbstractJsonObject;
class JsonTokenizer;
class JsonArrayObject;
class JsonObject;
class AbstractJsonToken;
class JsonStringValue;
class JsonBooleanValue;
class JsonNumericValue;
class AbstractJsonValue;
class JsonValuePair;

class JsonParser {
public:
	explicit JsonParser(JsonTokenizer* tokenizer);
	virtual ~JsonParser();

	AbstractJsonObject* parse();

	AbstractJsonObject* allObjects();
	bool testAllObjects(bool consum);

	JsonObject* jsonObject();
	bool testJsonObject(bool consume);
	JsonArrayObject* jsonArrayObject();
	bool testJsonArrayObject(bool consume);
	JsonValuePair* jsonValuePair();
	bool testJsonValuePair(bool consume);

	AbstractJsonValue* abstractJsonValue();
	bool testAbstractJsonValue(bool consume);

	JsonStringValue* jsonStringValue();
	bool testJsonStringValue(bool consume);
	JsonBooleanValue* jsonBooleanValue();
	bool testJsonBooleanValue(bool consume);
	JsonNumericValue* jsonNumericValue();
	bool testJsonNumericValue(bool consume);

private:
	void checkNotEof();
	const AbstractJsonToken* foreseeToken();
	AbstractJsonToken* getToken();

	void checkTokenType(const AbstractJsonToken* token, TokenType type, const wchar_t* required);

private:
	JsonTokenizer* tokenizer;
};

} /* namespace codablecash */

#endif /* JSON_JSONPARSER_H_ */
