#include "Parser.h"
using namespace std;

Parser::Parser(TokenStream* ts, Logger lg) :ts(ts), logger(lg)
{
	if (ts == nullptr)
	{
		throw invalid_argument("비어 있는 토큰 스트림");
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

	if (ts->peek().kind == TokenKind::PLUS || ts->peek().kind == TokenKind::MINUS || ts->peek().kind == TokenKind::NOT)
	{
		const Token& op = ts->peek();
		ts->next();

		Expr* expr = parse_unary_expr();
		if (!expr)
		{
			logger.log(op.line, op.col);
			throw ParserException("단항 연산자 뒤에 표현식이 없음", op.line, op.col);
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
			throw ParserException("*/% 연산자 우항 파싱 실패", op.line, op.col);
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
			throw ParserException("+- 연산자 우항 파싱 실패", op.line, op.col);
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
			throw ParserException("관계 연산자 우항 파싱 실패", op.line, op.col);
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
			throw ParserException("등가 연산자 우항 파싱 실패", op.line, op.col);
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
			throw ParserException("AND 연산자 우항 파싱 실패", op.line, op.col);
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
			throw ParserException("OR 연산자 우항 파싱 실패", op.line, op.col);
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
			throw ParserException("좌변은 식별자나 배열 접근이어야 함", lhs->get_location().first, lhs->get_location().second);
		}

		const Token& op = ts->peek();
		ts->next();
		Expr* rhs = parse_assign_expr();
		if (!rhs)
		{
			logger.log(op.line, op.col);
			throw ParserException("할당 연산자 우항 파싱 실패", op.line, op.col);
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
		throw ParserException("정의되지 않은 타입", tk.line, tk.col);
	}
	ts->next();

	int dim = 0;

	if (ts->peek().kind == TokenKind::LBRACKET)
	{
		if (tk.kind == TokenKind::KW_VOID)
		{
			logger.log(tk.line, tk.col);
			throw ParserException("void 타입은 배열로 사용할 수 없음", tk.line, tk.col);
		}
	}

	while (ts->peek().kind == TokenKind::LBRACKET)
	{
		ts->next();
		if (ts->peek().kind != TokenKind::RBRACKET)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("오른쪽 대괄호 누락", ts->get_line(), ts->get_col());
		}

		dim++;
		ts->next();

		if (dim > 2)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("배열 차원은 최대 2차원까지 허용", ts->get_line(), ts->get_col());
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
		throw ParserException("void형으로 변수 선언 불가", line, col);
	}

	if (ts->peek().kind != TokenKind::IDENTIFIER)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("식별자 누락", ts->get_line(), ts->get_col());
	}

	string name = ts->peek().lexeme;
	ts->next();
	if (ts->peek().kind != TokenKind::ASSIGN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("변수 선언 시 초기화되어야 함", ts->get_line(), ts->get_col());
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
	throw ParserException("break 구문 오류", tk.line, tk.col);
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
	throw ParserException("continue 구문 오류", tk.line, tk.col);
}

Stmt* Parser::parse_return_stmt()
{
	int line = ts->get_line(), col = ts->get_col();
	if (ts->peek().kind != TokenKind::KW_RETURN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("return 구문 오류", line, col);
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
		throw ParserException("if 구문 오류", ts->get_line(), ts->get_col());
	}
	ts->next();
	if (ts->peek().kind != TokenKind::LPAREN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("왼쪽 소괄호 누락", ts->get_line(), ts->get_col());
	}
	ts->next();
	Expr* condition = parse_expr();
	if (ts->peek().kind != TokenKind::RPAREN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("오른쪽 소괄호 누락", ts->get_line(), ts->get_col());
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
				throw ParserException("왼쪽 소괄호 누락", ts->get_line(), ts->get_col());
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
		throw ParserException("while 구문 오류", ts->get_line(), ts->get_col());
	}
	ts->next();
	if (ts->peek().kind != TokenKind::LPAREN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("왼쪽 소괄호 누락", ts->get_line(), ts->get_col());
	}
	ts->next();
	Expr* condition = parse_expr();
	if (ts->peek().kind != TokenKind::RPAREN)
	{
		logger.log(ts->get_line(), ts->get_col());
		throw ParserException("오른쪽 소괄호 누락", ts->get_line(), ts->get_col());
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
		throw ParserException("for 구문 오류", ts->get_line(), ts->get_col());
	}
	ts->next();
	if (ts->peek().kind != TokenKind::LPAREN)
	{
		throw ParserException("왼쪽 소괄호 누락", ts->get_line(), ts->get_col());
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
		throw ParserException("조건식 내 세미콜론 누락", ts->get_line(), ts->get_col());
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
		throw ParserException("조건식 내 세미콜론 누락", ts->get_line(), ts->get_col());
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
		throw ParserException("오른쪽 괄호 누락", ts->get_line(), ts->get_col());
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
		throw ParserException("왼쪽 중괄호 누락", ts->get_line(), ts->get_col());
	}
	ts->next();
	while (ts->peek().kind != TokenKind::RBRACE)
	{
		if (ts->eof())
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("오른쪽 중괄호 누락", ts->get_line(), ts->get_col());
		}
		stmts.emplace_back(parse_stmt());
	}
	ts->next();
	return new BlockStmt(stmts, line, col);
}

Stmt* Parser::parse_func_decl_stmt()
{
	int line = ts->peek().line, col = ts->peek().col;
	//타입
	Type* type = parse_type();

	//이름
	string name = ts->peek().lexeme;
	ts->next();

	//파라미터  -- 왼쪽 괄호까지 디스패처가 확인
	vector<Param*> params;
	ts->next();
	while (ts->peek().kind != TokenKind::RPAREN)
	{
		//파라미터 타입
		Type* p_type = parse_type();
		
		//파라미터 이름
		if (ts->peek().kind != TokenKind::IDENTIFIER)
		{
			logger.log(ts->get_line(), ts->get_col());
			throw ParserException("파라미터 이름이 식별자가 아님", ts->get_line(), ts->get_col());
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
				throw ParserException("파라미터 선언 오류", ts->get_line(), ts->get_col());
			}
		}
		else break;
	}
	//루프 탈출->오른쪽 괄호
	if (ts->peek().kind != TokenKind::RPAREN)
	{
		logger.log(line, col);
		throw ParserException("오른쪽 소괄호 누락", line, col);
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
			throw ParserException("타입 뒤에 식별자가 아님", ts->get_line(), ts->get_col());
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
		throw ParserException("세미콜론 누락", ts->get_line(), ts->get_col());
	}
	const Token& ret = ts->peek();
	ts->next();
	return ret;
}