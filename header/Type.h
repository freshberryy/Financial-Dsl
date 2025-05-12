#pragma once
#include <string>
#include "ASTNode.h"
#include "ParserException.h"

enum class TypeKind
{
	INT, FLOAT, BOOL, STRING, VOID, ARRAY1D, ARRAY2D
};

class Type : public ASTNode
{
public:

	Type(std::string base_type, int dim, int line, int col)
		:ASTNode(line, col), base_type(base_type), dim(dim){}

	std::string to_string() const;
	void dump(std::ostream& os, int indent = 0) const;
	TypeKind get_type() const;

private:
	std::string base_type;
	int dim;
};