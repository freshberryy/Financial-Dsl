#pragma once
#include <string>
#include <vector>
#include "ASTNode.h"

class Expr : public ASTNode
{
public:

	Expr(int line, int col) : ASTNode(line, col) {}

	virtual ~Expr();

	virtual std::string to_string() = 0;
	virtual std::string get_type() = 0;

};

class LiteralExpr : public Expr
{
public:
	LiteralExpr(std::string v, std::string t, int line, int col) : Expr(line, col), value(v), type(t) {}

	std::string to_string() const;

	std::string get_type()const;

private:
	std::string value, type;
};

class IdentifierExpr : public Expr
{
public:
	IdentifierExpr(std::string n, int l, int c):Expr(l, c), name(n){}

	std::string to_string() const;

	std::string get_type()const;

private:
	std::string name;
};

class UnaryExpr : public Expr
{
public:
	UnaryExpr(Expr* ex, std::string op, int line, int col) :Expr(line, col), operand(ex), op(op) {}

	~UnaryExpr();

	std::string to_string() const;

	std::string get_type()const;
private:
	Expr* operand;
	std::string op;
};

class BinaryExpr : public Expr
{
public:
	BinaryExpr(Expr* lhs, std::string op, Expr* rhs, int line, int col) :Expr(line, col), lhs(lhs), rhs(rhs), op(op) {}

	~BinaryExpr();

	std::string to_string() const;

	std::string get_type()const;

private:
	Expr* lhs;
	Expr* rhs;
	std::string op;
};

class AssignExpr : public Expr
{
public:
	AssignExpr(Expr* lhs, Expr* rhs, int line, int col) :Expr(line, col), lhs(lhs), rhs(rhs) {}

	~AssignExpr();

	std::string to_string() const;

private:
	Expr* lhs;
	Expr* rhs;
};

class ArrayAccessExpr : public Expr
{
public:
	ArrayAccessExpr(Expr* base, const std::vector<Expr*>& indices, int line, int col) : Expr(line, col), base(base), indices(std::move(indices)) {}

	~ArrayAccessExpr();

	std::string to_string() const;



private:
	Expr* base;
	std::vector<Expr*>indices;
};

class FunctionCallExpr : public Expr
{
public:
	FunctionCallExpr(Expr* callee, const std::vector<Expr*>& args, int line, int col) : Expr(line, col), callee(callee), args(std::move(args)){}

	~FunctionCallExpr();

	std::string to_string() const;

	std::string get_type()const;
private:
	Expr* callee;
	std::vector<Expr*> args;
};
