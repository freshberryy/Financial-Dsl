#include "Stmt.h"
#include "FunctionUtil.h"

Stmt::~Stmt() {}

ExprStmt::~ExprStmt()
{
	delete expr;
}

VarDeclStmt::~VarDeclStmt()
{
	delete init;
}

ReturnStmt::~ReturnStmt()
{
	delete expr;
}

IfStmt::~IfStmt()
{
	delete condition;
	delete then_branch;
	for (int i = 0; i < else_if_branches.size(); i++)
	{
		Expr* e = else_if_branches[i].first;
		BlockStmt* b = else_if_branches[i].second;

		delete e;
		delete b;
	}
	delete else_branch;
}

WhileStmt::~WhileStmt()
{
	delete condition;
	delete body;
}

ForStmt::~ForStmt()
{
	delete init;
	delete cond;
	delete post;
	delete body;
}

BlockStmt::~BlockStmt()
{
	for (Stmt* s : statements)
	{
		delete s;
	}
}

FuncDeclStmt::~FuncDeclStmt()
{
	delete prototype;
	delete body;
}


std::string ExprStmt::to_string() const
{
	return "ExprStmt";
}

std::string VarDeclStmt::to_string() const
{
	return "VarDeclStmt";
}

std::string ReturnStmt::to_string() const
{
	return "ReturnStmt";
}

std::string IfStmt::to_string() const
{
	return "IfStmt";
}

std::string WhileStmt::to_string() const
{
	return "WhileStmt";
}

std::string ForStmt::to_string() const
{
	return "ForStmt";
}

std::string BlockStmt::to_string() const
{
	return "BlockStmt";
}

std::string FuncDeclStmt::to_string() const
{
	return "FuncDeclStmt";
}

