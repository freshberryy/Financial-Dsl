#include "Stmt.h"
#include "FunctionUtil.h"
using namespace std;

Stmt::~Stmt() {}

ExprStmt::~ExprStmt()
{
	delete expr;
}

VarDeclStmt::~VarDeclStmt()
{
	delete type;
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
	for (Stmt* s : stmts)
	{
		delete s;
	}
}

void BlockStmt::add_stmt(Stmt* stmt)
{
	stmts.push_back(stmt);
}

FuncDeclStmt::~FuncDeclStmt()
{
	delete prototype;
	delete body;
}


string ExprStmt::to_string() const
{
	return expr ? expr->to_string() + ";" : ";";
}

string VarDeclStmt::to_string() const
{
	string result = type->to_string() + " " + name;
	if (init) result += " = " + init->to_string();
	result += ";";
	return result;
}

string ReturnStmt::to_string() const
{
	if (expr)
		return "return " + expr->to_string() + ";";
	else
		return "return;";
}

string BreakStmt::to_string() const
{
	return "break;";
}

string ContinueStmt::to_string() const
{
	return "continue;";
}

string IfStmt::to_string() const
{
	ostringstream oss;
	oss << "if (" << condition->to_string() << ") " << then_branch->to_string();
	for (const auto& [cond, block] : else_if_branches)
	{
		oss << " else if (" << cond->to_string() << ") " << block->to_string();
	}
	if (else_branch)
		oss << " else " << else_branch->to_string();
	return oss.str();
}

string WhileStmt::to_string() const
{
	return "while (" + condition->to_string() + ") " + body->to_string();
}

string ForStmt::to_string() const
{
	string init_str = init ? init->to_string() : "";
	string cond_str = cond ? cond->to_string() : "";
	string post_str = post ? post->to_string() : "";
	return "for (" + init_str + "; " + cond_str + "; " + post_str + ") " + body->to_string();
}

string BlockStmt::to_string() const
{
	ostringstream oss;
	oss << "{\n";
	for (auto stmt : stmts)
	{
		oss << "  " << stmt->to_string() << "\n";
	}
	oss << "}";
	return oss.str();
}

string FuncDeclStmt::to_string() const
{
	return prototype->to_string() + " " + body->to_string();
}

static void print_indent(ostream& os, int indent)
{
	os << setw(indent) << ' ';
}

void ExprStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "ExprStmt\n";
	if (expr) expr->dump(os, indent + 2);
}

void VarDeclStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "VarDeclStmt: " << type->to_string() << " " << name << "\n";
	if (init) init->dump(os, indent + 2);
}

void ReturnStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "ReturnStmt\n";
	if (expr) expr->dump(os, indent + 2);
}

void BreakStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "BreakStmt\n";
}

void ContinueStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "ContinueStmt\n";
}

void IfStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "IfStmt\n";
	print_indent(os, indent + 2);
	os << "Condition:\n";
	condition->dump(os, indent + 4);
	print_indent(os, indent + 2);
	os << "Then:\n";
	then_branch->dump(os, indent + 4);

	for (const auto& [cond, block] : else_if_branches)
	{
		print_indent(os, indent + 2);
		os << "ElseIf:\n";
		print_indent(os, indent + 4);
		os << "Condition:\n";
		cond->dump(os, indent + 6);
		print_indent(os, indent + 4);
		os << "Block:\n";
		block->dump(os, indent + 6);
	}

	if (else_branch)
	{
		print_indent(os, indent + 2);
		os << "Else:\n";
		else_branch->dump(os, indent + 4);
	}
}

void WhileStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "WhileStmt\n";
	print_indent(os, indent + 2);
	os << "Condition:\n";
	condition->dump(os, indent + 4);
	print_indent(os, indent + 2);
	os << "Body:\n";
	body->dump(os, indent + 4);
}

void ForStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "ForStmt\n";
	if (init)
	{
		print_indent(os, indent + 2);
		os << "Init:\n";
		init->dump(os, indent + 4);
	}
	if (cond)
	{
		print_indent(os, indent + 2);
		os << "Condition:\n";
		cond->dump(os, indent + 4);
	}
	if (post)
	{
		print_indent(os, indent + 2);
		os << "Post:\n";
		post->dump(os, indent + 4);
	}
	print_indent(os, indent + 2);
	os << "Body:\n";
	body->dump(os, indent + 4);
}

void BlockStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "BlockStmt\n";
	for (auto stmt : stmts)
	{
		stmt->dump(os, indent + 2);
	}
}

void FuncDeclStmt::dump(ostream& os, int indent) const
{
	print_indent(os, indent);
	os << "FuncDeclStmt\n";
	print_indent(os, indent + 2);
	os << "Prototype: " << prototype->to_string() << "\n";
	print_indent(os, indent + 2);
	os << "Body:\n";
	body->dump(os, indent + 4);
}