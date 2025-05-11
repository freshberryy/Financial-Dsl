#include "Logger.h"
using namespace std;
using namespace std::chrono;

string Logger::get_current_time()
{
	auto now = system_clock::now();
	time_t now_t = system_clock::to_time_t(now);
	auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

	struct tm timeinfo;
	localtime_s(&timeinfo, &now_t);

	stringstream ss;
	ss << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
	ss << "." << setw(3) << setfill('0') << ms.count();

	return ss.str();
}

string Logger::log(ErrorCode code, int row, int col)
{
	stringstream ss;

	ss << "[에러] " << get_current_time()
		<< " | " << error_code_to_string(code)
		<< " | line " << row << ", column " << col
		<< endl;

	string ret = ss.str();

	cerr << ret;

	return ret;
}

string Logger::log(int line, int col)
{
	stringstream ss;

	ss << "[에러] " << get_current_time()
		<< " | line " << line << ", column " << col
		<< " | ";

	string ret = ss.str();

	cerr << ret;

	return ret;
}
