#pragma once
#include <iostream>
#include <vector>
#include "Lexer.h"
#include "TokenStream.h"
#include "Parser.h"
#include "Expr.h"
#include "Logger.h"

//Expr
inline void test_parse_primary_expr(Token single_token)
{
	std::vector<Token> tokens = { single_token, Token(TokenKind::END_OF_FILE, "", single_token.line, single_token.col + 1) };
	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);

	try
	{
		Expr* expr = parser.parse_primary_expr();

		if (expr)
		{
			std::cout << "[PASS] Parsed AST Node:\n";
			expr->dump(std::cout, 0);
			delete expr;
		}
		else
		{
			std::cerr << "[FAIL] Returned nullptr\n";
		}
	}
	catch (const ParserException& e)
	{
		std::cerr << "[FAIL] ParserException: " << e.what() << "\n";
	}
}

inline void test_leaf()
{
	std::cout << "=== Testing IntLiteralExpr ===\n";
	test_parse_primary_expr(Token(TokenKind::INT_LITERAL, "123", 1, 1));

	std::cout << "\n=== Testing IdentifierExpr ===\n";
	test_parse_primary_expr(Token(TokenKind::IDENTIFIER, "abc", 2, 1));

	std::cout << "\n=== Testing FloatLiteralExpr ===\n";
	test_parse_primary_expr(Token(TokenKind::FLOAT_LITERAL, "3.14", 3, 1));

	std::cout << "\n=== Testing Invalid Token ===\n";
	test_parse_primary_expr(Token(TokenKind::PLUS, "+", 4, 1));
}

inline void run_postfix_expr_test(const std::string& test_name, const std::vector<Token>& tokens)
{
	std::cout << "=== Test: " << test_name << " ===\n";

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);

	try
	{
		Expr* expr = parser.parse_postfix_expr();
		std::cout << "[FAIL] 예외 없음. AST 출력:\n";
		expr->dump(std::cout, 0);
		delete expr;
	}
	catch (const ParserException& e)
	{
		std::cout << "[PASS] 예외 발생: " << e.what() << "\n";
	}
}

inline void test_postfix_edge_case()
{
	run_postfix_expr_test("연속된 함수 호출", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::RPAREN, ")", 1, 5),
		Token(TokenKind::LPAREN, "(", 1, 6),
		Token(TokenKind::INT_LITERAL, "1", 1, 7),
		Token(TokenKind::RPAREN, ")", 1, 8),
		Token(TokenKind::END_OF_FILE, "", 1, 9)
		});

	run_postfix_expr_test("함수 호출 후 배열 접근", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::RPAREN, ")", 1, 5),
		Token(TokenKind::LBRACKET, "[", 1, 6),
		Token(TokenKind::INT_LITERAL, "1", 1, 7),
		Token(TokenKind::RBRACKET, "]", 1, 8),
		Token(TokenKind::END_OF_FILE, "", 1, 9)
		});

	run_postfix_expr_test("함수 호출 괄호 누락", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::INT_LITERAL, "1", 1, 5),
		Token(TokenKind::COMMA, ",", 1, 6),
		Token(TokenKind::INT_LITERAL, "2", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		});

	run_postfix_expr_test("함수 콤마로 시작", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::COMMA, ",", 1, 5),
		Token(TokenKind::INT_LITERAL, "1", 1, 6),
		Token(TokenKind::RPAREN, ")", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		});

	run_postfix_expr_test("함수 인자 중간 빈 값", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::INT_LITERAL, "1", 1, 5),
		Token(TokenKind::COMMA, ",", 1, 6),
		Token(TokenKind::COMMA, ",", 1, 7),
		Token(TokenKind::INT_LITERAL, "2", 1, 8),
		Token(TokenKind::RPAREN, ")", 1, 9),
		Token(TokenKind::END_OF_FILE, "", 1, 10)
		});

	run_postfix_expr_test("대괄호 닫힘 누락", {
		Token(TokenKind::IDENTIFIER, "a", 1, 1),
		Token(TokenKind::LBRACKET, "[", 1, 2),
		Token(TokenKind::INT_LITERAL, "1", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});

	run_postfix_expr_test("3차원 배열 접근", {
		Token(TokenKind::IDENTIFIER, "a", 1, 1),
		Token(TokenKind::LBRACKET, "[", 1, 2),
		Token(TokenKind::INT_LITERAL, "1", 1, 3),
		Token(TokenKind::RBRACKET, "]", 1, 4),
		Token(TokenKind::LBRACKET, "[", 1, 5),
		Token(TokenKind::INT_LITERAL, "2", 1, 6),
		Token(TokenKind::RBRACKET, "]", 1, 7),
		Token(TokenKind::LBRACKET, "[", 1, 8),
		Token(TokenKind::INT_LITERAL, "3", 1, 9),
		Token(TokenKind::RBRACKET, "]", 1, 10),
		Token(TokenKind::END_OF_FILE, "", 1, 11)
		});
}

