#ifndef J_LOG_H
#define J_LOG_H

#include <memory>
#include <vector>
#include <stdarg.h>
#include "util/jbuffer.hpp"
#include "concurrent/jmutex.hpp"
#include "concurrent/jqueue.hpp"


// Warn: 不要在多进程环境中使用此日志框架，pthread中的东西经过fork之后会变得复杂且难以控制

#define TRACE(fmt, args...)\
        do  \
        {   \
            jarvis::jlog::LogTrace("%s %d %s " fmt, __FILE__, __LINE__, __FUNCTION__, ## args);   \
        }   \
        while (0)

#define DEBUG(fmt, args...)\
        do  \
        {   \
            jarvis::jlog::LogDebug("%s %d %s " fmt, __FILE__, __LINE__, __FUNCTION__, ## args);   \
        }   \
        while (0)

#define INFO(fmt, args...)\
        do  \
        {   \
            jarvis::jlog::LogInfo("%s %d %s " fmt, __FILE__, __LINE__, __FUNCTION__, ## args);   \
        }   \
        while (0)

#define WARN(fmt, args...)\
        do  \
        {   \
            jarvis::jlog::LogWarn("%s %d %s " fmt, __FILE__, __LINE__, __FUNCTION__, ## args);   \
        }   \
        while (0)

#define ERROR(fmt, args...)\
        do  \
        {   \
            jarvis::jlog::LogError("%s %d %s " fmt, __FILE__, __LINE__, __FUNCTION__, ## args);   \
        }   \
        while (0)

namespace jarvis
{

namespace jlog
{

enum LOGLEVEL
{
    TRACE = 0,
    DEBUG,
    INFO,
    WARN,
    ERROR
};

int OpenLog();
LOGLEVEL SetLogLevel(LOGLEVEL l);

// log
int Log(LOGLEVEL l, const char *buf, int n);

// 最好是直接重定向，减少不必要的复制
int LogTrace(const char *fmt, ...);
int LogDebug(const char *fmt, ...);
int LogInfo(const char *fmt, ...);
int LogWarn(const char *fmt, ...);
int LogError(const char *fmt, ...);

}   // namespace jlog

}   // namespace jarvis

#endif
