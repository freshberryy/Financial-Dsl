#pragma once
#include <stdexcept>
#include <string>

class ParserException : public std::invalid_argument
{
public:
    ParserException(const std::string& msg, int line, int col)
        : std::invalid_argument(msg), line(line), col(col) {}

    int get_line() const { return line; }
    int get_col() const { return col; }

private:
    int line, col;
};