inline void run_expr_edge_test(const std::string& test_name, const std::vector<Token>& tokens)
{
	std::cout << "=== Edge Test: " << test_name << " ===\n";

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);

	try
	{
		Expr* expr = parser.parse_add_expr(); 
		std::cout << "[FAIL] 예외 없음. AST 출력:\n";
		expr->dump(std::cout, 0);
		delete expr;
	}
	catch (const ParserException& e)
	{
		std::cout << "[PASS] 예외 발생: " << e.what() << "\n";
	}
}

inline void run_expr_dump_test(const std::string& test_name, const std::vector<Token>& tokens)
{
	std::cout << "=== Dump Test: " << test_name << " ===\n";

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);

	try
	{
		Expr* expr = parser.parse_add_expr();
		if (expr)
		{
			std::cout << "[PASS] AST 출력:\n";
			expr->dump(std::cout, 0);
			delete expr;
		}
		else
		{
			std::cout << "[FAIL] 반환된 AST가 null\n";
		}
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] 예외 발생: " << e.what() << "\n";
	}
}

inline void run_expr_to_string_test(const std::string& test_name,
	const std::vector<Token>& tokens,
	const std::string& expected_output)
{
	std::cout << "=== to_string Test: " << test_name << " ===\n";

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);

	try
	{
		Expr* expr = parser.parse_add_expr();
		if (expr)
		{
			std::string result = expr->to_string();
			std::cout << "-기대값: " << expected_output << ", 실제값: " << result << "\n";
			delete expr;
		}
		else
		{
			std::cout << "[FAIL] 반환된 AST가 null\n";
		}
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] 예외 발생: " << e.what() << "\n";
	}
}


