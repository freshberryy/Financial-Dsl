#pragma once
#include <vector>

class Stmt;

class ProgramNode
{
public:
	ProgramNode(const std::vector<Stmt*>& s, int line, int col) : statements(s){}

	~ProgramNode() { for (Stmt* s : statements) delete s; }

private:
	std::vector<Stmt*> statements;
};
