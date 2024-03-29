/*
 * ValidationError.h
 *
 *  Created on: 2019/06/01
 *      Author: iizuka
 */

#ifndef SC_ANALYZE_VALIDATIONERROR_H_
#define SC_ANALYZE_VALIDATIONERROR_H_
#include <initializer_list>

#include "base/ArrayList.h"

namespace alinous {
class CodeElement;
class UnicodeString;

class ValidationError {
public:
	static constexpr int ERROR = 10;
	static constexpr int WARNING = 1;

	static constexpr int CODE_CLASS_ALREADY_EXISTS = 1;
	static constexpr int CODE_WRONG_IMPORT_FORMAT = 2;
	static constexpr int CODE_WRONG_CLASS_NAME = 3;
	static constexpr int CODE_WRONG_TYPE_NAME = 4;
	static constexpr int CODE_NO_RETURN_METHOD_VALUE = 5;

	static constexpr int CODE_VIRTUAL_FUNC_WITH_DIFFERENT_RETURN = 10;
	static constexpr int CODE_VIRTUAL_FUNC_WITH_DIFFERENT_STATIC = 11;

	static constexpr int CODE_WRONG_FUNC_CALL_NAME = 20;
	static constexpr int CODE_WRONG_FUNC_CALL_AMBIGOUS = 21;
	static constexpr int CODE_WRONG_FUNC_CALL_CANT_CALL_NOSTATIC = 22;
	static constexpr int CODE_WRONG_FUNC_CALL_CANT_INCOMPATIBLE_THIS = 23;
	static constexpr int CODE_WRONG_FUNC_CALL_CANT_USE_THIS = 24;
	static constexpr int CODE_WRONG_FUNC_CALL_CANT_USE_SUPER_CONSTRUCTOR = 25;

	static constexpr int CODE_CLASS_MEMBER_ALREADY_EXISTS = 30;
	static constexpr int CODE_CLASS_MEMBER_DOES_NOT_EXISTS = 31;
	static constexpr int CODE_CLASS_MEMBER_AND_STACK_VARIABLE_DO_NOT_EXISTS = 32;
	static constexpr int CODE_CLASS_MEMBER_VOID = 33;
	static constexpr int CODE_CLASS_DOES_NOT_IMPLEMENET_METHOD = 34;

	static constexpr int CODE_ALLOCATION_TYPE_DOES_NOT_EXISTS = 40;
	static constexpr int CODE_ALLOCATION_TYPE_IS_INTERFACE = 41;

	static constexpr int CODE_ARRAY_INDEX_MUST_BE_NUMERIC = 50;
	static constexpr int CODE_ARRAY_INDEX_OVERFLOW = 51;

	static constexpr int CODE_TYPE_INCOMPATIBLE = 60;
	static constexpr int CODE_TYPE_DOES_NOT_EXISTS = 61;

	static constexpr int CODE_ARITHMETIC_NON_INTEGER = 70;

	static constexpr int CODE_LOGICAL_EXP_NON_BOOL = 71;
	static constexpr int CODE_LOGICAL_EXP_NON_PRIMITIVE = 72;

	static constexpr int CODE_CTRL_STMT_EXHIBITED = 71;

	static constexpr int CODE_THROW_STMT_REQUIRE_EXCEPTION = 80;
	static constexpr int CODE_CATCH_STMT_REQUIRE_EXCEPTION = 81;

	static constexpr int CODE_CAST_TYPE_INCOMPATIBLE = 90;
	static constexpr int CODE_CAST_TYPE_NOT_EXIST = 91;

	static constexpr int CODE_DUPLICATED_VARIABLE = 100;

	static constexpr int CODE_CONSTRUCTOR_MUST_BE_FIRST_STMT = 120;

	static constexpr int DB_TYPE_NOT_EXISTS = 1001;
	static constexpr int DB_NO_PRIMARY_KEY = 1002;
	static constexpr int DB_LENGTH_IS_NOT_INTEGER = 1002;
	static constexpr int DB_LENGTH_IS_NOT_CORRECT_INTEGER = 1003;
	static constexpr int DB_CREATE_TABLE_COLUMN_DUPLICATED_NAME = 1004;

	static constexpr int SQL_INSERT_VALUES_NUMBERS = 1100;


	static constexpr int SQL_CONDITION_WRONG_FUNC_NAME = 1900;
	static constexpr int SQL_CONDITION_WRONG_FUNC_ARG_NUM = 1901;

	static constexpr int SQL_CONDITION_WRONG_DISTINCT_USE = 2000;

	//static constexpr int CODE_WRONG_TYPE_NAME = 4;


	ValidationError(int type, int errorCode, CodeElement* element, const UnicodeString* msg, std::initializer_list<const UnicodeString*> params);
	virtual ~ValidationError();

	int getErrorCode() const noexcept;
private:
	int type;
	int errorCode;
	CodeElement* element;
	UnicodeString* message;
	ArrayList<UnicodeString> msgParams;
};

} /* namespace alinous */

#endif /* SC_ANALYZE_VALIDATIONERROR_H_ */
