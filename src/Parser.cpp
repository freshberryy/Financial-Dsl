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
	return parse_assign_expr();
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

	if (ts->peek().kind == TokenKind::PLUS || ts->peek().kind == TokenKind::MINUS || ts->peek().kind == TokenKind::NOT)
	{
		const Token& op = ts->peek();
		ts->next();

		Expr* expr = parse_unary_expr();
		if (!expr)
		{
			logger.log(op.line, op.col);
			throw ParserException("���� ������ �ڿ� ǥ������ ����", op.line, op.col);
		}
		return new UnaryExpr(expr, token_kind_to_string(op.kind), op.line, op.col);
	}
	else
	{
		return parse_postfix_expr();
	}
}


Expr* Parser::parse_mul_expr()
{
	Expr* lhs = parse_unary_expr();
	while (ts->peek().kind == TokenKind::MUL || ts->peek().kind == TokenKind::DIV || ts->peek().kind == TokenKind::MOD)
	{
		const Token& op = ts->peek();
		ts->next();
		Expr* rhs = parse_unary_expr();
		if (!rhs)
		{
			logger.log(op.line, op.col);
			throw ParserException("*/% ������ ���� �Ľ� ����", op.line, op.col);
		}
		lhs = new BinaryExpr(lhs, token_kind_to_string(op.kind), rhs, op.line, op.col);
	}
	return lhs;
}


Expr* Parser::parse_add_expr()
{
	Expr* lhs = parse_mul_expr();
	while (ts->peek().kind == TokenKind::PLUS || ts->peek().kind == TokenKind::MINUS)
	{
		const Token& op = ts->peek();
		ts->next();
		Expr* rhs = parse_mul_expr();
		if (!rhs)
		{
			logger.log(op.line, op.col);
			throw ParserException("+- ������ ���� �Ľ� ����", op.line, op.col);
		}
		lhs = new BinaryExpr(lhs, token_kind_to_string(op.kind), rhs, op.line, op.col);
	}
	return lhs;
}

Expr* Parser::parse_relational_expr()
{
	Expr* lhs = parse_add_expr();
	while (ts->peek().kind == TokenKind::GREATER || ts->peek().kind == TokenKind::GREATER_EQUAL || ts->peek().kind == TokenKind::LESS || ts->peek().kind == TokenKind::LESS_EQUAL)
	{
		const Token& op = ts->peek();
		ts->next();
		Expr* rhs = parse_add_expr();
		if (!rhs)
		{
			logger.log(op.line, op.col);
			throw ParserException("���� ������ ���� �Ľ� ����", op.line, op.col);
		}
		lhs = new BinaryExpr(lhs, token_kind_to_string(op.kind), rhs, op.line, op.col);
	}
	return lhs;
}


Expr* Parser::parse_equality_expr()
{
	Expr* lhs = parse_relational_expr();
	while (ts->peek().kind == TokenKind::EQUAL || ts->peek().kind == TokenKind::NOT_EQUAL)
	{
		const Token& op = ts->peek();
		ts->next();
		Expr* rhs = parse_relational_expr();
		if (!rhs)
		{
			logger.log(op.line, op.col);
			throw ParserException("� ������ ���� �Ľ� ����", op.line, op.col);
		}
		lhs = new BinaryExpr(lhs, token_kind_to_string(op.kind), rhs, op.line, op.col);
	}
	return lhs;
}

Expr* Parser::parse_and_expr()
{
	Expr* lhs = parse_equality_expr();
	while (ts->peek().kind == TokenKind::AND)
	{
		const Token& op = ts->peek();
		ts->next();
		Expr* rhs = parse_equality_expr();
		if (!rhs)
		{
			logger.log(op.line, op.col);
			throw ParserException("AND ������ ���� �Ľ� ����", op.line, op.col);
		}
		lhs = new BinaryExpr(lhs, token_kind_to_string(op.kind), rhs, op.line, op.col);
	}
	return lhs;
}

Expr* Parser::parse_or_expr()
{
	Expr* lhs = parse_and_expr();
	while (ts->peek().kind == TokenKind::OR)
	{
		const Token& op = ts->peek();
		ts->next();
		Expr* rhs = parse_and_expr();
		if (!rhs)
		{
			logger.log(op.line, op.col);
			throw ParserException("OR ������ ���� �Ľ� ����", op.line, op.col);
		}
		lhs = new BinaryExpr(lhs, token_kind_to_string(op.kind), rhs, op.line, op.col);
	}
	return lhs;
}

