#pragma once

class ASTNode
{
public:

	ASTNode() {}

	ASTNode(int line, int col) :line(line), col(col) {}

	virtual ~ASTNode() {}

	std::pair<int, int> get_location() const { return std::make_pair(line, col); };

protected:
	int line;
	int col;
};