/**
 * @brief 封装glog 库
 */

#ifndef INITGLOG_H
#define INITGLOG_H

#include <string>
#include "glog/logging.h"

class InitGlog
{
private:
    /* data */
public:
    InitGlog(/* args */);
    InitGlog(const std::string &programName, const std::string &logPath);
    void ShutDownLog();
    ~InitGlog();
};

#endif