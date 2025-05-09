#pragma once
#include <string>
#include "TokenKind.h"

struct Token
{
	Token(TokenKind kind, const std::string& lexeme, int line, int col) :kind(kind), lexeme(lexeme), line(line), col(col) {}

	TokenKind kind;
	std::string lexeme;
	int line;
	int col;
};