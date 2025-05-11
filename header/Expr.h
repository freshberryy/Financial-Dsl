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

	virtual bool can_be_lhs() const = 0;

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

	bool can_be_lhs() const;

	void dump(std::ostream& os, int indent = 0) const;
	IntLiteralExpr(const IntLiteralExpr&) = delete;
	IntLiteralExpr& operator=(const IntLiteralExpr&) = delete;
	IntLiteralExpr(IntLiteralExpr&&) = delete;
	IntLiteralExpr& operator=(IntLiteralExpr&&) = delete;

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
	bool can_be_lhs() const;

	void dump(std::ostream& os, int indent = 0) const;
	FloatLiteralExpr(const FloatLiteralExpr&) = delete;
	FloatLiteralExpr& operator=(const FloatLiteralExpr&) = delete;
	FloatLiteralExpr(FloatLiteralExpr&&) = delete;
	FloatLiteralExpr& operator=(FloatLiteralExpr&&) = delete;

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
	bool can_be_lhs() const;

	void dump(std::ostream& os, int indent = 0) const;
	StringLiteralExpr(const StringLiteralExpr&) = delete;
	StringLiteralExpr& operator=(const StringLiteralExpr&) = delete;
	StringLiteralExpr(StringLiteralExpr&&) = delete;
	StringLiteralExpr& operator=(StringLiteralExpr&&) = delete;

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
	bool can_be_lhs() const;

	void dump(std::ostream& os, int indent = 0) const;
	BoolLiteralExpr(const BoolLiteralExpr&) = delete;
	BoolLiteralExpr& operator=(const BoolLiteralExpr&) = delete;
	BoolLiteralExpr(BoolLiteralExpr&&) = delete;
	BoolLiteralExpr& operator=(BoolLiteralExpr&&) = delete;

private:
	std::string value;
};


class IdentifierExpr : public Expr
{
public:
	IdentifierExpr(std::string n, int l, int c):Expr(l, c), name(n){}

	std::string to_string() const;

	std::string get_type()const;
	bool can_be_lhs() const;
	void dump(std::ostream& os, int indent = 0) const;
	IdentifierExpr(const IdentifierExpr&) = delete;
	IdentifierExpr& operator=(const IdentifierExpr&) = delete;
	IdentifierExpr(IdentifierExpr&&) = delete;
	IdentifierExpr& operator=(IdentifierExpr&&) = delete;

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
	bool can_be_lhs() const;
	void dump(std::ostream& os, int indent = 0) const;
	UnaryExpr(const UnaryExpr&) = delete;
	UnaryExpr& operator=(const UnaryExpr&) = delete;
	UnaryExpr(UnaryExpr&&) = delete;
	UnaryExpr& operator=(UnaryExpr&&) = delete;
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
	bool can_be_lhs() const;
	void dump(std::ostream& os, int indent = 0) const;
	BinaryExpr(const BinaryExpr&) = delete;
	BinaryExpr& operator=(const BinaryExpr&) = delete;
	BinaryExpr(BinaryExpr&&) = delete;
	BinaryExpr& operator=(BinaryExpr&&) = delete;

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
	bool can_be_lhs() const;
	AssignExpr(const AssignExpr&) = delete;
	AssignExpr& operator=(const AssignExpr&) = delete;
	AssignExpr(AssignExpr&&) = delete;
	AssignExpr& operator=(AssignExpr&&) = delete;
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
	bool can_be_lhs() const;
	Array1DAccessExpr(const Array1DAccessExpr&) = delete;
	Array1DAccessExpr& operator=(const Array1DAccessExpr&) = delete;
	Array1DAccessExpr(Array1DAccessExpr&&) = delete;
	Array1DAccessExpr& operator=(Array1DAccessExpr&&) = delete;
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
	bool can_be_lhs() const;
	Array2DAccessExpr(const Array2DAccessExpr&) = delete;
	Array2DAccessExpr& operator=(const Array2DAccessExpr&) = delete;
	Array2DAccessExpr(Array2DAccessExpr&&) = delete;
	Array2DAccessExpr& operator=(Array2DAccessExpr&&) = delete;
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
	bool can_be_lhs() const;
	FunctionCallExpr(const FunctionCallExpr&) = delete;
	FunctionCallExpr& operator=(const FunctionCallExpr&) = delete;
	FunctionCallExpr(FunctionCallExpr&&) = delete;
	FunctionCallExpr& operator=(FunctionCallExpr&&) = delete;
private:
	Expr* callee;
	std::vector<Expr*> args;
};
