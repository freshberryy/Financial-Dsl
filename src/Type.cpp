#include "Type.h"
#include <iostream>
#include <iomanip>

using namespace std;

string Type::to_string() const
{
	string result = base_type;
	for (int i = 0; i < dim; ++i)
		result += "[]";
	return result;
}

void Type::dump(ostream& os, int indent) const
{
	os << setw(indent) << ' ';
	auto [line, col] = get_location();
	os << "Type: " << to_string() << " (line " << line << ", col " << col << ")\n";
}

TypeKind Type::get_type() const
{
	if (dim == 1) return TypeKind::ARRAY1D;
	if (dim == 2) return TypeKind::ARRAY2D;
	if (base_type == "int") return TypeKind::INT;
	if (base_type == "float") return TypeKind::FLOAT;
	if (base_type == "bool") return TypeKind::BOOL;
	if (base_type == "string") return TypeKind::STRING;
	if (base_type == "void") return TypeKind::VOID;

	throw ParserException("정의되지 않은 타입", get_location().first, get_location().second);
}


