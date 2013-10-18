#include "Log.hpp"

Log* Log::_log_instance = NULL;

Log* Log::Instance()
{
    if(!_log_instance)
    {
        _log_instance = new Log();
    }

    return _log_instance;
}

Log::Log()
{

}

void Log::log(std::string s)
{
    std::cout << s << std::endl;
}



