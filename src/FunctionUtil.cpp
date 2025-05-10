#include "FunctionUtil.h"
using namespace std;

FunctionPrototype::~FunctionPrototype()
{
	for (Param* p : params)
	{
		delete p;
	}
}

FunctionSymbol::~FunctionSymbol()
{
	delete proto;
	delete body;
}

static void print_indent(ostream& os, int indent)
{
	os << setw(indent) << ' ';
}

string FunctionPrototype::to_string() const
{
	ostringstream oss;
	oss << return_type.to_string() << " " << name << "(";
	for (size_t i = 0; i < params.size(); ++i)
	{
		if (i > 0) oss << ", ";
		oss << params[i]->to_string();
	}
	oss << ")";
	return oss.str();
}

string Param::to_string() const
{
	return type.to_string() + " " + name;
}

void FunctionPrototype::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "FunctionPrototype: " << name << "\n";
	print_indent(os, indent + 2);
	os << "Return Type: " << return_type.to_string() << "\n";
	print_indent(os, indent + 2);
	os << "Parameters:\n";
	for (auto param : params)
	{
		param->dump(os, indent + 4);
	}
}

void Param::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "Param: " << type.to_string() << " " << name << "\n";
}