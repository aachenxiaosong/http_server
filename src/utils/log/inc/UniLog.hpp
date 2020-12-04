#ifndef UTILS_LOG_INC_UNI_LOG_HPP_
#define UTILS_LOG_INC_UNI_LOG_HPP_

#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Util/LoggingConfigurator.h"
#include <stdarg.h>

using namespace std;

#define DEFAULT_LOG_MAX_LEN (1024 * 100)
#define LOG_CONFIG_PATH "./config/log_conf.properties"
#define LOGT(tag, fmt, ...) UniLog::log(UniLog::LOG_TRACK, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGW(tag, fmt, ...) UniLog::log(UniLog::LOG_WARN, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGE(tag, fmt, ...) UniLog::log(UniLog::LOG_ERROR, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__)


class UniLog {
public:
    enum LogType {
        LOG_TRACK = 0,
        LOG_WARN,
        LOG_ERROR
    } ;
private:
    static void writeLog(LogType type, const char* tag, const char* file, int line, const char* fmt, va_list args) {
        char buf[DEFAULT_LOG_MAX_LEN] = {0};
        vsnprintf(buf, DEFAULT_LOG_MAX_LEN, fmt, args);
        Poco::Logger &logger = Poco::Logger::get(tag);
        if (type == LOG_TRACK && logger.information())
        {
            logger.information(buf, file, line);
        } else if (type == LOG_WARN && logger.warning())
        {
            logger.warning(buf, file, line);
        } else if (type == LOG_ERROR && logger.error())
        {
            logger.error(buf, file, line);
        }
    }
public:
    static void init() {
        Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConf = new Poco::Util::PropertyFileConfiguration(LOG_CONFIG_PATH);
        Poco::Util::LoggingConfigurator log_configurator;
        log_configurator.configure(pConf);
    }
    static void log(LogType type, const char* tag, const char* file, int line, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        writeLog(type, tag, file, line, fmt, args);
        va_end(args);
    }
    static void log(LogType type, const string& tag, const char* file, int line, const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        writeLog(type, tag.c_str(), file, line, fmt, args);
        va_end(args);
    }
    static void log(LogType type, const char* tag, const char* file, int line, const string& fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        writeLog(type, tag, file, line, fmt.c_str(), args);
        va_end(args);
    }
    static void log(LogType type, const string& tag, const char* file, int line, const string& fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        writeLog(type, tag.c_str(), file, line, fmt.c_str(), args);
        va_end(args);
    }
};

#endif  //  UTILS_LOG_INC_UNI_LOG_HPP_