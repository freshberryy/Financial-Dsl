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
Expr* Parser::parse_literal()
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

	logger.log(ErrorCode::PARSER_INVALID_LITERAL, ts->getLine(), ts->getCol());
	return nullptr;
}


Expr* Parser::parse_identifier()
{
	if (ts->peek().kind != TokenKind::IDENTIFIER)
	{
		logger.log(ErrorCode::PARSER_EXPECTED_IDENTIFIER, ts->getLine(), ts->getCol());
		return nullptr;
	}
	Token tok = ts->next();
	return new IdentifierExpr(tok.lexeme, tok.line, tok.col);
}

Expr* Parser::parse_primary_expr()
{
	Token tok = ts->peek();

	if (tok.kind == TokenKind::INT_LITERAL || tok.kind == TokenKind::FLOAT_LITERAL || tok.kind == TokenKind::STRING_LITERAL || tok.kind == TokenKind::BOOL_LITERAL)
	{
		return parse_literal();
	}

	if (tok.kind == TokenKind::IDENTIFIER) return parse_identifier();

	if (tok.kind == TokenKind::LPAREN)
	{
		ts->next();
		Expr* expr = parse_expr();
		if (!ts->match(TokenKind::RPAREN))
		{
			logger.log(ErrorCode::PARSER_UNCLOSED_PAREN, ts->getLine(), ts->getCol());
			return nullptr;
		}
		return expr;
	}
	logger.log(ErrorCode::PARSER_EXPECTED_EXPRESSION, tok.line, tok.col);
	return nullptr;
}

Expr* Parser::parse_postfix_expr()
{
	if (ts->peek().kind != TokenKind::IDENTIFIER)
	{
		logger.log(ErrorCode::PARSER_EXPECT_IDENTIFIER_FOR_POSTFIX, ts->getLine(), ts->getCol());
		throw std::invalid_argument("postfix_expr는 식별자로 시작해야 합니다.");
	}

	Expr* expr = parse_identifier(); 
	const Token& tok = ts->peek();

	if (tok.kind == TokenKind::LPAREN)
	{
		ts->next();
		vector<Expr*> args =  parse_arg_list();
		//parse_arg_list()에서 반드시 괄호 안의 식을 모두 소비해야 함.
		ts->expect(TokenKind::RPAREN);

		return new FunctionCallExpr(expr, args, tok.line, tok.col);
	}

	if (ts->peek().kind == TokenKind::LBRACKET)
	{
		Token lb = ts->next();
		Expr* idx1 = parse_expr();
		ts->expect(TokenKind::RBRACKET);

		if (ts->peek().kind == TokenKind::LBRACKET)
		{
			ts->next();
			Expr* idx2 = parse_expr();
			ts->expect(TokenKind::RBRACKET);
			return new Array2DAccessExpr(expr, idx1, idx2, lb.line, lb.col);
		}
		else
		{
			return new Array1DAccessExpr(expr, idx1, lb.line, lb.col);
		}
	}
	return expr;
}

vector<Expr*> Parser::parse_arg_list()
{
	vector<Expr*> args;

	if (ts->peek().kind == TokenKind::RPAREN) return args;

	int cnt = 0;

	while (true)
	{
		Expr* arg = parse_expr();
		if (!arg)
		{
			logger.log(ErrorCode::PARSER_INVALID_ARG_LIST, ts->getLine(), ts->getCol());
			return {};
		}
		args.push_back(arg);

		if (!ts->match(TokenKind::COMMA)) break;
		cnt++;

		//인자가 255개 초과면 오류
		if (cnt > 255)
		{
			logger.log(ErrorCode::PARSER_CALL_ARGUMENT_COUNT_MISMATCH, ts->getLine(), ts->getCol());
			args.pop_back();
			break;
		}
	}

	return args;
}

