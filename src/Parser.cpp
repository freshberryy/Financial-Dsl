#include "Parser.h"
using namespace std;

Parser::Parser(TokenStream* ts, Logger lg) :ts(ts), logger(lg)
{
	if (ts == nullptr)
	{
		throw invalid_argument("비어 있는 토큰 스트림");
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
			throw ParserException("오른쪽 소괄호 누락", ts->get_line(), ts->get_col());
		}
		ts->next();
		return expr;
	}
	logger.log(tok.line, tok.col);
	throw ParserException("primary_expr 시작 토큰이 아님", tok.line, tok.col);
}


Expr* Parser::parse_postfix_expr()
{
	Expr* expr = parse_primary_expr();

	//괄호를 가리킬 것으로 예상
	const Token& tk = ts->peek();

	if (tk.kind == TokenKind::LPAREN)
	{
		//식을 가리킬 것으로 예상
		ts->next();
		vector<Expr*> args = parse_arg_list();
		if (ts->peek().kind != TokenKind::RPAREN)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("오른쪽 소괄호 누락", ts->get_line(), ts->get_col());
		}
		ts->next();
		if (ts->peek().kind == TokenKind::LPAREN)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("연속된 함수 호출은 허용되지 않음", ts->get_line(), ts->get_col());
		}
		if (ts->peek().kind == TokenKind::LBRACKET)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("함수에 대한 배열 접근은 허용되지 않음", ts->get_line(), ts->get_col());
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
			throw ParserException("오른쪽 대괄호 누락", ts->get_line(), ts->get_col());
		}
		if (ts->peek().kind == TokenKind::LBRACKET)
		{
			Expr* idx2 = parse_expr();
			if (!idx2)
			{
				logger.log(ts->get_line(), ts->get_col());
				throw ParserException("배열 인덱스 누락", ts->get_line(), ts->get_col());
			}
			if (!ts->match(TokenKind::RBRACKET))
			{
				logger.log(ts->get_line(), ts->get_col());
				throw ParserException("오른쪽 대괄호 누락", ts->get_line(), ts->get_col());
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
			throw ParserException("3차원 이상 배열은 허용되지 않음", ts->get_line(), ts->get_col());
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
		throw ParserException("인자 목록이 콤마로 시작", ts->get_line(), ts->get_col());
	}

	//식 하나 push
	args.push_back(parse_expr());

	//콤마 있으니 루프
	while (ts->match(TokenKind::COMMA))
	{
		//콤마 다음 식으로
		if (ts->peek().kind == TokenKind::COMMA)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("인자 없이 연속된 콤마", ts->get_line(), ts->get_col());
		}
		//식 소비
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
			throw ParserException("단항 연산자 뒤에 표현식이 없음", tk.line, tk.col);
		}
		return new UnaryExpr(expr, tk.lexeme, tk.line, tk.col);
	}
	else
	{
		return parse_postfix_expr();
	}
}

