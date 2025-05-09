#include "TokenStream.h"

using namespace std;

int TokenStream::getLine() const
{
	if (eof()) return -1;
	return tokens[idx].line;
}

int TokenStream::getCol() const
{
	if (eof()) return -1;
	return tokens[idx].col;
}

bool TokenStream::eof() const
{
	return idx >= tokens.size();
}

const Token& TokenStream::peek()
{
	if (eof())
	{
		logger.log(ErrorCode::PARSER_TOKEN_STREAM_OVERFLOW, getLine(), getCol());
		throw invalid_argument("��ū ��Ʈ�� �����÷ο�");
	}
	return tokens[idx];
}

const Token& TokenStream::next()
{
	if (eof())
	{
		logger.log(ErrorCode::PARSER_TOKEN_STREAM_OVERFLOW, getLine(), getCol());
		throw invalid_argument("��ū ��Ʈ�� �����÷ο�");
	}
	return tokens[idx++];
}

const Token& TokenStream::expect(TokenKind kind)
{
	if (eof())
	{
		logger.log(ErrorCode::PARSER_TOKEN_STREAM_OVERFLOW, getLine(), getCol());
		throw invalid_argument("��ū ��Ʈ�� �����÷ο�");
	}

	if (tokens[idx].kind != kind)
	{
		logger.log(ErrorCode::PARSER_UNEXPECTED_TOKEN, getLine(), getCol());
		throw invalid_argument("������ ��ū�� �ƴ�");
	}

	const Token& ret = tokens[idx];
	idx++;
	return ret;
}

bool TokenStream::match(TokenKind kind)
{
	if (!eof() && tokens[idx].kind == kind)
	{
		idx++;
		return true;
	}
	return false;
}