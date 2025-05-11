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