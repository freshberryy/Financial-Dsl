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
		std::cout << "[FAIL] ���� ����. AST ���:\n";
		expr->dump(std::cout, 0);
		delete expr;
	}
	catch (const ParserException& e)
	{
		std::cout << "[PASS] ���� �߻�: " << e.what() << "\n";
	}
}

inline void test_postfix_edge_case()
{
	run_postfix_expr_test("���ӵ� �Լ� ȣ��", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::RPAREN, ")", 1, 5),
		Token(TokenKind::LPAREN, "(", 1, 6),
		Token(TokenKind::INT_LITERAL, "1", 1, 7),
		Token(TokenKind::RPAREN, ")", 1, 8),
		Token(TokenKind::END_OF_FILE, "", 1, 9)
		});

	run_postfix_expr_test("�Լ� ȣ�� �� �迭 ����", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::RPAREN, ")", 1, 5),
		Token(TokenKind::LBRACKET, "[", 1, 6),
		Token(TokenKind::INT_LITERAL, "1", 1, 7),
		Token(TokenKind::RBRACKET, "]", 1, 8),
		Token(TokenKind::END_OF_FILE, "", 1, 9)
		});

	run_postfix_expr_test("�Լ� ȣ�� ��ȣ ����", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::INT_LITERAL, "1", 1, 5),
		Token(TokenKind::COMMA, ",", 1, 6),
		Token(TokenKind::INT_LITERAL, "2", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		});

	run_postfix_expr_test("�Լ� �޸��� ����", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::COMMA, ",", 1, 5),
		Token(TokenKind::INT_LITERAL, "1", 1, 6),
		Token(TokenKind::RPAREN, ")", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		});

	run_postfix_expr_test("�Լ� ���� �߰� �� ��", {
		Token(TokenKind::IDENTIFIER, "foo", 1, 1),
		Token(TokenKind::LPAREN, "(", 1, 4),
		Token(TokenKind::INT_LITERAL, "1", 1, 5),
		Token(TokenKind::COMMA, ",", 1, 6),
		Token(TokenKind::COMMA, ",", 1, 7),
		Token(TokenKind::INT_LITERAL, "2", 1, 8),
		Token(TokenKind::RPAREN, ")", 1, 9),
		Token(TokenKind::END_OF_FILE, "", 1, 10)
		});

	run_postfix_expr_test("���ȣ ���� ����", {
		Token(TokenKind::IDENTIFIER, "a", 1, 1),
		Token(TokenKind::LBRACKET, "[", 1, 2),
		Token(TokenKind::INT_LITERAL, "1", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});

	run_postfix_expr_test("3���� �迭 ����", {
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
		std::cout << "[FAIL] ���� ����. AST ���:\n";
		expr->dump(std::cout, 0);
		delete expr;
	}
	catch (const ParserException& e)
	{
		std::cout << "[PASS] ���� �߻�: " << e.what() << "\n";
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
			std::cout << "[PASS] AST ���:\n";
			expr->dump(std::cout, 0);
			delete expr;
		}
		else
		{
			std::cout << "[FAIL] ��ȯ�� AST�� null\n";
		}
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] ���� �߻�: " << e.what() << "\n";
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
			std::cout << "-��밪: " << expected_output << ", ������: " << result << "\n";
			delete expr;
		}
		else
		{
			std::cout << "[FAIL] ��ȯ�� AST�� null\n";
		}
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] ���� �߻�: " << e.what() << "\n";
	}
}


