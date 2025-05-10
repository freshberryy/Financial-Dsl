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
    std::cout << "입력: " << input << std::endl;

    Lexer lex;
    // 1. 토큰화
    std::vector<Token> tokens = lex.tokenize(input); // 반드시 구현되어 있어야 함

    // 2. 파서 구성
    Logger logger;
    TokenStream ts(tokens);
    Parser parser(&ts, logger);

    // 3. 파싱 및 출력
    try
    {
        Expr* expr = parser.parse_expr();
        expr->dump(std::cout, 0);  // 들여쓰기 기반 트리 출력
        std::cout << std::endl;
        delete expr;
    }
    catch (const std::exception& e)
    {
        std::cerr << "예외 발생: " << e.what() << std::endl;
    }
}

int main()
{
    test_expr_parsing("(1 + 2");
    test_expr_parsing("(1 + 2");
    test_expr_parsing("x * y + z % 2");
    return 0;
}
