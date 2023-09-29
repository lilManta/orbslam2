/**
 * @brief 封装glog 库
 */

#ifndef INITGLOG_H
#define INITGLOG_H

#include <string>
#include "glog/logging.h"

namespace ORB_SLAM2
{

    class InitGlog
    {
    private:
        /* data */
    public:
        InitGlog(/* args */);
        InitGlog(const std::string &programName,const std::string &logPath);
        void ShutDownLog();
        ~InitGlog();
    };

    InitGlog::InitGlog(/* args */)
    {
    }

    InitGlog::InitGlog(const std::string &programName,const std::string &logPath)
    {
        google::InitGoogleLogging(programName.c_str());                      // 使用glog之前必须先初始化库，仅需执行一次，括号内为程序名
        FLAGS_alsologtostderr = true;                                          // 是否将日志输出到文件和stderr
        FLAGS_colorlogtostderr = true;                                         // 是否启用不同颜色显示
        google::SetLogDestination(google::GLOG_INFO, (logPath+"/INFO_").c_str());       // INFO级别的日志都存放到logs目录下且前缀为INFO_
        google::SetLogDestination(google::GLOG_WARNING, (logPath+"/WARNING_").c_str()); // WARNING级别的日志都存放到logs目录下且前缀为WARNING_
        google::SetLogDestination(google::GLOG_ERROR, (logPath+"/ERROR_").c_str());     // ERROR级别的日志都存放到logs目录下且前缀为ERROR_
        google::SetLogDestination(google::GLOG_FATAL, (logPath+"/FATAL_").c_str());     // FATAL级别的日志都存放到logs目录下且前缀为FATAL_
    }

    void InitGlog::ShutDownLog()
    {
        google::ShutdownGoogleLogging();
    }

    InitGlog::~InitGlog()
    {
    }

}

#endif