inline void test_unary_binary()
{
	// ����
	run_expr_edge_test("���� ������ �� ǥ���� ����", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::END_OF_FILE, "", 1, 2)
		});

	run_expr_edge_test("���� ������ �� ǥ���� ����", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_edge_test("���� ���� ������ (1 + *)", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::MUL, "*", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_edge_test("���� ���� ������ ���� ����", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::MINUS, "-", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_edge_test("��ȣ �� �������", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::RPAREN, ")", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_edge_test("��ȣ ���� ����", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::PLUS, "+", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_edge_test("�߸��� ��ū ����", {
		Token(TokenKind::COMMA, ",", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_edge_test("���ӵ� *, / �����ڸ� ����", {
		Token(TokenKind::MUL, "*", 1, 1),
		Token(TokenKind::DIV, "/", 1, 2),
		Token(TokenKind::MOD, "%", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});

	run_expr_edge_test("���� + + �߸��� ����", {
		Token(TokenKind::PLUS, "+", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::PLUS, "+", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});


	// dump
	run_expr_dump_test("���� -1", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_expr_dump_test("���� 1 + 2", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});

	run_expr_dump_test("���� �켱 1 + 2 * 3", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::MUL, "*", 1, 4),
		Token(TokenKind::INT_LITERAL, "3", 1, 5),
		Token(TokenKind::END_OF_FILE, "", 1, 6)
		});

	run_expr_dump_test("�켱���� �׽�Ʈ: (1 + 2) * 3", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::PLUS, "+", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::RPAREN, ")", 1, 5),
		Token(TokenKind::MUL, "*", 1, 6),
		Token(TokenKind::INT_LITERAL, "3", 1, 7),
		Token(TokenKind::END_OF_FILE, "", 1, 8)
		});

	run_expr_dump_test("���� - ���� ����", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::INT_LITERAL, "2", 1, 2),
		Token(TokenKind::MUL, "*", 1, 3),
		Token(TokenKind::INT_LITERAL, "3", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_dump_test("���� ���� ������ ---3", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::MINUS, "-", 1, 2),
		Token(TokenKind::MINUS, "-", 1, 3),
		Token(TokenKind::INT_LITERAL, "3", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_dump_test("ȥ�� 1 + -2 * 3", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::MINUS, "-", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::MUL, "*", 1, 5),
		Token(TokenKind::INT_LITERAL, "3", 1, 6),
		Token(TokenKind::END_OF_FILE, "", 1, 7)
		});

	run_expr_dump_test("��ȣ ���� ����", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::MINUS, "-", 1, 2),
		Token(TokenKind::INT_LITERAL, "7", 1, 3),
		Token(TokenKind::RPAREN, ")", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_expr_dump_test("����/����/����/������ ȥ��", {
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

	run_expr_dump_test("��ȣ�� �켱���� ����", {
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
	run_expr_to_string_test("���� -1", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		}, "-1");

	run_expr_to_string_test("���� 1 + 2", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		}, "1 + 2");

	run_expr_to_string_test("���� �켱 1 + 2 * 3", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::PLUS, "+", 1, 2),
		Token(TokenKind::INT_LITERAL, "2", 1, 3),
		Token(TokenKind::MUL, "*", 1, 4),
		Token(TokenKind::INT_LITERAL, "3", 1, 5),
		Token(TokenKind::END_OF_FILE, "", 1, 6)
		}, "1 + 2 * 3");

	run_expr_to_string_test("��ȣ ǥ���� (1 + 2) * 3", {
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
			std::cout << "[PASS] AST ����:\n";
			expr->dump(std::cout, 0);
			delete expr;
		}
		else
		{
			std::cout << "[FAIL] ��ȯ�� AST�� null\n";
		}
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] ���� �߻�: " << e.what() << "\n";
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
			std::cout << "[FAIL] AST�� null\n";
			return;
		}
		std::string result = expr->to_string();
		std::cout << "- ��밪: " << expected << ", ������: " << result << "\n";
		delete expr;
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] ���� �߻�: " << e.what() << "\n";
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


	run_relational_expr_dump_test("������ �ǿ����� ����", {
	Token(TokenKind::INT_LITERAL, "1", 1, 1),
	Token(TokenKind::LESS, "<", 1, 2),
	Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_relational_expr_dump_test("���� �ǿ����� ����", {
		Token(TokenKind::LESS, "<", 1, 1),
		Token(TokenKind::INT_LITERAL, "1", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_relational_expr_dump_test("���� ������ <<", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::LESS, "<", 1, 2),
		Token(TokenKind::LESS, "<", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_relational_expr_dump_test("������ ��ū ����", {
		Token(TokenKind::COMMA, ",", 1, 1),
		Token(TokenKind::LESS, "<", 1, 2),
		Token(TokenKind::INT_LITERAL, "3", 1, 3),
		Token(TokenKind::END_OF_FILE, "", 1, 4)
		});

	run_relational_expr_dump_test("������ �� ��ȣ ���� ���� ����", {
		Token(TokenKind::INT_LITERAL, "1", 1, 1),
		Token(TokenKind::GREATER, ">", 1, 2),
		Token(TokenKind::LPAREN, "(", 1, 3),
		Token(TokenKind::INT_LITERAL, "2", 1, 4),
		Token(TokenKind::END_OF_FILE, "", 1, 5)
		});

	run_relational_expr_dump_test("���� �� ����", {
		Token(TokenKind::LESS_EQUAL, "<=", 1, 1),
		Token(TokenKind::INT_LITERAL, "5", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});

	run_relational_expr_dump_test("���� �����ڸ� ����", {
		Token(TokenKind::MINUS, "-", 1, 1),
		Token(TokenKind::END_OF_FILE, "", 1, 2)
		});

	run_relational_expr_dump_test("��ȣ�� ����", {
		Token(TokenKind::LPAREN, "(", 1, 1),
		Token(TokenKind::RPAREN, ")", 1, 2),
		Token(TokenKind::END_OF_FILE, "", 1, 3)
		});


	run_relational_expr_dump_test("�߰� �ǿ����� ����", {
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
			std::cout << "[FAIL] AST�� null\n";
			return;
		}
		std::cout << "[PASS] AST ����:\n";
		expr->dump(std::cout, 0);
		delete expr;
	}
	catch (const ParserException& e)
	{
		std::cout << "[FAIL] ���� �߻�: " << e.what() << "\n";
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