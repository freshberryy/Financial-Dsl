#include "Expr.h"

Expr::~Expr() {}

UnaryExpr::~UnaryExpr()
{
	delete operand;
}

BinaryExpr::~BinaryExpr()
{
	delete lhs;
	delete rhs;
}

AssignExpr::~AssignExpr()
{
	delete lhs;
	delete rhs;
}

ArrayAccessExpr::~ArrayAccessExpr()
{
	delete base;

	for (int i = 0; i < indices.size(); i++)
	{
		delete indices[i];
	}
}

FunctionCallExpr::~FunctionCallExpr()
{
	delete callee;
	for (Expr* e : args)
	{
		delete e;
	}
}


std::string LiteralExpr::to_string() const
{
	return "Literal(" + value + " : " + type + ")";
}

std::string IdentifierExpr::to_string() const
{
	return "Identifier(" + name + ")";
}

std::string UnaryExpr::to_string() const
{
	return "Unary(" + op + " " + (operand ? operand->to_string() : "null") + ")";
}

std::string BinaryExpr::to_string() const
{
	return "Binary(" + (lhs ? lhs->to_string() : "null") + " " + op + " " + (rhs ? rhs->to_string() : "null") + ")";
}

std::string AssignExpr::to_string() const
{
	return "Assign(" + (lhs ? lhs->to_string() : "null") + " = " + (rhs ? rhs->to_string() : "null") + ")";
}

std::string ArrayAccessExpr::to_string() const
{
	std::string result = "ArrayAccess(" + (base ? base->to_string() : "null") + "[";
	for (size_t i = 0; i < indices.size(); ++i)
	{
		if (i > 0) result += ", ";
		result += (indices[i] ? indices[i]->to_string() : "null");
	}
	result += "])";
	return result;
}

std::string FunctionCallExpr::to_string() const
{
	std::string result = "Call(" + (callee ? callee->to_string() : "null") + "(";
	for (size_t i = 0; i < args.size(); ++i)
	{
		if (i > 0) result += ", ";
		result += (args[i] ? args[i]->to_string() : "null");
	}
	result += "))";
	return result;
}
