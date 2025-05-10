#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "ASTNode.h"

class Expr : public ASTNode
{
public:

	Expr(int line, int col) : ASTNode(line, col) {}

	virtual ~Expr();

	virtual std::string get_type() const = 0;

};

class IntLiteralExpr : public Expr
{
public:
	IntLiteralExpr(const std::string& v, int line, int col)
		: Expr(line, col), value(v)
	{
	}

	std::string to_string()const;

	std::string get_type()const;

	void dump(std::ostream& os, int indent = 0) const;

private:
	std::string value;
};

class FloatLiteralExpr : public Expr
{
public:
	FloatLiteralExpr(const std::string& v, int line, int col)
		: Expr(line, col), value(v)
	{
	}

	std::string to_string()const;

	std::string get_type()const;

	void dump(std::ostream& os, int indent = 0) const;

private:
	std::string value;
};

class StringLiteralExpr : public Expr
{
public:
	StringLiteralExpr(const std::string& v, int line, int col)
		: Expr(line, col), value(v)
	{
	}

	std::string to_string()const;

	std::string get_type()const;

	void dump(std::ostream& os, int indent = 0) const;

private:
	std::string value;
};

class BoolLiteralExpr : public Expr
{
public:
	BoolLiteralExpr(const std::string& v, int line, int col)
		: Expr(line, col), value(v)
	{
	}

	std::string to_string()const;

	std::string get_type()const;

	void dump(std::ostream& os, int indent = 0) const;

private:
	std::string value;
};


class IdentifierExpr : public Expr
{
public:
	IdentifierExpr(std::string n, int l, int c):Expr(l, c), name(n){}

	std::string to_string() const;

	std::string get_type()const;

	void dump(std::ostream& os, int indent = 0) const;

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

	void dump(std::ostream& os, int indent = 0) const;
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

	void dump(std::ostream& os, int indent = 0) const;

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
	std::string get_type()const;
	void dump(std::ostream& os, int indent = 0) const;
private:
	Expr* lhs;
	Expr* rhs;
};

class Array1DAccessExpr : public Expr
{
public:
	Array1DAccessExpr(Expr* base, Expr* index, int line, int col)
		: Expr(line, col), base(base), index(index)
	{
	}

	~Array1DAccessExpr();

	std::string to_string() const override;
	std::string get_type() const override;
	void dump(std::ostream& os, int indent = 0) const override;

private:
	Expr* base;
	Expr* index;
};

class Array2DAccessExpr : public Expr
{
public:
	Array2DAccessExpr(Expr* base, Expr* index1, Expr* index2, int line, int col)
		: Expr(line, col), base(base), index1(index1), index2(index2)
	{
	}

	~Array2DAccessExpr();

	std::string to_string() const override;
	std::string get_type() const override;
	void dump(std::ostream& os, int indent = 0) const override;

private:
	Expr* base;
	Expr* index1;
	Expr* index2;
};

class FunctionCallExpr : public Expr
{
public:
	FunctionCallExpr(Expr* callee, const std::vector<Expr*>& args, int line, int col) : Expr(line, col), callee(callee), args(args){}

	~FunctionCallExpr();

	std::string to_string() const;

	std::string get_type()const;
	void dump(std::ostream& os, int indent = 0) const;
private:
	Expr* callee;
	std::vector<Expr*> args;
};