Expr* Parser::parse_unary_expr()
{
	if (unary_depth >= 256)
	{
		logger.log(ErrorCode::PARSER_INTERNAL_UNREACHABLE, ts->getLine(), ts->getCol());
		return nullptr;
	}
	unary_depth++;
	Expr* node;
	const Token& tok = ts->peek();
	if (tok.kind == TokenKind::PLUS || tok.kind == TokenKind::MINUS || tok.kind == TokenKind::NOT)
	{
		ts->next();
		node = parse_unary_expr();
		if (!node)
		{
			logger.log(ErrorCode::PARSER_INVALID_UNARY_EXPR, ts->getLine(), ts->getCol());
			unary_depth--;
			return nullptr;
		}
		unary_depth--;
		return new UnaryExpr(node, token_kind_to_string(tok.kind), tok.line, tok.col);
	}
	else
	{
		unary_depth--;
		return parse_primary_expr();
	}
}

Expr* Parser::parse_mul_expr()
{
	Expr* lhs = parse_unary_expr();
	if (!lhs)
	{
		logger.log(ErrorCode::PARSER_INVALID_BINARY_EXPR, ts->getLine(), ts->getCol());
		return nullptr;
	}
	Expr* rhs;

	while (ts->peek().kind == TokenKind::MUL || ts->peek().kind == TokenKind::DIV || ts->peek().kind == TokenKind::MOD)
	{
		const Token& op = ts->peek();
		ts->next();
		rhs = parse_unary_expr();
		if (!rhs || !lhs)
		{
			logger.log(ErrorCode::PARSER_INVALID_BINARY_EXPR, ts->getLine(), ts->getCol());
			return nullptr;
		}
		lhs = new BinaryExpr(lhs, op.lexeme, rhs, op.line, op.col);
	}
	return lhs;
}

Expr* Parser::parse_add_expr()
{
	Expr* lhs = parse_mul_expr();
	if (!lhs)
	{
		logger.log(ErrorCode::PARSER_INVALID_BINARY_EXPR, ts->getLine(), ts->getCol());
		return nullptr;
	}
	while (ts->peek().kind == TokenKind::PLUS || ts->peek().kind == TokenKind::MINUS)
	{
		const Token& op = ts->peek();
		ts->next();
		Expr* rhs = parse_mul_expr();
		if (!rhs || !lhs)
		{
			logger.log(ErrorCode::PARSER_INVALID_BINARY_EXPR, ts->getLine(), ts->getCol());
			return nullptr;
		}
		lhs = new BinaryExpr(lhs, op.lexeme, rhs, op.line, op.col);
	}
	return lhs;
}


Expr* Parser::parse_expr()
{
	//식에 속한 토큰을 모두 소비해야 함.
	return parse_add_expr();
}


//Type
Type* Parser::parse_type()
{
	Token tok = ts->peek();
	if (tok.kind != TokenKind::KW_INT && tok.kind != TokenKind::KW_FLOAT && tok.kind != TokenKind::KW_STRING && tok.kind != TokenKind::KW_BOOL)
	{
		logger.log(ErrorCode::PARSER_EXPECTED_TYPE_KEYWORD, ts->getLine(), ts->getCol());
		return nullptr;
	}
	ts->next();
	string type_s = tok.lexeme;
	int dim = 0;

	while (ts->match(TokenKind::LBRACKET))
	{
		if (!ts->match(TokenKind::RBRACKET))
		{
			logger.log(ErrorCode::PARSER_UNCLOSED_ARRAY_ACCESS, ts->getLine(), ts->getCol());
			return nullptr;
		}
		dim++;
		type_s += "[]";
	}

	if (dim > 2)
	{
		logger.log(ErrorCode::PARSER_ARRAY_DECL_INVALID_DIMENSION, ts->getLine(), ts->getCol());
		return nullptr;
	}
	Type* node = new Type(type_s, dim, ts->getLine(), ts->getCol());

	return node;
}

//util
bool Parser::is_semicolon()
{
	if (!ts->match(TokenKind::SEMICOLON))
	{
		logger.log(ErrorCode::PARSER_EXPECTED_SEMICOLON, ts->getLine(), ts->getCol());
		return false;
	}
	return true;
}