inline void test_unary_binary()
{
	// 엣지
	run_expr_edge_test("단항 연산자 뒤 표현식 없음", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::END_OF_FILE, "", 1, 2)
		});

	run_expr_edge_test("이항 연산자 뒤 표현식 없음", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_edge_test("연속 이항 연산자 (1 + *)", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::MUL, "*", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_edge_test("연속 단항 연산자 끝에 없음", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::MINUS, "-", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_edge_test("괄호 안 비어있음", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::RPAREN, ")", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_edge_test("괄호 닫힘 누락", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::PLUS, "+", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_edge_test("잘못된 토큰 시작", {
		Token(TokenKind::COMMA, ",", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_edge_test("연속된 *, / 연산자만 존재", {
		Token(TokenKind::MUL, "*", 1, 1),
		Token(TokenKind::DIV, "/", 1, 2),
		Token(TokenKind::MOD, "%", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});

	run_expr_edge_test("단항 + + 잘못된 연쇄", {
		Token(TokenKind::PLUS, "+", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::PLUS, "+", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});


	// dump
	run_expr_dump_test("단항 -1", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_dump_test("이항 1 + 2", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});

	run_expr_dump_test("곱셈 우선 1 + 2 * 3", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::MUL, "*", 1, 4),
		Token(TokenKind::INT_LITERAL, "3", 1, 5),
		Token(TokenKind::END_OF_FILE, "", 1, 6)
		});

	run_expr_dump_test("우선순위 테스트: (1 + 2) * 3", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::PLUS, "+", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::RPAREN, ")", 1, 5),
		Token(TokenKind::MUL, "*", 1, 6),
		Token(TokenKind::INT_LITERAL, "3", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		});

	run_expr_dump_test("단항 - 붙은 곱셈", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::INT_LITERAL, "2", 1, 2),
		Token(TokenKind::MUL, "*", 1, 3),
		Token(TokenKind::INT_LITERAL, "3", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_dump_test("여러 단항 연산자 ---3", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::MINUS, "-", 1, 2),
		Token(TokenKind::MINUS, "-", 1, 3),
		Token(TokenKind::INT_LITERAL, "3", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_dump_test("혼합 1 + -2 * 3", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::MINUS, "-", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::MUL, "*", 1, 5),
		Token(TokenKind::INT_LITERAL, "3", 1, 6),
		Token(TokenKind::END_OF_FILE, "", 1, 7)
		});

	run_expr_dump_test("괄호 내부 단항", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::MINUS, "-", 1, 2),
		Token(TokenKind::INT_LITERAL, "7", 1, 3),
		Token(TokenKind::RPAREN, ")", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_dump_test("덧셈/뺄셈/곱셈/나눗셈 혼합", {
		Token(TokenKind::INT_LITERAL, "8", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "6", 1, 3),
		Token(TokenKind::MUL, "*", 1, 4),
		Token(TokenKind::INT_LITERAL, "2", 1, 5),
		Token(TokenKind::MINUS, "-", 1, 6),
		Token(TokenKind::INT_LITERAL, "1", 1, 7),
		Token(TokenKind::DIV, "/", 1, 8),
		Token(TokenKind::INT_LITERAL, "5", 1, 9),
		Token(TokenKind::END_OF_FILE, "", 1, 10)
		});

	run_expr_dump_test("괄호로 우선순위 조정", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::PLUS, "+", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::RPAREN, ")", 1, 5),
		Token(TokenKind::MUL, "*", 1, 6),
		Token(TokenKind::LPAREN, "(", 1, 7),
		Token(TokenKind::INT_LITERAL, "3", 1, 8),
		Token(TokenKind::MINUS, "-", 1, 9),
		Token(TokenKind::INT_LITERAL, "4", 1, 10),
		Token(TokenKind::RPAREN, ")", 1, 11),
		Token(TokenKind::END_OF_FILE, "", 1, 12)
		});

	//to_string
	run_expr_to_string_test("단항 -1", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		}, "-1");

	run_expr_to_string_test("이항 1 + 2", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		}, "1 + 2");

	run_expr_to_string_test("곱셈 우선 1 + 2 * 3", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::MUL, "*", 1, 4),
		Token(TokenKind::INT_LITERAL, "3", 1, 5),
		Token(TokenKind::END_OF_FILE, "", 1, 6)
		}, "1 + 2 * 3");

	run_expr_to_string_test("괄호 표현식 (1 + 2) * 3", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::PLUS, "+", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::RPAREN, ")", 1, 5),
		Token(TokenKind::MUL, "*", 1, 6),
		Token(TokenKind::INT_LITERAL, "3", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		}, "(1 + 2) * 3");

}

inline void run_relational_expr_dump_test(const std::string& test_name, const std::vector<Token>& tokens)
{
	std::cout << "=== Dump Test: " << test_name << " ===\n";

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);

	try
	{
		Expr* expr = parser.parse_relational_expr();
		if (expr)
		{
			std::cout << "[PASS] AST 구조:\n";
			expr->dump(std::cout, 0);
			delete expr;
		}
		else
		{
			std::cout << "[FAIL] 반환된 AST가 null\n";
		}
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] 예외 발생: " << e.what() << "\n";
	}
}

inline void run_relational_expr_to_string_test(const std::string& test_name, const std::vector<Token>& tokens, const std::string& expected)
{
	std::cout << "=== to_string Test: " << test_name << " ===\n";

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);

	try
	{
		Expr* expr = parser.parse_relational_expr();
		if (!expr)
		{
			std::cout << "[FAIL] AST가 null\n";
			return;
		}
		std::string result = expr->to_string();
		std::cout << "- 기대값: " << expected << ", 실제값: " << result << "\n";
		delete expr;
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] 예외 발생: " << e.what() << "\n";
	}
}

