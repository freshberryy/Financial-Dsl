#include "Lexer.h"
#include "TokenKind.h"
#include "Token.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

struct TokenExpectation
{
	TokenKind kind;
	string lexeme;
};

void run_error_only_test(const string& name, const string& input)
{
	cout << "[TEST: " << name << "]" << endl;
	Lexer lexer;
	vector<Token> tokens = lexer.tokenize(input);

	bool error_found = false;

	for (const auto& t : tokens)
	{
		if (t.kind == TokenKind::UNKNOWN)
		{
			cout << "UNKNOWN: " << t.lexeme << endl;
			error_found = true;
		}
	}

	for (const auto& log : lexer.logs)
	{
		if (log.find("[¿¡·¯]") != string::npos)
		{
			error_found = true;
			break;
		}
	}

	if (error_found)
		cout << "PASSED" << endl;
	else
		cout << "FAILED (no error found)" << endl;

	cout << "-------------------------" << endl;
}



int main()
{

}