Expr* Parser::parse_assign_expr()
{
	Expr* lhs = parse_or_expr();
	
	while (ts->peek().kind == TokenKind::ASSIGN)
	{
		if (!lhs->can_be_lhs())
		{
			throw ParserException("�º��� �ĺ��ڳ� �迭 �����̾�� ��", lhs->get_location().first, lhs->get_location().second);
		}

		const Token& op = ts->peek();
		ts->next();
		Expr* rhs = parse_assign_expr();
		if (!rhs)
		{
			logger.log(op.line, op.col);
			throw ParserException("�Ҵ� ������ ���� �Ľ� ����", op.line, op.col);
		}
		return new AssignExpr(lhs, rhs, op.line, op.col);
	}
	return lhs;
}

//Type

Type* Parser::parse_type()
{
	string type;
	const Token& tk = ts->peek();
	if (tk.kind == TokenKind::KW_INT || tk.kind == TokenKind::KW_FLOAT || tk.kind == TokenKind::KW_BOOL || tk.kind == TokenKind::KW_STRING || tk.kind == TokenKind::KW_VOID)
	{
		type += token_kind_to_string(ts->peek().kind);
	}
	else
	{
		logger.log(tk.line, tk.col);
		throw ParserException("���ǵ��� ���� Ÿ��", tk.line, tk.col);
	}
	ts->next();

	int dim = 0;

	if (ts->peek().kind == TokenKind::LBRACKET)
	{
		if (tk.kind == TokenKind::KW_VOID)
		{
			logger.log(tk.line, tk.col);
			throw ParserException("void Ÿ���� �迭�� ����� �� ����", tk.line, tk.col);
		}
	}

	while (ts->peek().kind == TokenKind::LBRACKET)
	{
		ts->next();
		if (ts->peek().kind != TokenKind::RBRACKET)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("������ ���ȣ ����", ts->get_line(), ts->get_col());
		}

		dim++;
		ts->next();

		if (dim > 2)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("�迭 ������ �ִ� 2�������� ���", ts->get_line(), ts->get_col());
		}
	}
	
	return new Type(type, dim, tk.line, tk.col);
}


//Stmt
Stmt* Parser::parse_expr_stmt()
{
	int line = ts->get_line(), col = ts->get_col();

	Expr* expr = parse_expr();
	expect_semicolon();
	return new ExprStmt(expr, line, col);
}

Stmt* Parser::parse_var_decl_stmt()
{
	int line = ts->get_line(), col = ts->get_col();

	Type* type = parse_type();
	if (type->get_type() == TypeKind::VOID)
	{
		throw ParserException("void������ ���� ���� �Ұ�", line, col);
	}

	if (ts->peek().kind != TokenKind::IDENTIFIER)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("�ĺ��� ����", ts->get_line(), ts->get_col());
	}

	string name = ts->peek().lexeme;
	ts->next();
	if (ts->peek().kind != TokenKind::ASSIGN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("���� ���� �� �ʱ�ȭ�Ǿ�� ��", ts->get_line(), ts->get_col());
	}
	ts->next();
	Expr* expr = parse_expr();


	expect_semicolon();

	return new VarDeclStmt(type, name, expr, line, col);
}

Stmt* Parser::parse_break_stmt()
{
	const Token& tk = ts->peek();
	if (tk.kind == TokenKind::KW_BREAK)
	{
		ts->next();
		expect_semicolon();
		return new BreakStmt(tk.line, tk.col);
	}

	throw ParserException("break ���� ����", tk.line, tk.col);
}

Stmt* Parser::parse_continue_stmt()
{
	const Token& tk = ts->peek();
	if (tk.kind == TokenKind::KW_CONTINUE)
	{
		ts->next();
		expect_semicolon();
		return new ContinueStmt(tk.line, tk.col);
	}

	throw ParserException("continue ���� ����", tk.line, tk.col);
}

Stmt* Parser::parse_return_stmt()
{
	int line = ts->get_line(), col = ts->get_col();
	if (ts->peek().kind != TokenKind::KW_RETURN)
	{
		throw ParserException("return ���� ����", line, col);
	}
	Expr* expr = nullptr;

	if (ts->peek().kind == TokenKind::SEMICOLON)
	{
		expr = new VoidExpr(line, col);
	}
	else
	{
		expr = parse_expr();
		expect_semicolon();
	}
	return new ReturnStmt(expr, line, col);
}

//TODO: 12-12

//util
const Token& Parser::expect_semicolon()
{
	if (ts->peek().kind != TokenKind::SEMICOLON)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("�����ݷ� ����", ts->get_line(), ts->get_col());
	}
	const Token& ret = ts->peek();
	ts->next();
	return ret;
}