inline void test_relational_expr()
{
	run_relational_expr_to_string_test("1 < 2", {
	Token(TokenKind::INT_LITERAL, "1", 1, 1),
	Token(TokenKind::LESS, "<", 1, 2),
	Token(TokenKind::INT_LITERAL, "2", 1, 3),
	Token(TokenKind::END_OF_FILE, "", 1, 4)
		}, "1 < 2");

	run_relational_expr_to_string_test("3 > 2 + 1", {
		Token(TokenKind::INT_LITERAL, "3", 1, 1),
		Token(TokenKind::GREATER, ">", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::PLUS, "+", 1, 4),
		Token(TokenKind::INT_LITERAL, "1", 1, 5),
		Token(TokenKind::END_OF_FILE, "", 1, 6)
		}, "3 > 2 + 1");

	run_relational_expr_to_string_test("5 + 2 >= 7", {
		Token(TokenKind::INT_LITERAL, "5", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::GREATER_EQUAL, ">=", 1, 4),
		Token(TokenKind::INT_LITERAL, "7", 1, 5),
		Token(TokenKind::END_OF_FILE, "", 1, 6)
		}, "5 + 2 >= 7");

	run_relational_expr_to_string_test("1 + 2 < 3 * 4", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::LESS, "<", 1, 4),
		Token(TokenKind::INT_LITERAL, "3", 1, 5),
		Token(TokenKind::MUL, "*", 1, 6),
		Token(TokenKind::INT_LITERAL, "4", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		}, "1 + 2 < 3 * 4");

	run_relational_expr_to_string_test("a <= b", {
		Token(TokenKind::IDENTIFIER, "a", 1, 1),
		Token(TokenKind::LESS_EQUAL, "<=", 1, 2),
		Token(TokenKind::IDENTIFIER, "b", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		}, "a <= b");

	run_relational_expr_to_string_test("a + b >= c * d", {
		Token(TokenKind::IDENTIFIER, "a", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::IDENTIFIER, "b", 1, 3),
		Token(TokenKind::GREATER_EQUAL, ">=", 1, 4),
		Token(TokenKind::IDENTIFIER, "c", 1, 5),
		Token(TokenKind::MUL, "*", 1, 6),
		Token(TokenKind::IDENTIFIER, "d", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		}, "a + b >= c * d");

	run_relational_expr_to_string_test("3 * 3 > 5 + 2", {
		Token(TokenKind::INT_LITERAL, "3", 1, 1),
		Token(TokenKind::MUL, "*", 1, 2),
		Token(TokenKind::INT_LITERAL, "3", 1, 3),
		Token(TokenKind::GREATER, ">", 1, 4),
		Token(TokenKind::INT_LITERAL, "5", 1, 5),
		Token(TokenKind::PLUS, "+", 1, 6),
		Token(TokenKind::INT_LITERAL, "2", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		}, "3 * 3 > 5 + 2");

	run_relational_expr_to_string_test("(((1))) < ((2))", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 2),
		Token(TokenKind::LPAREN, "(", 1, 3),
		Token(TokenKind::INT_LITERAL, "1", 1, 4),
		Token(TokenKind::RPAREN, ")", 1, 5),
		Token(TokenKind::RPAREN, ")", 1, 6),
		Token(TokenKind::RPAREN, ")", 1, 7),
		Token(TokenKind::LESS, "<", 1, 8),
		Token(TokenKind::LPAREN, "(", 1, 9),
		Token(TokenKind::LPAREN, "(", 1, 10),
		Token(TokenKind::INT_LITERAL, "2", 1, 11),
		Token(TokenKind::RPAREN, ")", 1, 12),
		Token(TokenKind::RPAREN, ")", 1, 13),
		Token(TokenKind::END_OF_FILE, "", 1, 14)
		}, "1 < 2");

	run_relational_expr_to_string_test("x * y <= 100", {
		Token(TokenKind::IDENTIFIER, "x", 1, 1),
		Token(TokenKind::MUL, "*", 1, 2),
		Token(TokenKind::IDENTIFIER, "y", 1, 3),
		Token(TokenKind::LESS_EQUAL, "<=", 1, 4),
		Token(TokenKind::INT_LITERAL, "100", 1, 5),
		Token(TokenKind::END_OF_FILE, "", 1, 6)
		}, "x * y <= 100");

	run_relational_expr_to_string_test("5 <= 5 <= 5", {
		Token(TokenKind::INT_LITERAL, "5", 1, 1),
		Token(TokenKind::LESS_EQUAL, "<=", 1, 2),
		Token(TokenKind::INT_LITERAL, "5", 1, 3),
		Token(TokenKind::LESS_EQUAL, "<=", 1, 4),
		Token(TokenKind::INT_LITERAL, "5", 1, 5),
		Token(TokenKind::END_OF_FILE, "", 1, 6)
		}, "5 <= 5 <= 5");


	run_relational_expr_dump_test("오른쪽 피연산자 없음", {
	Token(TokenKind::INT_LITERAL, "1", 1, 1),
	Token(TokenKind::LESS, "<", 1, 2),
	Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_relational_expr_dump_test("왼쪽 피연산자 없음", {
		Token(TokenKind::LESS, "<", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_relational_expr_dump_test("연속 연산자 <<", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::LESS, "<", 1, 2),
		Token(TokenKind::LESS, "<", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_relational_expr_dump_test("비정상 토큰 시작", {
		Token(TokenKind::COMMA, ",", 1, 1),
		Token(TokenKind::LESS, "<", 1, 2),
		Token(TokenKind::INT_LITERAL, "3", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});

	run_relational_expr_dump_test("연산자 뒤 괄호 열고 닫지 않음", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::GREATER, ">", 1, 2),
		Token(TokenKind::LPAREN, "(", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_relational_expr_dump_test("좌측 식 없음", {
		Token(TokenKind::LESS_EQUAL, "<=", 1, 1),
		Token(TokenKind::INT_LITERAL, "5", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_relational_expr_dump_test("단항 연산자만 존재", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::END_OF_FILE, "", 1, 2)
		});

	run_relational_expr_dump_test("괄호만 존재", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::RPAREN, ")", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});


	run_relational_expr_dump_test("중간 피연산자 누락", {
		Token(TokenKind::INT_LITERAL, "5", 1, 1),
		Token(TokenKind::GREATER_EQUAL, ">=", 1, 2),
		Token(TokenKind::GREATER_EQUAL, ">=", 1, 3),
		Token(TokenKind::INT_LITERAL, "5", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

}

inline void run_full_expr_test(const std::string& test_name, const std::vector<Token>& tokens)
{
	std::cout << "=== Full Expression Test: " << test_name << " ===\n";

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);

	try
	{
		Expr* expr = parser.parse_expr();  
		if (!expr)
		{
			std::cout << "[FAIL] AST가 null\n";
			return;
		}
		std::cout << "[PASS] AST 구조:\n";
		expr->dump(std::cout, 0);
		delete expr;
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] 예외 발생: " << e.what() << "\n";
	}
}

inline void test_full_expr()
{
	run_full_expr_test("a = 1 + 2 * 3", {
		Token(TokenKind::IDENTIFIER, "a", 1, 1),
		Token(TokenKind::ASSIGN, "=", 1, 2),
		Token(TokenKind::INT_LITERAL, "1", 1, 3),
		Token(TokenKind::PLUS, "+", 1, 4),
		Token(TokenKind::INT_LITERAL, "2", 1, 5),
		Token(TokenKind::MUL, "*", 1, 6),
		Token(TokenKind::INT_LITERAL, "3", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		});

	run_full_expr_test("x = (y + 2) * 4", {
		Token(TokenKind::IDENTIFIER, "x", 1, 1),
		Token(TokenKind::ASSIGN, "=", 1, 2),
		Token(TokenKind::LPAREN, "(", 1, 3),
		Token(TokenKind::IDENTIFIER, "y", 1, 4),
		Token(TokenKind::PLUS, "+", 1, 5),
		Token(TokenKind::INT_LITERAL, "2", 1, 6),
		Token(TokenKind::RPAREN, ")", 1, 7),
		Token(TokenKind::MUL, "*", 1, 8),
		Token(TokenKind::INT_LITERAL, "4", 1, 9),
		Token(TokenKind::END_OF_FILE, "", 1, 10)
		});

	run_full_expr_test("result = a && b || c", {
		Token(TokenKind::IDENTIFIER, "result", 1, 1),
		Token(TokenKind::ASSIGN, "=", 1, 2),
		Token(TokenKind::IDENTIFIER, "a", 1, 3),
		Token(TokenKind::AND, "&&", 1, 4),
		Token(TokenKind::IDENTIFIER, "b", 1, 5),
		Token(TokenKind::OR, "||", 1, 6),
		Token(TokenKind::IDENTIFIER, "c", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		});

	run_full_expr_test("flag = (x > 3) && (y < 5)", {
		Token(TokenKind::IDENTIFIER, "flag", 1, 1),
		Token(TokenKind::ASSIGN, "=", 1, 2),
		Token(TokenKind::LPAREN, "(", 1, 3),
		Token(TokenKind::IDENTIFIER, "x", 1, 4),
		Token(TokenKind::GREATER, ">", 1, 5),
		Token(TokenKind::INT_LITERAL, "3", 1, 6),
		Token(TokenKind::RPAREN, ")", 1, 7),
		Token(TokenKind::AND, "&&", 1, 8),
		Token(TokenKind::LPAREN, "(", 1, 9),
		Token(TokenKind::IDENTIFIER, "y", 1, 10),
		Token(TokenKind::LESS, "<", 1, 11),
		Token(TokenKind::INT_LITERAL, "5", 1, 12),
		Token(TokenKind::RPAREN, ")", 1, 13),
		Token(TokenKind::END_OF_FILE, "", 1, 14)
		});
}

inline void test_if_case1()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_IF, "if", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 3));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 1, 4));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 5));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 7));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 2, 1));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 2, 3));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 2, 5));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 6));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 3, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== IF CASE 1 ===\n";
	Stmt* stmt = parser.parse_if_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_if_case2()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_IF, "if", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 3));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 1, 4));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 5));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 7));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 2, 1));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 2, 3));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 2, 5));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 6));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::KW_ELSE, "else", 3, 3));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 3, 8));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 4, 1));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 4, 3));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "2", 4, 5));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 4, 6));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 5, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 5, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== IF CASE 2 ===\n";
	Stmt* stmt = parser.parse_if_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_if_case3()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_IF, "if", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 4));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 1, 5));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 6));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 8));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "a", 2, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 2, 4));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 2, 6));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 7));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::KW_ELSE_IF, "else if", 3, 3));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 3, 12));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 3, 13));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 3, 14));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 3, 16));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "a", 4, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 4, 4));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "2", 4, 6));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 4, 7));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 5, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 5, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== IF CASE 3 ===\n";
	Stmt* stmt = parser.parse_if_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_if_case4()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_IF, "if", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 4));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 1, 5));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 6));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 8));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "a", 2, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 2, 4));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 2, 6));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 7));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::KW_ELSE_IF, "else if", 3, 3));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 3, 12));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 3, 13));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 3, 14));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 3, 16));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "a", 4, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 4, 4));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "2", 4, 6));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 4, 7));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 5, 1));
	tokens.push_back(Token(TokenKind::KW_ELSE, "else", 5, 3));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 5, 8));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "a", 6, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 6, 4));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "3", 6, 6));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 6, 7));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 7, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 7, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== IF CASE 4 ===\n";
	Stmt* stmt = parser.parse_if_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_if_case5()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_IF, "if", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 4));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 1, 5));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 6));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 8));

	tokens.push_back(Token(TokenKind::KW_IF, "if", 2, 2));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 2, 5));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 2, 6));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 2, 7));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 2, 9));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "a", 3, 3));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 3, 5));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 3, 7));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 3, 8));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 4, 2));

	tokens.push_back(Token(TokenKind::RBRACE, "}", 5, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 5, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== IF CASE 5 ===\n";
	Stmt* stmt = parser.parse_if_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}


inline void test_if()
{
	test_if_case1();
	test_if_case2();
	test_if_case3();
	test_if_case4();
	test_if_case5();
}

inline void test_while_case1()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_WHILE, "while", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 7));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 1, 8));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 9));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 11));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== WHILE CASE 1 ===\n";
	Stmt* stmt = parser.parse_while_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_while_case2()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_WHILE, "while", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 7));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 1, 8));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 9));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 11));
	tokens.push_back(Token(TokenKind::KW_BREAK, "break", 2, 2));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 7));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 3, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== WHILE CASE 2 ===\n";
	Stmt* stmt = parser.parse_while_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_while_case3()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_WHILE, "while", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 7));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 1, 8));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 9));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 11));
	tokens.push_back(Token(TokenKind::KW_CONTINUE, "continue", 2, 2));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 10));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 3, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== WHILE CASE 3 ===\n";
	Stmt* stmt = parser.parse_while_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_while_case4()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_WHILE, "while", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 7));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 1, 8));
	tokens.push_back(Token(TokenKind::AND, "&&", 1, 9));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 1, 12));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 13));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 15));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "a", 2, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 2, 4));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 2, 6));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 7));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 3, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== WHILE CASE 4 ===\n";
	Stmt* stmt = parser.parse_while_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_while_case5()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_WHILE, "while", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 7));
	tokens.push_back(Token(TokenKind::BOOL_LITERAL, "true", 1, 8));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 12));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 14));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== WHILE CASE 5 ===\n";
	Stmt* stmt = parser.parse_while_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_while()
{
	test_while_case1();
	test_while_case2();
	test_while_case3();
	test_while_case4();
	test_while_case5();
}

