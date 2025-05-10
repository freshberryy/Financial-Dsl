#pragma once
#include <vector>
#include <stdexcept>
#include "ErrorCode.h"
#include "Token.h"      
#include "TokenKind.h"
#include "Logger.h"

class TokenStream
{
public:
	TokenStream(const vector<Token>& v) : tokens(v) {}

	int getLine() const;

	int getCol() const;

	bool eof() const;

	const Token& peek();
	const Token& peek(int offset) ;

	const Token& next();

	const Token& expect(TokenKind);

	bool match(TokenKind);

	const Token& previous();

private:
	std::vector<Token> tokens;
	Logger logger;
	int idx = 0;
};