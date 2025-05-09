#include "TokenRegex.h"
using namespace std;

unordered_map<TokenKind, regex> regex_map;


void init_regex_map()
{
    // 키워드
    regex_map[TokenKind::KW_INT] = regex(R"(\bint\b)");
    regex_map[TokenKind::KW_FLOAT] = regex(R"(\bfloat\b)");
    regex_map[TokenKind::KW_BOOL] = regex(R"(\bbool\b)");
    regex_map[TokenKind::KW_STRING] = regex(R"(\bstring\b)");
    regex_map[TokenKind::KW_IF] = regex(R"(\bif\b)");
    regex_map[TokenKind::KW_VOID] = regex(R"(\bvoid\b)");
    regex_map[TokenKind::KW_ELSE] = regex(R"(\belse\b)");
    regex_map[TokenKind::KW_IF_ELSE] = regex(R"(\bif_else\b)"); 
    regex_map[TokenKind::KW_FOR] = regex(R"(\bfor\b)");
    regex_map[TokenKind::KW_WHILE] = regex(R"(\bwhile\b)");
    regex_map[TokenKind::KW_RETURN] = regex(R"(\breturn\b)");
    regex_map[TokenKind::KW_BREAK] = regex(R"(\bbreak\b)");
    regex_map[TokenKind::KW_CONTINUE] = regex(R"(\bcontinue\b)");
    regex_map[TokenKind::KW_FUNC] = regex(R"(\bfunc\b)");

    // 식별자
    regex_map[TokenKind::IDENTIFIER] = regex(R"([a-zA-Z_][a-zA-Z0-9_]*)");

    // 연산자
    regex_map[TokenKind::PLUS] = regex(R"(\+)");
    regex_map[TokenKind::MINUS] = regex(R"(\-)");
    regex_map[TokenKind::MUL] = regex(R"(\*)");
    regex_map[TokenKind::DIV] = regex(R"(\/)");
    regex_map[TokenKind::MOD] = regex(R"(%)");
    regex_map[TokenKind::EQUAL] = regex(R"(==)");
    regex_map[TokenKind::NOT_EQUAL] = regex(R"(!=)");
    regex_map[TokenKind::LESS] = regex(R"(<)");
    regex_map[TokenKind::GREATER] = regex(R"(>)");
    regex_map[TokenKind::LESS_EQUAL] = regex(R"(<=)");
    regex_map[TokenKind::GREATER_EQUAL] = regex(R"(>=)");
    regex_map[TokenKind::AND] = regex(R"(&&)");
    regex_map[TokenKind::OR] = regex(R"(\|\|)");
    regex_map[TokenKind::ASSIGN] = regex(R"(=)");
    regex_map[TokenKind::NOT] = regex(R"(!)");

    // 구두점
    regex_map[TokenKind::LBRACKET] = regex(R"(\[)");
    regex_map[TokenKind::RBRACKET] = regex(R"(\])");
    regex_map[TokenKind::LBRACE] = regex(R"(\{)");
    regex_map[TokenKind::RBRACE] = regex(R"(\})");
    regex_map[TokenKind::LPAREN] = regex(R"(\()");
    regex_map[TokenKind::RPAREN] = regex(R"(\))");
    regex_map[TokenKind::COMMA] = regex(R"(,)");
    regex_map[TokenKind::SEMICOLON] = regex(R"(;)");

    // 리터럴
    regex_map[TokenKind::INT_LITERAL] = regex(R"([0-9]+)");
    regex_map[TokenKind::FLOAT_LITERAL] = regex(R"(([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))");
    regex_map[TokenKind::BOOL_LITERAL] = regex(R"(\b(true|false)\b)");
    regex_map[TokenKind::STRING_LITERAL] = regex(R"("([^"\\]|\\["\\nrt])*")");

    // 기타
    regex_map[TokenKind::WHITESPACE] = regex(R"([ \t\n\r]+)");
    regex_map[TokenKind::COMMENT] = regex(R"(#.*)");
    regex_map[TokenKind::UNKNOWN] = regex(R"(.)");      //fallback
}

