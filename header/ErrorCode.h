#pragma once
#include <string>

enum class ErrorCode
{
	LEXER_UNKNOWN_TOKEN,
	LEXER_INVALID_NUMBER,
	LEXER_UNCLOSED_STRING,
	LEXER_OVERLONG_TOKEN,
};

inline std::string error_code_to_string(ErrorCode code)
{
	switch (code)
	{
	case ErrorCode::LEXER_UNKNOWN_TOKEN:
		return "LEXER_UNKNOWN_TOKEN";
	case ErrorCode::LEXER_INVALID_NUMBER:
		return "LEXER_INVALID_NUMBER";
	case ErrorCode::LEXER_UNCLOSED_STRING:
		return "LEXER_UNCLOSED_STRING";
	case ErrorCode::LEXER_OVERLONG_TOKEN:
		return "LEXER_OVERLONG_TOKEN";
	default:
		return "UNKNOWN_ERROR_CODE";
	}
}
