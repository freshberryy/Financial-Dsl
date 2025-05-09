#pragma once
#include <string>
#include <vector>
#include "Type.h"
#include "Stmt.h"

class Param;

class FunctionPrototype
{
public:
	FunctionPrototype(std::string n, std::vector<Param*>p, Type rt) : name(n), params(p),  return_type(rt){}


	~FunctionPrototype();

private:
	std::string name;
	std::vector<Param*> params;
	Type return_type;
};

class Param
{
public:
	Param(Type t, std::string name): type(t), name(name){}


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