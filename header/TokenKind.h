#pragma once
#include <unordered_map>
#include <string>
#include <stdexcept>
using  namespace std;

enum class TokenKind
{
	//Ű����
	KW_INT, KW_FLOAT, KW_BOOL, KW_STRING, KW_IF, KW_VOID, KW_ELSE, KW_ELSE_IF, KW_FOR, KW_WHILE, KW_RETURN, KW_BREAK, KW_CONTINUE, 

	//�ĺ���
	IDENTIFIER,

	//������
	PLUS, MINUS, MUL, DIV, MOD, EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL, NOT, AND, OR, ASSIGN,

	//������
	LBRACKET, RBRACKET, LBRACE, RBRACE, LPAREN, RPAREN, COMMA, SEMICOLON,

	//���ͷ�
	INT_LITERAL, FLOAT_LITERAL, BOOL_LITERAL, STRING_LITERAL,

	//��Ÿ
	END_OF_FILE, WHITESPACE, COMMENT, UNKNOWN
};

inline const char* token_kind_to_string(TokenKind kind)
{
	switch (kind)
	{
	case TokenKind::KW_INT: return "int";
	case TokenKind::KW_FLOAT: return "float";
	case TokenKind::KW_BOOL: return "bool";
	case TokenKind::KW_STRING: return "string";
	case TokenKind::KW_IF: return "if";
	case TokenKind::KW_VOID: return "void";
	case TokenKind::KW_ELSE: return "else";
	case TokenKind::KW_ELSE_IF: return "if_else";
	case TokenKind::KW_FOR: return "for";
	case TokenKind::KW_WHILE: return "while";
	case TokenKind::KW_RETURN: return "return";
	case TokenKind::KW_BREAK: return "break";
	case TokenKind::KW_CONTINUE: return "continue";

	case TokenKind::IDENTIFIER: return "identifier";

	case TokenKind::PLUS: return "+";
	case TokenKind::MINUS: return "-";
	case TokenKind::MUL: return "*";
	case TokenKind::DIV: return "/";
	case TokenKind::MOD: return "%";
	case TokenKind::EQUAL: return "==";
	case TokenKind::NOT_EQUAL: return "!=";
	case TokenKind::LESS: return "<";
	case TokenKind::GREATER: return ">";
	case TokenKind::LESS_EQUAL: return "<=";
	case TokenKind::GREATER_EQUAL: return ">=";
	case TokenKind::NOT: return "!";
	case TokenKind::AND: return "&&";
	case TokenKind::OR: return "||";
	case TokenKind::ASSIGN: return "=";

	case TokenKind::LBRACKET: return "[";
	case TokenKind::RBRACKET: return "]";
	case TokenKind::LBRACE: return "{";
	case TokenKind::RBRACE: return "}";
	case TokenKind::LPAREN: return "(";
	case TokenKind::RPAREN: return ")";
	case TokenKind::COMMA: return ",";
	case TokenKind::SEMICOLON: return ";";

	case TokenKind::INT_LITERAL: return "int_literal";
	case TokenKind::FLOAT_LITERAL: return "float_literal";
	case TokenKind::BOOL_LITERAL: return "bool_literal";
	case TokenKind::STRING_LITERAL: return "string_literal";

	case TokenKind::END_OF_FILE: return "eof";
	case TokenKind::WHITESPACE: return "whitespace";
	case TokenKind::COMMENT: return "comment";
	case TokenKind::UNKNOWN: return "unknown";

	default: return "invalid_token";
	}
}


inline TokenKind token_kind_from_string(const string& str)
{
    // Ű����
    if (str == "KW_INT")        return TokenKind::KW_INT;
    if (str == "KW_FLOAT")      return TokenKind::KW_FLOAT;
    if (str == "KW_BOOL")       return TokenKind::KW_BOOL;
    if (str == "KW_STRING")     return TokenKind::KW_STRING;
    if (str == "KW_IF")         return TokenKind::KW_IF;
    if (str == "KW_VOID")       return TokenKind::KW_VOID;
    if (str == "KW_ELSE")       return TokenKind::KW_ELSE;
    if (str == "KW_IF_ELSE")    return TokenKind::KW_ELSE_IF;
    if (str == "KW_FOR")        return TokenKind::KW_FOR;
    if (str == "KW_WHILE")      return TokenKind::KW_WHILE;
    if (str == "KW_RETURN")     return TokenKind::KW_RETURN;
    if (str == "KW_BREAK")      return TokenKind::KW_BREAK;
    if (str == "KW_CONTINUE")   return TokenKind::KW_CONTINUE;

    // �ĺ���
    if (str == "IDENTIFIER")    return TokenKind::IDENTIFIER;

    // ������
    if (str == "PLUS")          return TokenKind::PLUS;
    if (str == "MINUS")         return TokenKind::MINUS;
    if (str == "MUL")           return TokenKind::MUL;
    if (str == "DIV")           return TokenKind::DIV;
    if (str == "MOD")           return TokenKind::MOD;
    if (str == "EQUAL")         return TokenKind::EQUAL;
    if (str == "NOT_EQUAL")     return TokenKind::NOT_EQUAL;
    if (str == "LESS")          return TokenKind::LESS;
    if (str == "GREATER")       return TokenKind::GREATER;
    if (str == "LESS_EQUAL")    return TokenKind::LESS_EQUAL;
    if (str == "GREATER_EQUAL") return TokenKind::GREATER_EQUAL;
    if (str == "NOT")           return TokenKind::NOT;
    if (str == "AND")           return TokenKind::AND;
    if (str == "OR")            return TokenKind::OR;
    if (str == "ASSIGN")        return TokenKind::ASSIGN;

    // ������
    if (str == "LBRACKET")      return TokenKind::LBRACKET;
    if (str == "RBRACKET")      return TokenKind::RBRACKET;
    if (str == "LBRACE")        return TokenKind::LBRACE;
    if (str == "RBRACE")        return TokenKind::RBRACE;
    if (str == "LPAREN")        return TokenKind::LPAREN;
    if (str == "RPAREN")        return TokenKind::RPAREN;
    if (str == "COMMA")         return TokenKind::COMMA;
    if (str == "SEMICOLON")     return TokenKind::SEMICOLON;

    // ���ͷ�
    if (str == "INT_LITERAL")   return TokenKind::INT_LITERAL;
    if (str == "FLOAT_LITERAL") return TokenKind::FLOAT_LITERAL;
    if (str == "BOOL_LITERAL")  return TokenKind::BOOL_LITERAL;
    if (str == "STRING_LITERAL")return TokenKind::STRING_LITERAL;

    // ��Ÿ
    if (str == "WHITESPACE")    return TokenKind::WHITESPACE;
    if (str == "COMMENT")       return TokenKind::COMMENT;
    if (str == "END_OF_FILE")   return TokenKind::END_OF_FILE;
    if (str == "UNKNOWN")       return TokenKind::UNKNOWN;

    throw invalid_argument("���� ��ū �����Դϴ�: " + str);
}

