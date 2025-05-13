#pragma once

#include <vector>
#include <stdexcept>
#include "TokenStream.h"
#include "Logger.h"
#include "Stmt.h"
#include "Expr.h"
#include "Token.h"
#include "Type.h"
#include "ParserException.h"
#include "FunctionUtil.h"

class ProgramNode;

class Parser
{
public:
	Parser(TokenStream* ts, Logger lg);


	ASTNode* parse_program();


	Expr* parse_primary_expr();
	Expr* parse_postfix_expr();
	std::vector<Expr*> parse_arg_list();
	Expr* parse_unary_expr();
	Expr* parse_mul_expr();
	Expr* parse_add_expr();
	Expr* parse_relational_expr();
	Expr* parse_equality_expr();
	Expr* parse_and_expr();
	Expr* parse_or_expr();
	Expr* parse_expr();
	Expr* parse_assign_expr();

	Type* parse_type();

	Stmt* parse_expr_stmt();
	Stmt* parse_var_decl_stmt();
	Stmt* parse_break_stmt();
	Stmt* parse_continue_stmt();
	Stmt* parse_return_stmt();
	Stmt* parse_if_stmt();
	Stmt* parse_while_stmt();
	Stmt* parse_for_stmt();
	BlockStmt* parse_block_stmt();
	Stmt* parse_func_decl_stmt();
	Stmt* parse_stmt();
	
	const Token& expect_semicolon();

private:

	

	TokenStream* ts;
	std::vector<Stmt*> stmts;
	Logger logger;
	int unary_depth = 0;
};