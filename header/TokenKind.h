#pragma once
#include <unordered_map>
#include <string>
#include <stdexcept>
using  namespace std;

enum class TokenKind
{
	//키워드
	KW_INT, KW_FLOAT, KW_BOOL, KW_STRING, KW_IF, KW_VOID, KW_ELSE, KW_IF_ELSE, KW_FOR, KW_WHILE, KW_RETURN, KW_BREAK, KW_CONTINUE, KW_FUNC,

	//식별자
	IDENTIFIER,

	//연산자
	PLUS, MINUS, MUL, DIV, MOD, EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL, NOT, AND, OR, ASSIGN,

	//구두점
	LBRACKET, RBRACKET, LBRACE, RBRACE, LPAREN, RPAREN, COMMA, SEMICOLON,

	//리터럴
	INT_LITERAL, FLOAT_LITERAL, BOOL_LITERAL, STRING_LITERAL,

	//기타
	END_OF_FILE, WHITESPACE, COMMENT, UNKNOWN
};

inline const char* token_kind_to_string(TokenKind kind)
{
    switch (kind)
    {
        // 키워드
    case TokenKind::KW_INT:        return "KW_INT";
    case TokenKind::KW_FLOAT:      return "KW_FLOAT";
    case TokenKind::KW_BOOL:       return "KW_BOOL";
    case TokenKind::KW_STRING:     return "KW_STRING";
    case TokenKind::KW_IF:         return "KW_IF";
    case TokenKind::KW_VOID:       return "KW_VOID";
    case TokenKind::KW_ELSE:       return "KW_ELSE";
    case TokenKind::KW_IF_ELSE:    return "KW_IF_ELSE";
    case TokenKind::KW_FOR:        return "KW_FOR";
    case TokenKind::KW_WHILE:      return "KW_WHILE";
    case TokenKind::KW_RETURN:     return "KW_RETURN";
    case TokenKind::KW_BREAK:      return "KW_BREAK";
    case TokenKind::KW_CONTINUE:   return "KW_CONTINUE";
    case TokenKind::KW_FUNC:       return "KW_FUNC";

        // 식별자
    case TokenKind::IDENTIFIER:    return "IDENTIFIER";

        // 연산자
    case TokenKind::PLUS:          return "PLUS";
    case TokenKind::MINUS:         return "MINUS";
    case TokenKind::MUL:           return "MUL";
    case TokenKind::DIV:           return "DIV";
    case TokenKind::MOD:           return "MOD";
    case TokenKind::EQUAL:         return "EQUAL";
    case TokenKind::NOT_EQUAL:     return "NOT_EQUAL";
    case TokenKind::LESS:          return "LESS";
    case TokenKind::GREATER:       return "GREATER";
    case TokenKind::LESS_EQUAL:    return "LESS_EQUAL";
    case TokenKind::GREATER_EQUAL: return "GREATER_EQUAL";
    case TokenKind::NOT:           return "NOT";
    case TokenKind::AND:           return "AND";
    case TokenKind::OR:            return "OR";
    case TokenKind::ASSIGN:        return "ASSIGN";

        // 구두점
    case TokenKind::LBRACKET:      return "LBRACKET";
    case TokenKind::RBRACKET:      return "RBRACKET";
    case TokenKind::LBRACE:        return "LBRACE";
    case TokenKind::RBRACE:        return "RBRACE";
    case TokenKind::LPAREN:        return "LPAREN";
    case TokenKind::RPAREN:        return "RPAREN";
    case TokenKind::COMMA:         return "COMMA";
    case TokenKind::SEMICOLON:     return "SEMICOLON";

        // 리터럴
    case TokenKind::INT_LITERAL:   return "INT_LITERAL";
    case TokenKind::FLOAT_LITERAL: return "FLOAT_LITERAL";
    case TokenKind::BOOL_LITERAL:  return "BOOL_LITERAL";
    case TokenKind::STRING_LITERAL:return "STRING_LITERAL";

        // 기타
    case TokenKind::WHITESPACE:    return "WHITESPACE";
    case TokenKind::COMMENT:       return "COMMENT";
    case TokenKind::END_OF_FILE:   return "END_OF_FILE";
    case TokenKind::UNKNOWN:       return "UNKNOWN";
    }
    return "UNKNOWN";
}


inline TokenKind token_kind_from_string(const string& str)
{
    // 키워드
    if (str == "KW_INT")        return TokenKind::KW_INT;
    if (str == "KW_FLOAT")      return TokenKind::KW_FLOAT;
    if (str == "KW_BOOL")       return TokenKind::KW_BOOL;
    if (str == "KW_STRING")     return TokenKind::KW_STRING;
    if (str == "KW_IF")         return TokenKind::KW_IF;
    if (str == "KW_VOID")       return TokenKind::KW_VOID;
    if (str == "KW_ELSE")       return TokenKind::KW_ELSE;
    if (str == "KW_IF_ELSE")    return TokenKind::KW_IF_ELSE;
    if (str == "KW_FOR")        return TokenKind::KW_FOR;
    if (str == "KW_WHILE")      return TokenKind::KW_WHILE;
    if (str == "KW_RETURN")     return TokenKind::KW_RETURN;
    if (str == "KW_BREAK")      return TokenKind::KW_BREAK;
    if (str == "KW_CONTINUE")   return TokenKind::KW_CONTINUE;
    if (str == "KW_FUNC")       return TokenKind::KW_FUNC;

    // 식별자
    if (str == "IDENTIFIER")    return TokenKind::IDENTIFIER;

    // 연산자
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

    // 구두점
    if (str == "LBRACKET")      return TokenKind::LBRACKET;
    if (str == "RBRACKET")      return TokenKind::RBRACKET;
    if (str == "LBRACE")        return TokenKind::LBRACE;
    if (str == "RBRACE")        return TokenKind::RBRACE;
    if (str == "LPAREN")        return TokenKind::LPAREN;
    if (str == "RPAREN")        return TokenKind::RPAREN;
    if (str == "COMMA")         return TokenKind::COMMA;
    if (str == "SEMICOLON")     return TokenKind::SEMICOLON;

    // 리터럴
    if (str == "INT_LITERAL")   return TokenKind::INT_LITERAL;
    if (str == "FLOAT_LITERAL") return TokenKind::FLOAT_LITERAL;
    if (str == "BOOL_LITERAL")  return TokenKind::BOOL_LITERAL;
    if (str == "STRING_LITERAL")return TokenKind::STRING_LITERAL;

    // 기타
    if (str == "WHITESPACE")    return TokenKind::WHITESPACE;
    if (str == "COMMENT")       return TokenKind::COMMENT;
    if (str == "END_OF_FILE")   return TokenKind::END_OF_FILE;
    if (str == "UNKNOWN")       return TokenKind::UNKNOWN;

    throw invalid_argument("없는 토큰 종류입니다: " + str);
}

