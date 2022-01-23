#include "generate-string.h"
#include <ctime>

std::string generateString()
{
    std::time_t cur_time = std::time(nullptr);
    std::tm *p_tm = std::localtime(&cur_time);
    std::string time_str = std::asctime(p_tm);
    time_str.pop_back();
    return time_str;
}