inline void test_for_case1()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_FOR, "for", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 4));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 5));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 1, 6));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "0", 1, 7));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 8));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 9));
	tokens.push_back(Token(TokenKind::LESS, "<", 1, 10));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "10", 1, 11));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 13));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 14));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 1, 15));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 16));
	tokens.push_back(Token(TokenKind::PLUS, "+", 1, 17));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 1, 18));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 19));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 21));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FOR CASE 1 ===\n";
	Stmt* stmt = parser.parse_for_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_for_case2()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_FOR, "for", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 4));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 5));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 1, 6));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "0", 1, 7));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 8));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 9)); // 조건 없음
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 10));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 1, 11));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 12));
	tokens.push_back(Token(TokenKind::PLUS, "+", 1, 13));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 1, 14));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 15));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 17));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FOR CASE 2 ===\n";
	Stmt* stmt = parser.parse_for_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}


inline void test_for_case3()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_FOR, "for", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 4));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 5));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 1, 6));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "0", 1, 7));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 8));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 9));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 10));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 12));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FOR CASE 3 ===\n";
	Stmt* stmt = parser.parse_for_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_for_case4()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_FOR, "for", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 4));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 5));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 6));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 7));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 1, 8));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 9));
	tokens.push_back(Token(TokenKind::PLUS, "+", 1, 10));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 1, 11));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 12));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 14));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FOR CASE 4 ===\n";
	Stmt* stmt = parser.parse_for_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_for_case5()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_FOR, "for", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 4));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 5));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 6));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 7));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 9));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FOR CASE 5 ===\n";
	Stmt* stmt = parser.parse_for_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_for_case6()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_FOR, "for", 1, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 4));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 5));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 1, 6));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "0", 1, 7));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 8));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 9));
	tokens.push_back(Token(TokenKind::LESS, "<", 1, 10));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "5", 1, 11));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 1, 12));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 13));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 1, 14));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 1, 15));
	tokens.push_back(Token(TokenKind::PLUS, "+", 1, 16));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 1, 17));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 18));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 20));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "sum", 2, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 2, 6));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "sum", 2, 8));
	tokens.push_back(Token(TokenKind::PLUS, "+", 2, 12));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "i", 2, 14));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 15));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 3, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FOR CASE 6 ===\n";
	Stmt* stmt = parser.parse_for_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_for()
{
	test_for_case1();
	test_for_case2();
	test_for_case3();
	test_for_case4();
	test_for_case5();
	test_for_case6();
}

