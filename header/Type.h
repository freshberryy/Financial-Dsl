#pragma once
#include <string>


class Type
{
public:
	std::string to_string();

private:
	std::string base_type;
	int dim;
};