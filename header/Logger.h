#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "ErrorCode.h"


class Logger
{
public:
	std::string get_current_time();
	std::string log(ErrorCode, int, int);
	std::string log(int, int);
};