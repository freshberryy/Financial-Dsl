#include "FunctionUtil.h"

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



