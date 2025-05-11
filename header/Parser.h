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

class ProgramNode;

class Parser
{
public:
	Parser(TokenStream* ts, Logger lg);

	ASTNode* parse();

	ASTNode* parse_program();


	Expr* parse_literal();
	Expr* parse_identifier();
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
	Expr* parse_lhs();
	Expr* parse_assign_expr();

	Type* parse_type();


	
	bool is_semicolon();

private:

	

	TokenStream* ts;
	std::vector<Stmt*> stmts;
	Logger logger;
	int unary_depth = 0;
};