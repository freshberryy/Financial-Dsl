#include "Parser.h"
using namespace std;

Parser::Parser(TokenStream* ts, Logger lg) :ts(ts), logger(lg)
{
	if (ts == nullptr)
	{
		throw invalid_argument("��� �ִ� ��ū ��Ʈ��");
	}
}


ASTNode* Parser::parse_program()
{
	int line = ts->peek().line, col = ts->peek().col;

	vector<Stmt*> stmts;
	while (!ts->eof())
	{
		stmts.push_back(parse_stmt());
	}
	return new BlockStmt(stmts, line, col);
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

	logger.log(tk.line, tk.col);
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
	logger.log(tk.line, tk.col);
	throw ParserException("continue ���� ����", tk.line, tk.col);
}

Stmt* Parser::parse_return_stmt()
{
	int line = ts->get_line(), col = ts->get_col();
	if (ts->peek().kind != TokenKind::KW_RETURN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("return ���� ����", line, col);
	}
	ts->next();
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

Stmt* Parser::parse_if_stmt()
{
	int line = ts->peek().line, col = ts->peek().col;

	if (ts->peek().kind != TokenKind::KW_IF)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("if ���� ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	if (ts->peek().kind != TokenKind::LPAREN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("���� �Ұ�ȣ ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	Expr* condition = parse_expr();
	if (ts->peek().kind != TokenKind::RPAREN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("������ �Ұ�ȣ ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	BlockStmt* then_branch = parse_block_stmt();
	vector<pair<Expr*, BlockStmt*>>else_if_branches;
	if (ts->peek().kind == TokenKind::KW_ELSE_IF)
	{
		while (ts->peek().kind == TokenKind::KW_ELSE_IF)
		{
			ts->next();
			if (ts->peek().kind != TokenKind::LPAREN)
			{
				logger.log(ts->get_line(), ts->get_col());
				throw ParserException("���� �Ұ�ȣ ����", ts->get_line(), ts->get_col());
			}
			Expr* con = parse_expr();
			BlockStmt* blo = parse_block_stmt();
			else_if_branches.emplace_back(con, blo);
		}
	}
	
	BlockStmt* else_branch = nullptr;
	if (ts->peek().kind == TokenKind::KW_ELSE)
	{
		ts->next();
		else_branch = parse_block_stmt();
	}
	return new IfStmt(condition, then_branch, else_if_branches, else_branch, line, col);
}

Stmt* Parser::parse_while_stmt()
{
	int line = ts->peek().line, col = ts->peek().col;
	if (ts->peek().kind != TokenKind::KW_WHILE)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("while ���� ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	if (ts->peek().kind != TokenKind::LPAREN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("���� �Ұ�ȣ ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	Expr* condition = parse_expr();
	if (ts->peek().kind != TokenKind::RPAREN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("������ �Ұ�ȣ ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	BlockStmt* body = parse_block_stmt();

	return new WhileStmt(condition, body, line, col);
}

Stmt* Parser::parse_for_stmt()
{
	int line = ts->peek().line, col = ts->peek().col;
	if (ts->peek().kind != TokenKind::KW_FOR)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("for ���� ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	if (ts->peek().kind != TokenKind::LPAREN)
	{
		throw ParserException("���� �Ұ�ȣ ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	Expr* init;
	try
	{
		init = parse_expr();
	}
	catch (const exception&)
	{
		init = nullptr;
	}
	if (ts->peek().kind != TokenKind::SEMICOLON)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("���ǽ� �� �����ݷ� ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	Expr* cond;
	try
	{
		cond = parse_expr();
	}
	catch (const exception&)
	{
		cond = nullptr;
	}
	if (ts->peek().kind != TokenKind::SEMICOLON)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("���ǽ� �� �����ݷ� ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	Expr* post;
	try
	{
		post = parse_expr();
	}
	catch (const exception&)
	{
		post = nullptr;
	}
	if (ts->peek().kind != TokenKind::RPAREN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("������ ��ȣ ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	BlockStmt* body = parse_block_stmt();

	return new ForStmt(init, cond, post, body, line, col);
}

BlockStmt* Parser::parse_block_stmt()
{
	int line = ts->peek().line, col = ts->peek().col;
	vector<Stmt*> stmts;
	if (ts->peek().kind != TokenKind::LBRACE)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("���� �߰�ȣ ����", ts->get_line(), ts->get_col());
	}
	ts->next();
	while (ts->peek().kind != TokenKind::RBRACE)
	{
		if (ts->eof())
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("������ �߰�ȣ ����", ts->get_line(), ts->get_col());
		}
		stmts.emplace_back(parse_stmt());
	}
	ts->next();
	return new BlockStmt(stmts, line, col);
}

Stmt* Parser::parse_func_decl_stmt()
{
	int line = ts->peek().line, col = ts->peek().col;
	//Ÿ��
	Type* type = parse_type();

	//�̸�
	string name = ts->peek().lexeme;
	ts->next();

	//�Ķ����  -- ���� ��ȣ���� ����ó�� Ȯ��
	vector<Param*> params;
	ts->next();
	while (ts->peek().kind != TokenKind::RPAREN)
	{
		//�Ķ���� Ÿ��
		Type* p_type = parse_type();
		
		//�Ķ���� �̸�
		if (ts->peek().kind != TokenKind::IDENTIFIER)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("�Ķ���� �̸��� �ĺ��ڰ� �ƴ�", ts->get_line(), ts->get_col());
		}
		string p_name = ts->peek().lexeme;
		ts->next();

		params.emplace_back(new Param(p_type, p_name, p_type->get_location().first, p_type->get_location().second));

		if (ts->peek().kind == TokenKind::COMMA)
		{
			ts->next();
			if (ts->peek().kind == TokenKind::RPAREN)
			{
				logger.log(ts->get_line(), ts->get_col());
				throw ParserException("�Ķ���� ���� ����", ts->get_line(), ts->get_col());
			}
		}
		else break;
	}
	//���� Ż��->������ ��ȣ
	if (ts->peek().kind != TokenKind::RPAREN)
	{
		logger.log(line, col);
		throw ParserException("������ �Ұ�ȣ ����", line, col);
	}
	ts->next();

	FunctionPrototype* proto = new FunctionPrototype(name, params, type, line, col);

	BlockStmt* body = parse_block_stmt();

	return new FuncDeclStmt(proto, body, line, col);
}

Stmt* Parser::parse_stmt()
{
	TokenKind kind = ts->peek().kind;

	switch (kind)
	{
	case TokenKind::KW_INT:
	case TokenKind::KW_FLOAT:
	case TokenKind::KW_BOOL:
	case TokenKind::KW_STRING:
	case TokenKind::KW_VOID: 
	{
		TokenKind next = ts->peek(1).kind;

		if (next == TokenKind::IDENTIFIER)
		{
			TokenKind nnext = ts->peek(2).kind;
			if (nnext == TokenKind::LPAREN)
			{
				return parse_func_decl_stmt();
			}
			else
			{
				return parse_var_decl_stmt();
			}
		}
		else
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("Ÿ�� �ڿ� �ĺ��ڰ� �ƴ�", ts->get_line(), ts->get_col());
		}
	}

	case TokenKind::KW_IF:
		return parse_if_stmt();

	case TokenKind::KW_WHILE:
		return parse_while_stmt();

	case TokenKind::KW_FOR:
		return parse_for_stmt();

	case TokenKind::KW_BREAK:
		return parse_break_stmt();

	case TokenKind::KW_CONTINUE:
		return parse_continue_stmt();

	case TokenKind::KW_RETURN:
		return parse_return_stmt();

	case TokenKind::LBRACE:
		return parse_block_stmt();

	default:
		return parse_expr_stmt();  
	}
}


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