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

//���� ��ū ��ȯ��, �Һ� x
const Token& TokenStream::peek()
{
	if (eof())
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("��ū ��Ʈ�� �����÷ο�");
	}
	return tokens[idx];
}

const Token& TokenStream::peek(int offset) 
{
	if (idx + offset >= tokens.size())
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("��ū ��Ʈ�� �����÷ο�");
	}
	return tokens[idx + offset];
}

//���� ��ū ��ȯ �� idx++, �Һ� o
const Token& TokenStream::next()
{
	if (eof())
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("��ū ��Ʈ�� �����÷ο�");
	}
	return tokens[idx++];
}

//����� ��ū�̸� ��ȯ �� idx++, �Һ� o
const Token& TokenStream::expect(TokenKind kind)
{
	if (eof())
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("��ū ��Ʈ�� �����÷ο�");
	}

	if (tokens[idx].kind != kind)
	{
		logger.log(get_line(), get_col());
		throw invalid_argument("������ ��ū�� �ƴ�");
	}

	const Token& ret = tokens[idx];
	idx++;
	return ret;
}

//����� ��ū�̸� ��ȯ �� idx++, �Һ� o
bool TokenStream::match(TokenKind kind)
{
	if (!eof() && tokens[idx].kind == kind)
	{
		idx++;
		return true;
	}
	return false;
}

//���� ��ū ��ȯ, �Һ� x
const Token& TokenStream::previous()
{
	if (idx == 0) return tokens[0];
	return tokens[idx - 1];
}