#include "TokenStream.h"

using namespace std;

int TokenStream::get_line()  const
{
	if (eof()) return -1;
	return tokens[idx].line;
}

int TokenStream::get_col() const
{
	if (eof()) return -1;
	return tokens[idx].col;
}

bool TokenStream::eof() const
{
	return idx >= tokens.size();
}

//현재 토큰 반환만, 소비 x
const Token& TokenStream::peek()
{
	if (eof())
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("토큰 스트림 오버플로우");
	}
	return tokens[idx];
}

const Token& TokenStream::peek(int offset) 
{
	if (idx + offset >= tokens.size())
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("토큰 스트림 오버플로우");
	}
	return tokens[idx + offset];
}

//현재 토큰 반환 후 idx++, 소비 o
const Token& TokenStream::next()
{
	if (eof())
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("토큰 스트림 오버플로우");
	}
	return tokens[idx++];
}

//기대한 토큰이면 반환 후 idx++, 소비 o
const Token& TokenStream::expect(TokenKind kind)
{
	if (eof())
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("토큰 스트림 오버플로우");
	}

	if (tokens[idx].kind != kind)
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("예상한 토큰이 아님");
	}

	const Token& ret = tokens[idx];
	idx++;
	return ret;
}

//기대한 토큰이면 반환 후 idx++, 소비 o
bool TokenStream::match(TokenKind kind)
{
	if (!eof() && tokens[idx].kind == kind)
	{
		idx++;
		return true;
	}
	return false;
}

//직전 토큰 반환, 소비 x
const Token& TokenStream::previous()
{
	if (idx == 0) return tokens[0];
	return tokens[idx - 1];
}