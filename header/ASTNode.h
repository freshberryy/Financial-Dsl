#pragma once
#include<string>
#include<iostream>

class ASTNode
{
public:

	ASTNode() {}

	ASTNode(int line, int col) :line(line), col(col) {}

	virtual ~ASTNode() {}

	virtual void dump(std::ostream& os, int indent = 0) const = 0;
	virtual std::string to_string() const = 0;

	std::pair<int, int> get_location() const { return std::make_pair(line, col); };

protected:
	int line;
	int col;
};