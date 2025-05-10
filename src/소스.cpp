#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include "TokenStream.h"
#include "Parser.h"
#include "Expr.h"
#include "Logger.h"
#include "Lexer.h"

using namespace std;

void test_expr_parsing(const std::string& input)
{
    std::cout << "�Է�: " << input << std::endl;

    Lexer lex;
    // 1. ��ūȭ
    std::vector<Token> tokens = lex.tokenize(input); // �ݵ�� �����Ǿ� �־�� ��

    // 2. �ļ� ����
    Logger logger;
    TokenStream ts(tokens);
    Parser parser(&ts, logger);

    // 3. �Ľ� �� ���
    try
    {
        Expr* expr = parser.parse_expr();
        expr->dump(std::cout, 0);  // �鿩���� ��� Ʈ�� ���
        std::cout << std::endl;
        delete expr;
    }
    catch (const std::exception& e)
    {
        std::cerr << "���� �߻�: " << e.what() << std::endl;
    }
}

int main()
{
    test_expr_parsing("(1 + 2");
    test_expr_parsing("(1 + 2");
    test_expr_parsing("x * y + z % 2");
    return 0;
}
