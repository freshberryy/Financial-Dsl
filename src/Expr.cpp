#include "Expr.h"
using namespace std;

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

Array1DAccessExpr::~Array1DAccessExpr()
{
	delete base;
	delete index;
}

Array2DAccessExpr::~Array2DAccessExpr()
{
	delete base;
	delete index1;
	delete index2;
}

FunctionCallExpr::~FunctionCallExpr()
{
	delete callee;
	for (Expr* e : args)
	{
		delete e;
	}
}


string IntLiteralExpr::to_string() const
{
	return value;
}

string IntLiteralExpr::get_type() const
{
	return "int";
}

string FloatLiteralExpr::to_string() const
{
	return value;
}

string FloatLiteralExpr::get_type() const
{
	return "float";
}

string StringLiteralExpr::to_string() const
{
	return "\"" + value + "\"";
}

string StringLiteralExpr::get_type() const
{
	return "string";
}

string BoolLiteralExpr::to_string() const
{
	return value == "true" ? "true" : "false";
}

string BoolLiteralExpr::get_type() const
{
	return "bool";
}

string IdentifierExpr::to_string() const
{
	return name;
}

string IdentifierExpr::get_type() const
{
	return "unknown";
}

string UnaryExpr::to_string() const
{
	return "(" + op + operand->to_string() + ")";
}

string UnaryExpr::get_type() const
{
	return operand->get_type();
}

string BinaryExpr::to_string() const
{
	return "(" + lhs->to_string() + " " + op + " " + rhs->to_string() + ")";
}

string BinaryExpr::get_type() const
{
	string ltype = lhs->get_type();
	string rtype = rhs->get_type();
	return (ltype == rtype) ? ltype : "unknown";
}

string AssignExpr::to_string() const
{
	return "(" + lhs->to_string() + " = " + rhs->to_string() + ")";
}

string AssignExpr::get_type() const
{
	return rhs->get_type();
}

string Array1DAccessExpr::to_string() const
{
	return base->to_string() + "[" + index->to_string() + "]";
}

string Array1DAccessExpr::get_type() const
{
	string base_type = base->get_type();
	if (base_type.size() >= 2 && base_type.substr(base_type.size() - 2) == "[]")
		return base_type.substr(0, base_type.size() - 2);
	return "unknown";
}

string Array2DAccessExpr::to_string() const
{
	return base->to_string() + "[" + index1->to_string() + "]" + "[" + index2->to_string() + "]";
}

string Array2DAccessExpr::get_type() const
{
	string base_type = base->get_type();
	if (base_type.size() >= 4 && base_type.substr(base_type.size() - 4) == "[][]")
		return base_type.substr(0, base_type.size() - 4);
	return "unknown";
}

string FunctionCallExpr::to_string() const
{
	ostringstream oss;
	oss << callee->to_string() << "(";
	for (size_t i = 0; i < args.size(); ++i)
	{
		if (i > 0) oss << ", ";
		oss << args[i]->to_string();
	}
	oss << ")";
	return oss.str();
}

string FunctionCallExpr::get_type() const
{
	return "unknown";
}


static void print_indent(ostream& os, int indent)
{
	os << setw(indent) << ' ';
}

void IntLiteralExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "IntLiteralExpr: " << to_string() << "\n";
}

void FloatLiteralExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "FloatLiteralExpr: " << to_string() << "\n";
}

void StringLiteralExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "StringLiteralExpr: " << to_string() << "\n";
}

void BoolLiteralExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "BoolLiteralExpr: " << to_string() << "\n";
}

void IdentifierExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "IdentifierExpr: " << to_string() << "\n";
}

void UnaryExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "UnaryExpr: " << to_string() << "\n";
	operand->dump(os, indent + 2);
}

void BinaryExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "BinaryExpr: " << to_string() << "\n";
	lhs->dump(os, indent + 2);
	rhs->dump(os, indent + 2);
}

void AssignExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "AssignExpr: " << to_string() << "\n";
	lhs->dump(os, indent + 2);
	rhs->dump(os, indent + 2);
}

void Array1DAccessExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "Array1DAccessExpr: " << to_string() << "\n";
	base->dump(os, indent + 2);
	index->dump(os, indent + 4);
}

void Array2DAccessExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "Array2DAccessExpr: " << to_string() << "\n";
	base->dump(os, indent + 2);
	index1->dump(os, indent + 4);
	index2->dump(os, indent + 4);
}

void FunctionCallExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "FunctionCallExpr: " << to_string() << "\n";
	callee->dump(os, indent + 2);
	for (auto arg : args)
	{
		arg->dump(os, indent + 4);
	}
}

void VoidExpr::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "VoidExpr: " << to_string() << "\n";
}


bool IntLiteralExpr::can_be_lhs() const
{
	return false;
}

bool FloatLiteralExpr::can_be_lhs() const
{
	return false;
}

bool StringLiteralExpr::can_be_lhs() const
{
	return false;
}

bool BoolLiteralExpr::can_be_lhs() const
{
	return false;
}

bool UnaryExpr::can_be_lhs() const
{
	return false;
}

bool BinaryExpr::can_be_lhs() const
{
	return false;
}

bool AssignExpr::can_be_lhs() const
{
	return false;
}

bool FunctionCallExpr::can_be_lhs() const
{
	return false;
}


bool IdentifierExpr::can_be_lhs() const
{
	return true;
}

bool Array1DAccessExpr::can_be_lhs() const
{
	return true;
}

bool Array2DAccessExpr::can_be_lhs() const
{
	return true;
}

string VoidExpr::to_string() const
{
	return "void";
}

string VoidExpr::get_type() const
{
	return "void";
}



bool VoidExpr::can_be_lhs() const
{
	return false;
}