#pragma once
#include <string>
#include "TokenKind.h"

struct Token
{
	Token(TokenKind kind, const std::string& lexeme, int row, int col) :kind(kind), lexeme(lexeme), row(row), col(col) {}

	TokenKind kind;
	std::string lexeme;
	int row;
	int col;
};