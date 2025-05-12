#include "TokenPriority.h"
using namespace std;

unordered_map<TokenKind, int> token_priority_map;

void init_token_priority_map()
{
    // Ű���� 
    token_priority_map[TokenKind::KW_INT] = 1;
    token_priority_map[TokenKind::KW_FLOAT] = 1;
    token_priority_map[TokenKind::KW_BOOL] = 1;
    token_priority_map[TokenKind::KW_STRING] = 1;
    token_priority_map[TokenKind::KW_IF] = 1;
    token_priority_map[TokenKind::KW_VOID] = 1;
    token_priority_map[TokenKind::KW_ELSE] = 1;
    token_priority_map[TokenKind::KW_ELSE_IF] = 1;
    token_priority_map[TokenKind::KW_FOR] = 1;
    token_priority_map[TokenKind::KW_WHILE] = 1;
    token_priority_map[TokenKind::KW_RETURN] = 1;
    token_priority_map[TokenKind::KW_BREAK] = 1;
    token_priority_map[TokenKind::KW_CONTINUE] = 1;

    // ������ 
    token_priority_map[TokenKind::PLUS] = 2;
    token_priority_map[TokenKind::MINUS] = 2;
    token_priority_map[TokenKind::MUL] = 2;
    token_priority_map[TokenKind::DIV] = 2;
    token_priority_map[TokenKind::MOD] = 2;
    token_priority_map[TokenKind::EQUAL] = 2;
    token_priority_map[TokenKind::NOT_EQUAL] = 2;
    token_priority_map[TokenKind::LESS] = 2;
    token_priority_map[TokenKind::GREATER] = 2;
    token_priority_map[TokenKind::LESS_EQUAL] = 2;
    token_priority_map[TokenKind::GREATER_EQUAL] = 2;
    token_priority_map[TokenKind::NOT] = 2;
    token_priority_map[TokenKind::AND] = 2;
    token_priority_map[TokenKind::OR] = 2;
    token_priority_map[TokenKind::ASSIGN] = 2;

    // ������ 
    token_priority_map[TokenKind::LBRACKET] = 3;
    token_priority_map[TokenKind::RBRACKET] = 3;
    token_priority_map[TokenKind::LBRACE] = 3;
    token_priority_map[TokenKind::RBRACE] = 3;
    token_priority_map[TokenKind::LPAREN] = 3;
    token_priority_map[TokenKind::RPAREN] = 3;
    token_priority_map[TokenKind::COMMA] = 3;
    token_priority_map[TokenKind::SEMICOLON] = 3;

    // ���ͷ� 
    token_priority_map[TokenKind::INT_LITERAL] = 4;
    token_priority_map[TokenKind::FLOAT_LITERAL] = 4;
    token_priority_map[TokenKind::BOOL_LITERAL] = 4;
    token_priority_map[TokenKind::STRING_LITERAL] = 4;

    // �ĺ��� 
    token_priority_map[TokenKind::IDENTIFIER] = 5;

    // ���� �� �ּ� 
    token_priority_map[TokenKind::WHITESPACE] = 6;
    token_priority_map[TokenKind::COMMENT] = 6;

    // ��Ÿ 
    token_priority_map[TokenKind::END_OF_FILE] = 7;
    token_priority_map[TokenKind::UNKNOWN] = 7;
}
