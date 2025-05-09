#include "Lexer.h"
using namespace std;


pair<TokenKind, string> Lexer::find_longest_match(const string& input)
{
	int max_len = 0, ret_pri = INT_MAX;
	TokenKind sel = TokenKind::UNKNOWN;
	string ret = "";

	for (auto [kind, re] : regex_map)
	{
		smatch match;
		if (regex_search(input, match, re))
		{
			if (!match.empty() && match.position() == 0)
			{
				string lexem = match.str();
				int len = lexem.size(), pri = token_priority_map[kind];

				if ((len > max_len) || ((len == max_len) && (pri < ret_pri)))
				{
					max_len = len;
					sel = kind;
					ret = lexem;
					ret_pri = pri;
				}
			}
		}
	}

	return make_pair( sel, ret );
}

void Lexer::advance_at_whitespace(const string& lex)
{
	for (int i = 0; i < lex.size(); i++)
	{
		char c = lex[i];

		if (c == '\r')
		{
			if (i + 1 < lex.size() && lex[i + 1] == '\n')
			{
				i++;
				offset++;
			}
			col = 1;
			row++;
		}
		else if (c == '\n')
		{
			col = 1;
			row++;
		}
		else if (c == '\t')
		{
			int tmp = tab_width - ((col - 1) % tab_width);
			col += tmp;
		}
		else
		{
			col++;
		}
		offset++;
	}
}

void Lexer::advance(int len)
{
	offset += len;
	col += len;
}

void Lexer::print_logs() const
{
	cout << "-----·Î    ±×-----\n";
	for (const auto& x : logs)
	{
		cout << x;
	}
}

bool Lexer::skip_token(TokenKind kind) const
{
	return kind == TokenKind::WHITESPACE || kind == TokenKind::COMMENT;
}

vector<Token> Lexer::tokenize(const string& s)
{
	row = 1;
	col = 1;
	offset = 0;
	vector<Token> ret;

	while (offset < s.size())
	{
		auto [kind, lexeme] = find_longest_match(s.substr(offset));

		if (lexeme.size() > 256)
		{
			logger.log(ErrorCode::LEXER_OVERLONG_TOKEN, row, col);
			ret.emplace_back(kind, lexeme.substr(0, 5), row, col);
			advance(lexeme.size());
			continue;
		}

		if (kind == TokenKind::UNKNOWN || lexeme.empty())
		{
			logger.log(ErrorCode::LEXER_UNKNOWN_TOKEN, row, col);
			ret.emplace_back(kind, lexeme, row, col);
			advance(lexeme.size());
			continue;
		}

		if (skip_token(kind))
		{
			advance_at_whitespace(lexeme);
			continue;
		} 

		if (kind == TokenKind::FLOAT_LITERAL)
		{
			if (lexeme[0] == '.' || lexeme.back() == '.')
			{
				logger.log(ErrorCode::LEXER_INVALID_NUMBER, row, col);
				ret.emplace_back(kind, lexeme, row, col);
				advance(lexeme.size());
				continue;
			}
		}

		if (kind == TokenKind::STRING_LITERAL)
		{
			if (count(lexeme.begin(), lexeme.end(), '"') % 2 == 1)
			{
				logger.log(ErrorCode::LEXER_UNCLOSED_STRING, row, col);
				ret.emplace_back(kind, lexeme, row, col);
				advance(lexeme.size());
				continue;
			}
		}

	
		ret.emplace_back(kind, lexeme, row, col);
		advance(lexeme.size());
		
	}
	ret.emplace_back(TokenKind::END_OF_FILE, "", row, col);
	return ret;
}
