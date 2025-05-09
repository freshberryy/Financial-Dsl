#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <climits>
#include "Token.h"
#include "TokenKind.h"
#include "TokenRegex.h"
#include "TokenPriority.h"
#include "Logger.h"
#include "ErrorCode.h"

class Lexer
{
public:

	Lexer()
	{
		init_regex_map();
		init_token_priority_map();
	}

	std::vector<Token> tokenize(const std::string& );

	void print_logs() const;
	std::vector<std::string> logs;

private:
	int row = 1, col = 1, offset = 0, tab_width = 4;
	

	Logger logger;

	pair<TokenKind, std::string> find_longest_match(const std::string& );

	void advance_at_whitespace(const std::string&);

	void advance(int);

	bool skip_token(TokenKind) const;


};
