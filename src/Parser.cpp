#include "Parser.h"
using namespace std;

Parser::Parser(TokenStream* ts, Logger lg) :ts(ts), logger(lg)
{
	if (ts == nullptr)
	{
		throw invalid_argument("��� �ִ� ��ū ��Ʈ��");
	}
}

ASTNode* Parser::parse()
{

	return parse_program();
}

ASTNode* Parser::parse_program()
{
	while (!ts->eof())
	{
		ts->next();
		continue;
	}
	return nullptr;
}



//Expr
Expr* Parser::parse_expr()
{
	return parse_primary_expr();
}

Expr* Parser::parse_primary_expr()
{
	const Token& tok = ts->peek();

	if (tok.kind == TokenKind::INT_LITERAL)
	{
		ts->next();
		return new IntLiteralExpr(tok.lexeme, tok.line, tok.col);
	}
	if (tok.kind == TokenKind::FLOAT_LITERAL)
	{
		ts->next();
		return new FloatLiteralExpr(tok.lexeme, tok.line, tok.col);
	}
	if (tok.kind == TokenKind::STRING_LITERAL)
	{
		ts->next();
		return new StringLiteralExpr(tok.lexeme, tok.line, tok.col);
	}
	if (tok.kind == TokenKind::BOOL_LITERAL)
	{
		ts->next();
		return new BoolLiteralExpr(tok.lexeme, tok.line, tok.col);
	}
	if (tok.kind == TokenKind::IDENTIFIER)
	{
		ts->next();
		return new IdentifierExpr(tok.lexeme, tok.line, tok.col);
	}
	if (tok.kind == TokenKind::LPAREN)
	{
		ts->next();
		Expr* expr = parse_expr();
		
		if (ts->peek().kind != TokenKind::RPAREN)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("������ �Ұ�ȣ ����", ts->get_line(), ts->get_col());
		}
		ts->next();
		return expr;
	}
	logger.log(tok.line, tok.col);
	throw ParserException("primary_expr ���� ��ū�� �ƴ�", tok.line, tok.col);
}


Expr* Parser::parse_postfix_expr()
{
	Expr* expr = parse_primary_expr();

	//��ȣ�� ����ų ������ ����
	const Token& tk = ts->peek();

	if (tk.kind == TokenKind::LPAREN)
	{
		//���� ����ų ������ ����
		ts->next();
		vector<Expr*> args = parse_arg_list();
		if (ts->peek().kind != TokenKind::RPAREN)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("������ �Ұ�ȣ ����", ts->get_line(), ts->get_col());
		}
		ts->next();
		if (ts->peek().kind == TokenKind::LPAREN)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("���ӵ� �Լ� ȣ���� ������ ����", ts->get_line(), ts->get_col());
		}
		if (ts->peek().kind == TokenKind::LBRACKET)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("�Լ��� ���� �迭 ������ ������ ����", ts->get_line(), ts->get_col());
		}
		return new FunctionCallExpr(expr, args, expr->get_location().first, expr->get_location().second);
	}

	if (tk.kind == TokenKind::LBRACKET)
	{
		ts->next();
		Expr* idx1 = parse_expr();
		if (!ts->match(TokenKind::RBRACKET))
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("������ ���ȣ ����", ts->get_line(), ts->get_col());
		}
		if (ts->peek().kind == TokenKind::LBRACKET)
		{
			Expr* idx2 = parse_expr();
			if (!idx2)
			{
				logger.log(ts->get_line(), ts->get_col());
				throw ParserException("�迭 �ε��� ����", ts->get_line(), ts->get_col());
			}
			if (!ts->match(TokenKind::RBRACKET))
			{
				logger.log(ts->get_line(), ts->get_col());
				throw ParserException("������ ���ȣ ����", ts->get_line(), ts->get_col());
			}
			return new Array2DAccessExpr(expr, idx1, idx2, expr->get_location().first, expr->get_location().second);
		}
		else if(ts->eof())
		{
			return new Array1DAccessExpr(expr, idx1, expr->get_location().first, expr->get_location().second);
		}
		else
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("3���� �̻� �迭�� ������ ����", ts->get_line(), ts->get_col());
		}
	}
	return expr;
}


vector<Expr*> Parser::parse_arg_list()
{
	vector<Expr*> args;

	if (ts->peek().kind == TokenKind::RPAREN) return args;


	if (ts->peek().kind == TokenKind::COMMA)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("���� ����� �޸��� ����", ts->get_line(), ts->get_col());
	}

	//�� �ϳ� push
	args.push_back(parse_expr());

	//�޸� ������ ����
	while (ts->match(TokenKind::COMMA))
	{
		//�޸� ���� ������
		if (ts->peek().kind == TokenKind::COMMA)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("���� ���� ���ӵ� �޸�", ts->get_line(), ts->get_col());
		}
		//�� �Һ�
		args.push_back(parse_expr());
	}
	return args;
}


Expr* Parser::parse_unary_expr()
{
	const Token& tk = ts->peek();

	if (tk.kind == TokenKind::PLUS || tk.kind == TokenKind::MINUS || tk.kind == TokenKind::NOT)
	{
		ts->next();

		Expr* expr = parse_unary_expr();
		if (!expr)
		{
			logger.log(tk.line, tk.col);
			throw ParserException("���� ������ �ڿ� ǥ������ ����", tk.line, tk.col);
		}
		return new UnaryExpr(expr, tk.lexeme, tk.line, tk.col);
	}
	else
	{
		return parse_postfix_expr();
	}
}

