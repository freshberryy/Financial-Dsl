#pragma once
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "ASTNode.h"
#include "Type.h"
#include "Expr.h"

class BlockStmt;
class FunctionPrototype;


class Stmt : public ASTNode
{
public:
	Stmt() {}
	Stmt(int line, int col) : ASTNode(line, col) {}
	virtual ~Stmt();

	Stmt(const Stmt&) = delete;
	Stmt& operator=(const Stmt&) = delete;
	Stmt(Stmt&&) = delete;
	Stmt& operator=(Stmt&&) = delete;
};


class ExprStmt : public Stmt
{
public:
	ExprStmt(Expr* expr, int line, int col) : Stmt(line, col), expr(expr) {}
	std::string to_string() const override;

	~ExprStmt();

	ExprStmt(const ExprStmt&) = delete;
	ExprStmt& operator=(const ExprStmt&) = delete;
	ExprStmt(ExprStmt&&) = delete;
	ExprStmt& operator=(ExprStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;

private:
	Expr* expr;
};

class VarDeclStmt : public Stmt
{
public:
	VarDeclStmt(Type* type, std::string name, Expr* init, int line, int col)
		: Stmt(line, col), type(type), name(name), init(init)
	{
	}
	std::string to_string() const override;

	~VarDeclStmt();

	VarDeclStmt(const VarDeclStmt&) = delete;
	VarDeclStmt& operator=(const VarDeclStmt&) = delete;
	VarDeclStmt(VarDeclStmt&&) = delete;
	VarDeclStmt& operator=(VarDeclStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;

private:
	Type* type;
	std::string name;
	Expr* init;
};

class ReturnStmt : public Stmt
{
public:
	ReturnStmt(Expr* e, int line, int col) : Stmt(line, col), expr(e) {}
	std::string to_string() const override;

	~ReturnStmt();

	ReturnStmt(const ReturnStmt&) = delete;
	ReturnStmt& operator=(const ReturnStmt&) = delete;
	ReturnStmt(ReturnStmt&&) = delete;
	ReturnStmt& operator=(ReturnStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;

private:
	Expr* expr;
};

class BreakStmt : public Stmt
{
public:
	BreakStmt(int line, int col) : Stmt(line, col) {}
	std::string to_string() const override;

	BreakStmt(const BreakStmt&) = delete;
	BreakStmt& operator=(const BreakStmt&) = delete;
	BreakStmt(BreakStmt&&) = delete;
	BreakStmt& operator=(BreakStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;
};

class ContinueStmt : public Stmt
{
public:
	ContinueStmt(int line, int col) : Stmt(line, col) {}
	std::string to_string() const override;

	ContinueStmt(const ContinueStmt&) = delete;
	ContinueStmt& operator=(const ContinueStmt&) = delete;
	ContinueStmt(ContinueStmt&&) = delete;
	ContinueStmt& operator=(ContinueStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;
};

class IfStmt : public Stmt
{
public:
	IfStmt(Expr* c, BlockStmt* tb, std::vector<std::pair<Expr*, BlockStmt*>> eib, BlockStmt* eb, int line, int col)
		: Stmt(line, col), condition(c), then_branch(tb), else_if_branches(eib), else_branch(eb)
	{
	}
	std::string to_string() const override;

	~IfStmt();

	IfStmt(const IfStmt&) = delete;
	IfStmt& operator=(const IfStmt&) = delete;
	IfStmt(IfStmt&&) = delete;
	IfStmt& operator=(IfStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;

private:
	Expr* condition;
	BlockStmt* then_branch;
	std::vector<std::pair<Expr*, BlockStmt*>> else_if_branches;
	BlockStmt* else_branch;
};

class WhileStmt : public Stmt
{
public:
	WhileStmt(Expr* c, BlockStmt* b, int line, int col) : Stmt(line, col), condition(c), body(b) {}
	std::string to_string() const override;

	~WhileStmt();

	WhileStmt(const WhileStmt&) = delete;
	WhileStmt& operator=(const WhileStmt&) = delete;
	WhileStmt(WhileStmt&&) = delete;
	WhileStmt& operator=(WhileStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;

private:
	Expr* condition;
	BlockStmt* body;
};

class ForStmt : public Stmt
{
public:
	ForStmt(Expr* init, Expr* c, Expr* p, BlockStmt* b, int line, int col)
		: Stmt(line, col), init(init), cond(c), post(p), body(b)
	{
	}
	std::string to_string() const override;

	~ForStmt();

	ForStmt(const ForStmt&) = delete;
	ForStmt& operator=(const ForStmt&) = delete;
	ForStmt(ForStmt&&) = delete;
	ForStmt& operator=(ForStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;

private:
	Expr* init;
	Expr* cond;
	Expr* post;
	BlockStmt* body;
};

class BlockStmt : public Stmt
{
public:
	BlockStmt(int line, int col)
		: Stmt(line, col)
	{
		stmts.clear();
	}
	std::string to_string() const override;

	~BlockStmt();

	BlockStmt(const BlockStmt&) = delete;
	BlockStmt& operator=(const BlockStmt&) = delete;
	BlockStmt(BlockStmt&&) = delete;
	BlockStmt& operator=(BlockStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;
	void add_stmt(Stmt* stmt);

private:
	std::vector<Stmt*> stmts;
};

class FuncDeclStmt : public Stmt
{
public:
	FuncDeclStmt(FunctionPrototype* pro, BlockStmt* bo, int line, int col)
		: Stmt(line, col), prototype(pro), body(bo)
	{
	}
	std::string to_string() const override;

	~FuncDeclStmt();

	FuncDeclStmt(const FuncDeclStmt&) = delete;
	FuncDeclStmt& operator=(const FuncDeclStmt&) = delete;
	FuncDeclStmt(FuncDeclStmt&&) = delete;
	FuncDeclStmt& operator=(FuncDeclStmt&&) = delete;
	void dump(std::ostream& os, int indent = 0) const;
private:
	FunctionPrototype* prototype;
	BlockStmt* body;
};