inline void test_block_case1()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 1));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 1, 2));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 1, 3));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== BLOCK CASE 1 ===\n";
	BlockStmt* block = parser.parse_block_stmt();
	block->dump(std::cout, 0);
	delete block;
}

inline void test_block_case2()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 1));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 2, 1));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 2, 2));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 2, 3));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 4));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 3, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== BLOCK CASE 2 ===\n";
	BlockStmt* block = parser.parse_block_stmt();
	block->dump(std::cout, 0);
	delete block;
}

inline void test_block_case3()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 1));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 2, 1));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 2, 2));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 2, 3));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 4));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 3, 1));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 3, 2));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "2", 3, 3));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 3, 4));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 4, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 4, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== BLOCK CASE 3 ===\n";
	BlockStmt* block = parser.parse_block_stmt();
	block->dump(std::cout, 0);
	delete block;
}

inline void test_block_case4()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 1));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 2, 1));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 2, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 2, 3));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 2, 4));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 5));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 4, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 4, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== BLOCK CASE 4 ===\n";
	BlockStmt* block = parser.parse_block_stmt();
	block->dump(std::cout, 0);
	delete block;
}

inline void test_block_case5()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 1));

	// if (x) { y = 1; }
	tokens.push_back(Token(TokenKind::KW_IF, "if", 2, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 2, 4));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 2, 5));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 2, 6));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 2, 8));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 3, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 3, 3));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 3, 4));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 3, 5));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 4, 1));

	// while (y) { x = 0; }
	tokens.push_back(Token(TokenKind::KW_WHILE, "while", 5, 1));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 5, 7));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 5, 8));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 5, 9));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 5, 11));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 6, 2));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 6, 3));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "0", 6, 4));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 6, 5));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 7, 1));

	tokens.push_back(Token(TokenKind::RBRACE, "}", 8, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 8, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== BLOCK CASE 5 ===\n";
	BlockStmt* block = parser.parse_block_stmt();
	block->dump(std::cout, 0);
	delete block;
}

inline void test_block()
{
	test_block_case1();
	test_block_case2();
	test_block_case3();
	test_block_case4();
	test_block_case5();
}

inline void test_func_case1()
{
	//파라미터 없음
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_INT, "int", 1, 1));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "f", 1, 5));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 6));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 7));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 9));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FUNC CASE 1 (no param) ===\n";
	Stmt* stmt = parser.parse_func_decl_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_func_case2()
{
	//파라미터 여러 개
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_INT, "int", 1, 1));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "add", 1, 5));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 8));
	tokens.push_back(Token(TokenKind::KW_INT, "int", 1, 9));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "a", 1, 13));
	tokens.push_back(Token(TokenKind::COMMA, ",", 1, 14));
	tokens.push_back(Token(TokenKind::KW_INT, "int", 1, 16));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "b", 1, 20));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 21));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 23));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FUNC CASE 2 (multi param) ===\n";
	Stmt* stmt = parser.parse_func_decl_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_func_case3()
{
	//배열 파라미터
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_VOID, "void", 1, 1));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "process", 1, 6));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 13));
	tokens.push_back(Token(TokenKind::KW_INT, "int", 1, 14));
	tokens.push_back(Token(TokenKind::LBRACKET, "[", 1, 17));
	tokens.push_back(Token(TokenKind::RBRACKET, "]", 1, 18));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "arr", 1, 20));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 23));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 25));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FUNC CASE 3 (array param) ===\n";
	Stmt* stmt = parser.parse_func_decl_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_func_case4()
{
	//빈 본문
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_BOOL, "bool", 1, 1));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "check", 1, 6));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 11));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 12));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 14));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 2, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 2, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FUNC CASE 4 (empty body) ===\n";
	Stmt* stmt = parser.parse_func_decl_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_func_case5()
{
	//리턴
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_STRING, "string", 1, 1));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "hello", 1, 8));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 13));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 14));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 16));
	tokens.push_back(Token(TokenKind::KW_RETURN, "return", 2, 2));
	tokens.push_back(Token(TokenKind::STRING_LITERAL, "\"hi\"", 2, 9));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 2, 13));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 3, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 3, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FUNC CASE 5 (return statement) ===\n";
	Stmt* stmt = parser.parse_func_decl_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_func_case6()
{
	std::vector<Token> tokens;
	tokens.push_back(Token(TokenKind::KW_VOID, "void", 1, 1));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "nested", 1, 6));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 1, 12));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 1, 13));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 1, 15));

	// if (x) { while (y) { z = 1; } }
	tokens.push_back(Token(TokenKind::KW_IF, "if", 2, 2));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 2, 5));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "x", 2, 6));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 2, 7));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 2, 9));
	tokens.push_back(Token(TokenKind::KW_WHILE, "while", 3, 3));
	tokens.push_back(Token(TokenKind::LPAREN, "(", 3, 9));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "y", 3, 10));
	tokens.push_back(Token(TokenKind::RPAREN, ")", 3, 11));
	tokens.push_back(Token(TokenKind::LBRACE, "{", 3, 13));
	tokens.push_back(Token(TokenKind::IDENTIFIER, "z", 4, 4));
	tokens.push_back(Token(TokenKind::ASSIGN, "=", 4, 5));
	tokens.push_back(Token(TokenKind::INT_LITERAL, "1", 4, 6));
	tokens.push_back(Token(TokenKind::SEMICOLON, ";", 4, 7));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 5, 3));
	tokens.push_back(Token(TokenKind::RBRACE, "}", 6, 2));

	tokens.push_back(Token(TokenKind::RBRACE, "}", 7, 1));
	tokens.push_back(Token(TokenKind::END_OF_FILE, "", 7, 2));

	TokenStream ts(tokens);
	Logger logger;
	Parser parser(&ts, logger);
	std::cout << "=== FUNC CASE 6 (nested if/while) ===\n";
	Stmt* stmt = parser.parse_func_decl_stmt();
	stmt->dump(std::cout, 0);
	delete stmt;
}

inline void test_func()
{
	test_func_case1();
	test_func_case2();
	test_func_case3();
	test_func_case4();
	test_func_case5();
	test_func_case6();
}

