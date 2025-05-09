#pragma once
#include <string>
#include <unordered_map>
#include "TokenKind.h"

extern std::unordered_map<TokenKind, int> token_priority_map;

void init_token_priority_map();