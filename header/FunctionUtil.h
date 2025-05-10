#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "Type.h"
#include "Stmt.h"
#include "ASTNode.h"

class Param;

class FunctionPrototype : public ASTNode
{
public:
	FunctionPrototype(std::string n, std::vector<Param*>p, Type rt, int line, int col) : ASTNode(line, col), name(n), params(p),  return_type(rt){}
	std::string to_string() const;
	void dump(std::ostream& os, int indent = 0) const;
	~FunctionPrototype();

private:
	std::string name;
	std::vector<Param*> params;
	Type return_type;
};

class Param : public ASTNode
{
public:
	Param(Type t, std::string name, int line, int col):ASTNode(line, col), type(t), name(name){}

	std::string to_string() const;
	void dump(std::ostream& os, int indent = 0) const;
private:
	Type type;
	std::string name;
};

class FunctionSymbol
{
public:
	FunctionSymbol(FunctionPrototype* p, BlockStmt* b, int s) :proto(p), body(b), scope_depth(s) {}

	~FunctionSymbol();


private:
	FunctionPrototype* proto;
	BlockStmt* body;
	int scope_depth;
};