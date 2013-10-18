#ifndef LOG_HPP
#define LOG_HPP

#include <cstddef>
#include <string>
#include <iostream>

class Log
{
public:
    static Log* Instance();
    ~Log();

    void log(std::string message);

private:
    Log();
    Log(Log const&);
    Log& operator=(Log const&);
    static Log *_log_instance;
};

#endif
