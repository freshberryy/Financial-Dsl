#pragma once
#include <string>
#include <regex>
#include <unordered_map>
#include "TokenKind.h"

extern std::unordered_map<TokenKind, std::regex> regex_map;

void init_regex_map();