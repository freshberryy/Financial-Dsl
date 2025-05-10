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

